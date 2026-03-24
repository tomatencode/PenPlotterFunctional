#pragma once
#include "JobStatusUpdate.hpp"

class JobObserver {
public:
    virtual ~JobObserver() = default;
    
    /**
     * Called when a job event occurs (start, pause, resume, abort, complete)
     * @param update Contains event type and job details
     */
    virtual void onJobEvent(const JobStatusUpdate& update) = 0;
};
