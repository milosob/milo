

#pragma once


#include <milo/traits.h>

#include <milo/detail/arch/x86.h>
#include <milo/detail/arch/any.h>


namespace milo::detail
{
    using arch_active = traits::disjunction<
        traits::boolean<arch_x86>,
        traits::boolean<arch_any>
    >::type;
    
    static_assert(arch_active::value);
}
