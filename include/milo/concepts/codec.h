

#pragma once


#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept codec =
    requires
    {
        typename t_0::properties::codec;
    } &&
    requires
    {
        t_0::ratio_bytes_size;
        t_0::ratio_chars_size;
    } &&
    requires(
        const uint8_t* from_ptr,
        size_t from_size,
        uint8_t* to_ptr
    )
    {
        {
        t_0::encode(
            from_ptr,
            from_size,
            to_ptr
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const char* from_ptr,
        size_t from_size,
        char* to_ptr
    )
    {
        {
        t_0::encode(
            from_ptr,
            from_size,
            to_ptr
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const uint8_t* from_ptr,
        size_t from_size
    )
    {
        {
        t_0::encode_size(
            from_ptr,
            from_size
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const char* from_ptr,
        size_t from_size
    )
    {
        {
        t_0::encode_size(
            from_ptr,
            from_size
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const uint8_t* from_ptr,
        size_t from_size,
        uint8_t* to_ptr,
        error& error
    )
    {
        {
        t_0::decode(
            from_ptr,
            from_size,
            to_ptr,
            error
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const char* from_ptr,
        size_t from_size,
        char* to_ptr,
        error& error
    )
    {
        {
        t_0::decode(
            from_ptr,
            from_size,
            to_ptr,
            error
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const uint8_t* from_ptr,
        size_t from_size
    )
    {
        {
        t_0::decode_size(
            from_ptr,
            from_size
        )
        } noexcept -> same<size_t>;
    } &&
    requires(
        const char* from_ptr,
        size_t from_size
    )
    {
        {
        t_0::decode_size(
            from_ptr,
            from_size
        )
        } noexcept -> same<size_t>;
    };

    template<typename t_0>
    concept codec_base_16 =
    requires
    {
        requires codec<t_0>;
    } &&
    requires
    {
        typename t_0::properties::codec_base_16;
    };
    
    template<typename t_0>
    concept codec_base_64 =
    requires
    {
        requires codec<t_0>;
    } &&
    requires
    {
        typename t_0::properties::codec_base_64;
    };

    /*
     * @formatter:on
     */
}
