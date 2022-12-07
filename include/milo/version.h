

#pragma once


#include <milo/inner.h>


namespace milo::version
{
    /**
     * The major version of the library.
     */
    constexpr uint16_t major = MILO_CONFIG_VERSION_MAJOR;
    
    /**
     * The minor version of the library.
     */
    constexpr uint16_t minor = MILO_CONFIG_VERSION_MINOR;
    
    /**
     * The patch version of the library.
     */
    constexpr uint16_t patch = MILO_CONFIG_VERSION_PATCH;
    
    /**
     * The tweak version of the library.
     */
    constexpr uint16_t tweak = MILO_CONFIG_VERSION_TWEAK;
    
    /**
     * The version of the library.
     * The version is represented as 64-bit integer where each 16-bit sub integer encodes the specific part.
     * The encoding is [major, minor, patch, tweak].
     */
    constexpr uint64_t full =
        uint64_t(major) << 48 |
        uint64_t(minor) << 32 |
        uint64_t(patch) << 16 |
        uint64_t(tweak);
    
    static_assert(full != 0x00);
}
