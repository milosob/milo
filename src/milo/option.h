

#pragma once


#include <milo/internal.h>


namespace milo::option
{
    template<size_t t_value>
    using digest_size = internal::option_digest_size_suite::option<t_value>;
    
    template<typename t_type>
    using impl_chooser = internal::option_impl_chooser_suite::option<t_type>;
    
    template<typename t_type>
    using impl_invoker = internal::option_impl_invoker_suite::option<t_type>;
    
    template<typename t_type>
    using impl_cpltime = internal::option_impl_cpltime_suite::option<t_type>;
    
    template<typename t_type>
    using impl_runtime = internal::option_impl_runtime_suite::option<t_type>;
    
    template<typename t_type>
    using impl_enforce = internal::option_impl_enforce_suite::option<t_type>;
}
