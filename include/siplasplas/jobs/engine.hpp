#ifndef SIPLASPLAS_JOBS_ENGINE_HPP
#define SIPLASPLAS_JOBS_ENGINE_HPP

#include "worker.hpp"
#include <siplasplas/constexpr/arrayview.hpp>
#include <random>

namespace cpp
{

namespace jobs
{

/**
 * \ingroup jobs
 * \brief Manages a set of workers where jobs can be submitted to
 */
class SIPLASPLAS_JOBS_EXPORT Engine
{
public:
    /**
     * \brief Initializes an engine with /p workerThreads workers and
     * a maximum of \p jobsPerThread jobs per worker
     *
     * \param workerThreads The number of workers this engine will manage.
     * One of these workers will be run in the caller thread, which means only
     * n-1 workers will have background threads polling for work to do.
     * See Worker::wait().
     * \param jobsPerThread Maximum number of jobs that can be allocated by
     * thread. Once this limit is reached, the worker stops returning storage
     * for more jobs, which means no more jobs can be submitted to the worker.
     */
    Engine(std::size_t workerThreads, std::size_t jobsPerThread);
    Engine(const Engine&) = delete;

    /**
     * \brief Returns one of the workers, randomnly picked from all
     * the available workers in the engine
     *
     * Note allocating and submitting jobs from a thread different from
     * the worker associated to the caller thread has undefined behavior.
     * See `threadWorker()`.
     */
    Worker* randomWorker();

    /**
     * \brief Returns the worker associated to this thread.
     *
     * \returns A pointer to the worker which is using the caller thread to
     * process work. Nullptr if there is no worker associated to the
     * caller thread
     */
    Worker* threadWorker();

    /**
     * \brief Returns a view to the set of workers, which can be used to gather
     * statistics
     */
    cpp::constexp::ConstArrayView<std::unique_ptr<Worker>> workers() const;

private:
    std::vector<std::unique_ptr<Worker>> _workers;
    std::default_random_engine _randomEngine;
    std::uniform_int_distribution<std::size_t> _dist;
};

}

}

#endif // SIPLASPLAS_JOBS_ENGINE_HPP
