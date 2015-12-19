
#include "messaging/messaging.hpp"

using namespace ::cpp::examples;

struct TransformMatrix
{
    float values[4][4];
};

namespace appmsgs
{
    using ActionMessage = Message<1, float, std::string, TransformMatrix>;
    using LoggingMessage = Message<2, std::string, int>;
}

// Google search "CRTP C++" ;)
struct MyClass : public HandlerFor<MyClass, appmsgs::ActionMessage>,
	public MessageHandler<MyClass, 2, std::string, int>
{
    using HandlerFor<MyClass, appmsgs::ActionMessage>::receive;
    using MessageHandler<MyClass, 2, std::string, int>::receive;

    void process(const std::string& str) const
    {
        std::cout << "Received a string message!" << std::endl;
    }

    void process(const TransformMatrix& matrix) const
    {
        std::cout << "Received a matrix message!" << std::endl;
    }

    void process(const std::string& str1, const std::string& str2) const
    {
        std::cout << "Received a two strings event!" << std::endl;
    }
};

using namespace std::string_literals;

int main()
{
    TransformMatrix matrix;

    matrix.values[0][0] = 1; // ...

    appmsgs::ActionMessage hello{"hola caracola"s};
    appmsgs::ActionMessage number{10.0f};
    appmsgs::ActionMessage transform{matrix};

    MyClass myObject;

    myObject.receive(hello);
    myObject.receive(number);
    myObject.receive(transform);
    myObject.receive(appmsgs::LoggingMessage{"hello"s, "world"s});

	std::cin.get();
}
