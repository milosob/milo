

#pragma once


#include <milo/traits.h>

#include <milo/detail/arch/x86.h>
#include <milo/detail/arch/any.h>


namespace milo::arch
{
    using x86 = detail::arch_x86_impl;
    
    using any = detail::arch_any_impl;
    
    /**
     * Active architecture.
     */
    using active = traits::disjunction<
        traits::boolean<x86>,
        traits::boolean<any>
    >::type;
    
    static_assert(active::value);
}
