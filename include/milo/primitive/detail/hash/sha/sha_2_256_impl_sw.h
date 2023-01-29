

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    class hash_sha_2_256_impl_sw
    {
    public:
        
        static
        constexpr size_t block_size = 64;
    
    private:
        
        static
        constexpr uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
            0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
            0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
            0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
            0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
            0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
            0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
            0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
            0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };
    
    public:
        
        template<
            meta::byte t_src
        >
        static
        constexpr auto
        blocks(
            size_t a_blocks,
            uint32_t* a_state_ptr,
            const t_src* a_src_ptr
        ) noexcept(true) -> void
        {
            uint32_t schedule[64];
            uint32_t a, b, c, d, e, f, g, h;
            uint32_t ap, bp, cp, dp, ep, fp, gp, hp;
            uint32_t t_1;
            uint32_t t_2;
            
            a = a_state_ptr[0];
            b = a_state_ptr[1];
            c = a_state_ptr[2];
            d = a_state_ptr[3];
            e = a_state_ptr[4];
            f = a_state_ptr[5];
            g = a_state_ptr[6];
            h = a_state_ptr[7];
            
            /*
             * @formatter:off
             */
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_CH(a_0, a_1, a_2) \
                    ((a_2) ^ ((a_0) & ((a_1) ^ (a_2))))                       \

            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_MAJ(a_0, a_1, a_2) \
                    (((a_0) & (a_1)) ^ ((a_2) & ((a_0) ^ (a_1))))              \

            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_LOWER_0(a_0) \
                    (internal::bit_rotr(a_0,  7) ^ internal::bit_rotr(a_0, 18) ^ (a_0) >> 3)
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_LOWER_1(a_0) \
                    (internal::bit_rotr(a_0, 17) ^ internal::bit_rotr(a_0, 19) ^ (a_0) >> 10)
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_UPPER_0(a_0) \
                    (internal::bit_rotr(a_0,  2) ^ internal::bit_rotr(a_0, 13) ^ internal::bit_rotr(a_0, 22))
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_UPPER_1(a_0) \
                    (internal::bit_rotr(a_0,  6) ^ internal::bit_rotr(a_0, 11) ^ internal::bit_rotr(a_0, 25))
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_T_1(a_e, a_f, a_g, a_h, a_k, a_s) \
                    (                                                                         \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_UPPER_1(a_e) +          \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_CH(a_e, a_f, a_g) +           \
                        a_h +                                                                 \
                        a_k +                                                                 \
                        a_s                                                                   \
                    )                                                                         \

            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_T_2(a_a, a_b, a_c)                \
                    (                                                                         \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_UPPER_0(a_a) +          \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_MAJ(a_a, a_b, a_c)            \
                    )                                                                         \

            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SCHEDULE_00_15(a_i) \
                    schedule[a_i] = internal::memory_load_be<uint32_t>(a_src_ptr, a_i);
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SCHEDULE_16_63(a_i)                     \
                    schedule[a_i] =                                                                 \
                        schedule[a_i - 7] +                                                         \
                        schedule[a_i - 16] +                                                        \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_LOWER_1(schedule[a_i - 2]) +  \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SIGMA_LOWER_0(schedule[a_i - 15]);  \

            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_ROUND(a_i) \
                    t_1 = MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_T_1(e, f, g, h, k[a_i], schedule[a_i]); \
                    t_2 = MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_T_2(a, b, c);                           \
                                                                       \
                    h = g;                                             \
                    g = f;                                             \
                    f = e;                                             \
                    e = d + t_1;                                       \
                    d = c;                                             \
                    c = b;                                             \
                    b = a;                                             \
                    a = t_1 + t_2;                                     \

            /*
             * @formatter:on
             */
            
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
                
                /*
                 * @formatter:off
                 */
    
                for (size_t j = 0; j < 16; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SCHEDULE_00_15(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_ROUND(j)
                }
    
                for (size_t j = 16; j < 64; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_SCHEDULE_16_63(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_2_256_SW_ROUND(j)
                }
    
                /*
                 * @formatter:on
                 */
                
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
            
            a_state_ptr[0] = a;
            a_state_ptr[1] = b;
            a_state_ptr[2] = c;
            a_state_ptr[3] = d;
            a_state_ptr[4] = e;
            a_state_ptr[5] = f;
            a_state_ptr[6] = g;
            a_state_ptr[7] = h;
        }
    };
}
