

#pragma once


#include <milo/internal.h>


/**
 * The name of the library
 */
#define MILO_NAME                       MILO_INTERNAL_PROJECT_NAME

/**
 * The major version of the library.
 */
#define MILO_VERSION_MAJOR              MILO_INTERNAL_PROJECT_VERSION_MAJOR

/**
 * The minor version of the library.
 */
#define MILO_VERSION_MINOR              MILO_INTERNAL_PROJECT_VERSION_MINOR

/**
 * The patch version of the library.
 */
#define MILO_VERSION_PATCH              MILO_INTERNAL_PROJECT_VERSION_PATCH

/**
 * The aggregated integral version of the library.
 * The version is represented as 64-bit integer where each 16-bit sub integer encodes the specific part.
 * The encoding is [major, minor, patch, reserved].
 */
#define MILO_VERSION_INTEGRAL           MILO_INTERNAL_PROJECT_VERSION_INTEGRAL

/**
 * The aggregated semantic string version of the library.
 * The encoding is "major.minor.patch".
 */
#define MILO_VERSION_SEMANTIC           MILO_INTERNAL_PROJECT_VERSION_SEMANTIC
