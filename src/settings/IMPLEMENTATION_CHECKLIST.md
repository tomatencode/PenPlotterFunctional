# Implementation Checklist

This document outlines which files need modifications to integrate the settings architecture.

## Phase 1: Fill in the TODOs (Complete the `SettingsRepository` implementation)

- [ ] **`SettingsRepository.cpp`** — Uncomment and implement the NVS code:
  - `persistNetworkSettings()` — uncomment the `prefs.put*` lines
  - `persistMachineSettings()` — uncomment the `prefs.put*` lines
  - `persistPenSettings()` — uncomment the `prefs.put*` lines
  - `loadNetworkSettings()` — uncomment the `prefs.get*` lines
  - `loadMachineSettings()` — uncomment the `prefs.get*` lines
  - `loadPenSettings()` — uncomment the `prefs.get*` lines
  - Add validation in each `update*()` method if needed

---

## Phase 2: Wire Up the Core System (`Tasks.cpp` and `ApplicationManager`)

### `src/systemServices/Tasks.cpp`

- [ ] Include `<settings/SettingsRepository.hpp>`
- [ ] Include `<systemServices/RuntimeSettings.hpp>`
- [ ] In `startSystemTasks()`:
  - [ ] Create static `RuntimeSettings runtimeSettings;`
  - [ ] Create static `SettingsRepository settingsRepo(runtimeSettings);`
  - [ ] Call `settingsRepo.init();` to load from NVS at startup
  - [ ] Pass both `runtimeSettings` and `settingsRepo` to `ApplicationManager` constructor
  - [ ] Pass `runtimeSettings` to `PlottingManager` constructor

### `src/systemServices/applicationManager/ApplicationManager.hpp`

- [ ] Add `RuntimeSettings&` member variable
- [ ] Add `SettingsRepository&` member variable
- [ ] Add both as constructor parameters

### `src/systemServices/applicationManager/ApplicationManager.cpp`

- [ ] Update constructor to store the references
- [ ] In `init()`: Call `_settingsRepository.init()` (will be called twice but idempotent)
- [ ] Pass `_settingsRepository` to `UiOrchestrator` constructor (so screens can access it)
- [ ] Pass `_settingsRepository` to `WebInterface` constructor
- [ ] Register `WebInterface` as observer: `_settingsRepository.addObserver(_webInterface);`

### `src/systemServices/plottingManager/PlottingManager.hpp`

- [ ] Add `RuntimeSettings&` member variable
- [ ] Add it as constructor parameter

### `src/systemServices/plottingManager/PlottingManager.cpp`

- [ ] Update constructor to store the reference
- [ ] Pass `_runtimeSettings` to constructors of:
  - [ ] `MotionExecuter` (needs to read drawing/travel feed rates)
  - [ ] `GCodeExecuter` (may need pen angles)
  - [ ] `ServoPen` (needs pen up/down angles)

---

## Phase 3: Wire Up Motion Hardware (Core 1 reading RuntimeSettings)

### `src/motion/MotionExecuter.hpp`

- [ ] Add `RuntimeSettings&` constructor parameter
- [ ] Store as member variable

### `src/motion/MotionExecuter.cpp`

- [ ] Replace hardcoded `FEED_RATE_DRAW_MM_PER_S` with `_runtimeSettings.drawFeedRate()`
- [ ] Replace hardcoded `FEED_RATE_TRAVEL_MM_PER_S` with `_runtimeSettings.travelFeedRate()`
- [ ] Replace hardcoded `STEPS_PER_MM` with `_runtimeSettings.stepsPerMm()`

### `src/gcode/GCodeExecuter.hpp`

- [ ] Add `RuntimeSettings&` constructor parameter (if pen commands are here)

### `src/gcode/GCodeExecuter.cpp`

- [ ] If it handles pen up/down, use `_runtimeSettings.penUpAngle()` and `.penDownAngle()`

### `src/hardware/pen/ServoPen.hpp`

- [ ] Add `RuntimeSettings&` constructor parameter

### `src/hardware/pen/ServoPen.cpp`

- [ ] Replace hardcoded `PEN_UP_ANGLE` with `_runtimeSettings.penUpAngle()`
- [ ] Replace hardcoded `PEN_DOWN_ANGLE` with `_runtimeSettings.penDownAngle()`

### `src/motion/HomingController.hpp` *(if homing settings should be dynamic)*

- [ ] Add `RuntimeSettings&` constructor parameter

### `src/motion/HomingController.cpp`

