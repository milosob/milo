

#pragma once


#include <bit>

#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/traits.h>

#include <milo/detail/compiler.h>


namespace milo::detail
{
    constexpr bool endian_big = std::endian::big == std::endian::native;
    
    constexpr bool endian_little = std::endian::little == std::endian::native;
    
    template<
        concepts::integral_u t_int
    >
    constexpr auto
    endian_swap_sw(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if constexpr (concepts::same<t_int, uint8_t>)
        {
            return a_int;
        }
        
        if constexpr (concepts::same<t_int, uint16_t>)
        {
            return
                a_int << 8 |
                a_int >> 8;
        }
        
        if constexpr (concepts::same<t_int, uint32_t>)
        {
            return
                a_int >> 24 |
                a_int << 24 |
                (a_int & 0x00ff0000) >> 8 |
                (a_int & 0x0000ff00) << 8;
        }
        
        if constexpr (concepts::same<t_int, uint64_t>)
        {
            return
                a_int >> 56 |
                a_int << 56 |
                (a_int & 0x00ff000000000000) >> 40 |
                (a_int & 0x000000000000ff00) << 40 |
                (a_int & 0x0000ff0000000000) >> 24 |
                (a_int & 0x0000000000ff0000) << 24 |
                (a_int & 0x000000ff00000000) >> 8 |
                (a_int & 0x00000000ff000000) << 8;
        }
        
        static_assert(sizeof(t_int) <= 8);
    }
    
    template<
        concepts::integral_u t_int
    >
    constexpr auto
    endian_swap_bi(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if constexpr (concepts::same<t_int, uint8_t>)
        {
            return a_int;
        }
        
        if constexpr (concepts::same<t_int, uint16_t>)
        {
            if constexpr (compiler_msvc)
            {
                return _byteswap_ushort(a_int);
            }
            else
            {
                return __builtin_bswap16(a_int);
            }
        }
        
        if constexpr (concepts::same<t_int, uint32_t>)
        {
            if constexpr (compiler_msvc)
            {
                return _byteswap_ulong(a_int);
            }
            else
            {
                return __builtin_bswap32(a_int);
            }
        }
        
        if constexpr (concepts::same<t_int, uint64_t>)
        {
            if constexpr (compiler_msvc)
            {
                return _byteswap_uint64(a_int);
            }
            else
            {
                return __builtin_bswap64(a_int);
            }
        }
        
        static_assert(sizeof(t_int) <= 8);
    }
    
    template<
        concepts::integral_u t_int
    >
    constexpr auto
    endian_swap(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if MILO_CONSTEVAL
        {
            return endian_swap_sw(a_int);
        }
        else
        {
            if constexpr (compiler_has_builtin_bswap)
            {
                return endian_swap_bi(a_int);
            }
            else
            {
                return endian_swap_sw(a_int);
            }
        }
    }
    
    template<
        concepts::integral_s t_int
    >
    constexpr auto
    endian_swap(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        using to_type = t_int;
        using as_type = traits::to_integral_unsigned_t<t_int>;
        
        return to_type(endian_swap(as_type(a_int)));
    }
    
    template<
        concepts::integral t_int
    >
    constexpr auto
    endian_bigof(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if constexpr (endian_little)
        {
            a_int = endian_swap(a_int);
        }
        
        return a_int;
    }
    
    template<
        concepts::integral t_int
    >
    constexpr auto
    endian_littleof(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if constexpr (endian_big)
        {
            a_int = endian_swap(a_int);
        }
        
        return a_int;
    }
}
