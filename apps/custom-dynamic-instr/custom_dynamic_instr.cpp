// MIT License
//
// Copyright (c) 2020, The Regents of the University of California,
// through Lawrence Berkeley National Laboratory (subject to receipt of any
// required approvals from the U.S. Dept. of Energy).  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "timemory/library.h"
#include "timemory/timemory.hpp"

using namespace tim::component;
//
//--------------------------------------------------------------------------------------//
//
TIMEMORY_DECLARE_COMPONENT(flops_per_bytes)
TIMEMORY_STATISTICS_TYPE(component::flops_per_bytes, double)
#if !defined(TIMEMORY_USE_PAPI)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_available, component::flops_per_bytes, false_type)
#endif
//
//--------------------------------------------------------------------------------------//
//
namespace tim
{
namespace component
{
struct flops_per_bytes : public base<flops_per_bytes, std::array<long long, 2>>
{
    using value_type = std::array<long long, 2>;
    using hw_t       = tim::component::papi_tuple<PAPI_DP_OPS, PAPI_LST_INS>;

    static std::string label() { return "flops_per_bytes"; }
    static std::string description()
    {
        return "double-precision flops per load-store instr";
    }
    static void thread_init(storage_type*) { hw_t::thread_init(nullptr); }

    void start()
    {
        m_hw.start();
        value = m_hw.get_value();
    }
    void stop()
    {
        m_hw.stop();
        value = m_hw.get_value();
        accum += value;
    }
    double get() const
    {
        return (m_hw.get()[1] > 0.0) ? (m_hw.get()[0] / m_hw.get()[1]) : 0.0;
    }

private:
    hw_t m_hw;
};
}  // namespace component
}  // namespace tim
//
//--------------------------------------------------------------------------------------//
//
extern "C" void
timemory_register_custom_dynamic_instr()
{
    PRINT_HERE("%s", "Registering the custom dynamic instrumentation");

    // insert monotonic clock component into structure
    // used by timemory-run in --mode=trace
    user_trace_bundle::global_init(nullptr);
    user_trace_bundle::configure<monotonic_clock>();

    // insert monotonic clock component into structure
    // used by timemory-run in --mode=region
    timemory_add_components("monotonic_clock");

    // if PAPI enabled at compile-time and run-time
    using hw_t = typename flops_per_bytes::hw_t;
    if(tim::trait::runtime_enabled<hw_t>::get())
    {
        // used by timemory when --mode=trace
        user_trace_bundle::configure<flops_per_bytes>();
        // used by timemory when --mode=region
        user_global_bundle::configure<flops_per_bytes>();
    }
}
//
//--------------------------------------------------------------------------------------//
//
extern "C" void
timemory_deregister_custom_dynamic_instr()
{
    PRINT_HERE("%s", "Deregistering the custom dynamic instrumentation");
}
//
//--------------------------------------------------------------------------------------//
//
