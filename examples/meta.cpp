
#include <tuple>
#include <siplasplas/utility/meta.hpp>

using namespace cpp;

using is_pointer = meta::defer<std::is_pointer>;

using pointers = meta::filter_t<is_pointer, std::tuple<int, char*, bool*, float>>;

int main()
{
	static_assert(std::is_same<pointers, std::tuple<char*, bool*>>::value, "???");
}