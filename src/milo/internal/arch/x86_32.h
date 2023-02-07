

#pragma once


#include <milo/internal/base.h>


#if defined(__i386__) || \
    defined(_WIN32)
    #define MILO_INTERNAL_ARCH_X86_32 true
#else
    #define MILO_INTERNAL_ARCH_X86_32 false
#endif

namespace milo::internal
{
    /*
     * @formatter:off
     */
    
    #if MILO_INTERNAL_ASM && MILO_INTERNAL_ARCH_X86_32
    
    extern "C"
    auto
    milo_internal_arch_x86_32_cpuid(
        uint32_t* a_info_ptr,
        uint32_t a_eax,
        uint32_t a_ecx
    ) noexcept(true) -> void;
    
    #endif
    
    /*
     * @formatter:on
     */
}
