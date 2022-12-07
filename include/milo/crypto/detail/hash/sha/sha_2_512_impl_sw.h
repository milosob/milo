

#pragma once


#include <milo/inner.h>


namespace milo::crypto::detail
{
    class hash_sha_2_512_impl_sw
    {
    public:
        
        static
        constexpr size_t block_size = 128;
    
    private:
        
        static
        constexpr uint64_t k[80] = {
            0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
            0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
            0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
            0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
            0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
            0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
            0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
            0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
            0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
            0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
            0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
            0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
            0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
            0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
            0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
            0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
            0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
            0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
            0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
            0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
        };
    
    private:
        
        static
        constexpr auto
        ch(
            uint64_t a_0,
            uint64_t a_1,
            uint64_t a_2
        ) noexcept(true) -> uint64_t
        {
            return a_2 ^ (a_0 & (a_1 ^ a_2));
        }
        
        static
        constexpr auto
        maj(
            uint64_t a_0,
            uint64_t a_1,
            uint64_t a_2
        ) noexcept(true) -> uint64_t
        {
            return (a_0 & a_1) ^ (a_2 & (a_0 ^ a_1));
        }
        
        static
        constexpr auto
        sigma_lower_0(
            uint64_t a_0
        ) noexcept(true) -> uint64_t
        {
            return
                inner::bit_rotr(
                    a_0,
                    1
                ) ^
                inner::bit_rotr(
                    a_0,
                    8
                ) ^
                a_0 >> 7;
        }
        
        static
        constexpr auto
        sigma_lower_1(
            uint64_t a_0
        ) noexcept(true) -> uint64_t
        {
            return
                inner::bit_rotr(
                    a_0,
                    19
                ) ^
                inner::bit_rotr(
                    a_0,
                    61
                ) ^
                a_0 >> 6;
        }
        
        static
        constexpr auto
        sigma_upper_0(
            uint64_t a_0
        ) noexcept(true) -> uint64_t
        {
            return
                inner::bit_rotr(
                    a_0,
                    28
                ) ^
                inner::bit_rotr(
                    a_0,
                    34
                ) ^
                inner::bit_rotr(
                    a_0,
                    39
                );
        }
        
        static
        constexpr auto
        sigma_upper_1(
            uint64_t a_w_0
        ) noexcept(true) -> uint64_t
        {
            return
                inner::bit_rotr(
                    a_w_0,
                    14
                ) ^
                inner::bit_rotr(
                    a_w_0,
                    18
                ) ^
                inner::bit_rotr(
                    a_w_0,
                    41
                );
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
            uint64_t* a_h_ptr
        ) noexcept(true) -> void
        {
            uint64_t schedule[80];
            uint64_t a, b, c, d, e, f, g, h;
            uint64_t ap, bp, cp, dp, ep, fp, gp, hp;
            
            a = a_h_ptr[0];
            b = a_h_ptr[1];
            c = a_h_ptr[2];
            d = a_h_ptr[3];
            e = a_h_ptr[4];
            f = a_h_ptr[5];
            g = a_h_ptr[6];
            h = a_h_ptr[7];
            
            for (size_t i = 0; i < a_blocks; i += 1)
            {
                ap = a;
                bp = b;
                cp = c;
                dp = d;
                ep = e;
                fp = f;
                gp = g;
                hp = h;
                
                for (size_t j = 0; j < 16; j += 1)
                {
                    schedule[j] = inner::memory_load_be<uint64_t>(
                        a_src_ptr,
                        j
                    );
                    
                    uint64_t t_1 =
                        h +
                        sigma_upper_1(
                            e
                        ) +
                        ch(
                            e,
                            f,
                            g
                        ) +
                        k[j] +
                        schedule[j];
                    
                    uint64_t t_2 =
                        sigma_upper_0(
                            a
                        ) +
                        maj(
                            a,
                            b,
                            c
                        );
                    
                    h = g;
                    g = f;
                    f = e;
                    e = d + t_1;
                    d = c;
                    c = b;
                    b = a;
                    a = t_1 + t_2;
                }
                
                for (size_t j = 16; j < 80; j += 1)
                {
                    schedule[j] =
                        sigma_lower_1(schedule[j - 2]) +
                        schedule[j - 7] +
                        sigma_lower_0(schedule[j - 15]) +
                        schedule[j - 16];
                    
                    uint64_t t_1 =
                        h +
                        sigma_upper_1(
                            e
                        ) +
                        ch(
                            e,
                            f,
                            g
                        ) +
                        k[j] +
                        schedule[j];
                    
                    uint64_t t_2 =
                        sigma_upper_0(
                            a
                        ) +
                        maj(
                            a,
                            b,
                            c
                        );
                    
                    h = g;
                    g = f;
                    f = e;
                    e = d + t_1;
                    d = c;
                    c = b;
                    b = a;
                    a = t_1 + t_2;
                }
                
                a += ap;
                b += bp;
                c += cp;
                d += dp;
                e += ep;
                f += fp;
                g += gp;
                h += hp;
                
                a_src_ptr += block_size;
            }
            
            a_h_ptr[0] = a;
            a_h_ptr[1] = b;
            a_h_ptr[2] = c;
            a_h_ptr[3] = d;
            a_h_ptr[4] = e;
            a_h_ptr[5] = f;
            a_h_ptr[6] = g;
            a_h_ptr[7] = h;
        }
    };
}
