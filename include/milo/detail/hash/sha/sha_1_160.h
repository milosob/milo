

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>

#include <milo/detail/hash/impl.h>
#include <milo/detail/hash/sha/sha_1_160_impl_sw.h>
#include <milo/detail/hash/sha/sha_1_160_impl_hw_x86_sha_v_1.h>
#include <milo/detail/hash/sha/sha_1_160_impl_hw_x86_sha_v_2.h>
#include <milo/detail/impl.h>
#include <milo/detail/option.h>
#include <milo/detail/update.h>


namespace milo::detail
{
    template<
        size_t t_bits,
        typename... t_options
    >
    class hash_sha_1_160
    {
    public:
        
        struct impl_type
            : impl_proxy<
                impl_domain_runtime,
                hash_impl_chooser,
                hash_impl_invoker,
                impl_cpltime<
                    hash_sha_1_160_impl_sw
                >,
                impl_runtime<
                    hash_sha_1_160_impl_hw_x86_sha_v_2,
                    hash_sha_1_160_impl_hw_x86_sha_v_1,
                    hash_sha_1_160_impl_sw
                >,
                t_options...
            >
        {
            static
            constexpr auto block_size = hash_sha_1_160_impl_sw::block_size;
        };
    
    public:
        
        struct properties
        {
            using hash [[maybe_unused]] = int;
            
            using hash_sha_1 [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t bits = t_bits;
        
        static
        constexpr size_t block_size = impl_type::block_size;
        
        static
        constexpr size_t digest_size =
            option_digest_size_suite::query_default_v<
                bits / 8,
                t_options...
            >;
    
    private:
        
        uint32_t m_h[5]{};
        
        uint64_t m_processed_bytes = 0;
        
        uint8_t m_buffer[block_size * 2]{};
        
        size_t m_buffer_size = 0;
    
    public:
        
        constexpr hash_sha_1_160() noexcept(true) = default;
        
        constexpr hash_sha_1_160(hash_sha_1_160&& object) noexcept(true) = default;
        
        constexpr hash_sha_1_160(const hash_sha_1_160& object) noexcept(true) = default;
        
        constexpr ~hash_sha_1_160() noexcept(true)
        {
            memory::erase(m_h);
            memory::erase(m_processed_bytes);
            memory::erase(m_buffer);
            memory::erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const hash_sha_1_160& object
        ) noexcept(true) -> hash_sha_1_160& = default;
    
    public:
        
        /**
         * This function initializes the computation.
         */
        constexpr auto
        initialize(
        ) noexcept(true) -> void
        {
            static_assert(
                t_bits == 160
            );
            
            if constexpr (bits == 160)
            {
                m_h[0] = 0x67452301;
                m_h[1] = 0xefcdab89;
                m_h[2] = 0x98badcfe;
                m_h[3] = 0x10325476;
                m_h[4] = 0xc3d2e1f0;
            }
            
            m_processed_bytes = 0;
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
            m_processed_bytes += a_message_size;
            
            m_buffer_size = update_block_soak<
                impl_type
            >(
                m_buffer,
                m_buffer_size,
                a_message_ptr,
                a_message_size,
                m_h
            );
        }
        
        /**
         * This function completes the computation.
         */
        constexpr auto
        finalize(
        ) noexcept(true) -> void
        {
            size_t left_size = m_buffer_size;
            size_t last_size = size_t(block_size) << size_t(left_size >= 56);
            
            m_buffer[left_size] = 128;
            
            for (auto i = left_size + 1; i < last_size - 8; i += 1)
            {
                m_buffer[i] = 0;
            }
            
            memory::stor_be<uint64_t>(
                m_buffer + last_size - 8,
                0,
                m_processed_bytes << 3
            );
            
            impl_type::template invoke<
                0
            >(
                m_buffer,
                last_size / block_size,
                m_h
            );
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
            
            memory::copy_be(
                a_digest_ptr,
                m_h,
                a_digest_size
            );
            
            return a_digest_size;
        }
    };
}
