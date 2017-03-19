#ifndef SIPLASPLAS_JOBS_JOB
#define SIPLASPLAS_JOBS_JOB

#include <siplasplas/jobs/export.hpp>
#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/utility/destroy.hpp>
#include <ctti/type_id.hpp>
#include <atomic>
#include <type_traits>
#include <cstdint>
#include <cstring>
#include <thread>

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

/**
 * \brief Represents a unit of work to be executed by the system
 *
 * Work is submitted to the system in the form of Jobs. A Job basically
 * stores a pointer to the function implementing the work. When the job
 * is scheduled the function will be invoked in the thread of the worker
 * that picked the job. The job is considered done if its function has been executed
 * and **all its child jobs are done too** (See `Job::finished()`).
 *
 * Users can wait for the completion of a job by calling `Engine::wait()`.
 *
 * Finally, user defined POD data can be associated to the job during its construction,
 * which is available later by calling `Job::getData()`. For non-POD data, see `jobs::closure()`.
 */
class SIPLASPLAS_JOBS_EXPORT Job
{
public:
    /**
     * \brief Default constructs a job. Used by pool pre-allocation only.
     */
    Job() = default;

    /**
     * \brief Initializes a job given the function to execute and a
     * parent job
     *
     * \param jobFunction Function to be executed when the job is processed. Must be
     * a pointer to function of signature `void(*)(Job&)`. See `JobFunction`.
     * \param parent Parent job, if any. When a job is associated to an existing parent
     * one, the parent will not be considered finished until the child job is finished first.
     * This allows to implement fork-join by simply associating multiple jobs to a parent job,
     * and waiting for the finalization of the parent. If nullptr the job has no parent job.
     */
    Job(JobFunction jobFunction, Job* parent = nullptr);

    /**
     * \brief Initializes a job given a function to execute, data associated with the job, and
     * a parent job
     *
     * \tparam Data Must be a POD type
     * \param jobFunction Function to be executed when the job is processed
     * \param data Data associated with the job. The data is memcpy-ed into the
     * job, and can be accessed later using `Job::getData()`
     * \param parent Parent job. If nullptr the ob has no associated parent job
     */
    template<typename Data>
    Job(JobFunction jobFunction, const Data& data, Job* parent = nullptr) :
        Job{jobFunction, parent}
    {
        setData(data);
    }

    /**
     * \brief Executes the job function in the caller thread.
     *
     * Invokes the job function from the caller thread, passing itself as argument
     * for the job function. After executing the function, tries to mark the job as finished.
     */
    void run();

    /**
     * \brief Checks whether the job and all its child jobs have been run
     */
    bool finished() const;

    /**
     * \brief Returns the number of children unfinished jobs left
     */
    std::uint32_t unfinishedChildrenJobs() const;

    /**
     * \brief Returns the parent of the job
     *
     * \returns A pointer to the parent job, nullptr if the job has no parent
     */
    Job* parent() const;

    /**
     * \brief Returns the function associated with the job
     */
    JobFunction function() const;

    /**
     * \brief Discards the job, marking it as processed even if the job function
     * has not been executed.
     */
    void discard();

    /**
     * \brief Changes the job function so an action is executed when the
     * job is marked as finished. Invoking this method outside the job function
     * has undefined behavior.
     *
     * \param jobFunction Function that will be invoked when the job finishes
     */
    void whenFinished(JobFunction jobFunction);

private:
    struct Payload
    {
        JobFunction function;
        Job* parent;
        std::atomic<std::uint32_t> unfinishedChildrenJobs;
    };

    Payload _payload;

    static constexpr const std::size_t JOB_PAYLOAD_SIZE = sizeof(Payload);
    static constexpr const std::size_t JOB_MAX_PADDING_SIZE = cpp::cachelineBytes() * 2;
    static constexpr const std::size_t JOB_PADDING_SIZE = JOB_MAX_PADDING_SIZE - JOB_PAYLOAD_SIZE;
    static_assert(JOB_PAYLOAD_SIZE < JOB_MAX_PADDING_SIZE, "Job payload does not fit in a cache line");

    char _padding[JOB_PADDING_SIZE];

    void finish();
    void incrementUnfinishedChildrenJobs();
    bool decrementUnfinishedChildrenJobs();

    template<typename Data>
    std::enable_if_t<std::is_pod<Data>::value && sizeof(Data) <= JOB_PADDING_SIZE>
    setData(const Data& data)
    {
        std::memcpy(&_padding[0], &data, sizeof(Data));
    }

public:
    /**
     * \brief Returns a const reference to the data associated to the job
     *
     * \tparam Data Must be the same type passed to the job constructor. The behavior
     * is undefined if this type is different.
     *
     * The behavior is undefined if this function is invoked on jobs with no
     * data associated
     */
    template<typename Data>
    const Data& getData() const
    {
        return *reinterpret_cast<const Data*>(data());
    }

    /**
     * \brief Returns a non-const reference to the data associated to the job
     *
     * \tparam Data Must be the same type passed to the job constructor. The behavior
     * is undefined if this type is different.
     *
     * The behavior is undefined if this function is invoked on jobs with no
     * data associated
     */
    template<typename Data>
    Data& getData()
    {
        return *reinterpret_cast<Data*>(data());
    }

    /**
     * \brief Returns the storage address of the data associated with the job
     */
    void* data();

    /**
     * \brief Returns the storage address of the data associated with the job
     */
    const void* data() const;

    /**
     * \brief Constructs an object in the data storage of the job
     *
     * This function constructs in place an object of the given type
     * at the beginning of the job data storage. Users must manually
     * invoke object destructor later.
     *
     * \tparam T Type of the object that will be constructed
     * \param args Constructor arguments
     */
    template<typename T, typename... Args>
    void constructData(Args&&... args)
    {
        static_assert(sizeof(T) <= JOB_MAX_PADDING_SIZE, "Objects of that type do not fit in "
            "the job data storage");

        cpp::construct<T>(data(), std::forward<Args>(args)...);
    }

    /**
     * \brief Returns the maximum size of the data that can be associated with a job
     */
    static constexpr std::size_t maxDataSize()
    {
        return JOB_PAYLOAD_SIZE;
    }
};

static_assert(std::is_pod<Job>::value, "Job type must be a POD");

std::ostream& operator<<(std::ostream& os, const Job& job);

}

}

#endif // SIPLASPLAS_JOBS_JOB
