

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>

#include <milo/inner/memory.h>


namespace milo::crypto::detail
{
    class mac_poly_1305_impl_sw
    {
    public:
        
        static
        constexpr size_t block_size = 16;
    
    public:
        
        template<
            concepts::byte t_src
        >
        static
        constexpr auto
        process(
            const t_src* a_src_ptr,
            size_t a_blocks,
            const uint32_t m_key_ptr[8],
            uint32_t a_acc_ptr[5],
            uint32_t a_add
        ) noexcept(true) -> void
        {
            uint64_t acc[5];
            
            acc[0] = a_acc_ptr[0];
            acc[1] = a_acc_ptr[1];
            acc[2] = a_acc_ptr[2];
            acc[3] = a_acc_ptr[3];
            acc[4] = a_acc_ptr[4];
            
            for (size_t i = 0; i < a_blocks; i += 1)
            {
                uint64_t t_0 = inner::memory_load_le<uint32_t>(
                    a_src_ptr,
                    0
                );
                uint64_t t_1 = inner::memory_load_le<uint32_t>(
                    a_src_ptr,
                    1
                );
                uint64_t t_2 = inner::memory_load_le<uint32_t>(
                    a_src_ptr,
                    2
                );
                uint64_t t_3 = inner::memory_load_le<uint32_t>(
                    a_src_ptr,
                    3
                );
                
                t_0 += acc[0];
                t_1 += acc[1] + (t_0 >> 32);
                t_2 += acc[2] + (t_1 >> 32);
                t_3 += acc[3] + (t_2 >> 32);
                
                acc[0] = uint32_t(t_0);
                acc[1] = uint32_t(t_1);
                acc[2] = uint32_t(t_2);
                acc[3] = uint32_t(t_3);
                acc[4] += uint32_t(t_3 >> 32) + a_add;
                
                t_0 =
                    m_key_ptr[0] * acc[0] + m_key_ptr[7] * acc[1] +
                    m_key_ptr[6] * acc[2] + m_key_ptr[5] * acc[3] + m_key_ptr[4] * acc[4];
                
                t_1 =
                    m_key_ptr[1] * acc[0] + m_key_ptr[0] * acc[1] +
                    m_key_ptr[7] * acc[2] + m_key_ptr[6] * acc[3] + m_key_ptr[5] * acc[4];
                
                t_2 =
                    m_key_ptr[2] * acc[0] + m_key_ptr[1] * acc[1] +
                    m_key_ptr[0] * acc[2] + m_key_ptr[7] * acc[3] + m_key_ptr[6] * acc[4];
                
                t_3 =
                    m_key_ptr[3] * acc[0] + m_key_ptr[2] * acc[1] +
                    m_key_ptr[1] * acc[2] + m_key_ptr[0] * acc[3] + m_key_ptr[7] * acc[4];
                
                uint64_t c = (m_key_ptr[0] & 0b11) * acc[4] + (t_3 >> 32);
                
                uint64_t a_0 = uint32_t(t_0) + (c >> 2) * 5;
                uint64_t a_1 = uint32_t(t_1) + (a_0 >> 32) + (t_0 >> 32);
                uint64_t a_2 = uint32_t(t_2) + (a_1 >> 32) + (t_1 >> 32);
                uint64_t a_3 = uint32_t(t_3) + (a_2 >> 32) + (t_2 >> 32);
                uint64_t a_4 = (a_3 >> 32) + (c & 0b11);
                
                acc[0] = uint32_t(a_0);
                acc[1] = uint32_t(a_1);
                acc[2] = uint32_t(a_2);
                acc[3] = uint32_t(a_3);
                acc[4] = uint32_t(a_4);
                
                a_src_ptr += block_size;
            }
            
            a_acc_ptr[0] = acc[0];
            a_acc_ptr[1] = acc[1];
            a_acc_ptr[2] = acc[2];
            a_acc_ptr[3] = acc[3];
            a_acc_ptr[4] = acc[4];
        }
    };
}
