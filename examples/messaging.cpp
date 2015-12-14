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
        _args[0].template visit<void>(visitor);
    }

private:
    std::array<Arg, Arity> _args;
};

template<std::size_t Arity, typename T>
struct MessageHandler
{
    void receive(const Message<Arity>& message) const
    {
        message.process([this](const auto&... args)
        {
            // Calls process() of your class (T)
            static_cast<const T*>(this)->process(args...);
        });
    }
};

struct MyClass : public MessageHandler<1, MyClass>
{
    void process(const std::string& str) const
    {
        std::cout << "Received a string message!" << std::endl;
    }

    void process(float number) const
    {
        std::cout << "Received a float message!" << std::endl;
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
