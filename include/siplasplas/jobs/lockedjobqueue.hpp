#ifndef SIPLASPLAS_JOBS_LOCKEDJOBQUEUE_HPP
#define SIPLASPLAS_JOBS_LOCKEDJOBQUEUE_HPP

#include "job.hpp"
#include <mutex>
#include <vector>

namespace cpp
{

namespace jobs
{

class SIPLASPLAS_JOBS_EXPORT LockedJobQueue
{
public:
    LockedJobQueue(std::size_t maxJobs);

    bool push(Job* job);
    Job* pop();
    Job* steal();
    std::size_t size() const;
    bool empty() const;

private:
    mutable std::mutex _lock;
    std::vector<Job*> _jobs;
    int _top, _bottom;
};

}

}

#endif // SIPLASPLAS_JOBS_LOCKEDJOBQUEUE_HPP
