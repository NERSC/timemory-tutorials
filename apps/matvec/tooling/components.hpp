
#include <timemory/components/base.hpp>
#include <timemory/components/timing/wall_clock.hpp>

namespace tools
{
// for overload resolution, e.g. foo.store(normalize_tag{}, size_t)
struct normalize_tag;
//
struct matvec;
}  // namespace tools

namespace tools
{
}

TIMEMORY_DECLARE_COMPONENT(normalized_wall_clock)

TIMEMORY_SET_COMPONENT_API(component::normalized_wall_clock, tools::matvec,
                           category::timing, os::agnostic)

TIMEMORY_STATISTICS_TYPE(component::normalized_wall_clock, double)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_timing_category, component::normalized_wall_clock,
                               true_type)
TIMEMORY_DEFINE_CONCRETE_TRAIT(uses_timing_units, component::normalized_wall_clock,
                               true_type)

namespace tim
{
namespace trait
{
template <>
struct derivation_types<component::normalized_wall_clock>
{
    using type = type_list<type_list<component::wall_clock>>;
};
}
}

namespace tim
{
namespace component
{
struct normalized_wall_clock : public base<normalized_wall_clock, int64_t>
{
    using ratio_t    = std::nano;
    using value_type = int64_t;
    using base_type  = base<normalized_wall_clock, value_type>;

    static std::string label();
    static std::string description();

    static const short    precision = 3;
    static const short    width     = 8;
    static const fmtflags format_flags =
        std::ios_base::scientific | std::ios_base::dec | std::ios_base::showpoint;

    void   store(tools::normalize_tag&&, size_t _data_size);
    double get() const noexcept;
    double get_display() const noexcept;

    bool assemble(wall_clock* wc);
    bool derive(wall_clock* wc);

private:
    size_t data_size = 1;
};
}  // namespace component
}
