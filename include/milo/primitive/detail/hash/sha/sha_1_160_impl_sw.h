

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    class hash_sha_1_160_impl_sw
    {
    public:
        
        static
        constexpr size_t block_size = 64;
    
    private:
        
        static
        constexpr uint32_t k[4] = {
            0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6
        };
    
    private:
        
        static
        constexpr auto
        ch(
            uint32_t a_0,
            uint32_t a_1,
            uint32_t a_2
        ) noexcept(true) -> uint32_t
        {
            return a_2 ^ (a_0 & (a_1 ^ a_2));
        }
        
        static
        constexpr auto
        maj(
            uint32_t a_0,
            uint32_t a_1,
            uint32_t a_2
        ) noexcept(true) -> uint32_t
        {
            return (a_0 & a_1) ^ (a_2 & (a_0 ^ a_1));
        }
        
        static
        constexpr auto
        parity(
            uint32_t a_0,
            uint32_t a_1,
            uint32_t a_2
        ) noexcept(true) -> uint32_t
        {
            return a_0 ^ a_1 ^ a_2;
        }
    
    public:
        
        template<
            meta::byte t_src
        >
        static
        constexpr auto
        process(
            const t_src* a_src_ptr,
            size_t a_blocks,
            uint32_t* a_h_ptr
        ) noexcept(true) -> void
        {
            uint32_t schedule[80];
            uint32_t a, b, c, d, e;
            uint32_t ap, bp, cp, dp, ep;
            
            a = a_h_ptr[0];
            b = a_h_ptr[1];
            c = a_h_ptr[2];
            d = a_h_ptr[3];
            e = a_h_ptr[4];
            
            for (size_t i = 0; i < a_blocks; i += 1)
            {
                ap = a;
                bp = b;
                cp = c;
                dp = d;
                ep = e;
                
                for (size_t j = 0; j < 16; j += 1)
                {
                    schedule[j] = internal::memory_load_be<uint32_t>(
                        a_src_ptr,
                        j
                    );
                    
                    uint32_t t_1 =
                        internal::bit_rotl(
                            a,
                            5
                        ) +
                        ch(
                            b,
                            c,
                            d
                        ) +
                        e +
                        k[0] +
                        schedule[j];
                    
                    e = d;
                    d = c;
                    c = internal::bit_rotr(
                        b,
                        2
                    );
                    b = a;
                    a = t_1;
                }
                
                for (size_t j = 16; j < 20; j += 1)
                {
                    schedule[j] =
                        internal::bit_rotl(
                            schedule[j - 3] ^
                            schedule[j - 8] ^
                            schedule[j - 14] ^
                            schedule[j - 16],
                            1
                        );
                    
                    uint32_t t_1 =
                        internal::bit_rotl(
                            a,
                            5
                        ) +
                        ch(
                            b,
                            c,
                            d
                        ) +
                        e +
                        k[0] +
                        schedule[j];
                    
                    e = d;
                    d = c;
                    c = internal::bit_rotr(
                        b,
                        2
                    );
                    b = a;
                    a = t_1;
                }
                
                for (size_t j = 20; j < 40; j += 1)
                {
                    schedule[j] =
                        internal::bit_rotl(
                            schedule[j - 3] ^
                            schedule[j - 8] ^
                            schedule[j - 14] ^
                            schedule[j - 16],
                            1
                        );
                    
                    uint32_t t_1 =
                        internal::bit_rotl(
                            a,
                            5
                        ) +
                        parity(
                            b,
                            c,
                            d
                        ) +
                        e +
                        k[1] +
                        schedule[j];
                    
                    e = d;
                    d = c;
                    c = internal::bit_rotr(
                        b,
                        2
                    );
                    b = a;
                    a = t_1;
                }
                
                for (size_t j = 40; j < 60; j += 1)
                {
                    schedule[j] =
                        internal::bit_rotl(
                            schedule[j - 3] ^
                            schedule[j - 8] ^
                            schedule[j - 14] ^
                            schedule[j - 16],
                            1
                        );
                    
                    uint32_t t_1 =
                        internal::bit_rotl(
                            a,
                            5
                        ) +
                        maj(
                            b,
                            c,
                            d
                        ) +
                        e +
                        k[2] +
                        schedule[j];
                    
                    e = d;
                    d = c;
                    c = internal::bit_rotr(
                        b,
                        2
                    );
                    b = a;
                    a = t_1;
                }
                
                for (size_t j = 60; j < 80; j += 1)
                {
                    schedule[j] =
                        internal::bit_rotl(
                            schedule[j - 3] ^
                            schedule[j - 8] ^
                            schedule[j - 14] ^
                            schedule[j - 16],
                            1
                        );
                    
                    uint32_t t_1 =
                        internal::bit_rotl(
                            a,
                            5
                        ) +
                        parity(
                            b,
                            c,
                            d
                        ) +
                        e +
                        k[3] +
                        schedule[j];
                    
                    e = d;
                    d = c;
                    c = internal::bit_rotr(
                        b,
                        2
                    );
                    b = a;
                    a = t_1;
                }
                
                a += ap;
                b += bp;
                c += cp;
                d += dp;
                e += ep;
                
                a_src_ptr += block_size;
            }
            
            a_h_ptr[0] = a;
            a_h_ptr[1] = b;
            a_h_ptr[2] = c;
            a_h_ptr[3] = d;
            a_h_ptr[4] = e;
        }
    };
}
