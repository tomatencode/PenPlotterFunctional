#pragma once

enum class JobEvent {
    STARTED = 0,
    PAUSED = 1,
    RESUMED = 2,
    ABORTED = 3,
    COMPLETED = 4
};

class JobObserver {
public:
    virtual ~JobObserver() = default;
    
    virtual void onJobEvent(const JobEvent& event) = 0;
};
