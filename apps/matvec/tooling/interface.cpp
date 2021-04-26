
#if !defined(USE_TIMEMORY)
#    define USE_TIMEMORY
#endif

#include "interface.hpp"

#include <timemory/timemory.hpp>
#include <timemory/defines.h>

#if defined(TIMEMORY_USE_EXTERN)
#    include <timemory/tools/timemory-mpip.h>
#    include <timemory/tools/timemory-ompt.h>
#endif

void
tools::initialize(int& argc, char**& argv)
{
    // API is disabled at compile-time
    if(!tim::trait::is_available<tools::matvec>::value)
        return;
    tim::settings::cout_output()        = false;
    tim::settings::flamegraph_output()  = false;
    tim::settings::collapse_threads()   = false;
    tim::settings::collapse_processes() = false;
    tim::timemory_init(argc, argv);
    for(int i = 1; i < argc; ++i)
    {
        if(strncmp(argv[i], "--", 2) == 0)
        {
            tim::timemory_argparse(&argc, &argv);
            break;
        }
    }
    std::cerr << "args:";
    for(int i = 0; i < argc; ++i)
        std::cerr << " " << argv[i];
    std::cerr << std::endl;

    activate_wrappers();
}

void
tools::finalize()
{
    deactivate_wrappers();
    // API is disabled at compile-time
    if(!tim::trait::is_available<tools::matvec>::value)
        return;
    tim::timemory_finalize();
}

tools::perf_data::perf_data()
: m_store{ false }
, m_data{ std::make_unique<tools::perf_bundle_t>("<unnamed>") }
{}

tools::perf_data::~perf_data()
{}

tools::perf_data::perf_data(const tim::string_view_t& name, bool store)
: m_store{ store }
, m_data{ std::make_unique<tools::perf_bundle_t>(name) }
{}

tools::perf_data&
tools::perf_data::start()
{
    if(m_store)
        m_data->push();
    else
        m_data->reset();
    m_data->start();
    return *this;
}

tools::perf_data&
tools::perf_data::stop()
{
    m_data->stop();
    if(m_store)
        m_data->pop();
    return *this;
}

tools::perf_data&
tools::perf_data::push()
{
    m_data->push();
    return *this;
}

tools::perf_data&
tools::perf_data::pop()
{
    m_data->pop();
    return *this;
}

tools::perf_data&
tools::perf_data::print(bool _cond, std::ostream& os, const tim::string_view_t& _tail) const
{
    if(_cond)
        os << *m_data << _tail;
    return *(const_cast<perf_data*>(this));
}

tools::perf_data&
tools::perf_data::print(std::ostream& os, const tim::string_view_t& _tail) const
{
    return print(true, os, _tail);
}

tools::perf_data&
tools::perf_data::reset(const tim::string_view_t& name)
{
    m_data->reset();
    if(!name.empty())
        m_data->rekey(name);
    return *this;
}

template <typename TagT, typename Tp>
tools::perf_data&
tools::perf_data::store(TagT&& _tag, Tp&& _val)
{
    m_data->store(std::forward<TagT>(_tag), std::forward<Tp>(_val));
    return *this;
}

// provide instantiation
#define INSTANTIATE_STORE(TAG, DTYPE)                                                    \
    template tools::perf_data& tools::perf_data::store<TAG, DTYPE>(TAG&&, DTYPE&&);

INSTANTIATE_STORE(tools::normalize_tag, int);
INSTANTIATE_STORE(tools::normalize_tag, long);
INSTANTIATE_STORE(tools::normalize_tag, unsigned int);
INSTANTIATE_STORE(tools::normalize_tag, unsigned long);
INSTANTIATE_STORE(tools::normalize_tag, unsigned long long);
