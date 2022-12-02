

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>

#include <milo/detail/hash/impl.h>
#include <milo/detail/hash/sha/sha_2_512_impl_sw.h>
#include <milo/detail/impl.h>
#include <milo/detail/option.h>
#include <milo/detail/update.h>


namespace milo::detail
{
    template<
        size_t t_bits,
        typename... t_options
    >
    class hash_sha_2_512
    {
    public:
        
        struct impl_type
            : impl_proxy<
                impl_domain_runtime,
                hash_impl_chooser,
                hash_impl_invoker,
                impl_cpltime<
                    hash_sha_2_512_impl_sw
                >,
                impl_runtime<
                    hash_sha_2_512_impl_sw
                >,
                t_options...
            >
        {
            static
            constexpr auto block_size = hash_sha_2_512_impl_sw::block_size;
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
            option_digest_size_suite::query_default_v<
                bits / 8,
                t_options...
            >;
    
    private:
        
        uint64_t m_h[8]{};
        
        uint64_t m_processed_bytes[2]{};
        
        uint8_t m_buffer[block_size * 2]{};
        
        size_t m_buffer_size = 0;
    
    public:
        
        constexpr hash_sha_2_512() noexcept(true) = default;
        
        constexpr hash_sha_2_512(hash_sha_2_512&& object) noexcept(true) = default;
        
        constexpr hash_sha_2_512(const hash_sha_2_512& object) noexcept(true) = default;
        
        constexpr  ~hash_sha_2_512() noexcept(true)
        {
            memory::erase(m_h);
            memory::erase(m_processed_bytes);
            memory::erase(m_buffer);
            memory::erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const hash_sha_2_512& object
        ) noexcept(true) -> hash_sha_2_512& = default;
    
    public:

        constexpr auto
        initialize(
        ) noexcept(true) -> void
        {
            static_assert(
                t_bits == 224 ||
                t_bits == 256 ||
                t_bits == 384 ||
                t_bits == 512
            );
            
            if constexpr (bits == 224)
            {
                m_h[0] = 0x8c3d37c819544da2;
                m_h[1] = 0x73e1996689dcd4d6;
                m_h[2] = 0x1dfab7ae32ff9c82;
                m_h[3] = 0x679dd514582f9fcf;
                m_h[4] = 0x0f6d2b697bd44da8;
                m_h[5] = 0x77e36f7304c48942;
                m_h[6] = 0x3f9d85a86a1d36c8;
                m_h[7] = 0x1112e6ad91d692a1;
            }
            
            if constexpr (bits == 256)
            {
                m_h[0] = 0x22312194fc2bf72c;
                m_h[1] = 0x9f555fa3c84c64c2;
                m_h[2] = 0x2393b86b6f53b151;
                m_h[3] = 0x963877195940eabd;
                m_h[4] = 0x96283ee2a88effe3;
                m_h[5] = 0xbe5e1e2553863992;
                m_h[6] = 0x2b0199fc2c85b8aa;
                m_h[7] = 0x0eb72ddc81c52ca2;
            }
            
            if constexpr (bits == 384)
            {
                m_h[0] = 0xcbbb9d5dc1059ed8;
                m_h[1] = 0x629a292a367cd507;
                m_h[2] = 0x9159015a3070dd17;
                m_h[3] = 0x152fecd8f70e5939;
                m_h[4] = 0x67332667ffc00b31;
                m_h[5] = 0x8eb44a8768581511;
                m_h[6] = 0xdb0c2e0d64f98fa7;
                m_h[7] = 0x47b5481dbefa4fa4;
            }
            
            if constexpr (bits == 512)
            {
                m_h[0] = 0x6a09e667f3bcc908;
                m_h[1] = 0xbb67ae8584caa73b;
                m_h[2] = 0x3c6ef372fe94f82b;
                m_h[3] = 0xa54ff53a5f1d36f1;
                m_h[4] = 0x510e527fade682d1;
                m_h[5] = 0x9b05688c2b3e6c1f;
                m_h[6] = 0x1f83d9abfb41bd6b;
                m_h[7] = 0x5be0cd19137e2179;
            }
            
            m_processed_bytes[0] = 0;
            m_processed_bytes[1] = 0;
            m_buffer_size = 0;
        }

        template<
            concepts::byte t_message
        >
        constexpr auto
        update(
            const t_message* a_message_ptr,
            size_t a_message_size
        ) noexcept(true) -> void
        {
            m_processed_bytes[1] += m_processed_bytes[0] > (UINT64_MAX - a_message_size);
            m_processed_bytes[0] += a_message_size;
            
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

        constexpr auto
        finalize(
        ) noexcept(true) -> void
        {
            size_t left_size = m_buffer_size;
            size_t last_size = size_t(block_size) << size_t(left_size >= 112);
            
            m_buffer[left_size] = 128;
            
            for (auto i = left_size + 1; i < last_size - 16; i += 1)
            {
                m_buffer[i] = 0;
            }
            
            memory::stor_be<uint64_t>(
                m_buffer + last_size - 16,
                0,
                m_processed_bytes[1] << 3 | m_processed_bytes[0] >> 61
            );
            
            memory::stor_be<uint64_t>(
                m_buffer + last_size - 16,
                1,
                m_processed_bytes[0] << 3
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
