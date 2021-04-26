
#pragma once

#if defined(USE_TIMEMORY)
#    include <timemory/macros/attributes.hpp>
#    include <timemory/backends.hpp>
#    include <timemory/components/types.hpp>
#    include <timemory/manager.hpp>
#    include <timemory/types.hpp>
#    include <timemory/variadic.hpp>

#    include "components.hpp"
#else
#    if !defined(TIMEMORY_CODE)
#        define TIMEMORY_CODE(...)
#    endif
#endif

#include <iostream>

namespace tools
{
// defined in components.hpp
struct normalize_tag
{};

void
initialize(int& argc, char**& argv);

void
finalize();

void
activate_wrappers();

void
deactivate_wrappers();

template <typename T>
void
add_metadata(const char*, T&&);

struct matvec;

#if defined(USE_TIMEMORY)
struct matvec
: tim::concepts::api
{};

using perf_quirk_t =
    tim::quirk::config<tim::quirk::explicit_push, tim::quirk::explicit_pop>;

using perf_bundle_t =
    tim::component_bundle<matvec, perf_quirk_t, tim::component::wall_clock,
                          tim::component::normalized_wall_clock,
                          tim::component::cpu_clock, tim::component::cpu_util,
                          tim::component::peak_rss, tim::component::user_global_bundle>;

struct perf_data
{
    perf_data();
    perf_data(const tim::string_view_t& name, bool store = tim::get_env("MATVEC_STORE", true));
    ~perf_data();

    perf_data& start();
    perf_data& stop();
    perf_data& push();
    perf_data& pop();
    perf_data& reset(const tim::string_view_t& = "");
    perf_data& print(bool, std::ostream& = std::cerr, const tim::string_view_t& = "\n") const;
    perf_data& print(std::ostream& = std::cerr, const tim::string_view_t& = "\n") const;

    template <typename TagT, typename Tp>
    perf_data& store(TagT&&, Tp&&);

    friend std::ostream& operator<<(std::ostream& os, const perf_data& obj)
    {
        obj.print(os);
        return os;
    }

private:
    bool m_store = true;
    std::unique_ptr<perf_bundle_t> m_data;
};

#else

struct perf_data
{
    template <typename... Args>
    perf_data(Args&&...) {}

    perf_data& start() { return *this; }
    perf_data& stop() { return *this; }
    perf_data& push() { return *this; }
    perf_data& pop() { return *this; }

    template <typename... Args>
    perf_data& store(Args&&...)
    {
        return *this;
    }

    template <typename... Args>
    perf_data& reset(Args&&...)
    {
        return *this;
    }

    template <typename... Args>
    perf_data& print(Args&&...) const
    {
        return *const_cast<perf_data*>(this);
    }
};

#endif

}  // namespace tools

template <typename T>
void
tools::add_metadata(const char* name, T&& data)
{
#if defined(USE_TIMEMORY)
    tim::manager::add_metadata(name, std::forward<T>(data));
#else
    (void) name;
    (void) data;
#endif
}
