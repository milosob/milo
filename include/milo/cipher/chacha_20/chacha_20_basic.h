

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>
#include <milo/update.h>


namespace milo::cipher
{
    template<
        typename t_block,
        typename... t_options
    >
    requires
    requires
    {
        requires concepts::block_prod<t_block, uint32_t*>;
    }
    class chacha_20_basic
    {
    public:
        
        using type = chacha_20_basic;
        
        using block_type = t_block;
    
    public:
        
        struct properties
        {
            using cipher_type [[maybe_unused]] = type;
            
            using cipher_stream_type [[maybe_unused]] = type;
            
            using cipher_chacha_20_type [[maybe_unused]] = type;
        };
    
    public:
        
        static
        constexpr size_t key_size = 32;
        
        static
        constexpr size_t iv_size = 16;
        
        static
        constexpr size_t block_size = block_type::block_size;
    
    private:
        
        uint32_t m_state[16];
        
        uint8_t m_buffer[block_size];
        
        size_t m_buffer_size;
    
    public:
        
        constexpr chacha_20_basic() noexcept(true) = default;
        
        constexpr chacha_20_basic(type&& object) noexcept(true) = default;
        
        constexpr chacha_20_basic(const type& object) noexcept(true) = default;
        
        constexpr ~chacha_20_basic() noexcept(true)
        {
            memory::erase(m_state);
            memory::erase(m_buffer);
            memory::erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(const type& object) noexcept(true) -> type& = default;
    
    public:
        
        /**
         * This function initializes context.
         *
         * @tparam t_key
         * Key type.
         * @tparam t_iv
         * Iv type.
         * @param a_key_ptr
         * Key pointer.
         * @param a_key_size
         * Key size.
         * @param a_iv_ptr
         * Iv pointer.
         * @param a_iv_size
         * Iv size.
         */
        template<
            concepts::byte t_key,
            concepts::byte t_iv
        >
        constexpr auto
        initialize(
            const t_key* a_key_ptr,
            size_t a_key_size,
            const t_iv* a_iv_ptr,
            size_t a_iv_size
        ) noexcept(true) -> void
        {
            m_state[0] = 0x61707865;
            m_state[1] = 0x3320646e;
            m_state[2] = 0x79622d32;
            m_state[3] = 0x6b206574;
            
            memory::init_le(
                m_state + 4,
                8,
                a_key_ptr,
                a_key_size
            );
            
            memory::init_le(
                m_state + 12,
                4,
                a_iv_ptr,
                a_iv_size
            );
            
            m_buffer_size = 0;
        }
        
        /**
         * This function encrypts plaintext.
         *
         * @tparam t_plaintext
         * Plaintext type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @param a_plaintext_ptr
         * Plaintext pointer.
         * @param a_plaintext_size
         * Plaintext size.
         * @param a_ciphertext_ptr
         * Ciphertext pointer.
         * @return
         * Ciphertext size.
         */
        template<
            concepts::byte t_plaintext,
            concepts::byte t_ciphertext
        >
        constexpr auto
        encrypt(
            const t_plaintext* a_plaintext_ptr,
            size_t a_plaintext_size,
            t_ciphertext* a_ciphertext_ptr
        ) noexcept(true) -> size_t
        {
            m_buffer_size = update::block_prod_xor<block_type>(
                m_buffer,
                m_buffer_size,
                a_ciphertext_ptr,
                a_plaintext_ptr,
                a_plaintext_size,
                m_state
            );
            
            return a_plaintext_size;
        }
        
        /**
         * This function calculates maximum ciphertext size.
         *
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_plaintext_ptr
         * Plaintext pointer.
         * @param a_plaintext_size
         * Plaintext size.
         * @return
         * Ciphertext size.
         */
        template<
            concepts::byte t_plaintext
        >
        constexpr auto
        encrypt_size(
            [[maybe_unused]]
            const t_plaintext* a_plaintext_ptr,
            size_t a_plaintext_size
        ) noexcept(true) -> size_t
        {
            return a_plaintext_size;
        }
        
        /**
         * This function decrypts ciphertext.
         *
         * @tparam t_ciphertext
         * Ciphertext type.
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_ciphertext_ptr
         * Ciphertext pointer.
         * @param a_ciphertext_size
         * Ciphertext size.
         * @param a_plaintext_ptr
         * Plaintext pointer.
         * @return
         * Plaintext size.
         */
        template<
            concepts::byte t_ciphertext,
            concepts::byte t_plaintext
        >
        constexpr auto
        decrypt(
            const t_ciphertext* a_ciphertext_ptr,
            size_t a_ciphertext_size,
            t_plaintext* a_plaintext_ptr
        ) noexcept(true) -> size_t
        {
            m_buffer_size = update::block_prod_xor<block_type>(
                m_buffer,
                m_buffer_size,
                a_plaintext_ptr,
                a_ciphertext_ptr,
                a_ciphertext_size,
                m_state
            );
            
            return a_ciphertext_size;
        }
        
        /**
         * This function calculates maximum plaintext size.
         *
         * @tparam t_ciphertext
         * Ciphertext type.
         * @param a_ciphertext_ptr
         * Ciphertext pointer.
         * @param a_ciphertext_size
         * Ciphertext size.
         * @return
         * Plaintext size.
         */
        template<
            concepts::byte t_ciphertext
        >
        constexpr auto
        decrypt_size(
            [[maybe_unused]]
            const t_ciphertext* a_ciphertext_ptr,
            size_t a_ciphertext_size
        ) noexcept(true) -> size_t
        {
            return a_ciphertext_size;
        }
    };
}
