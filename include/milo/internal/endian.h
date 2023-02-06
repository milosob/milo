

#pragma once


#include <bit>

#include <milo/meta.h>

#include <milo/internal/base.h>
#include <milo/internal/compiler.h>


namespace milo::internal
{
    constexpr bool endian_big = std::endian::big == std::endian::native;
    
    constexpr bool endian_little = std::endian::little == std::endian::native;
    
    template<
        meta::integral_u t_int
    >
    constexpr auto
    endian_swap_sw(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if constexpr (meta::same<t_int, uint8_t>)
        {
            return a_int;
        }
        
        if constexpr (meta::same<t_int, uint16_t>)
        {
            return
                a_int << 8 |
                a_int >> 8;
        }
        
        if constexpr (meta::same<t_int, uint32_t>)
        {
            return
                a_int >> 24 |
                a_int << 24 |
                (a_int & 0x00ff0000) >> 8 |
                (a_int & 0x0000ff00) << 8;
        }
        
        if constexpr (meta::same<t_int, uint64_t>)
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
        meta::integral_u t_int
    >
    constexpr auto
    endian_swap_bi(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if constexpr (meta::same<t_int, uint8_t>)
        {
            return a_int;
        }
        
        if constexpr (meta::same<t_int, uint16_t>)
        {
            #if MILO_INTERNAL_COMPILER_GCC || MILO_INTERNAL_COMPILER_CLANG || MILO_INTERNAL_COMPILER_ICC
            return __builtin_bswap16(a_int);
            #endif
            
            #if MILO_INTERNAL_COMPILER_MSVC
            return _byteswap_ushort(a_int);
            #endif
        }
        
        if constexpr (meta::same<t_int, uint32_t>)
        {
            #if MILO_INTERNAL_COMPILER_GCC || MILO_INTERNAL_COMPILER_CLANG || MILO_INTERNAL_COMPILER_ICC
            return __builtin_bswap32(a_int);
            #endif
            
            #if MILO_INTERNAL_COMPILER_MSVC
            return _byteswap_ulong(a_int);
            #endif
        }
        
        if constexpr (meta::same<t_int, uint64_t>)
        {
            #if MILO_INTERNAL_COMPILER_GCC || MILO_INTERNAL_COMPILER_CLANG || MILO_INTERNAL_COMPILER_ICC
            return __builtin_bswap64(a_int);
            #endif
            
            #if MILO_INTERNAL_COMPILER_MSVC
            return _byteswap_uint64(a_int);
            #endif
        }
        
        static_assert(sizeof(t_int) <= 8);
    }
    
    template<
        meta::integral_u t_int
    >
    constexpr auto
    endian_swap(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        if MILO_INTERNAL_CONSTEVAL
        {
            return endian_swap_sw(a_int);
        }
        else
        {
            #if MILO_INTERNAL_COMPILER_GCC || \
                MILO_INTERNAL_COMPILER_CLANG || \
                MILO_INTERNAL_COMPILER_ICC || \
                MILO_INTERNAL_COMPILER_MSVC
            
            return endian_swap_bi(a_int);
            
            #else
            
            return endian_swap_sw(a_int);
            
            #endif
        }
    }
    
    template<
        meta::integral_s t_int
    >
    constexpr auto
    endian_swap(
        t_int a_int
    ) noexcept(true) -> t_int
    {
        using to_type = t_int;
        using as_type = meta::to_integral_unsigned<t_int>;
        
        return to_type(endian_swap(as_type(a_int)));
    }
    
    template<
        meta::integral t_int
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
        meta::integral t_int
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
