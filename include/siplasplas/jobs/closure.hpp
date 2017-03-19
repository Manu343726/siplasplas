#ifndef SIPLASPLAS_JOBS_CLOSURE_HPP
#define SIPLASPLAS_JOBS_CLOSURE_HPP

#include "job.hpp"
#include <siplasplas/utility/memberfunctor.hpp>
#include <siplasplas/utility/destroy.hpp>
#include <siplasplas/utility/staticif.hpp>

namespace cpp
{

namespace jobs
{

template<typename Function>
class Closure : public cpp::MemberFunctor<Function>
{
public:
    using cpp::MemberFunctor<Function>::MemberFunctor;

    template<typename... Args>
    void run(Job& job, Args&&... args)
    {
        cpp::MemberFunctor<Function>::invoke(job, std::forward<Args>(args)...);
    }
};

template<typename Function>
Job* closure(Job* job, Function function, Job* parent = nullptr)
{
    if(job == nullptr)
    {
        return nullptr;
    }

    // What this function does is to inject a non-pod payload
    // into a previously allocated job. Since jobs are PODs, we must
    // manually destroy the payload after running the job. This is safe
    // as long as there are no other C++ semantics involved (copy, move,
    // etc) which is true for our preallocated array of POD jobs

    auto jobFunction = [](Job& job)
    {
        cpp::staticIf<sizeof(Closure<Function>) <= Job::maxDataSize()>([&job](auto)
        {
            auto& closure = job.getData<Closure<Function>>();
            closure.run(job);

            // Install a finished callback to destroy the closure
            // when the job is marked as finished. This allows any child job
            // to capture references to the parent closure
            job.whenFinished([](Job& job)
            {
                cpp::destroy(job.getData<Closure<Function>>());
            });
        }).Else([&job](auto)
        {
            auto& closure = job.getData<std::unique_ptr<Closure<Function>>>();
            closure->run(job);

            // Install a finished callback to destroy the closure
            // when the job is marked as finished. This allows any child job
            // to capture references to the parent closure
            job.whenFinished([](Job& job)
            {
                cpp::destroy(job.getData<std::unique_ptr<Closure<Function>>>());
            });
        });
    };

    // Initialize the allocated job:
    cpp::construct<Job>(job, jobFunction, parent);

    cpp::staticIf<sizeof(Closure<Function>) <= Job::maxDataSize()>([&job, function](auto)
    {
        // Construct the closure in the job payload:
        job->constructData<Closure<Function>>(function);
    }).Else([&job, function](auto)
    {
        // The closure object does not fit in the job payload,
        // dynamically allocate it:
        job->constructData<std::unique_ptr<Closure<Function>>>(std::make_unique<Closure<Function>>(function));
    });

    return job;
}

}

}

#endif // SIPLASPLAS_JOBS_CLOSURE_HPP
