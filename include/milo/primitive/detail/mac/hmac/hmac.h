

#pragma once


#include <milo/inner.h>


namespace milo::primitive::detail
{
    template<
        meta::primitive::hash t_hash,
        typename... t_options
    >
    class mac_hmac
    {
    public:
        
        using hash_type = t_hash;
    
    public:
        
        struct properties
        {
            using mac [[maybe_unused]] = int;
            
            using mac_hmac [[maybe_unused]] = int;
            
            using prf [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t key_size = 0;
        
        static
        constexpr size_t digest_size =
            inner::option_digest_size_suite::query_default_v<
                hash_type::digest_size,
                t_options...
            >;
    
    private:
        
        static
        constexpr size_t block_size = hash_type::block_size;
    
    private:
        
        hash_type m_hash;
        
        uint8_t m_buffer[block_size * 2]{};
    
    public:
        
        constexpr mac_hmac() noexcept(true) = default;
        
        constexpr mac_hmac(mac_hmac&& object) noexcept(true) = default;
        
        constexpr mac_hmac(const mac_hmac& object) noexcept(true) = default;
        
        constexpr ~mac_hmac() noexcept(true)
        {
            inner::memory_erase(m_buffer);
        }
    
    public:
        
        constexpr auto
        operator =(
            const mac_hmac& object
        ) noexcept(true) -> mac_hmac& = default;
    
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
            if (a_key_size > block_size)
            {
                m_hash.initialize();
                m_hash.update(
                    a_key_ptr,
                    a_key_size
                );
                m_hash.finalize();
                m_hash.digest(
                    m_buffer
                );
                
                inner::memory_copy(
                    m_buffer + block_size,
                    m_buffer,
                    hash_type::digest_size
                );
                
                a_key_size = hash_type::digest_size;
            }
            else
            {
                inner::memory_copy(
                    m_buffer,
                    a_key_ptr,
                    a_key_size
                );
                
                inner::memory_copy(
                    m_buffer + block_size,
                    a_key_ptr,
                    a_key_size
                );
            }
            
            inner::memory_set(
                m_buffer + a_key_size,
                0,
                block_size - a_key_size
            );
            
            inner::memory_set(
                m_buffer + block_size + a_key_size,
                0,
                block_size - a_key_size
            );
            
            for (size_t i = 0; i < block_size; i += 1)
            {
                m_buffer[i] ^= 0x5C;
                m_buffer[i + block_size] ^= 0x36;
            }
            
            m_hash.initialize();
            m_hash.update(
                m_buffer + block_size,
                block_size
            );
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
            m_hash.update(
                a_message_ptr,
                a_message_size
            );
        }
        
        constexpr auto
        finalize(
        ) noexcept(true) -> void
        {
            m_hash.finalize();
            m_hash.digest(
                m_buffer + block_size
            );
            
            m_hash.initialize();
            m_hash.update(
                m_buffer,
                block_size + hash_type::digest_size
            );
            m_hash.finalize();
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
            a_digest_size = inner::min(
                a_digest_size,
                digest_size
            );
            
            return m_hash.digest(
                a_digest_ptr,
                a_digest_size
            );
        }
    };
}
