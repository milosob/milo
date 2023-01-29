

#pragma once


#include <milo/internal.h>

#include <milo/primitive/detail/hash/impl.h>
#include <milo/primitive/detail/hash/sha/sha_2_256_impl_sw.h>
#include <milo/primitive/detail/hash/sha/sha_2_256_impl_hw_x86_64.h>


namespace milo::primitive::detail
{
    template<
        size_t t_bits,
        typename... t_options
    >
    class hash_sha_2_256
    {
    public:
        
        struct impl_type
            : internal::impl_proxy<
                hash_impl_chooser_block,
                hash_impl_invoker_block,
                internal::impl_cpltime<
                    hash_sha_2_256_impl_sw
                >,
                internal::impl_runtime<
                    hash_sha_2_256_impl_hw_x86_64_ni,
                    hash_sha_2_256_impl_sw
                >,
                t_options...
            >
        {
            static
            constexpr auto block_size = hash_sha_2_256_impl_sw::block_size;
        };
    
    public:
        
        struct properties
        {
            using hash [[maybe_unused]] = int;
            
            using hash_sha_2 [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t bits = t_bits;
        
        static
        constexpr size_t block_size = impl_type::block_size;
        
        static
        constexpr size_t digest_size =
            internal::option_digest_size_suite::query_default_v<
                bits / 8,
                t_options...
            >;
    
    private:
        
        uint32_t m_h[8]{};
        
        uint64_t m_processed_bytes = 0;
        
        uint8_t m_buffer[block_size * 2]{};
        
        size_t m_buffer_size = 0;
    
    public:
        
        constexpr hash_sha_2_256() noexcept(true) = default;
        
        constexpr hash_sha_2_256(hash_sha_2_256&& object) noexcept(true) = default;
        
        constexpr hash_sha_2_256(const hash_sha_2_256& object) noexcept(true) = default;
        
        constexpr ~hash_sha_2_256() noexcept(true)
        {
            internal::memory_erase(m_h);
            internal::memory_erase(m_processed_bytes);
            internal::memory_erase(m_buffer);
            internal::memory_erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const hash_sha_2_256& object
        ) noexcept(true) -> hash_sha_2_256& = default;
    
    public:
        
        constexpr auto
        initialize(
        ) noexcept(true) -> void
        {
            static_assert(
                t_bits == 224 ||
                t_bits == 256
            );
            
            if constexpr (bits == 224)
            {
                m_h[0] = 0xc1059ed8;
                m_h[1] = 0x367cd507;
                m_h[2] = 0x3070dd17;
                m_h[3] = 0xf70e5939;
                m_h[4] = 0xffc00b31;
                m_h[5] = 0x68581511;
                m_h[6] = 0x64f98fa7;
                m_h[7] = 0xbefa4fa4;
            }
            
            if constexpr (bits == 256)
            {
                m_h[0] = 0x6a09e667;
                m_h[1] = 0xbb67ae85;
                m_h[2] = 0x3c6ef372;
                m_h[3] = 0xa54ff53a;
                m_h[4] = 0x510e527f;
                m_h[5] = 0x9b05688c;
                m_h[6] = 0x1f83d9ab;
                m_h[7] = 0x5be0cd19;
            }
            
            m_processed_bytes = 0;
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
            m_processed_bytes += a_message_size;
            
            m_buffer_size = internal::update_block_soak<
                impl_type
            >(
                m_buffer,
                m_buffer_size,
                a_message_ptr,
                a_message_size,
                m_h
            );
        }
        
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
            
            internal::memory_stor_be<uint64_t>(
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
            
            internal::memory_copy_be(
                a_digest_ptr,
                m_h,
                a_digest_size
            );
            
            return a_digest_size;
        }
    };
}
