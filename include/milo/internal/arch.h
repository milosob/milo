

#pragma once


#include <milo/meta.h>

#include <milo/internal/arch/x86_32.h>
#include <milo/internal/arch/x86_64.h>
#include <milo/internal/arch/any.h>


namespace milo::internal
{
    using arch_active = meta::disjunction<
        meta::asbool<arch_x86_64::value, arch_x86_64>,
        meta::asbool<arch_any::value, arch_any>
    >::type;
    
    static_assert(arch_active::value);
}
