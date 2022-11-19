

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/options.h>
#include <milo/memory.h>


namespace milo::detail
{
    template<
        concepts::hash t_hash,
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
            option_digest_size_query<
                option_digest_size<
                    hash_type::digest_size
                >,
                t_options...
            >::value;
    
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
            memory::erase(m_buffer);
        }
    
    public:
        
        constexpr auto
        operator =(
            const mac_hmac& object
        ) noexcept(true) -> mac_hmac& = default;
    
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
                
                memory::copy(
                    m_buffer + block_size,
                    m_buffer,
                    hash_type::digest_size
                );
                
                a_key_size = hash_type::digest_size;
            }
            else
            {
                memory::copy(
                    m_buffer,
                    a_key_ptr,
                    a_key_size
                );
                
                memory::copy(
                    m_buffer + block_size,
                    a_key_ptr,
                    a_key_size
                );
            }
            
            memory::set(
                m_buffer + a_key_size,
                0,
                block_size - a_key_size
            );
            
            memory::set(
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
            m_hash.update(
                a_message_ptr,
                a_message_size
            );
        }
        
        /**
         * This function completes the computation.
         */
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
            
            return m_hash.digest(
                a_digest_ptr,
                a_digest_size
            );
        }
    };
}
