

#pragma once


#include <milo/detail/option.h>


namespace milo::option
{
    template<size_t t_value>
    using digest_size = milo::detail::option_digest_size_suite::option<t_value>;
    
    template<typename t_type>
    using impl_scope = milo::detail::option_impl_scope_suite::option<t_type>;
    
    template<typename t_type>
    using impl_constexpr = milo::detail::option_impl_constexpr_suite::option<t_type>;
    
    template<typename t_type>
    using impl_runtime = milo::detail::option_impl_runtime_suite::option<t_type>;
}
