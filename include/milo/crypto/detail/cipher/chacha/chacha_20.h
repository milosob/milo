

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>

#include <milo/crypto/detail/cipher/chacha/chacha_20_impl.h>
#include <milo/crypto/detail/cipher/chacha/chacha_20_impl_sw.h>
#include <milo/inner/impl.h>
#include <milo/inner/memory.h>
#include <milo/inner/option.h>
#include <milo/inner/update.h>


namespace milo::crypto::detail
{
    template<
        typename... t_options
    >
    class cipher_chacha_20
    {
    public:
        
        struct impl_type
            : inner::impl_proxy<
                inner::impl_domain_runtime,
                cipher_chacha_20_impl_chooser,
                cipher_chacha_20_impl_invoker,
                inner::impl_cpltime<
                    cipher_chacha_20_impl_sw
                >,
                inner::impl_runtime<
                    cipher_chacha_20_impl_sw
                >,
                t_options...
            >
        {
            static
            constexpr auto block_size = cipher_chacha_20_impl_sw::block_size;
        };
    
    public:
        
        struct properties
        {
            using cipher [[maybe_unused]] = int;
            
            using cipher_stream [[maybe_unused]] = int;
            
            using cipher_chacha_20 [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t key_size = 32;
        
        static
        constexpr size_t iv_size = 16;
        
        static
        constexpr size_t block_size = impl_type::block_size;
    
    private:
        
        uint32_t m_state[16];
        
        uint8_t m_buffer[block_size];
        
        size_t m_buffer_size;
    
    public:
        
        constexpr cipher_chacha_20() noexcept(true) = default;
        
        constexpr cipher_chacha_20(cipher_chacha_20&& object) noexcept(true) = default;
        
        constexpr cipher_chacha_20(const cipher_chacha_20& object) noexcept(true) = default;
        
        constexpr ~cipher_chacha_20() noexcept(true)
        {
            inner::memory_erase(m_state);
            inner::memory_erase(m_buffer);
            inner::memory_erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const cipher_chacha_20& object
        ) noexcept(true) -> cipher_chacha_20& = default;
    
    public:
        
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
            
            inner::memory_init_le(
                m_state + 4,
                8,
                a_key_ptr,
                a_key_size
            );
            
            inner::memory_init_le(
                m_state + 12,
                4,
                a_iv_ptr,
                a_iv_size
            );
            
            m_buffer_size = 0;
        }
        
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
            m_buffer_size = inner::update_block_prod_xor<
                impl_type
            >(
                m_buffer,
                m_buffer_size,
                a_ciphertext_ptr,
                a_plaintext_ptr,
                a_plaintext_size,
                m_state
            );
            
            return a_plaintext_size;
        }
        
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
            m_buffer_size = inner::update_block_prod_xor<
                impl_type
            >(
                m_buffer,
                m_buffer_size,
                a_plaintext_ptr,
                a_ciphertext_ptr,
                a_ciphertext_size,
                m_state
            );
            
            return a_ciphertext_size;
        }
        
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
