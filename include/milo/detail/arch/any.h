

#pragma once


#include <milo/common.h>


namespace milo::detail
{
    struct arch_any_ise_strict
        : private common::disabled
    {
        template<
            typename t_impl
        >
        static
        constexpr auto
        impl(
        ) noexcept(true) -> bool
        requires
        (!
            requires{
                typename t_impl::requirements::arch;
            })
        {
            return true;
        }
    };
    
    struct arch_any_ise_native
        : arch_any_ise_strict
    {
    };
    
    struct arch_any_ise_runtime
        : arch_any_ise_strict
    {
        /*
         *  This is compile-time ise.
         */
    };
    
    struct ise
        : private common::disabled
    {
        using strict = arch_any_ise_strict;
        
        using native = arch_any_ise_native;
        
        using runtime = arch_any_ise_runtime;
    };
    
    struct arch_any_impl
        : private common::disabled
    {
        static
        constexpr bool value = true;
        
        using ise = ise;
    };
}
