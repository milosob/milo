

#pragma once


#include <milo/internal/compiler.h>


#if MILO_INTERNAL_COMPILER_GCC || MILO_INTERNAL_COMPILER_CLANG || MILO_INTERNAL_COMPILER_ICC
    #define MILO_INTERNAL_ATTRIBUTE_INLINE_true         __attribute__((always_inline)) inline
    #define MILO_INTERNAL_ATTRIBUTE_INLINE_false        __attribute__((noinline))
#endif

#if MILO_INTERNAL_COMPILER_MSVC
    #define MILO_INTERNAL_ATTRIBUTE_INLINE_true         __forceinline inline
    #define MILO_INTERNAL_ATTRIBUTE_INLINE_false        __declspec(noinline)
#endif

#define MILO_INTERNAL_ATTRIBUTE_INLINE(a_bool)      MILO_INTERNAL_ATTRIBUTE_INLINE_##a_bool

