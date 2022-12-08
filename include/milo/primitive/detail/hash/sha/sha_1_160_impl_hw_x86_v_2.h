

#pragma once


#include <milo/inner.h>


#if MILO_ARCH_X86_ISE_SSE_1 && \
    MILO_ARCH_X86_ISE_SSE_2 && \
    MILO_ARCH_X86_ISE_SSE_4_1 && \
    MILO_ARCH_X86_ISE_SHA_1 && \
    MILO_COMPILER_CLANG
    
#include <immintrin.h>


namespace milo::primitive::detail
{
    class hash_sha_1_160_impl_hw_x86_v_2
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
            using load_type = __m128i_u;
            using stor_type = __m128i_u;
            using vect_type = __m128i;
            
            vect_type schedule[20];
            vect_type state_0;
            vect_type state_1;
            vect_type state_2;
    
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
                for (size_t j = 0; j < 4; j += 1)
                {
                    schedule[j] = _mm_shuffle_epi8(
                        _mm_loadu_si128(
                            reinterpret_cast<const load_type*>(a_src_ptr) + j
                        ),
                        shuffle_mask
                    );
                }
                
                for (size_t j = 4; j < 20; j += 1)
                {
                    schedule[j] = _mm_sha1msg2_epu32(
                        _mm_xor_si128(
                            _mm_sha1msg1_epu32(
                                schedule[j - 4],
                                schedule[j - 3]
                            ),
                            schedule[j - 2]
                        ),
                        schedule[j - 1]
                    );
                }
                
                /*
                 * Adding curr e to schedule[0].
                 */
                
                schedule[0] = _mm_add_epi32(
                    state_1,
                    schedule[0]
                );
                
                /*
                 * Adding next e to schedule[1].
                 */
                
                schedule[1] = _mm_sha1nexte_epu32(
                    state_0,
                    schedule[1]
                );
                
                /*
                 * Rounds 0 to 3.
                 */
                
                state_2 = state_0;
                
                state_2 = _mm_sha1rnds4_epu32(
                    state_2,
                    schedule[0],
                    0
                );
                
                /*
                 * Rounds 4 to 19.
                 */
                
                for (size_t j = 1; j < 5; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_2,
                        schedule[j + 1]
                    );
    
                    state_2 = _mm_sha1rnds4_epu32(
                        state_2,
                        schedule[j],
                        0
                    );
                }
    
                /*
                 * Rounds 20 to 39.
                 */
                
                for (size_t j = 5; j < 10; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_2,
                        schedule[j + 1]
                    );
    
                    state_2 = _mm_sha1rnds4_epu32(
                        state_2,
                        schedule[j],
                        1
                    );
                }
    
                /*
                 * Rounds 39 to 59.
                 */
                
                for (size_t j = 10; j < 15; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_2,
                        schedule[j + 1]
                    );
    
                    state_2 = _mm_sha1rnds4_epu32(
                        state_2,
                        schedule[j],
                        2
                    );
                }
    
                /*
                 * Rounds 60 to 75.
                 */
                
                for (size_t j = 15; j < 19; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_2,
                        schedule[j + 1]
                    );
    
                    state_2 = _mm_sha1rnds4_epu32(
                        state_2,
                        schedule[j],
                        3
                    );
                }
                
                /*
                 * Merge state.
                 */
                
                state_1 = _mm_sha1nexte_epu32(
                    state_2,
                    state_1
                );
    
                /*
                 * Rounds 76 to 79.
                 */
                
                state_2 = _mm_sha1rnds4_epu32(
                    state_2,
                    schedule[19],
                    3
                );
    
                /*
                 * Merge state.
                 */
                
                state_0 = _mm_add_epi32(
                    state_2,
                    state_0
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

namespace milo::primitive::detail
{
    using hash_sha_1_160_impl_hw_x86_v_2 = void;
}

#endif
