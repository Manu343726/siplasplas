#ifndef SIPLASPLAS_JOBS_ENGINE_HPP
#define SIPLASPLAS_JOBS_ENGINE_HPP

#include "worker.hpp"
#include <siplasplas/constexpr/arrayview.hpp>

namespace cpp
{

namespace jobs
{

class SIPLASPLAS_JOBS_EXPORT Engine
{
public:
    Engine(std::size_t workerThreads, std::size_t jobsPerThread);
    Engine(const Engine&) = delete;

    Worker* randomWorker();
    Worker* threadWorker();

    cpp::constexp::ConstArrayView<std::unique_ptr<Worker>> workers() const;

private:
    std::vector<std::unique_ptr<Worker>> _workers;
};

}

}

#endif // SIPLASPLAS_JOBS_ENGINE_HPP
