

#pragma once


#include <milo/inner.h>


#if MILO_ARCH_X86_ISE_SSE_1 && \
    MILO_ARCH_X86_ISE_SSE_2 && \
    MILO_ARCH_X86_ISE_SSSE_3 && \
    MILO_ARCH_X86_ISE_SHA_2


#include <immintrin.h>


namespace milo::primitive::detail
{
    class hash_sha_2_256_impl_hw_x86_v_1
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
                        
                        using ssse_3 = int;
                        
                        using sha_2 = int;
                    };
                };
            };
        };
    
    private:
        
        static
        constexpr __m128i k[16] =
            {
                __m128i{int64_t(0x71374491428a2f98), int64_t(0xe9b5dba5b5c0fbcf)},
                __m128i{int64_t(0x59f111f13956c25b), int64_t(0xab1c5ed5923f82a4)},
                __m128i{int64_t(0x12835b01d807aa98), int64_t(0x550c7dc3243185be)},
                __m128i{int64_t(0x80deb1fe72be5d74), int64_t(0xc19bf1749bdc06a7)},
                __m128i{int64_t(0xefbe4786e49b69c1), int64_t(0x240ca1cc0fc19dc6)},
                __m128i{int64_t(0x4a7484aa2de92c6f), int64_t(0x76f988da5cb0a9dc)},
                __m128i{int64_t(0xa831c66d983e5152), int64_t(0xbf597fc7b00327c8)},
                __m128i{int64_t(0xd5a79147c6e00bf3), int64_t(0x1429296706ca6351)},
                __m128i{int64_t(0x2e1b213827b70a85), int64_t(0x53380d134d2c6dfc)},
                __m128i{int64_t(0x766a0abb650a7354), int64_t(0x92722c8581c2c92e)},
                __m128i{int64_t(0xa81a664ba2bfe8a1), int64_t(0xc76c51a3c24b8b70)},
                __m128i{int64_t(0xd6990624d192e819), int64_t(0x106aa070f40e3585)},
                __m128i{int64_t(0x1e376c0819a4c116), int64_t(0x34b0bcb52748774c)},
                __m128i{int64_t(0x4ed8aa4a391c0cb3), int64_t(0x682e6ff35b9cca4f)},
                __m128i{int64_t(0x78a5636f748f82ee), int64_t(0x8cc7020884c87814)},
                __m128i{int64_t(0xa4506ceb90befffa), int64_t(0xc67178f2bef9a3f7)},
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
            
            vect_type state_0;
            vect_type state_1;
            vect_type state_0_save;
            vect_type state_1_save;
            
            {
                vect_type tmp_0;
                vect_type tmp_1;
                
                tmp_0 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_h_ptr)
                );
                
                tmp_1 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_h_ptr) + 1
                );
                
                /*
                 * Shuffle DCBA to CDAB.
                 * Shuffle HGFE to GHEF.
                 * Interleave lo 64 bits of CDAB and GHEF to get ABEF.
                 * Interleave hi 64 bits of CDAB and GHEF to get CDGH.
                 */
                
                tmp_0 = _mm_shuffle_epi32(
                    tmp_0,
                    0b10110001
                );
                
                tmp_1 = _mm_shuffle_epi32(
                    tmp_1,
                    0b10110001
                );
                
                state_0 = _mm_unpacklo_epi64(
                    tmp_1,
                    tmp_0
                );
                
                state_1 = _mm_unpackhi_epi64(
                    tmp_1,
                    tmp_0
                );
            }
            
            constexpr auto shuffle_mask = vect_type
                {
                    0x0405060700010203,
                    0x0c0d0e0f08090a0b
                };
            
            for (size_t i = 0; i < a_blocks; i += 1)
            {
                vect_type msg_0;
                vect_type msg_1;
                vect_type msg_2;
                vect_type msg_3;
                vect_type msg_4;
                vect_type msg_5;
                
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
                
                msg_1 = msg_0;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[0]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                /*
                 * Rounds 4 to 7.
                 */
                
                msg_0 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr) + 1
                );
                
                msg_0 = _mm_shuffle_epi8(
                    msg_0,
                    shuffle_mask
                );
                
                msg_2 = msg_0;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[1]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_1 = _mm_sha256msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                /*
                 * Rounds 8 to 11.
                 */
                
                msg_0 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr) + 2
                );
                
                msg_0 = _mm_shuffle_epi8(
                    msg_0,
                    shuffle_mask
                );
                
                msg_3 = msg_0;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[2]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_2 = _mm_sha256msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                /*
                 * Rounds 12 to 15.
                 */
                
                msg_0 = _mm_loadu_si128(
                    reinterpret_cast<const load_type*>(a_src_ptr) + 3
                );
                
                msg_0 = _mm_shuffle_epi8(
                    msg_0,
                    shuffle_mask
                );
                
                msg_4 = msg_0;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[3]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_4;
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_3,
                    4
                );
                
                msg_1 = _mm_add_epi32(
                    msg_1,
                    msg_5
                );
                
                msg_1 = _mm_sha256msg2_epu32(
                    msg_1,
                    msg_4
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_3 = _mm_sha256msg1_epu32(
                    msg_3,
                    msg_4
                );
                
                /*
                 * Rounds 16 to 19.
                 */
                
                msg_0 = msg_1;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[4]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_1;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_4,
                    4
                );
                
                msg_2 = _mm_add_epi32(
                    msg_2,
                    msg_5
                );
                
                msg_2 = _mm_sha256msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_4 = _mm_sha256msg1_epu32(
                    msg_4,
                    msg_1
                );
                
                /*
                 * Rounds 20 to 23.
                 */
                
                msg_0 = msg_2;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[5]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_2;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_1,
                    4
                );
                
                msg_3 = _mm_add_epi32(
                    msg_3,
                    msg_5
                );
                
                msg_3 = _mm_sha256msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_1 = _mm_sha256msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                /*
                 * Rounds 24 to 27.
                 */
                
                msg_0 = msg_3;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[6]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_3;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_2,
                    4
                );
                
                msg_4 = _mm_add_epi32(
                    msg_4,
                    msg_5
                );
                
                msg_4 = _mm_sha256msg2_epu32(
                    msg_4,
                    msg_3
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_2 = _mm_sha256msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                /*
                 * Rounds 28 to 31.
                 */
                
                msg_0 = msg_4;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[7]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_4;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_3,
                    4
                );
                
                msg_1 = _mm_add_epi32(
                    msg_1,
                    msg_5
                );
                
                msg_1 = _mm_sha256msg2_epu32(
                    msg_1,
                    msg_4
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_3 = _mm_sha256msg1_epu32(
                    msg_3,
                    msg_4
                );
                
                /*
                 * Rounds 32 to 35.
                 */
                
                msg_0 = msg_1;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[8]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_1;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_4,
                    4
                );
                
                msg_2 = _mm_add_epi32(
                    msg_2,
                    msg_5
                );
                
                msg_2 = _mm_sha256msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_4 = _mm_sha256msg1_epu32(
                    msg_4,
                    msg_1
                );
                
                /*
                 * Rounds 36 to 39.
                 */
                
                msg_0 = msg_2;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[9]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_2;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_1,
                    4
                );
                
                msg_3 = _mm_add_epi32(
                    msg_3,
                    msg_5
                );
                
                msg_3 = _mm_sha256msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_1 = _mm_sha256msg1_epu32(
                    msg_1,
                    msg_2
                );
                
                /*
                 * Rounds 40 to 43.
                 */
                
                msg_0 = msg_3;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[10]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_3;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_2,
                    4
                );
                
                msg_4 = _mm_add_epi32(
                    msg_4,
                    msg_5
                );
                
                msg_4 = _mm_sha256msg2_epu32(
                    msg_4,
                    msg_3
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_2 = _mm_sha256msg1_epu32(
                    msg_2,
                    msg_3
                );
                
                /*
                 * Rounds 44 to 47.
                 */
                
                msg_0 = msg_4;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[11]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_4;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_3,
                    4
                );
                
                msg_1 = _mm_add_epi32(
                    msg_1,
                    msg_5
                );
                
                msg_1 = _mm_sha256msg2_epu32(
                    msg_1,
                    msg_4
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_3 = _mm_sha256msg1_epu32(
                    msg_3,
                    msg_4
                );
                
                /*
                 * Rounds 48 to 51.
                 */
                
                msg_0 = msg_1;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[12]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_1;
                
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_4,
                    4
                );
                
                msg_2 = _mm_add_epi32(
                    msg_2,
                    msg_5
                );
                
                msg_2 = _mm_sha256msg2_epu32(
                    msg_2,
                    msg_1
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                msg_4 = _mm_sha256msg1_epu32(
                    msg_4,
                    msg_1
                );
                
                /*
                 * Rounds 52 to 55.
                 */
                
                msg_0 = msg_2;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[13]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_2;
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_1,
                    4
                );
                
                msg_3 = _mm_add_epi32(
                    msg_3,
                    msg_5
                );
                
                msg_3 = _mm_sha256msg2_epu32(
                    msg_3,
                    msg_2
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                /*
                 * Rounds 56 to 59.
                 */
                
                msg_0 = msg_3;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[14]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_5 = msg_3;
                msg_5 = _mm_alignr_epi8(
                    msg_5,
                    msg_2,
                    4
                );
                
                msg_4 = _mm_add_epi32(
                    msg_4,
                    msg_5
                );
                
                msg_4 = _mm_sha256msg2_epu32(
                    msg_4,
                    msg_3
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                /*
                 * Rounds 60 to 63.
                 */
                
                msg_0 = msg_4;
                
                msg_0 = _mm_add_epi32(
                    msg_0,
                    k[15]
                );
                
                state_1 = _mm_sha256rnds2_epu32(
                    state_1,
                    state_0,
                    msg_0
                );
                
                msg_0 = _mm_shuffle_epi32(
                    msg_0,
                    0b00001110
                );
                
                state_0 = _mm_sha256rnds2_epu32(
                    state_0,
                    state_1,
                    msg_0
                );
                
                /*
                 * Merge state.
                 */
                
                state_0 = _mm_add_epi32(
                    state_0,
                    state_0_save
                );
                
                state_1 = _mm_add_epi32(
                    state_1,
                    state_1_save
                );
                
                a_src_ptr += block_size;
            }
            
            {
                vect_type tmp_0;
                vect_type tmp_1;
                
                /*
                 * Interleave lo 64 bits to get CDAB.
                 * Interleave hi 64 bits to get GHEF.
                 * Shuffle CDAB to DCBA.
                 * Shuffle GHEF to HGFE.
                 */
                
                tmp_0 = _mm_unpackhi_epi64(
                    state_0,
                    state_1
                );
                
                tmp_1 = _mm_unpacklo_epi64(
                    state_0,
                    state_1
                );
                
                tmp_0 = _mm_shuffle_epi32(
                    tmp_0,
                    0b10110001
                );
                
                tmp_1 = _mm_shuffle_epi32(
                    tmp_1,
                    0b10110001
                );
                
                _mm_storeu_si128(
                    reinterpret_cast<stor_type*>(a_h_ptr),
                    tmp_0
                );
                
                _mm_storeu_si128(
                    reinterpret_cast<stor_type*>(a_h_ptr) + 1,
                    tmp_1
                );
            }
        }
    };
}

#else

namespace milo::primitive::detail
{
    using hash_sha_2_256_impl_hw_x86_v_1 = void;
}

#endif
