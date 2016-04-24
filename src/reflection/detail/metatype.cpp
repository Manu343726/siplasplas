#include "detail/metatype.hpp"

using namespace cpp;

MetaType::MetaTypeRegistry& MetaType::metaTypes()
{
    static MetaType::MetaTypeRegistry types;
    return types;
}
