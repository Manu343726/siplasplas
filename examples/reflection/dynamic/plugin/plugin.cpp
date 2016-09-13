#include "plugin.hpp"

#include <iostream>

void Plugin::run()
{
    std::cout << "Plugin!" << std::endl;
}

int Plugin::ALU::add(int a, int b)
{
    return a + b;
}

int Plugin::ALU::sub(int a, int b)
{
    return a - b;
}
