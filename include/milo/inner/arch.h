

#pragma once


#include <milo/meta.h>

#include <milo/inner/arch/x86.h>
#include <milo/inner/arch/any.h>


namespace milo::inner
{
    using arch_active = meta::disjunction<
        meta::asbool<arch_x86::value, arch_x86>,
        meta::asbool<arch_any::value, arch_x86>
    >::type;
    
    static_assert(arch_active::value);
}
