

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    class cipher_chacha_impl_sw_ietf
    {
    public:
        
        struct requirements
        {
        };
    
    public:
        
        static
        constexpr size_t block_size = 64;
        
        static
        constexpr size_t block_ratio = 1;
        
        static
        constexpr size_t rounds = 10;
    
    private:
        
        enum class operation
        {
            transform [[maybe_unused]],
            generate  [[maybe_unused]]
        };
        
        template<
            operation t_operation,
            meta::byte t_dst_ptr,
            meta::byte t_src_ptr
        >
        static
        constexpr auto
        stream(
            size_t a_blocks,
            const uint32_t* a_state_ptr,
            t_dst_ptr* a_dst_ptr,
            const t_src_ptr* a_src_ptr
        ) noexcept(true) -> size_t
        {
            uint32_t state_eval[16];
            uint32_t state_base[16];
            
            /*
             * TODO
             *  Code alignment issues in benchmarking app.
             *  Performance drops by 0.343 cycles per byte.
             */
            
            for (size_t i = 0; i < 16; i += 1)
            {
                state_base[i] = a_state_ptr[i];
            }
            
            /*
             * @formatter:off
             */
            
            #define MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(a_0, a_1, a_2, a_3)           \
                state_eval[a_0] = state_eval[a_0] + state_eval[a_1];                            \
                state_eval[a_3] = internal::bit_rotl(state_eval[a_3] ^ state_eval[a_0], 16);    \
                state_eval[a_2] = state_eval[a_2] + state_eval[a_3];                            \
                state_eval[a_1] = internal::bit_rotl(state_eval[a_1] ^ state_eval[a_2], 12);    \
                state_eval[a_0] = state_eval[a_0] + state_eval[a_1];                            \
                state_eval[a_3] = internal::bit_rotl(state_eval[a_3] ^ state_eval[a_0], 8);     \
                state_eval[a_2] = state_eval[a_2] + state_eval[a_3];                            \
                state_eval[a_1] = internal::bit_rotl(state_eval[a_1] ^ state_eval[a_2], 7);
            
            /*
             * @formatter:on
             */
            
            for (size_t i = 0; i < a_blocks; i += 1)
            {
                for (size_t j = 0; j < 16; j += 1)
                {
                    state_eval[j] = state_base[j];
                }
                
                for (size_t j = 0; j < rounds; j += 1)
                {
                    /*
                     * @formatter:off
                     */
                    
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(0, 4,  8, 12)
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(1, 5,  9, 13)
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(2, 6, 10, 14)
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(3, 7, 11, 15)
                    
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(0, 5, 10, 15)
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(1, 6, 11, 12)
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(2, 7,  8, 13)
                    MILO_PRIMITIVE_DETAIL_CHACHA_20_IMPL_SW_ROUND(3, 4,  9, 14)
                    
                    /*
                     * @formatter:on
                     */
                }
                
                for (size_t j = 0; j < 16; j += 1)
                {
                    state_eval[j] += state_base[j];
                }
                
                for (size_t j = 0; j < 16; j += 1)
                {
                    if constexpr (t_operation == operation::transform)
                    {
                        state_eval[j] ^= internal::memory_load_le<uint32_t>(
                            a_src_ptr,
                            j
                        );
                    }
                    
                    internal::memory_stor_le<uint32_t>(
                        a_dst_ptr,
                        j,
                        state_eval[j]
                    );
                }
                
                state_base[12] += 1;
                
                a_dst_ptr += block_size;
                
                if constexpr (t_operation == operation::transform)
                {
                    a_src_ptr += block_size;
                }
            }
            
            return a_blocks;
        }
    
    public:
        
        template<
            meta::byte t_dst_ptr,
            meta::byte t_src_ptr
        >
        static
        constexpr auto
        transform(
            size_t a_blocks,
            const uint32_t* a_state_ptr,
            t_dst_ptr* a_dst_ptr,
            const t_src_ptr* a_src_ptr
        ) noexcept(true) -> size_t
        {
            return stream<operation::transform>(
                a_blocks,
                a_state_ptr,
                a_dst_ptr,
                a_src_ptr
            );
        }
        
        template<
            meta::byte t_dst_ptr
        >
        static
        constexpr auto
        generate(
            size_t a_blocks,
            const uint32_t* a_state_ptr,
            t_dst_ptr* a_dst_ptr
        ) noexcept(true) -> size_t
        {
            return stream<operation::generate>(
                a_blocks,
                a_state_ptr,
                a_dst_ptr,
                static_cast<const uint8_t*>(nullptr)
            );
        }
    };
}
