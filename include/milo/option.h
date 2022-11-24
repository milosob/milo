

#pragma once


#include <milo/detail/option.h>


namespace milo::option
{
    template<size_t t_value>
    using digest_size = detail::option_digest_size_suite::option<t_value>;
    
    template<typename t_type>
    using impl_domain = detail::option_impl_domain_suite::option<t_type>;
    
    template<typename t_type>
    using impl_chooser = detail::option_impl_chooser_suite::option<t_type>;
    
    template<typename t_type>
    using impl_invoker = detail::option_impl_invoker_suite::option<t_type>;
    
    template<typename t_type>
    using impl_cpltime = detail::option_impl_cpltime_suite::option<t_type>;
    
    template<typename t_type>
    using impl_runtime = detail::option_impl_runtime_suite::option<t_type>;
}