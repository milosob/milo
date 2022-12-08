

#pragma once


#include <milo/internal.h>

#include <milo/primitive/detail/cipher/chacha/chacha_20_impl.h>
#include <milo/primitive/detail/cipher/chacha/chacha_20_impl_sw.h>


namespace milo::primitive::detail
{
    template<
        typename... t_options
    >
    class cipher_chacha_20
    {
    public:
        
        struct impl_type
            : internal::impl_proxy<
                internal::impl_domain_runtime,
                cipher_chacha_20_impl_chooser,
                cipher_chacha_20_impl_invoker,
                internal::impl_cpltime<
                    cipher_chacha_20_impl_sw
                >,
                internal::impl_runtime<
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
            internal::memory_erase(m_state);
            internal::memory_erase(m_buffer);
            internal::memory_erase(m_buffer_size);
        }
    
    public:
        
        constexpr auto
        operator =(
            const cipher_chacha_20& object
        ) noexcept(true) -> cipher_chacha_20& = default;
    
    public:
        
        template<
            meta::byte t_key,
            meta::byte t_iv
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
            
            internal::memory_init_le(
                m_state + 4,
                8,
                a_key_ptr,
                a_key_size
            );
            
            internal::memory_init_le(
                m_state + 12,
                4,
                a_iv_ptr,
                a_iv_size
            );
            
            m_buffer_size = 0;
        }
        
        template<
            meta::byte t_plaintext,
            meta::byte t_ciphertext
        >
        constexpr auto
        encrypt(
            const t_plaintext* a_plaintext_ptr,
            size_t a_plaintext_size,
            t_ciphertext* a_ciphertext_ptr
        ) noexcept(true) -> size_t
        {
            m_buffer_size = internal::update_block_prod_xor<
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
            meta::byte t_plaintext
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
            meta::byte t_ciphertext,
            meta::byte t_plaintext
        >
        constexpr auto
        decrypt(
            const t_ciphertext* a_ciphertext_ptr,
            size_t a_ciphertext_size,
            t_plaintext* a_plaintext_ptr
        ) noexcept(true) -> size_t
        {
            m_buffer_size = internal::update_block_prod_xor<
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
            meta::byte t_ciphertext
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
