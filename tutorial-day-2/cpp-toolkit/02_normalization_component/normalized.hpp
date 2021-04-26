
#include <timemory/components.hpp>
#include <timemory/components/base.hpp>
#include <timemory/mpl/types.hpp>

// for overload resolution, e.g. foo.store(normalize_tag{}, size_t)
struct normalization
{};

namespace tim
{
namespace component
{
//
//          FORWARD DECLARE COMPONENT
//
}

namespace trait
{
//
//          DEFINE TYPE-TRAITS
//
}  // namespace trait

namespace component
{
//
//          DEFINE normalized<T>
//
}

}  // namespace tim
