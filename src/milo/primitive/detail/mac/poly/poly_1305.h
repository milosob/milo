

#pragma once


#include <milo/internal.h>

#include <milo/primitive/detail/mac/poly/poly_1305_impl.h>
#include <milo/primitive/detail/mac/poly/poly_1305_impl_sw.h>


namespace milo::primitive::detail
{
    template<
        typename... t_options
    >
    class mac_poly_1305
    {
    private:
        
        struct impl_type
            : internal::impl_proxy<
                mac_poly_1305_impl_chooser,
                mac_poly_1305_impl_invoker,
                internal::impl_cpltime<
                    mac_poly_1305_impl_sw
                >,
                internal::impl_runtime<
                    mac_poly_1305_impl_sw
                >
            >
        {
            static
            constexpr auto block_size = mac_poly_1305_impl_sw::block_size;
        };
    
    public:
        
        struct properties
        {
            using mac [[maybe_unused]] = int;
            
            using mac_poly_1305 [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t key_size = 32;
        
        static
        constexpr size_t digest_size =
            internal::option_digest_size_suite::query_default_v<
                16,
                t_options...
            >;
    
    private:
        
        static
        constexpr size_t block_size = impl_type::block_size;
    
    private:
        
        uint32_t m_key_r[8]{};
        
        uint32_t m_key_s[4]{};
        
        uint32_t m_acc[5]{};
        
        uint8_t m_buffer[block_size]{};
        
        size_t m_buffer_size = 0;
    
    public:
        
        constexpr mac_poly_1305() noexcept(true) = default;
        
        constexpr mac_poly_1305(mac_poly_1305&& object) noexcept(true) = default;
        
        constexpr mac_poly_1305(const mac_poly_1305& object) noexcept(true) = default;
        
        constexpr ~mac_poly_1305() noexcept(true)
        {
            internal::memory_erase(m_key_r);
            internal::memory_erase(m_key_s);
            internal::memory_erase(m_acc);
            internal::memory_erase(m_buffer);
            internal::memory_erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const mac_poly_1305& object
        ) noexcept(true) -> mac_poly_1305& = default;
    
    public:
        
        template<
            meta::byte t_key
        >
        constexpr auto
        initialize(
            const t_key* a_key_ptr,
            size_t a_key_size
        ) noexcept(true) -> void
        {
            internal::memory_init(
                m_key_r,
                a_key_ptr,
                a_key_size
            );
            
            m_key_r[0] &= 0x0fffffff;
            m_key_r[1] &= 0x0ffffffc;
            m_key_r[2] &= 0x0ffffffc;
            m_key_r[3] &= 0x0ffffffc;
            m_key_s[0] = m_key_r[4];
            m_key_s[1] = m_key_r[5];
            m_key_s[2] = m_key_r[6];
            m_key_s[3] = m_key_r[7];
            m_key_r[4] = (m_key_r[0] >> 2) * 5;
            m_key_r[5] = (m_key_r[1] >> 2) * 5;
            m_key_r[6] = (m_key_r[2] >> 2) * 5;
            m_key_r[7] = (m_key_r[3] >> 2) * 5;
            m_acc[0] = 0;
            m_acc[1] = 0;
            m_acc[2] = 0;
            m_acc[3] = 0;
            m_acc[4] = 0;
            m_buffer_size = 0;
        }
        
        template<
            meta::byte t_message
        >
        constexpr auto
        update(
            const t_message* a_message_ptr,
            size_t a_message_size
        ) noexcept(true) -> void
        {
            m_buffer_size = internal::update_block_soak<
                impl_type
            >(
                m_buffer,
                m_buffer_size,
                a_message_ptr,
                a_message_size,
                m_key_r,
                m_acc,
                1
            );
        }
        
        constexpr auto
        finalize(
        ) noexcept(true) -> void
        {
            if (m_buffer_size > 0)
            {
                m_buffer[m_buffer_size] = 1;
                
                internal::memory_set(
                    m_buffer + m_buffer_size + 1,
                    0,
                    block_size - m_buffer_size - 1
                );
                
                impl_type::template invoke<0>(
                    1,
                    m_key_r,
                    m_acc,
                    0,
                    m_buffer
                );
            }
            
            uint64_t t_0 = uint64_t(m_acc[0]) + 5;
            uint64_t t_1 = uint64_t(m_acc[1]) + (t_0 >> 32);
            uint64_t t_2 = uint64_t(m_acc[2]) + (t_1 >> 32);
            uint64_t t_3 = uint64_t(m_acc[3]) + (t_2 >> 32);
            
            uint64_t mask = -((uint64_t(m_acc[4]) + (t_3 >> 32)) >> 2);
            uint64_t mask_neg = ~mask;
            
            t_0 = (t_0 & mask) | (m_acc[0] & mask_neg);
            t_1 = (t_1 & mask) | (m_acc[1] & mask_neg);
            t_2 = (t_2 & mask) | (m_acc[2] & mask_neg);
            t_3 = (t_3 & mask) | (m_acc[3] & mask_neg);
            
            t_0 &= 0xffffffff;
            t_1 &= 0xffffffff;
            t_2 &= 0xffffffff;
            t_3 &= 0xffffffff;
            
            t_0 += m_key_s[0];
            t_1 += m_key_s[1] + (t_0 >> 32);
            t_2 += m_key_s[2] + (t_1 >> 32);
            t_3 += m_key_s[3] + (t_2 >> 32);
            
            m_acc[0] = t_0;
            m_acc[1] = t_1;
            m_acc[2] = t_2;
            m_acc[3] = t_3;
        }
        
        template<
            meta::byte t_digest
        >
        constexpr auto
        digest(
            t_digest* a_digest_ptr,
            size_t a_digest_size = digest_size
        ) const noexcept(true) -> size_t
        {
            a_digest_size = internal::min(
                a_digest_size,
                digest_size
            );
            
            internal::memory_copy_le(
                a_digest_ptr,
                m_acc,
                a_digest_size
            );
            
            return a_digest_size;
        }
    };
}
