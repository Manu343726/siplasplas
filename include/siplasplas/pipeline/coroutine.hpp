#ifndef SIPLASPLAS_PIPELINE_COROUTINE_HPP
#define SIPLASPLAS_PIPELINE_COROUTINE_HPP

#include <boost/lockfree/queue.hpp>
#include <boost/coroutine2/all.hpp>
#include <memory>

namespace cpp
{

namespace pipeline
{
    namespace types
    {
        template<typename T>
        using Coro = boost::coroutines2::coroutine<T>;
        template<typename T>
        using Pull = typename Coro<T>::pull_type;
        template<typename T>
        using Push = typename Coro<T>::push_type;

        template<typename T>
        using Queue = boost::lockfree::queue<T>;
    }

    template<typename T>
    class CoroutineIO
    {
    public:
        std::size_t size() const
        {
            return _queue.size();
        }

        class input_iterator
        {
        public:
            T operator*()
            {
                return _queue.pop();
            }

            input_iterator& operator++()
            {
                return *this;
            }
            input_iterator operator++(int)
            {
                return *this;
            }

            friend bool operator==(const input_iterator& lhs, const input_iterator& rhs)
            {
                return lhs._queue == rhs._queue && lhs._queue->empty();
            }

            friend bool operator!=(const input_iterator& lhs, const input_iterator& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            Queue<T>* _queue;
        };
    private:
        Queue<T> _queue;
    };

    template<typename Input, typename Output>
    class Coroutine
    {
    public:
        using coro_t = boost::coroutines2::coroutine<Input>;

        template<typename Function>
        Coroutine(Function function) :
            _coro{function}
        {}

        void attach_in(const std::shared_ptr<Queue<Input>>& queue)
        {
            _input_queue = queue;
        }

        void attach_out(const std::shared_ptr<Queue<Output>>& queue)
        {
            _output_queue = queue;
        }

        const std::shared_ptr<Input>& in() const
        {
            return _input_queue;
        }

        const std::shared_ptr<Output>& out() const
        {
            return _output_queue;
        }

    private:
        Pull<Output> _coro;
        std::shared_ptr<Queue<Input>> _input_queue;
        std::shared_ptr<Queue<Output>> _output_queue;
    };
}

}

#endif // SIPLASPLAS_PIPELINE_COROUTINE_HPP
