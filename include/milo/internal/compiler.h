

#pragma once


#include <milo/internal/macro.h>

/*
 * @formatter:off
 */

#if defined __clang__
    #define MILO_INTERNAL_COMPILER_CLANG true
    #define MILO_INTERNAL_COMPILER_NAME "clang"
    #define MILO_INTERNAL_COMPILER_VERSION_MAJOR __clang_major__
    #define MILO_INTERNAL_COMPILER_VERSION_MINOR __clang_minor__
    #define MILO_INTERNAL_COMPILER_VERSION_PATCH __clang_patchlevel__
#else
    #define MILO_INTERNAL_COMPILER_CLANG false
#endif

#if defined _MSC_VER
    #define MILO_INTERNAL_COMPILER_MSVC true
    #define MILO_INTERNAL_COMPILER_NAME "msvc"
    /*
     * TODO
     */
    #define MILO_INTERNAL_COMPILER_VERSION_MAJOR 0
    #define MILO_INTERNAL_COMPILER_VERSION_MINOR 0
    #define MILO_INTERNAL_COMPILER_VERSION_PATCH 0
#else
    #define MILO_INTERNAL_COMPILER_MSVC false
#endif

#if defined __INTEL_COMPILER
    #define MILO_INTERNAL_COMPILER_ICC true
    #define MILO_INTERNAL_COMPILER_NAME "icc"
    /*
     * TODO
     */
    #define MILO_INTERNAL_COMPILER_VERSION_MAJOR 0
    #define MILO_INTERNAL_COMPILER_VERSION_MINOR 0
    #define MILO_INTERNAL_COMPILER_VERSION_PATCH 0
#else
    #define MILO_INTERNAL_COMPILER_ICC false
#endif

#if defined __GNUG__     && \
    !MILO_INTERNAL_COMPILER_CLANG && \
    !MILO_INTERNAL_COMPILER_MSVC  && \
    !MILO_INTERNAL_COMPILER_ICC
    #define MILO_INTERNAL_COMPILER_GCC true
    #define MILO_INTERNAL_COMPILER_NAME "gcc"
    #define MILO_INTERNAL_COMPILER_VERSION_MAJOR __GNUC__
    #define MILO_INTERNAL_COMPILER_VERSION_MINOR __GNUC_MINOR__
    #define MILO_INTERNAL_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#else
    #define MILO_INTERNAL_COMPILER_GCC false
#endif

#define MILO_INTERNAL_COMPILER_VERSION \
        MILO_INTERNAL_MACRO_STRINGIFY_PASS(MILO_INTERNAL_COMPILER_VERSION_MAJOR) "." \
        MILO_INTERNAL_MACRO_STRINGIFY_PASS(MILO_INTERNAL_COMPILER_VERSION_MINOR) "." \
        MILO_INTERNAL_MACRO_STRINGIFY_PASS(MILO_INTERNAL_COMPILER_VERSION_PATCH)

/*
 * @formatter:on
 */
