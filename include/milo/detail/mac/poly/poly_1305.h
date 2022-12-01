

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>
#include <milo/update.h>

#include <milo/detail/mac/poly/poly_1305_impl.h>
#include <milo/detail/mac/poly/poly_1305_impl_sw.h>
#include <milo/detail/impl.h>
#include <milo/detail/option.h>


namespace milo::detail
{
    template<
        typename... t_options
    >
    class mac_poly_1305
    {
    private:
        
        struct impl_type
            : impl_proxy<
                impl_domain_runtime,
                mac_poly_1305_impl_chooser,
                mac_poly_1305_impl_invoker,
                impl_cpltime<
                    mac_poly_1305_impl_sw
                >,
                impl_runtime<
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
            option_digest_size_suite::query_default_v<
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
            memory::erase(m_key_r);
            memory::erase(m_key_s);
            memory::erase(m_acc);
            memory::erase(m_buffer);
            memory::erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const mac_poly_1305& object
        ) noexcept(true) -> mac_poly_1305& = default;
    
    public:
        
        /**
         * This function initializes context.
         *
         * @tparam t_key
         * Key type.
         * @param a_key_ptr
         * Key pointer.
         * @param a_key_size
         * Key size.
         */
        template<
            concepts::byte t_key
        >
        constexpr auto
        initialize(
            const t_key* a_key_ptr,
            size_t a_key_size
        ) noexcept(true) -> void
        {
            memory::init(
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
        
        /**
         * This function updates the message.
         *
         * @tparam t_message
         * Message type.
         * @param a_message_ptr
         * Message pointer.
         * @param a_message_size
         * Message size.
         */
        template<
            concepts::byte t_message
        >
        constexpr auto
        update(
            const t_message* a_message_ptr,
            size_t a_message_size
        ) noexcept(true) -> void
        {
            m_buffer_size = update::block_soak<
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
        
        /**
         * This function completes the computation.
         */
        constexpr auto
        finalize(
        ) noexcept(true) -> void
        {
            if (m_buffer_size > 0)
            {
                m_buffer[m_buffer_size] = 1;
                
                memory::set(
                    m_buffer + m_buffer_size + 1,
                    0,
                    block_size - m_buffer_size - 1
                );
                
                impl_type::template invoke<
                    0
                >(
                    m_buffer,
                    1,
                    m_key_r,
                    m_acc,
                    0
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
        
        /**
         * This function extracts digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest_ptr
         * Digest pointer.
         * @param a_digest_size
         * Digest size.
         * @return
         * Digest size.
         */
        template<
            concepts::byte t_digest
        >
        constexpr auto
        digest(
            t_digest* a_digest_ptr,
            size_t a_digest_size = digest_size
        ) const noexcept(true) -> size_t
        {
            a_digest_size = common::min(
                a_digest_size,
                digest_size
            );
            
            memory::copy_le(
                a_digest_ptr,
                m_acc,
                a_digest_size
            );
            
            return a_digest_size;
        }
    };
}
