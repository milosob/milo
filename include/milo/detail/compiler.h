

#pragma once

/*
 * @formatter:off
 */

#if defined __clang__
    #define MILO_COMPILER_CLANG 1
#else
    #define MILO_COMPILER_CLANG 0
#endif

#if defined _MSC_VER
    #define MILO_COMPILER_MSVC 1
#else
    #define MILO_COMPILER_MSVC 0
#endif

#if defined __INTEL_COMPILER
    #define MILO_COMPILER_ICC 1
#else
    #define MILO_COMPILER_ICC 0
#endif

#if defined __GNUG__     && \
    !MILO_COMPILER_CLANG && \
    !MILO_COMPILER_MSVC  && \
    !MILO_COMPILER_ICC
    #define MILO_COMPILER_GCC 1
#else
    #define MILO_COMPILER_GCC 0
#endif

/*
 * @formatter:on
 */

namespace milo::detail
{
    constexpr bool compiler_clang = MILO_COMPILER_CLANG;
    
    constexpr bool compiler_msvc = MILO_COMPILER_MSVC;
    
    constexpr bool compiler_icc = MILO_COMPILER_ICC;
    
    constexpr bool compiler_gcc = MILO_COMPILER_GCC;
    
    constexpr bool compiler_has_builtin_bswap = compiler_clang || compiler_msvc || compiler_icc || compiler_gcc;
}
