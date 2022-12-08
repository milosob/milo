

#pragma once


#include <milo/internal/base.h>


namespace milo::internal
{
    struct arch_any_ise_strict
    {
        template<
            typename t_impl
        >
        static
        constexpr auto
        check(
        ) noexcept(true) -> bool
        {
            return false;
        }
    };
    
    struct arch_any_ise_native
        : arch_any_ise_strict
    {
    };
    
    struct arch_any_ise_runtime
        : arch_any_ise_strict
    {
    };
    
    struct arch_any_ise
    {
        using strict = arch_any_ise_strict;
        
        using native = arch_any_ise_native;
        
        using runtime = arch_any_ise_runtime;
    };
    
    struct arch_any
    {
        static
        constexpr bool value = true;
        
        using ise = arch_any_ise;
    };
}
