
#if !defined(USE_TIMEMORY)
#    define USE_TIMEMORY
#endif

#include "interface.hpp"
#include "common.hpp"

#include <timemory/defines.h>
#include <timemory/timemory.hpp>

#if defined(TIMEMORY_USE_EXTERN)
#    include <timemory/tools/timemory-mpip.h>
#    include <timemory/tools/timemory-ompt.h>
#endif

TIMEMORY_DECLARE_COMPONENT(myexp)
TIMEMORY_DECLARE_COMPONENT(mycos)

using namespace tim::component;

using perf_bundle_t = tim::component_tuple<wall_clock>;
using exp_gotcha_t  = tim::component::gotcha<1, std::tuple<>, myexp>;
using cos_gotcha_t  = tim::component::gotcha<1, std::tuple<>, mycos>;
using rng_gotcha_t  = tim::component::gotcha<4, perf_bundle_t, tools::matvec>;
using rng_bundle_t  = tim::component_tuple<rng_gotcha_t>;

namespace tim
{
namespace component
{
struct myexp : concepts::component
{
    using value_type = void;
    using base_type  = void;
    double operator()(double v);
};
//
struct mycos : concepts::component
{
    using value_type = void;
    using base_type  = void;
    double operator()(double v);
};
}  // namespace component
}  // namespace tim

namespace
{
auto _use_math_wrappers = tim::get_env<bool>("USE_MATH_WRAPPERS", false);
auto _use_rng_wrappers = tim::get_env<bool>("USE_RNG_WRAPPERS", false);
auto _rng_bundle = std::unique_ptr<rng_bundle_t>{};
void
create_wrappers()
{
    if(_use_math_wrappers)
    {
        TIMEMORY_C_GOTCHA(exp_gotcha_t, 0, exp);
        TIMEMORY_C_GOTCHA(cos_gotcha_t, 0, cos);
    }

    if(!_rng_bundle)
    {
        rng_gotcha_t::get_initializer() = []() {
            TIMEMORY_CXX_GOTCHA(rng_gotcha_t, 0, common::generate_canonical);
            TIMEMORY_DERIVED_GOTCHA(rng_gotcha_t, 1,
                                    common::generate_random_in_range<double>,
                                    "_ZN6common24generate_random_in_rangeIdEET_S1_");
        };
        _rng_bundle = std::make_unique<rng_bundle_t>();
    }
}
}  // namespace

void
tools::activate_wrappers()
{
#if defined(TIMEMORY_USE_EXTERN)
    timemory_register_mpip();
#endif

    create_wrappers();
    if(_use_rng_wrappers)
        _rng_bundle->start();
}

void
tools::deactivate_wrappers()
{
    if(_use_rng_wrappers)
        _rng_bundle->stop();

#if defined(TIMEMORY_USE_EXTERN)
    timemory_deregister_mpip();
#endif
}

double
myexp::operator()(double v)
{
    static bool _once = (puts("using myexp"), true);
    (void) _once;
    return static_cast<double>(expf(static_cast<float>(v)));
}

double
mycos::operator()(double v)
{
    static bool _once = (puts("using mycos"), true);
    (void) _once;
    return static_cast<double>(cosf(static_cast<float>(v)));
}
