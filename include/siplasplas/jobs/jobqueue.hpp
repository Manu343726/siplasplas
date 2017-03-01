#ifndef SIPLASPLAS_JOBS_JOBQUEUE_HPP
#define SIPLASPLAS_JOBS_JOBQUEUE_HPP

#include "job.hpp"
#include <mutex>
#include <vector>

namespace cpp
{

namespace jobs
{

class JobQueue
{
public:
    JobQueue(std::size_t maxJobs);

    bool push(Job* job);
    Job* pop();
    Job* steal();
    std::size_t size() const;
    bool empty() const;

private:
    mutable std::mutex _mutex;
    std::vector<Job*> _jobs;
    std::atomic<int> _top, _bottom;

    int unsafeSize() const;
};

}

}

#endif // SIPLASPLAS_JOBS_JOBQUEUE_HPP
