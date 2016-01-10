
#include <iostream>
#include <string>

#include "reflection/detail/type_info.hpp"
#include "reflection/detail/any.hpp"
#include "reflection/detail/metatype.hpp"

using namespace std::string_literals;

int main()
{
	static_assert(cpp::TypeInfo::get<int>()(cpp::TypeTrait::is_integral), "???");

	cpp::detail::Any any{ 1 };
	cpp::detail::Any any2{ cpp::TypeInfo::get<std::string>() };
	auto intType = cpp::MetaType::get<int>();

	std::cout << intType.type().name() << std::endl;

	void* integer = cpp::MetaType::get("int").create();
    cpp::MetaType::get("int").destroy(integer);

	std::cout << cpp::MetaType::get("int").type().alignment() << std::endl;

	any2 = "hello"s;

	std::cout << any2.get<std::string>() << std::endl;

	std::cout << sizeof(any) << std::endl;
	std::cout << any.get<int>() << std::endl;

	std::cout << cpp::TypeInfo::get<std::string>().name() << std::endl;
}