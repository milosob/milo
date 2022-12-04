

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>

#include <milo/inner/arch.h>
#include <milo/inner/compiler.h>
#include <milo/inner/memory.h>


#if MILO_ARCH_X86_ISE_SSE_1 && \
    MILO_ARCH_X86_ISE_SSE_2 && \
    MILO_ARCH_X86_ISE_SSE_4_1 && \
    MILO_ARCH_X86_ISE_SHA_1
    
    
#include <immintrin.h>


namespace milo::crypto::detail
{
    class hash_sha_1_160_impl_hw_x86_v_1
    {
    public:
        
        struct requirements
        {
            struct arch
            {
                struct x86
                {
                    struct ise
                    {
                        using sse_1 = int;
                        
                        using sse_2 = int;
                        
                        using sse_4_1 = int;
                        
                        using sha_1 = int;
                    };
                };
            };
        };
    
    public:
        
        static
        constexpr size_t block_size = 64;
    
    public:
        
        template<
            concepts::byte t_src
        >
        static
        constexpr auto
        process(
            const t_src* a_src_ptr,
            size_t a_blocks,
            uint32_t* a_h_ptr
        ) noexcept(true) -> void
        {
            using load_type = __m128i_u;
            using stor_type = __m128i_u;
            using vect_type = __m128i;
            
            vect_type state_0;
            vect_type state_1;
            vect_type state_2;
            vect_type state_0_save;
            vect_type state_1_save;
            
            state_0 = _mm_loadu_si128(
                reinterpret_cast<const load_type*>(a_h_ptr)
            );
            
            state_0 = _mm_shuffle_epi32(
                state_0,
                0b00011011
            );
            
            state_1 = _mm_setzero_si128();
            
            state_1 = _mm_insert_epi32(
                state_1,
                int(a_h_ptr[4]),
                3
            );
            
            constexpr auto shuffle_mask = vect_type
                {
                    0x08090a0b0c0d0e0f,
                    0x0001020304050607
                };
            
            for (size_t i = 0; i < a_blocks; i += 1)
            {
                vect_type msg_0;
                vect_type msg_1;
                vect_type msg_2;
                vect_type msg_3;
                
                state_0_save = state_0;
                state_1_save = state_1;
                
                /*
                 * Rounds 0 to 3.
                 */
                
                msg_0 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr)
                );
                
                msg_0 = _mm_shuffle_epi8(
                    msg_0,
                    shuffle_mask
                );
                
                state_1 = _mm_add_epi32(
                    state_1,
                    msg_0
                );
                
                state_2 = state_0;
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    0
                );
                
                /*
                 * Rounds 4 to 7.
                 */
                
