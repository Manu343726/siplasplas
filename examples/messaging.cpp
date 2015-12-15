#include "variant/variant.hpp"

struct TransformMatrix
{
    float values[4][4];
};

using Arg = cpp::Variant<std::string, TransformMatrix, float>; 

template<std::size_t Arity>
class Message
{
public:
    template<typename... Ts>
    Message(Ts&&... args) :
        _args{std::forward<Ts>(args)...}
    {}

    template<typename Visitor>
    void process(Visitor&& visitor) const
    {
        // TODO: Multivisitor here over all args
        _args[0].template visit<void>(std::forward<Visitor>(visitor));
    }

private:
    std::array<Arg, Arity> _args;
};

template<std::size_t Arity, typename T>
struct MessageHandler
{
    template<typename...> class list {};
    template<typename...> using void_t = void;

    // processor class is a dispatcher to the T::process() function.
    // It just asks "Has T a process() overload to handle this event?"
    // If the answer is "yes", calls it, else does nothing.
    template<typename U, typename Args, typename = void>
    struct processor;

    template<typename U, typename... Args>
    struct processor<U, list<Args...>, void_t<decltype(std::declval<U>().process(std::declval<std::decay_t<Args>>()...))>>
    {
        static auto apply(U* This, Args&&... args)
        {
            This->process(std::forward<Args>(args)...);          
        }
    };

    template<typename U, typename... Args, typename _void>
    struct processor<U, list<Args...>, _void>
    {
        // Note a call to an empty function means no call at runtime
        static void apply(U* This, Args&&... args)
        {
            // nop
        }
    };

    template<typename... Args>
    void apply_processor(Args&&... args) const
    {
        processor<const T, list<Args...>>::apply(static_cast<const T*>(this),
                                           std::forward<Args>(args)...);
    }

    void receive(const Message<Arity>& message) const
    {
        message.process(
            [this](auto&&... args)
            {
                this->apply_processor(std::forward<decltype(args)>(args)...);
            }
        ); 
    }
};

// Google search "CRTP C++" ;)
struct MyClass : public MessageHandler<1, MyClass>
{
    void process(const std::string& str) const
    {
        std::cout << "Received a string message!" << std::endl;
    }

    void process(const TransformMatrix& matrix) const
    {
        std::cout << "Received a matrix message!" << std::endl;
    }
};

using namespace std::string_literals;

int main()
{
    using Message = Message<1>;

    TransformMatrix matrix;

    matrix.values[0][0] = 1; // ...

    Message hello{"hola caracola"s};
    Message number{10.0f};
    Message transform{matrix};

    MyClass myObject;

    myObject.receive(hello);
    myObject.receive(number);
    myObject.receive(transform);
}
