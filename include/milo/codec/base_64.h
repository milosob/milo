

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/error.h>


namespace milo::codec
{
    /**
     * Base64 coder.
     * @tparam t_options
     */
    template<
        typename... t_options
    >
    class base_64_basic
    {
    public:
        
        using type = base_64_basic;
    
    public:
        
        struct properties
        {
            using codec_type [[maybe_unused]] = type;
            
            using codec_base_64_type [[maybe_unused]] = type;
        };
    
    public:
        
        static
        constexpr size_t ratio_bytes_size = 3;
        
        static
        constexpr size_t ratio_chars_size = 4;
    
    private:
        
        static
        constexpr auto
        encode(
            uint8_t a_0
        ) noexcept(true) -> char
        {
            auto t = char(a_0);
            
            t += (
                (-uintptr_t(a_0 < 0x1a) & 0x41) |
                (-uintptr_t(uintptr_t(a_0 - 0x1a) < 0x1a) & 0x47) |
                (-uintptr_t(uintptr_t(a_0 - 0x34) < 0x0a) & 0xfc) |
                (-uintptr_t(a_0 == 0x3e) & 0xed) |
                (-uintptr_t(a_0 == 0x3f) & 0xf0)
            );
            
            return t;
        }
        
        static
        constexpr auto
        decode(
            char a_0
        ) noexcept(true) -> uint8_t
        {
            auto t = uint8_t(a_0);
            
            t -= (
                (-uintptr_t(uintptr_t(a_0 - 0x41) <= 0x19) & 0x41) |
                (-uintptr_t(uintptr_t(a_0 - 0x61) <= 0x19) & 0x47) |
                (-uintptr_t(uintptr_t(a_0 - 0x30) <= 0x09) & 0xfc) |
                (-uintptr_t(uintptr_t(a_0 == 0x2b)) & 0xed) |
                (-uintptr_t(uintptr_t(a_0 == 0x2f)) & 0xf0)
            );
            
            return t;
        }
    
    public:
        
        /**
         * This function encodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @param a_to_ptr
         * To pointer.
         * @return
         * To size.
         */
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
            
            auto full_size = a_from_size / 3;
            auto last_size = a_from_size % 3;
            
            for (size_t i = 0; i < full_size; i += 1)
            {
                auto b_0 = uint8_t(from_ptr[0]);
                auto b_1 = uint8_t(from_ptr[1]);
                auto b_2 = uint8_t(from_ptr[2]);
                auto c_0 = encode((b_0 >> 2));
                auto c_1 = encode((b_0 & 0b00000011) << 4 | b_1 >> 4);
                auto c_2 = encode((b_1 & 0b00001111) << 2 | b_2 >> 6);
                auto c_3 = encode((b_2 & 0b00111111));
                
                to_ptr[0] = t_to(c_0);
                to_ptr[1] = t_to(c_1);
                to_ptr[2] = t_to(c_2);
                to_ptr[3] = t_to(c_3);
                
                to_ptr += 4;
                from_ptr += 3;
            }
            
            if (last_size)
            {
                auto last = size_t(last_size == 2);
                auto mask = -last;
                auto mask_neg = ~mask;
                
                auto b_0 = uint8_t(from_ptr[0]);
                auto b_1 = uint8_t(from_ptr[0 + last] & mask);
                auto c_0 = (encode((b_0 >> 2)));
                auto c_1 = (encode((b_0 & 0b00000011) << 4 | b_1 >> 4));
                auto c_2 = (encode((b_1 & 0b00001111) << 2) & mask) | ('=' & mask_neg);
                auto c_3 = '=';
                
                to_ptr[0] = t_to(c_0);
                to_ptr[1] = t_to(c_1);
                to_ptr[2] = t_to(c_2);
                to_ptr[3] = t_to(c_3);
                
                full_size += 1;
            }
            
            return full_size * 4;
        }
        
        /**
         * This function calculates minimum bytes size after encoding.
         *
         * @tparam t_from
         * From type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @return
         * To size.
         */
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
            return (a_from_size * 4 / 3 + 3) & ~0b11;
        }
        
        /**
         * This function decodes bytes.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @param a_to_ptr
         * To pointer.
         * @param a_error
         * Error.
         * @return
         * To size.
         */
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
            
            auto full_size = a_from_size / 4;
            auto last_size = a_from_size % 4;
            
            a_error.clear();
            
            [[unlikely]]
            if (last_size != 0)
            {
                a_error = error::codec_base_64_decode;
                return 0;
            }
            
            for (size_t i = 1; i < full_size; i += 1)
            {
                auto c_0 = char(from_ptr[0]);
                auto c_1 = char(from_ptr[1]);
                auto c_2 = char(from_ptr[2]);
                auto c_3 = char(from_ptr[3]);
                auto b_0 = decode(c_0);
                auto b_1 = decode(c_1);
                auto b_2 = decode(c_2);
                auto b_3 = decode(c_3);
                
                [[unlikely]]
                if (
                    c_0 == b_0 ||
                    c_1 == b_1 ||
                    c_2 == b_2 ||
                    c_3 == b_3)
                {
                    a_error = error::codec_base_64_decode;
                    return to_ptr - a_to_ptr;
                }
                
                to_ptr[0] = t_to(b_0 << 2 | b_1 >> 4);
                to_ptr[1] = t_to(b_1 << 4 | b_2 >> 2);
                to_ptr[2] = t_to(b_2 << 6 | b_3);
                
                to_ptr += 3;
                from_ptr += 4;
            }
            
            if (full_size > 0)
            {
                auto c_0 = char(from_ptr[0]);
                auto c_1 = char(from_ptr[1]);
                auto c_2 = char(from_ptr[2]);
                auto c_3 = char(from_ptr[3]);
                auto b_0 = decode(c_0);
                auto b_1 = decode(c_1);
                auto b_2 = decode(c_2);
                auto b_3 = decode(c_3);
                auto p_0 = c_2 == '=';
                auto p_1 = c_3 == '=';
                
                [[unlikely]]
                if (
                    c_0 == b_0 ||
                    c_1 == b_1 ||
                    (p_0 && !p_1) ||
                    (c_2 == b_2 && !p_0) ||
                    (c_3 == b_3 && !p_1))
                {
                    a_error = error::codec_base_64_decode;
                    return to_ptr - a_to_ptr;
                }
                
                to_ptr[0] = t_to(b_0 << 2 | b_1 >> 4);
                
                if (!p_0)
                {
                    to_ptr[1] = t_to(b_1 << 4 | b_2 >> 2);
                }
                
                if (!p_1)
                {
                    to_ptr[2] = t_to(b_2 << 6 | b_3);
                }
                
                return to_ptr - a_to_ptr + 3 - size_t(p_0) - size_t(p_1);
            }
            
            return 0;
        }
        
        /**
         * This function calculates minimum bytes size after decoding.
         *
         * @tparam t_from
         * From type.
         * @param a_from_ptr
         * From pointer.
         * @param a_from_size
         * From size.
         * @return
         * To size.
         */
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
            size_t size = a_from_size * 3 / 4;
            
            if (a_from_size > 1 && char(a_from_ptr[a_from_size - 1]) == '=')
            {
                size -= 1;
                size -= a_from_size > 2 && char(a_from_ptr[a_from_size - 2]) == '=';
            }
            
            return size;
        }
    };
    
    /**
     * Base64 coder.
     */
    using base_64 = base_64_basic<>;
}
