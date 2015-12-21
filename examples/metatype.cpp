
#include "metatype/metatype.hpp"
#include <iostream>

struct BaseClass
{
    virtual ~BaseClass() = 0;
    virtual void talk() const = 0;
};

struct Duck : public BaseClass, public cpp::MetaTypeSystem::Register<Duck>
{
    Duck() = default;

    void talk() const override
    {
        std::cout << "Quack!" << std::endl;        
    }
};

struct Cat : public BaseClass, public cpp::MetaTypeSystem::Register<Cat>
{
    Cat() = default;

    void talk() const override
    {
        std::cout << "Mew!" << std::endl;        
    }
};

int main()
{
    cpp::MetaTypeSystem::registerMetatype<Cat>();

    auto cat = static_cast<BaseClass*>(cpp::MetaTypeSystem::create("Cat"));

    cat->talk();
}
