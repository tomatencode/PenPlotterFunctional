# Settings Architecture Design

## Quick API Overview

Instead of individual getters/setters (which don't scale), settings are **grouped into structs by domain**. This gives you:

- ✅ **Scalability**: Add 10 new settings? Add them to a struct, add one line to getter/setter, done.
- ✅ **Type safety**: Each setting is strongly typed; can't accidentally set the wrong type.
- ✅ **Clarity**: `repo.updateMachineSettings(s)` is obvious about what's being persisted.
- ✅ **NVS encapsulation**: All persistence logic is hidden; no confusion about when things get written to flash.

---

## The Three Files

### 1. `SettingsObserver.hpp` — Observer Interface

```cpp
struct NetworkSettings { std::string ssid; ... };
struct MachineSettings { float drawFeedRate; ... };
struct PenSettings { float upAngle; ... };

class SettingsObserver {
public:
    virtual void onNetworkSettingsChanged(const NetworkSettings&) {}
    virtual void onMachineSettingsChanged(const MachineSettings&) {}
    virtual void onPenSettingsChanged(const PenSettings&) {}
};
```

**Pattern**: Exactly like `JobObserver`. Observer subclasses override only the events they care about (default no-op implementations).

---

### 2. `SettingsRepository.hpp + .cpp` — Central Repository

**Public API:**

```cpp
// Initialize at startup (load from NVS)
void init();

// Getters (return current values)
NetworkSettings getNetworkSettings() const;
MachineSettings getMachineSettings() const;
PenSettings getPenSettings() const;

// Setters (persist to NVS + update RuntimeSettings + notify observers)
void updateNetworkSettings(const NetworkSettings&);
void updateMachineSettings(const MachineSettings&);
void updatePenSettings(const PenSettings&);

// Observer management
void addObserver(SettingsObserver* observer);
void removeObserver(SettingsObserver* observer);
```

**What happens when you call `updateMachineSettings()`:**

1. ✅ Validates the new settings (you implement this in the TODO)
2. ✅ Writes all fields to NVS namespace `"machine"` (flash persist)
3. ✅ Updates the in-RAM copy (`_machine`)
4. ✅ Updates `RuntimeSettings` (the atomic bridge to Core 1) so Core 1 sees the new values **immediately** on the next G-code command
5. ✅ Notifies all observers (e.g., `WebInterface` reconnects WiFi if credentials changed)

**NVS structure:**

Each settings group gets its own namespace:
- Namespace `"network"` contains: `"ssid"`, `"password"`, `"mdnsName"`
- Namespace `"machine"` contains: `"drawFeed"`, `"travelFeed"`, `"stepsPerMm"`, etc.
- Namespace `"pen"` contains: `"upAngle"`, `"downAngle"`

---

### 3. `RuntimeSettings.hpp` — Cross-Core Lock-Free Bridge

Lives in `systemServices/` because it's inter-core communication infrastructure (like `MotionState`).

```cpp
class RuntimeSettings {
public:
    // Setters (Core 0)
    void setDrawFeedRate(float v);
    void setTravelFeedRate(float v);
    void setPenUpAngle(float v);
    // ...

    // Getters (Core 1, called during G-code execution)
    float drawFeedRate() const;
    float travelFeedRate() const;
    float penUpAngle() const;
    // ...
};
```

**Key design point:** Uses `std::atomic<float>` with `memory_order_relaxed` — same pattern as `MotionState`. 

- **Core 0** (UI/Web) calls `setters` when the user saves a setting
- **Core 1** (Motion) calls `getters` in `MotionExecuter` / `GCodeExecuter` / `ServoPen` while executing G-code
- **No locks, no waiting**, zero latency for Core 1 to read new values

---

## How to Instantiate (in `Tasks.cpp`)

```cpp
void startSystemTasks() {
    // Existing:
    static FreeRtosQueue<GcodeMessage> gcodeQueue(32);
    static MotionState motionState;

    // NEW: Add this
    static RuntimeSettings runtimeSettings;
    static SettingsRepository settingsRepo(runtimeSettings);

    // Load settings from NVS at startup
    settingsRepo.init();

    // Pass both managers the references they need:
    static ApplicationManager appManager(
        gcodeQueue,
        motionState,
        runtimeSettings,  // NEW
        settingsRepo      // NEW
    );

    static PlottingManager plotManager(
        gcodeQueue,
        motionState,
        runtimeSettings   // NEW
    );

    // ... rest of task creation
}
```

---

## How to Use

### In UI Settings Screens

```cpp
// Inside a SettingsScreen subclass:

class MachineSettingsScreen : public Screen {
public:
    MachineSettingsScreen(Router* router, SettingsRepository& repo)
        : _repo(repo) {
        // Build UI widgets
        auto Button = std::make_unique<Button>(
            "Save",
            [this]() {
                MachineSettings updated{
                    20.5f,  // drawFeedRate
                    60.0f,  // travelFeedRate
                    // ... etc from UI text fields
                };
                _repo.updateMachineSettings(updated);  // Persist + notify + update Core 1
                router()->popScreen();
            }
        );
    }

private:
    SettingsRepository& _repo;
};
```

### In WebInterface

```cpp
class WebInterface : public SettingsObserver {
public:
    WebInterface(SettingsRepository& repo, /* ... */)
        : _repo(repo) {
        // Register to observe network settings changes
        repo.addObserver(this);
    }

    void onNetworkSettingsChanged(const NetworkSettings& newSettings) override {
        // WiFi credentials changed, reconnect with new SSID/password
        WiFi.disconnect();
        WiFi.begin(newSettings.ssid.c_str(), newSettings.password.c_str());
    }

    // REST API endpoint to change machine settings from web UI:
    void handlePostUpdateMachineSettings(const MachineSettings& received) {
        _repo.updateMachineSettings(received);  // Same method as UI screens!
    }

private:
    SettingsRepository& _repo;
};
```

### In Motion Execution (Core 1)

```cpp
// Inside GCodeExecuter or MotionExecuter on Core 1:

void MotionExecuter::executeLine(const GcodeMessage& cmd) {
    float speed = (cmd.isTravel) 
        ? _runtimeSettings.travelFeedRate()     // Read latest value
        : _runtimeSettings.drawFeedRate();

    // ^ No locks, atomic read from Core 0 (which updates it from SettingsRepository)
    
    float penUp = _runtimeSettings.penUpAngle();
    // Use it immediately
}
```

---

## Data Flow Diagram

```
WebInterface / UI Screen (Core 0)
    ↓
repo.updateMachineSettings(settings)
    ↓
SettingsRepository::updateMachineSettings()
    ├→ Write to NVS (flash, slow, but rare)
    ├→ Update _machine (RAM)
    ├→ _runtimeSettings.setDrawFeedRate(...)  (atomic store)
    ├→ notifyMachineSettingsChanged()
    │   └→ WebInterface::onMachineSettingsChanged() [if observing]
    │   └→ [Other observers...]
    └→ [DONE]
         ↓
    Core 1 thread (MotionExecuter) wakes up
         ↓
    _runtimeSettings.drawFeedRate()  (atomic load, sees new value)
         ↓
    [Immediately executes with NEW feed rate]

```

The key: **Instant effect on Core 1 because RuntimeSettings is a shared atomic, not a queue or message.**

---

## Next Steps

1. **Implement the TODO sections** in `SettingsRepository.cpp`:
   - `persistNetworkSettings()`, `loadNetworkSettings()`, etc.
   - Uncomment the `prefs.putFloat/putString/getFloat/getString` lines
   
2. **Update `machine_config.hpp`** to remove the final `constexpr` value, keep only as defaults in RuntimeSettings and Preferences fallback

3. **Wire up `ApplicationManager`**:
   - Accept `SettingsRepository&` and `RuntimeSettings&` in constructor
   - Create the repository with `new SettingsRepository(runtimeSettings)`
   - Call `repository.init()` in `ApplicationManager::init()`
   - Register `this` as an observer if WebInterface needs network settings changes
   - Pass `repository&` to UI screens

4. **Wire up `PlottingManager`**:
   - Accept `RuntimeSettings&` in constructor
   - Pass it to `MotionExecuter`, `GCodeExecuter`, `ServoPen` so they can read atomic values

5. **Implement the settings UI screens**:
   - `WifiSettingsScreen`, `MachineSettingsScreen`, etc.
   - Each accepts `SettingsRepository&` and calls `repo.update*()` on save

6. **Add REST endpoints** to `WebInterface` to change settings from the web UI

---

## Why This Design Scales

- **Add a new setting?** Add it to the struct, add one line to the setter/getter in SettingsRepository, add one line to RuntimeSettings if Core 1 needs it.
- **Add a new settings group?** Copy the pattern: create `struct XyzSettings`, add getter/setter/persist/load/notify triplets, done.
- **Settings source (UI vs Web)?** Both call the same `repo.update*()` method — unified code path, no duplication.
- **Settings effect timing?** Immediate on Core 1 (atomic read), while Core 0 gets observer notifications for anything that needs to react (WiFi reconnect, etc.).

---

## Safety Notes

- **std::atomic<float>** is safe but worth noting: floating-point atomics are not lock-free on all architectures. On ESP32, they are (using an internal lock, but Preferences already uses locks so not a concern). If you want zero locks, use `std::atomic<uint32_t>` and reinterpret_cast (more complex but zero overhead).
  
- **NVS operations** are slow. Only happen when user explicitly saves a setting, not in motion loop.

- **Observer notifications** happen on Core 0 inside `SettingsRepository::update*()`, so observers must be fast (no I/O, no long computations). WebInterface's `onNetworkSettingsChanged()` just calls `WiFi.begin()` which is itself non-blocking.

---