                msg_1 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr) + 1
                );
                
                msg_1 = _mm_shuffle_epi8(
                    msg_1,
                    shuffle_mask
                );
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_1
                );
                
                state_1 = state_0;
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    0
                );
                
                msg_0 = _mm_sha1msg1_epu32(
                    msg_0,
                    msg_1
                );
                
                /*
                 * Rounds 8 to 11.
                 */
                
                msg_2 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr) + 2
                );
                
                msg_2 = _mm_shuffle_epi8(
                    msg_2,
                    shuffle_mask
                );
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_2
                );
                
                state_2 = state_0;
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    0
                );
                
                msg_1 = _mm_sha1msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                msg_0 = _mm_xor_si128(
                    msg_0,
                    msg_2
                );
                
                /*
                 * Rounds 12 to 15.
                 */
                
                msg_3 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr) + 3
                );
                
                msg_3 = _mm_shuffle_epi8(
                    msg_3,
                    shuffle_mask
                );
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_3
                );
                
                state_1 = state_0;
                
                msg_0 = _mm_sha1msg2_epu32(
                    msg_0,
                    msg_3
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    0
                );
                
                msg_2 = _mm_sha1msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                msg_1 = _mm_xor_si128(
                    msg_1,
                    msg_3
                );
                
                /*
                 * Rounds 16 to 19.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_0
                );
                
                state_2 = state_0;
                
                msg_1 = _mm_sha1msg2_epu32(
                    msg_1,
                    msg_0
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    0
                );
                
                msg_3 = _mm_sha1msg1_epu32(
                    msg_3,
                    msg_0
                );
                
                msg_2 = _mm_xor_si128(
                    msg_2,
                    msg_0
                );
                
                /*
                 * Rounds 20 to 23.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_1
                );
                
                state_1 = state_0;
                
                msg_2 = _mm_sha1msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    1
                );
                
                msg_0 = _mm_sha1msg1_epu32(
                    msg_0,
                    msg_1
                );
                
                msg_3 = _mm_xor_si128(
                    msg_3,
                    msg_1
                );
                
                /*
                 * Rounds 24 to 27.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_2
                );
                
                state_2 = state_0;
                
                msg_3 = _mm_sha1msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    1
                );
                
                msg_1 = _mm_sha1msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                msg_0 = _mm_xor_si128(
                    msg_0,
                    msg_2
                );
                
                /*
                 * Rounds 28 to 31.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_3
                );
                
                state_1 = state_0;
                
                msg_0 = _mm_sha1msg2_epu32(
                    msg_0,
                    msg_3
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    1
                );
                
                msg_2 = _mm_sha1msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                msg_1 = _mm_xor_si128(
                    msg_1,
                    msg_3
                );
                
                /*
                 * Rounds 32 to 35.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_0
                );
                
                state_2 = state_0;
                
                msg_1 = _mm_sha1msg2_epu32(
                    msg_1,
                    msg_0
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    1
                );
                
                msg_3 = _mm_sha1msg1_epu32(
                    msg_3,
                    msg_0
                );
                
                msg_2 = _mm_xor_si128(
                    msg_2,
                    msg_0
                );
                
                /*
                 * Rounds 36 to 39.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_1
                );
                
                state_1 = state_0;
                
                msg_2 = _mm_sha1msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    1
                );
                
                msg_0 = _mm_sha1msg1_epu32(
                    msg_0,
                    msg_1
                );
                
                msg_3 = _mm_xor_si128(
                    msg_3,
                    msg_1
                );
                
                /*
                 * Rounds 40 to 43.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_2
                );
                
                state_2 = state_0;
                
                msg_3 = _mm_sha1msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    2
                );
                
                msg_1 = _mm_sha1msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                msg_0 = _mm_xor_si128(
                    msg_0,
                    msg_2
                );
                
                /*
                 * Rounds 44 to 47.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_3
                );
                
                state_1 = state_0;
                
                msg_0 = _mm_sha1msg2_epu32(
                    msg_0,
                    msg_3
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    2
                );
                
                msg_2 = _mm_sha1msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                msg_1 = _mm_xor_si128(
                    msg_1,
                    msg_3
                );
                
                /*
                 * Rounds 48 to 51.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_0
                );
                
                state_2 = state_0;
                
                msg_1 = _mm_sha1msg2_epu32(
                    msg_1,
                    msg_0
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    2
                );
                
                msg_3 = _mm_sha1msg1_epu32(
                    msg_3,
                    msg_0
                );
                
                msg_2 = _mm_xor_si128(
                    msg_2,
                    msg_0
                );
                
                /*
                 * Rounds 52 to 55.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_1
                );
                
                state_1 = state_0;
                
                msg_2 = _mm_sha1msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    2
                );
                
                msg_0 = _mm_sha1msg1_epu32(
                    msg_0,
                    msg_1
                );
                
                msg_3 = _mm_xor_si128(
                    msg_3,
                    msg_1
                );
                
                /*
                 * Rounds 56 to 59.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_2
                );
                
                state_2 = state_0;
                
                msg_3 = _mm_sha1msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    2
                );
                
                msg_1 = _mm_sha1msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                msg_0 = _mm_xor_si128(
                    msg_0,
                    msg_2
                );
                
                /*
                 * Rounds 60 to 63.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_3
                );
                
                state_1 = state_0;
                
                msg_0 = _mm_sha1msg2_epu32(
                    msg_0,
                    msg_3
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    3
                );
                
                msg_2 = _mm_sha1msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                msg_1 = _mm_xor_si128(
                    msg_1,
                    msg_3
                );
                
                /*
                 * Rounds 64 to 67.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_0
                );
                
                state_2 = state_0;
                
                msg_1 = _mm_sha1msg2_epu32(
                    msg_1,
                    msg_0
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    3
                );
                
                msg_3 = _mm_sha1msg1_epu32(
                    msg_3,
                    msg_0
                );
                
                msg_2 = _mm_xor_si128(
                    msg_2,
                    msg_0
                );
                
                /*
                 * Rounds 68 to 71.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_1
                );
                
                state_1 = state_0;
                
                msg_2 = _mm_sha1msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    3
                );
                
                msg_3 = _mm_xor_si128(
                    msg_3,
                    msg_1
                );
                
                /*
                 * Rounds 72 to 75.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    msg_2
                );
                
                state_2 = state_0;
                
                msg_3 = _mm_sha1msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_1,
                    3
                );
                
                /*
                 * Rounds 76 to 79.
                 */
                
                state_2 = _mm_sha1nexte_epu32(
                    state_2,
                    msg_3
                );
                
                state_1 = state_0;
                
                state_0 = _mm_sha1rnds4_epu32(
                    state_0,
                    state_2,
                    3
                );
                
                /*
                 * Merge state.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_1,
                    state_1_save
                );
                
                state_0 = _mm_add_epi32(
                    state_0,
                    state_0_save
                );
                
                a_src_ptr += block_size;
            }
            
            state_0 = _mm_shuffle_epi32(
                state_0,
                0b00011011
            );
            
            _mm_storeu_si128(
                reinterpret_cast<stor_type*>(a_h_ptr),
                state_0
            );
            
            a_h_ptr[4] = _mm_extract_epi32(
                state_1,
                3
            );
        }
    };
}

#else

namespace milo::crypto::detail
{
    using hash_sha_1_160_impl_hw_x86_v_1 = void;
}

#endif
