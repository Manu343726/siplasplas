/**
 * \defgroup jobs
 * \brief A job-stealing system for simple fork-join parallelism
 *
 * siplasplas-jobs implements a simple job system to provide task parallelization
 * in the form of fork-join parallelism.
 *
 * The main entity of the system is Engine, a class that manages a set of N-1 background
 * threads where work can be submitted to. If the engine is configured to work with N
 * threads, the engine instantiated N-1 background workers and one worker associated with
 * the engine thread (The thread that instanced and started the engine).
 * Each worker has an associated fixed-size pool of jobs, which means a limited amound of M
 * jobs can be submitted per worker only. Users may allocate jobs from the worker associated
 * to the current thread and submit them to the worker (Note the allocation and submission
 * of jobs are two isolated steps).
 *
 * Once all the work is done, users are free to clear all the worker pools by calling
 * `Engine::clear()`, so the engine can be reused again with all its resources available.
 */
