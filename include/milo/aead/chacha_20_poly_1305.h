

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>

#include <milo/cipher/chacha_20.h>
#include <milo/mac/poly.h>


namespace milo::aead
{
    template<
        concepts::cipher_chacha_20 t_cipher,
        concepts::mac_poly_1305 t_mac,
        typename... t_options
    >
    class chacha_20_poly_1305_basic
    {
    public:
        
        using type = chacha_20_poly_1305_basic;
        
        using cipher_type = t_cipher;
        
        using mac_type = t_mac;
    
    public:
        
        struct properties
        {
            using aead_type [[maybe_unused]] = type;
            
            using aead_chacha_20_poly_1305_type [[maybe_unused]] = type;
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
        
        constexpr chacha_20_poly_1305_basic() noexcept(true) = default;
        
        constexpr chacha_20_poly_1305_basic(type&& object) noexcept(true) = default;
        
        constexpr chacha_20_poly_1305_basic(const type& object) noexcept(true) = default;
        
        constexpr ~chacha_20_poly_1305_basic() noexcept(true) = default;
    
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
            constexpr auto init_cipher_iv_size = cipher_type::iv_size;
            uint8_t init_cipher_iv[init_cipher_iv_size]{};
            
            constexpr auto init_mac_key_size = cipher_type::block_size;
            uint8_t init_mac_key[init_mac_key_size]{};
            
            memory::init(
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
        
        /**
         * This function authenticates additional data.
         *
         * @tparam t_aad
         * Aad type.
         * @param a_aad_ptr
         * Aad ptr.
         * @param a_aad_size
         * Aad size.
         */
        template<
            concepts::byte t_aad
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
        
        /**
         * This function signalizes additional authenticated data is over.
         */
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
            const t_plaintext* a_plaintext_ptr,
            size_t a_plaintext_size
        ) noexcept(true) -> size_t
        {
            return m_cipher.encrypt_size(
                a_plaintext_ptr,
                a_plaintext_size
            );
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
            const t_ciphertext* a_ciphertext_ptr,
            size_t a_ciphertext_size
        ) noexcept(true) -> size_t
        {
            return m_cipher.decrypt_size(
                a_ciphertext_ptr,
                a_ciphertext_size
            );
        }
        
        /**
         * This function finalizes context.
         */
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
    
            memory::stor_le<uint64_t>(
                buffer_last,
                0,
                m_aad_processed_size
            );
    
            memory::stor_le<uint64_t>(
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
            return m_mac.digest(
                a_digest_ptr,
                a_digest_size
            );
        }
    };
    
    using chacha_20_poly_1305 = chacha_20_poly_1305_basic<cipher::chacha_20, mac::poly_1305>;
}
