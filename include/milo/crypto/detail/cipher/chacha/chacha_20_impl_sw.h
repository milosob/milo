

#pragma once


#include <milo/common.h>

#include <milo/inner/bit.h>
#include <milo/inner/memory.h>


namespace milo::crypto::detail
{
    class cipher_chacha_20_impl_sw
    {
    public:
        
        static
        constexpr size_t block_size = 64;
    
    private:
        
        static
        constexpr auto
        round(
            uint32_t* a_state_ptr,
            uint32_t a_0,
            uint32_t a_1,
            uint32_t a_2,
            uint32_t a_3
        ) noexcept(true) -> void
        {
            auto s_0 = a_state_ptr[a_0];
            auto s_1 = a_state_ptr[a_1];
            auto s_2 = a_state_ptr[a_2];
            auto s_3 = a_state_ptr[a_3];
            
            s_0 = s_0 + s_1;
            s_3 = inner::bit_rotl(
                s_3 ^ s_0,
                16
            );
            
            s_2 = s_2 + s_3;
            s_1 = inner::bit_rotl(
                s_1 ^ s_2,
                12
            );
            
            s_0 = s_0 + s_1;
            s_3 = inner::bit_rotl(
                s_3 ^ s_0,
                8
            );
            
            s_2 = s_2 + s_3;
            s_1 = inner::bit_rotl(
                s_1 ^ s_2,
                7
            );
            
            a_state_ptr[a_0] = s_0;
            a_state_ptr[a_1] = s_1;
            a_state_ptr[a_2] = s_2;
            a_state_ptr[a_3] = s_3;
        }
    
    public:
        
        static
        constexpr auto
        process(
            uint8_t* a_dst_ptr,
            uint32_t* a_state_ptr
        ) noexcept(true) -> void
        {
            uint32_t state[16];
            
            for (size_t i = 0; i < 16; i += 1)
            {
                state[i] = a_state_ptr[i];
            }
            
            for (size_t i = 0; i < 10; i += 1)
            {
                // @formatter:off
                round(state, 0, 4,  8, 12);
                round(state, 1, 5,  9, 13);
                round(state, 2, 6, 10, 14);
                round(state, 3, 7, 11, 15);
                round(state, 0, 5, 10, 15);
                round(state, 1, 6, 11, 12);
                round(state, 2, 7,  8, 13);
                round(state, 3, 4,  9, 14);
                // @formatter:on
            }
            
            for (size_t i = 0; i < 16; i += 1)
            {
                inner::memory_stor_le<uint32_t>(
                    a_dst_ptr,
                    i,
                    state[i] + a_state_ptr[i]
                );
            }
            
            a_state_ptr[12] += 1;
        }
    };
}
