
#include "components.hpp"
#include "interface.hpp"

namespace tim
{
namespace component
{
std::string
normalized_wall_clock::label()
{
    return "normalized_wall";
}

std::string
normalized_wall_clock::description()
{
    return "Wall-clock timer normalized to data size";
}

void
normalized_wall_clock::store(tools::normalize_tag&&, size_t _data_size)
{
    data_size = _data_size;
}

double
normalized_wall_clock::get() const noexcept
{
    return (static_cast<double>(load()) / ratio_t::den * get_unit()) / data_size;
}

double
normalized_wall_clock::get_display() const noexcept
{
    return get();
}

bool
normalized_wall_clock::assemble(wall_clock* wc)
{
    // set_is_invalid(wc == nullptr);
    return (wc != nullptr);
}

bool
normalized_wall_clock::derive(wall_clock* wc)
{
    if(wc)
    {
        if(tim::settings::verbose() > 1)
            printf("deriving data from wall_clock (data size = %lu)\n",
                   (unsigned long) data_size);
        value = wc->get_value();
        accum = wc->get_value();
    }
    return (wc != nullptr);
}

}  // namespace component
}  // namespace tim
