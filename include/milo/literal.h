

#pragma once


#include <milo/inner.h>

#include <milo/crypto/codec/apie.h>
#include <milo/crypto/codec/base.h>


namespace milo::literal
{
    auto
    operator "" _b(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::bytes_dynamic
    {
        return {
            reinterpret_cast<const uint8_t*>(a_ptr),
            reinterpret_cast<const uint8_t*>(a_ptr) + a_size
        };
    }
    
    auto
    operator "" _bv(
        const char* a_ptr,
        size_t a_size
    ) noexcept(true) -> container::bytes_const_view_dynamic
    {
        return {
            reinterpret_cast<const uint8_t*>(a_ptr),
            a_size
        };
    }
    
    constexpr auto
    operator "" _c(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::chars_dynamic
    {
        return {
            a_ptr,
            a_ptr + a_size
        };
    }
    
    constexpr auto
    operator "" _cv(
        const char* a_ptr,
        size_t a_size
    ) noexcept(true) -> container::chars_const_view_dynamic
    {
        return {
            a_ptr,
            a_ptr + a_size
        };
    }
    
    constexpr auto
    operator "" _base_16(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::bytes_dynamic
    {
        return crypto::codec::decode<
            crypto::codec::base_16,
            container::bytes_dynamic
        >(
            container::chars_const_view_dynamic(
                a_ptr,
                a_size
            )
        );
    }
    
    constexpr auto
    operator "" _base_64(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::bytes_dynamic
    {
        return crypto::codec::decode<
            crypto::codec::base_64,
            container::bytes_dynamic
        >(
            container::chars_const_view_dynamic(
                a_ptr,
                a_size
            )
        );
    }
}
