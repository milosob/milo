

#pragma once


#include <milo/internal/base.h>


namespace milo::internal
{
    struct arch_any_ise
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
    
    struct arch_any
    {
        static
        constexpr bool value = true;
        
        using ise = arch_any_ise;
    };
}
