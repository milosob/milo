

#pragma once


#include <milo/detail/impl.h>


namespace milo::impl
{
    namespace scope
    {
        using strict = detail::impl_scope_strict;
        
        using native = detail::impl_scope_native;
        
        using runtime = detail::impl_scope_runtime;
    }
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    static
    constexpr auto
    select_hook(
    ) noexcept(true)
    {
        return detail::impl_select_hook<
            t_scope,
            t_selector,
            t_impls...
        >();
    }
    
    template<
        typename t_scope,
        typename t_selector,
        typename... t_impls
    >
    using select_type =
        detail::impl_select_type<
            t_scope,
            t_selector,
            t_impls...
        >;
    
    template<
        typename t_scope,
        typename t_selector,
        typename t_invoker,
        typename t_constexpr,
        typename t_enforced,
        typename... t_impls
    >
    using proxy =
        detail::impl_proxy<
            t_scope,
            t_selector,
            t_invoker,
            t_constexpr,
            t_enforced,
            t_impls...
        >;
}
