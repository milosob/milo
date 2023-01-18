

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
    
    public:
        
        template<
            meta::byte t_src
        >
        static
        constexpr auto
        blocks(
            const t_src* a_src_ptr,
            size_t a_blocks,
            uint32_t* a_h_ptr
        ) noexcept(true) -> void
        {
            uint32_t schedule[80];
            uint32_t a, b, c, d, e;
            uint32_t ap, bp, cp, dp, ep;
            uint32_t t_1;
            
            a = a_h_ptr[0];
            b = a_h_ptr[1];
            c = a_h_ptr[2];
            d = a_h_ptr[3];
            e = a_h_ptr[4];
            
            /*
             * @formatter:off
             */
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_CH(a_0, a_1, a_2) \
                    ((a_2) ^ ((a_0) & ((a_1) ^ (a_2))))                       \

            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_MAJ(a_0, a_1, a_2) \
                    (((a_0) & (a_1)) ^ ((a_2) & ((a_0) ^ (a_1))))              \
            
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_PARITY(a_0, a_1, a_2) \
                    ((a_0) ^ (a_1) ^ (a_2))                                       \
            
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_T_1(a_a, a_b, a_c, a_d, a_e, a_k, a_s, a_function) \
                    (                                                                                          \
                        internal::bit_rotl(a_a, 5) +                                                           \
                        MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_##a_function(a_b, a_c, a_d) +                  \
                        a_e +                                                                                  \
                        a_k +                                                                                  \
                        a_s                                                                                    \
                    )
            
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_00_15(a_i) \
                    schedule[a_i] = internal::memory_load_be<uint32_t>(a_src_ptr, a_i);
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_16_79(a_i) \
                    schedule[a_i] = internal::bit_rotl(                         \
                        schedule[a_i - 3] ^                                     \
                        schedule[a_i - 8] ^                                     \
                        schedule[a_i - 14] ^                                    \
                        schedule[a_i - 16],                                     \
                        1                                                       \
                    );                                                          \
    
            #define MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_ROUND(a_i, a_k, a_function) \
                    t_1 = MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_T_1(a, b, c, d, e, k[a_k], schedule[a_i], a_function); \
                                                                                        \
                    e = d;                                                              \
                    d = c;                                                              \
                    c = internal::bit_rotr(b, 2);                                       \
                    b = a;                                                              \
                    a = t_1;                                                            \
            
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
                
                /*
                 * @formatter:off
                 */
                
                for (size_t j = 0; j < 16; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_00_15(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_ROUND(j, 0, CH)
                }
                
                for (size_t j = 16; j < 20; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_16_79(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_ROUND(j, 0, CH)
                }
                
                for (size_t j = 20; j < 40; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_16_79(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_ROUND(j, 1, PARITY)
                }
                
                for (size_t j = 40; j < 60; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_16_79(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_ROUND(j, 2, MAJ)
                }
                
                for (size_t j = 60; j < 80; j += 1)
                {
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_SCHEDULE_16_79(j)
                    MILO_PRIMITIVE_DETAIL_HASH_SHA_1_160_SW_ROUND(j, 3, PARITY)
                }
    
                /*
                 * @formatter:on
                 */
                
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
