#ifndef SIPLASPLAS_JOBS_JOB
#define SIPLASPLAS_JOBS_JOB

#include <siplasplas/jobs/export.hpp>
#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/utility/destroy.hpp>
#include <atomic>
#include <type_traits>
#include <cstdint>
#include <cstring>

namespace cpp
{

namespace jobs
{

class Job;

/**
 * \ingroup jobs
 * \brief Function to be executed by a job
 *
 * \param job Reference to the job the function belongs to
 */
using JobFunction = void(*)(Job& job);

class SIPLASPLAS_JOBS_EXPORT Job
{
public:
    Job() = default;
    Job(JobFunction jobFunction, Job* parent = nullptr);

    template<typename Data>
    Job(JobFunction jobFunction, const Data& data, Job* parent = nullptr) :
        Job{jobFunction, parent}
    {
        setData(data);
    }

    void run();
    bool finished() const;
    Job* parent() const;
    void discard();

private:
    JobFunction _function;
    Job* _parent;
    std::atomic<std::uint32_t> _unfinishedChildrenJobs;

    static constexpr const std::size_t JOB_PAYLOAD_SIZE = sizeof(_function) + sizeof(_parent) + sizeof(_unfinishedChildrenJobs);
    static constexpr const std::size_t JOB_PADDING_SIZE = cpp::cachelineBytes() - JOB_PAYLOAD_SIZE;
    static_assert(JOB_PAYLOAD_SIZE < cpp::cachelineBytes(), "Job payload does not fit in a cache line");

    char _padding[JOB_PADDING_SIZE];

    void finish();

    template<typename Data>
    std::enable_if_t<std::is_pod<Data>::value && sizeof(Data) <= JOB_PADDING_SIZE>
    setData(const Data& data)
    {
        std::memcpy(&_padding[0], &data, sizeof(Data));
    }

public:
    template<typename Data>
    const Data& getData() const
    {
        return *reinterpret_cast<const Data*>(data());
    }

    template<typename Data>
    Data& getData()
    {
        return *reinterpret_cast<Data*>(data());
    }

    void* data();
    const void* data() const;

    static constexpr std::size_t maxDataSize()
    {
        return JOB_PAYLOAD_SIZE;
    }
};

static_assert(std::is_pod<Job>::value, "Job type must be a POD");

}

}

#endif // SIPLASPLAS_JOBS_JOB
