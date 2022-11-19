

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

#if defined __GNUG__ &&         \
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

namespace milo::compiler
{
    constexpr bool clang = MILO_COMPILER_CLANG;
    
    constexpr bool msvc = MILO_COMPILER_MSVC;
    
    constexpr bool icc = MILO_COMPILER_ICC;
    
    constexpr bool gcc = MILO_COMPILER_GCC;
    
    constexpr bool has_builtin_bswap = clang || msvc || icc || gcc;
}