- [ ] Replace `HOMING_SPEED_STPS_PER_S` with `_runtimeSettings.homingSpeed()`
- [ ] Replace `HOMING_STALLGUARD_THRESHOLD` with `_runtimeSettings.stallguardThreshold()`

---

## Phase 4: Wire Up the UI

### `src/ui/UiOrchestrator.hpp`

- [ ] Add `SettingsRepository&` constructor parameter
- [ ] Store as member variable

### `src/ui/UiOrchestrator.cpp`

- [ ] Pass `_settingsRepository` to each settings screen constructor

### Create Settings Screens

Create each stub screen as a real editable form. Template:

```cpp
// src/ui/screens/settings/WifiSettingsScreen.hpp
class WifiSettingsScreen : public Screen {
public:
    WifiSettingsScreen(Router* router, SettingsRepository& settingsRepo) 
        : Screen(router), _settingsRepository(settingsRepo) {
        
        auto current = _settingsRepository.getNetworkSettings();
        
        auto layout = std::make_unique<LinearLayout>(LinearLayout::Direction::Vertical);
        
        layout->addChild(std::make_unique<HeaderLine>("WiFi Settings", HeaderLine::HasBack));
        
        // Add text input widgets for SSID, password
        // Add a Save button that calls:
        //   _settingsRepository.updateNetworkSettings({newSsid, newPassword, ...});
        
        setRootWidget(std::move(layout));
    }

private:
    SettingsRepository& _settingsRepository;
};
```

- [ ] **`WifiSettingsScreen.hpp`** — text fields for SSID, password, mDNS name + Save button
- [ ] **`MachineSettingsScreen.hpp`** — text/number fields for feed rates, steps/mm + Save button
- [ ] **`PenSettingsScreen.hpp`** — text/number fields for pen angles + Save button
- [ ] **`HomingSettingsScreen.hpp`** — text/number fields for homing speed, stallguard threshold + Save button

### `src/ui/screens/settings/SettingsScreen.hpp`

- [ ] Modify to pass `_settingsRepository` when creating sub-screens

---

## Phase 5: Wire Up Web Interface

### `src/webInterface/WebInterface.hpp`

- [ ] Inherit from `SettingsObserver`
- [ ] Add `SettingsRepository&` constructor parameter
- [ ] Store as member variable

### `src/webInterface/WebInterface.cpp`

- [ ] Implement `onNetworkSettingsChanged()` override:
  ```cpp
  void WebInterface::onNetworkSettingsChanged(const NetworkSettings& newSettings) {
      // Disconnect and reconnect with new credentials
      WiFi.disconnect();
      WiFi.begin(newSettings.ssid.c_str(), newSettings.password.c_str());
      // Optionally restart mDNS with new name
  }
  ```

### `src/webInterface/WebInterfaceHandlers.cpp`

- [ ] Add new POST handlers for updating settings:
  - `/api/network` — receive and update network settings
  - `/api/machine` — receive and update machine settings
  - `/api/pen` — receive and update pen settings
  - Example:
    ```cpp
    void WebInterface::handlePostUpdateMachine() {
        // Parse JSON body
        MachineSettings s = parseJson(server.arg("plain"));
        _settingsRepository.updateMachineSettings(s);
        server.send(200, "application/json", "{\"success\":true}");
    }
    ```

---

## Phase 6: Clean Up Config Headers

### `src/config/machine_config.hpp`

- [ ] Change from `constexpr float FEED_RATE_DRAW_MM_PER_S = 20.0f;` 
- [ ] To comment + documentation of intended defaults:
  ```cpp
  // Default feed rates (if not saved in NVS)
  // These values are hardcoded in RuntimeSettings and used as Preferences fallback
  constexpr float DEFAULT_FEED_RATE_DRAW_MM_PER_S = 20.0f;
  constexpr float DEFAULT_FEED_RATE_TRAVEL_MM_PER_S = 50.0f;
  ```
  (Or leave them as-is; they're now just defaults, not the active values)

### `src/config/wifi_config.hpp`

- [ ] Move SSID/PASSWORD to just be NVS defaults
- [ ] Add comment warning about plaintext hardcoding (consider moving to a `.env` or web provisioning at startup)

---

## Done!

Once all phases are complete:
- ✅ Settings persist across reboots (NVS)
- ✅ Settings take effect immediately on Core 1 (atomic RuntimeSettings bridge)
- ✅ Settings can be changed from UI screens (SettingsRepository API)
- ✅ Settings can be changed from web interface (REST endpoints + SettingsRepository)
- ✅ WebInterface reacts to network credential changes (observer pattern)
- ✅ No duplication between UI and web code paths (both use same `repo.update*()` methods)
