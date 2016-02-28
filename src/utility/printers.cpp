#include "printers.hpp"

#include <iomanip>
#include "memory_manip.hpp"

namespace cpp
{
    std::function<std::ostream&(std::ostream&)> print_memory(const char* begin, const char* end, std::size_t word, char byteSeparator)
    {
        return [=](std::ostream& os) -> std::ostream&
        {
            os << "Memory block [" << (void*)begin << ", " << (void*)end << "): ";
            os << std::hex
               << std::setw(2)
               << std::setfill('0');

            std::size_t blockLength = end - begin;

            for(std::size_t i = 0; i < blockLength; ++i)
            {
                if(word > 0 && i % word == 0)
                    os << "\n@" << reinterpret_cast<const void*>(begin + i) << ": " << byteSeparator;

                os << (int)cpp::detail::read_at<std::uint8_t>(begin + i) << byteSeparator;
            }

            return os << std::dec;
        };
    }

}