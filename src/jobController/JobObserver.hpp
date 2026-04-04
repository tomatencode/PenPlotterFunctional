#pragma once
#include <string>

enum class JobEvent {
    STARTED = 0,
    PAUSED = 1,
    RESUMED = 2,
    ABORTED = 3,
    COMPLETED = 4
};

struct JobEventType {
    JobEvent type;
    std::string filename;
};

// not ThreadSafe - only to be used on same core as JobController
class JobObserver {
public:
    virtual ~JobObserver() = default;
    
    virtual void onJobEvent(const JobEventType& event) = 0;
};
