

#pragma once


#include <milo/codec/apie.h>
#include <milo/codec/base.h>
#include <milo/common.h>
#include <milo/container.h>


namespace milo::literals
{
    auto
    operator "" _b(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::bytes_dynamic
    {
        return container::bytes_dynamic(
            reinterpret_cast<const uint8_t*>(a_ptr),
            reinterpret_cast<const uint8_t*>(a_ptr) + a_size
        );
    }
    
    auto
    operator "" _bv(
        const char* a_ptr,
        size_t a_size
    ) noexcept(true) -> container::bytes_const_view_dynamic
    {
        return container::bytes_const_view_dynamic(
            reinterpret_cast<const uint8_t*>(a_ptr),
            a_size
        );
    }
    
    constexpr auto
    operator "" _c(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::chars_dynamic
    {
        return container::chars_dynamic(
            a_ptr,
            a_ptr + a_size
        );
    }
    
    constexpr auto
    operator "" _cv(
        const char* a_ptr,
        size_t a_size
    ) noexcept(true) -> container::chars_const_view_dynamic
    {
        return container::chars_const_view_dynamic(
            a_ptr,
            a_ptr + a_size
        );
    }
    
    constexpr auto
    operator "" _base_16(
        const char* a_ptr,
        size_t a_size
    ) noexcept(false) -> container::bytes_dynamic
    {
        return codec::apie<codec::base_16>::decode<container::bytes_dynamic>(
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
        return codec::apie<codec::base_64>::decode<container::bytes_dynamic>(
            container::chars_const_view_dynamic(
                a_ptr,
                a_size
            )
        );
    }
}
