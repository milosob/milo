

#pragma once


#include <milo/arch.h>
#include <milo/bits.h>
#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>


#if MILO_ARCH_X86_ISE_SSE_1 && \
    MILO_ARCH_X86_ISE_SSE_2 && \
    MILO_ARCH_X86_ISE_SSE_4_1 && \
    MILO_ARCH_X86_ISE_SHA_2 && \
    false


#include <immintrin.h>

#else

namespace milo::detail
{
    using hash_sha_2_256_impl_hw_x86_sha2 = void;
}

#endif
