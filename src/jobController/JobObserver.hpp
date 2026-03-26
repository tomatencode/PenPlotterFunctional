#pragma once

enum class JobEvent {
    STARTED = 0,
    PAUSED = 1,
    RESUMED = 2,
    ABORTED = 3,
    COMPLETED = 4
};

struct JobEventType {
    JobEvent type;
    String filename;
};

class JobObserver {
public:
    virtual ~JobObserver() = default;
    
    virtual void onJobEvent(const JobEventType& event) = 0;
};
