
#include <tuple>
#include <siplasplas/utility/meta.hpp>
#include <iostream>

using namespace cpp;

using is_pointer = meta::defer<std::is_pointer>;

using pointers = meta::filter_t<is_pointer, std::tuple<int*, char*, bool*, float*>>;
using indices = meta::to_index_sequence_t<pointers>;

template<std::size_t... Is>
void print_index_sequence(meta::index_sequence<Is...>)
{
	for(std::size_t i : {Is...})
		std::cout << i << " ";
}

int main()
{
	//static_assert(std::is_same<pointers, std::tuple<char*, bool*>>::value, "???");
    
    print_index_sequence(meta::make_index_sequence<1000000>());
}