#include <siplasplas/jobs/job.hpp>
#include "logger.hpp"

using namespace cpp::jobs;

Job::Job(JobFunction jobFunction, Job* parent) :
    _function{jobFunction},
    _parent{parent},
    _unfinishedChildrenJobs{1}
{
    if(_parent != nullptr)
    {
        _parent->_unfinishedChildrenJobs++;
    }
}

void Job::run()
{
    _function(*this);
    finish();
}

bool Job::finished() const
{
    return _unfinishedChildrenJobs == 0;
}

Job* Job::parent() const
{
    return _parent;
}

void Job::finish()
{
    _unfinishedChildrenJobs--;

    if(finished() && _parent != nullptr)
    {
        _parent->finish();
    }
}

void Job::discard()
{
    finish();
}

void* Job::data()
{
    return &_padding[0];
}
