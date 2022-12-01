

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>

#include <milo/detail/arch.h>


#if MILO_ARCH_X86_ISE_SSE_1 && \
    MILO_ARCH_X86_ISE_SSE_2 && \
    MILO_ARCH_X86_ISE_SSE_4_1 && \
    MILO_ARCH_X86_ISE_SHA_1


#include <immintrin.h>


namespace milo::detail
{
    class hash_sha_1_160_impl_hw_x86_sha
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
            
            vect_type schedule[20];
            vect_type state_0;
            vect_type state_1;
            vect_type state_t;
            
            state_0 = _mm_shuffle_epi32(
                _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_h_ptr)
                ),
                0b00011011
            );
            
            state_1 = _mm_setzero_si128();
            state_1 = _mm_insert_epi32(
                state_1,
                int(a_h_ptr[4]),
                3
            );
            
            /*
             * Memory before load:
             *     | 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f |
             *
             * Register after load:
             * 127 | 0f 0e 0d 0c 0b 0a 09 08 07 06 05 04 03 02 01 00 | 0
             *
             * Register to schedule interpretation:
             * W0 := a[127:96]
             * W1 := a[95:64]
             * W2 := a[63:32]
             * W3 := a[31:0]
             *
             * After the load W0 is W3, correction is required.
             *
             * Shuffle control mask as 128-bit integer:
             * 127 | 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f | 0
             *
             * At the same time words byte ordering is set to big-endian.
             *
             * Packed right to left order:
             * [0] = 0x08090a0b0c0d0e0f
             * [1] = 0x0001020304050607
             */
            
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
                 * 4 rounds.
                 */
                state_t = _mm_sha1rnds4_epu32(
                    state_0,
                    schedule[0],
                    0
                );
                
                /*
                 * 16 rounds.
                 */
                for (size_t j = 1; j < 5; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_t,
                        schedule[j + 1]
                    );
                    
                    state_t = _mm_sha1rnds4_epu32(
                        state_t,
                        schedule[j],
                        0
                    );
                }
                
                /*
                 * 20 rounds.
                 */
                for (size_t j = 5; j < 10; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_t,
                        schedule[j + 1]
                    );
                    
                    state_t = _mm_sha1rnds4_epu32(
                        state_t,
                        schedule[j],
                        1
                    );
                }
                
                /*
                 * 20 rounds.
                 */
                for (size_t j = 10; j < 15; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_t,
                        schedule[j + 1]
                    );
                    
                    state_t = _mm_sha1rnds4_epu32(
                        state_t,
                        schedule[j],
                        2
                    );
                }
                
                /*
                 * 16 rounds.
                 */
                for (size_t j = 15; j < 19; j += 1)
                {
                    schedule[j + 1] = _mm_sha1nexte_epu32(
                        state_t,
                        schedule[j + 1]
                    );
                    
                    state_t = _mm_sha1rnds4_epu32(
                        state_t,
                        schedule[j],
                        3
                    );
                }
                
                /*
                 * Update e.
                 */
                state_1 = _mm_sha1nexte_epu32(
                    state_t,
                    state_1
                );
                
                /*
                 * 4 rounds.
                 */
                state_t = _mm_sha1rnds4_epu32(
                    state_t,
                    schedule[19],
                    3
                );
                
                /*
                 * Update a b c d.
                 */
                state_0 = _mm_add_epi32(
                    state_t,
                    state_0
                );
                
                a_src_ptr += block_size;
            }
            
            _mm_storeu_si128(
                reinterpret_cast<stor_type*>(a_h_ptr),
                _mm_shuffle_epi32(
                    state_0,
                    0b00011011
                )
            );
            
            a_h_ptr[4] = _mm_extract_epi32(
                state_1,
                3
            );
        }
    };
}

#else


namespace milo::detail
{
    using hash_sha_1_160_impl_hw_x86_sha = void;
}


#endif
