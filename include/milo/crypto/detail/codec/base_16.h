

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/error.h>


namespace milo::crypto::detail
{
    template<
        typename... t_options
    >
    class codec_base_16
    {
    public:
        
        struct properties
        {
            using codec [[maybe_unused]] = int;
            
            using codec_base_16 [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t ratio_bytes_size = 1;
        
        static
        constexpr size_t ratio_chars_size = 2;
    
    private:
        
        static
        constexpr auto
        encode(
            uint8_t a_0
        ) noexcept(true) -> char
        {
            auto t = char(a_0);
            
            t += (0x30 & -uintptr_t(a_0 < 0xa)) | (0x57 & -uintptr_t(a_0 >= 0xa));
            
            return t;
        }
        
        static
        constexpr auto
        decode(
            char a_0
        ) noexcept(true) -> uint8_t
        {
            auto t = uint8_t(a_0);
            
            t -= 0x30 & -uintptr_t(uintptr_t(a_0 - 0x30) <= 0x09);
            t -= 0x37 & -uintptr_t(uintptr_t(a_0 - 0x41) <= 0x05);
            t -= 0x57 & -uintptr_t(uintptr_t(a_0 - 0x61) <= 0x05);
            
            return t;
        }
    
    public:
        
        template<
            concepts::byte t_from,
            concepts::byte t_to
        >
        static
        constexpr auto
        encode(
            const t_from* a_from_ptr,
            size_t a_from_size,
            t_to* a_to_ptr
        ) noexcept(true) -> size_t
        {
            auto from_ptr = a_from_ptr;
            auto to_ptr = a_to_ptr;
            
            for (size_t i = 0; i < a_from_size; i += 1)
            {
                auto b = uint8_t(from_ptr[i]);
                
                to_ptr[0] = t_to(encode(b >> 4));
                to_ptr[1] = t_to(encode(b & 0b1111));
                
                to_ptr += 2;
            }
            
            return a_from_size * 2;
        }
        
        template<
            concepts::byte t_from
        >
        static
        constexpr auto
        encode_size(
            [[maybe_unused]]
            const t_from* a_from_ptr,
            size_t a_from_size
        ) noexcept(true) -> size_t
        {
            return a_from_size * 2;
        }
        
        template<
            concepts::byte t_from,
            concepts::byte t_to
        >
        static
        constexpr auto
        decode(
            const t_from* a_from_ptr,
            size_t a_from_size,
            t_to* a_to_ptr,
            error& a_error
        ) noexcept(true) -> size_t
        {
            auto from_ptr = a_from_ptr;
            auto to_ptr = a_to_ptr;
            
            auto full_size = a_from_size >> 1;
            auto last_size = a_from_size & 1;
            
            a_error.clear();
            
            if (last_size)
            {
                auto c_0 = char(from_ptr[0]);
                auto b_0 = decode(c_0);
                
                [[unlikely]]
                if (c_0 == b_0)
                {
                    a_error = error::codec_base_16_decode;
                    return 0;
                }
                
                to_ptr[0] = t_to(b_0);
                
                to_ptr += 1;
                from_ptr += 1;
            }
            
            for (size_t i = last_size; i < full_size; i += 1)
            {
                auto c_0 = char(from_ptr[0]);
                auto c_1 = char(from_ptr[1]);
                auto b_0 = decode(c_0);
                auto b_1 = decode(c_1);
                
                [[unlikely]]
                if (c_0 == b_0 || c_1 == b_1)
                {
                    a_error = error::codec_base_16_decode;
                    return from_ptr - a_from_ptr;
                }
                
                to_ptr[i] = t_to(b_0 << 4 | b_1);
                
                from_ptr += 2;
            }
            
            return full_size + last_size;
        }
        
        template<
            concepts::byte t_from
        >
        static
        constexpr auto
        decode_size(
            [[maybe_unused]]
            const t_from* a_from_ptr,
            size_t a_from_size
        ) noexcept(true) -> size_t
        {
            return (a_from_size >> 1) + (a_from_size & 1);
        }
    };
}
