

#pragma once


#include <milo/inner.h>


namespace milo::option
{
    template<size_t t_value>
    using digest_size = inner::option_digest_size_suite::option<t_value>;
    
    template<typename t_type>
    using impl_domain = inner::option_impl_domain_suite::option<t_type>;
    
    template<typename t_type>
    using impl_chooser = inner::option_impl_chooser_suite::option<t_type>;
    
    template<typename t_type>
    using impl_invoker = inner::option_impl_invoker_suite::option<t_type>;
    
    template<typename t_type>
    using impl_cpltime = inner::option_impl_cpltime_suite::option<t_type>;
    
    template<typename t_type>
    using impl_runtime = inner::option_impl_runtime_suite::option<t_type>;
}
