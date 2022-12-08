

#pragma once


#include <milo/inner.h>


namespace milo::primitive::detail
{
    template<
        meta::primitive::cipher_chacha_20 t_cipher,
        meta::primitive::mac_poly_1305 t_mac,
        typename... t_options
    >
    class aead_chacha_20_poly_1305
    {
    public:
        
        using cipher_type = t_cipher;
        
        using mac_type = t_mac;
    
    public:
        
        struct properties
        {
            using aead [[maybe_unused]] = int;
            
            using aead_chacha_20_poly_1305 [[maybe_unused]] = int;
        };
    
    public:
        
        static
        constexpr size_t key_size = cipher_type::key_size;
        
        static
        constexpr size_t iv_size = cipher_type::iv_size - 4;
        
        static
        constexpr size_t digest_size = mac_type::digest_size;
    
    private:
        
        cipher_type m_cipher;
        
        mac_type m_mac;
        
        uint64_t m_aad_processed_size;
        
        uint64_t m_edd_processed_size;
    
    public:
        
        constexpr aead_chacha_20_poly_1305() noexcept(true) = default;
        
        constexpr aead_chacha_20_poly_1305(aead_chacha_20_poly_1305&& object) noexcept(true) = default;
        
        constexpr aead_chacha_20_poly_1305(const aead_chacha_20_poly_1305& object) noexcept(true) = default;
        
        constexpr ~aead_chacha_20_poly_1305() noexcept(true) = default;
    
    public:
        
        constexpr auto
        operator =(
            const aead_chacha_20_poly_1305& object
        ) noexcept(true) -> aead_chacha_20_poly_1305& = default;
    
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
            constexpr auto init_cipher_iv_size = cipher_type::iv_size;
            uint8_t init_cipher_iv[init_cipher_iv_size]{};
            
            constexpr auto init_mac_key_size = cipher_type::block_size;
            uint8_t init_mac_key[init_mac_key_size]{};
            
            inner::memory_init(
                init_cipher_iv + sizeof(uint32_t),
                init_cipher_iv_size - sizeof(uint32_t),
                a_iv_ptr,
                a_iv_size
            );
            
            m_cipher.initialize(
                a_key_ptr,
                a_key_size,
                init_cipher_iv,
                init_cipher_iv_size
            );
            
            /*
             * Whole block encryption bumps counter by 1.
             */
            
            m_cipher.encrypt(
                init_mac_key,
                init_mac_key_size,
                init_mac_key
            );
            
            m_mac.initialize(
                init_mac_key,
                init_mac_key_size
            );
            
            m_aad_processed_size = 0;
            m_edd_processed_size = 0;
        }
        
        template<
            meta::byte t_aad
        >
        constexpr auto
        aad(
            const t_aad* a_aad_ptr,
            size_t a_aad_size
        ) noexcept(true) -> void
        {
            m_mac.update(
                a_aad_ptr,
                a_aad_size
            );
            
            m_aad_processed_size += a_aad_size;
        }
        
        constexpr auto
        aadover(
        ) noexcept(true) -> void
        {
            constexpr size_t buffer_zero_size = 16;
            constexpr uint8_t buffer_zero[buffer_zero_size]{};
            
            m_mac.update(
                buffer_zero,
                (buffer_zero_size - m_aad_processed_size % buffer_zero_size) & 0b1111
            );
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
            auto ciphertext_size = m_cipher.encrypt(
                a_plaintext_ptr,
                a_plaintext_size,
                a_ciphertext_ptr
            );
            
            m_mac.update(
                a_ciphertext_ptr,
                ciphertext_size
            );
            
            m_edd_processed_size += ciphertext_size;
            return ciphertext_size;
        }
        
        template<
            meta::byte t_plaintext
        >
        constexpr auto
        encrypt_size(
            const t_plaintext* a_plaintext_ptr,
            size_t a_plaintext_size
        ) noexcept(true) -> size_t
        {
            return m_cipher.encrypt_size(
                a_plaintext_ptr,
                a_plaintext_size
            );
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
            m_mac.update(
                a_ciphertext_ptr,
                a_ciphertext_size
            );
            
            auto plaintext_size = m_cipher.decrypt(
                a_ciphertext_ptr,
                a_ciphertext_size,
                a_plaintext_ptr
            );
            
            m_edd_processed_size += plaintext_size;
            return plaintext_size;
        }
        
        template<
            meta::byte t_ciphertext
        >
        constexpr auto
        decrypt_size(
            const t_ciphertext* a_ciphertext_ptr,
            size_t a_ciphertext_size
        ) noexcept(true) -> size_t
        {
            return m_cipher.decrypt_size(
                a_ciphertext_ptr,
                a_ciphertext_size
            );
        }
        
        constexpr auto
        finalize(
        ) noexcept(true) -> void
        {
            constexpr size_t buffer_zero_size = 16;
            constexpr uint8_t buffer_zero[buffer_zero_size]{};
            
            m_mac.update(
                buffer_zero,
                (buffer_zero_size - m_edd_processed_size % buffer_zero_size) & 0b1111
            );
            
            constexpr size_t buffer_last_size = 16;
            uint8_t buffer_last[buffer_last_size];
            
            inner::memory_stor_le<uint64_t>(
                buffer_last,
                0,
                m_aad_processed_size
            );
            
            inner::memory_stor_le<uint64_t>(
                buffer_last,
                1,
                m_edd_processed_size
            );
            
            m_mac.update(
                buffer_last,
                buffer_last_size
            );
            m_mac.finalize();
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
            return m_mac.digest(
                a_digest_ptr,
                a_digest_size
            );
        }
    };
}
