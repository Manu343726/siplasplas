#ifndef SIPLASPLAS_PIPELINE_H
#define SIPLASPLAS_PIPELINE_H

#include <boost/coroutine2/all.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <memory>

namespace cpp
{

namespace pipeline
{
    template<typename T>
    using Coro = boost::coroutines2::coroutine<T>;
    template<typename T>
    using Pull = typename Coro<T>::pull_type;
    template<typename T>
    using Push = typename Coro<T>::push_type;

    template<typename T, typename Return = T>
    struct StageIO
    {
        using Pull = typename Coro<T>::pull_type;
        using Push = typename Coro<Return>::push_type;

        StageIO(Pull& source, Push& sink) :
            _source{ &source },
            _sink{ &sink }
        {}

        Pull& source()
        {
            return *_source;
        }

        Push& sink()
        {
            return *_sink;
        }

        void operator()(const Return& value)
        {
            sink()(value);
        }

        friend StageIO& operator<<(StageIO& io, const Return& value)
        {
            io.sink()(value);
            return *this;
        }

        typename Pull::iterator begin()
        {
            return boost::coroutines2::detail::begin(source());
        }

        typename Pull::iterator end()
        {
            return boost::coroutines2::detail::end(source());
        }
    private:
        Pull* _source;
        Push* _sink;
    };

    template<typename T, typename Return = T>
    class Stage
    {
    public:
        using IO = StageIO<T, Return>;
        using BodySignature = void(IO&);
        using Body = std::function<BodySignature>;

        template<typename Source>
        Stage(Source source, Body body) :
            _source{ std::make_shared<Pull<T>>(source) },
            _current{ std::make_shared<Pull<Return>>(
        [&,this](Push<Return>& sink)
        {
            body(IO{*_source, sink});
        }) }
        {}

        template<typename _T, typename _Return>
        Stage(Stage<_T,_Return>& previous, Body body) :
            _source{previous._current},
           _current{ std::make_shared<Pull<Return>>(
        [&,this](Push<Return>& sink)
        {
            body(IO{*_source, sink});
        }) }
        {} 

        typename Pull<Return>::iterator begin()
        {
            return boost::coroutines2::detail::begin(*_current);
        }

        typename Pull<Return>::iterator end()
        {
            return boost::coroutines2::detail::end(*_current);
        }

    private:
        std::shared_ptr<Pull<T>> _source;
        std::shared_ptr<Pull<Return>> _current;
    };

    template<typename Source, typename Current, typename... Tail>
    class Pipeline
    {
    public:
        template<typename 
        Pipeline()
    private:
        Source _source;
        // todo: concurrent queue here
        Current _current;

        Pipeline<Current, Tail...> _next;
    };

    template<typename T>
    auto echo(const T& range)
    {
        return [=](Push<T>& sink)
        {
            for (const auto& e : range)
                sink(e);
        };
    }

    template<typename T>
    auto echo(const std::initializer_list<T>& range)
    {
        return [=](Push<T>& sink)
        {
            for (const auto& e : range)
                sink(e);
        };
    }
}

}

#endif // SIPLASPLAS_PIPELINE_H