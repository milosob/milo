

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>
#include <milo/error.h>

#include <milo/inner/forward.h>
#include <milo/inner/move.h>


namespace milo::crypto::aead
{
    /**
     * Aead wrapper that provides access to additional APIs.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::aead t_impl
    >
    class apie
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        static
        constexpr auto key_size = impl_type::key_size;
        
        static
        constexpr auto iv_size = impl_type::iv_size;
        
        static
        constexpr auto digest_size = impl_type::digest_size;
    
    private:
        
        impl_type m_impl;
        
        bool m_finalized_aad;
        
        bool m_finalized;
    
    public:
        
        constexpr apie() noexcept(true) = default;
        
        constexpr apie(apie&& object) noexcept(true) = default;
        
        constexpr apie(const apie& object) noexcept(true) = default;
        
        constexpr ~apie() noexcept(true) = default;
    
    public:
        
        constexpr auto
        operator =(
            const apie& object
        ) noexcept(true) -> apie& = default;
    
    public:
        
        /**
         * Constructs the object.
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
        constexpr
        apie(
            const t_key* a_key_ptr,
            size_t a_key_size,
            const t_iv* a_iv_ptr,
            size_t a_iv_size
        ) noexcept(true)
        {
            initialize(
                a_key_ptr,
                a_key_size,
                a_iv_ptr,
                a_iv_size
            );
        }
        
        /**
         * Constructs the object.
         *
         * @tparam t_key
         * Key type.
         * @tparam t_iv
         * Iv type.
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         */
        template<
            concepts::bytes t_key,
            concepts::bytes t_iv
        >
        constexpr
        apie(
            const t_key& a_key,
            const t_iv& a_iv
        ) noexcept(true)
        {
            initialize(
                a_key,
                a_iv
            );
        }
    
    private:
        
        constexpr auto
        do_finalize_aad(
        ) noexcept(true) -> void
        {
            if (m_finalized_aad)
            {
                return;
            }
            
            m_impl.aadover();
            m_finalized_aad = true;
        }
        
        constexpr auto
        do_finalize(
        ) noexcept(true) -> void
        {
            if (m_finalized)
            {
                return;
            }
            
            m_impl.finalize();
            m_finalized = true;
        }
    
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
            m_impl.initialize(
                a_key_ptr,
                a_key_size,
                a_iv_ptr,
                a_iv_size
            );
            
            m_finalized_aad = false;
            m_finalized = false;
        }
        
        /**
         * This function initializes context.
         *
         * @tparam t_key
         * Key type.
         * @tparam t_iv
         * Iv type.
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         */
        template<
            concepts::bytes t_key,
            concepts::bytes t_iv
        >
        constexpr auto
        initialize(
            const t_key& a_key,
            const t_iv& a_iv
        ) noexcept(true) -> void
        {
            return initialize(
                a_key.data(),
                a_key.size(),
                a_iv.data(),
                a_iv.size()
            );
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
            m_impl.aad(
                a_aad_ptr,
                a_aad_size
            );
        }
        
        /**
         * This function authenticates additional data.
         *
         * @tparam t_aad
         * Aad type.
         * @param a_aad
         * Aad.
         */
        template<
            concepts::bytes t_aad
        >
        constexpr auto
        aad(
            const t_aad& a_aad
        ) noexcept(true) -> void
        {
            aad(
                a_aad.data(),
                a_aad.size()
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
            do_finalize_aad();
            
            return m_impl.encrypt(
                a_plaintext_ptr,
                a_plaintext_size,
                a_ciphertext_ptr
            );
        }
        
        /**
         * This function encrypts plaintext.
         *
         * @tparam t_plaintext
         * Plaintext type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @param a_plaintext
         * Plaintext.
         * @param a_ciphertext
         * Ciphertext.
         * @return
         * Ciphertext size.
         */
        template<
            concepts::bytes t_plaintext,
            concepts::bytes t_ciphertext
        >
        constexpr auto
        encrypt(
            const t_plaintext& a_plaintext,
            t_ciphertext& a_ciphertext
        ) noexcept(concepts::container_static<t_ciphertext>) -> size_t
        {
            container::resize(
                a_ciphertext,
                encrypt_size(a_plaintext)
            );
            
            auto size = encrypt(
                a_plaintext.data(),
                a_plaintext.size(),
                a_ciphertext.data()
            );
            
            container::resize(
                a_ciphertext,
                size
            );
            
            return size;
        }
        
        /**
         * This function encrypts plaintext.
         *
         * @tparam t_ciphertext
         * Ciphertext type.
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_plaintext
         * Plaintext.
         * @return
         * Ciphertext.
         */
        template<
            concepts::bytes t_ciphertext = container::bytes_dynamic,
            concepts::bytes t_plaintext
        >
        constexpr auto
        encrypt(
            const t_plaintext& a_plaintext
        ) noexcept(false) -> t_ciphertext
        requires
        requires
        {
            requires concepts::container_dynamic<t_ciphertext>;
        }
        {
            t_ciphertext result;
            
            encrypt(
                a_plaintext,
                result
            );
            
            return result;
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
            return m_impl.encrypt_size(
                a_plaintext_ptr,
                a_plaintext_size
            );
        }
        
        /**
         * This function calculates ciphertext size.
         *
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_plaintext
         * Plaintext.
         * @return
         * Ciphertext size.
         */
        template<
            concepts::bytes t_plaintext
        >
        constexpr auto
        encrypt_size(
            const t_plaintext& a_plaintext
        ) noexcept(true) -> size_t
        {
            return encrypt_size(
                a_plaintext.data(),
                a_plaintext.size()
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
            do_finalize_aad();
            
            return m_impl.decrypt(
                a_ciphertext_ptr,
                a_ciphertext_size,
                a_plaintext_ptr
            );
        }
        
        /**
         * This function decrypts ciphertext.
         *
         * @tparam t_ciphertext
         * Ciphertext type.
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_ciphertext
         * Ciphertext.
         * @param a_plaintext
         * Plaintext.
         * @return
         * Plaintext size.
         */
        template<
            concepts::bytes t_ciphertext,
            concepts::bytes t_plaintext
        >
        constexpr auto
        decrypt(
            const t_ciphertext& a_ciphertext,
            t_plaintext& a_plaintext
        ) noexcept(concepts::container_static<t_plaintext>) -> size_t
        {
            container::resize(
                a_plaintext,
                decrypt_size(a_ciphertext)
            );
            
            auto size = decrypt(
                a_ciphertext.data(),
                a_ciphertext.size(),
                a_plaintext.data()
            );
            
            container::resize(
                a_plaintext,
                size
            );
            
            return size;
        }
        
        /**
         * This function decrypts ciphertext.
         *
         * @tparam t_plaintext
         * Plaintext type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @param a_ciphertext
         * Ciphertext.
         * @return
         * Plaintext.
         */
        template<
            concepts::bytes t_plaintext = container::bytes_dynamic,
            concepts::bytes t_ciphertext
        >
        constexpr auto
        decrypt(
            const t_ciphertext& a_ciphertext
        ) noexcept(false) -> t_plaintext
        requires
        requires
        {
            requires concepts::container_dynamic<t_plaintext>;
        }
        {
            t_plaintext result;
            
            decrypt(
                a_ciphertext,
                result
            );
            
            return result;
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
            return m_impl.decrypt_size(
                a_ciphertext_ptr,
                a_ciphertext_size
            );
        }
        
        /**
         * This function calculates plaintext size.
         *
         * @tparam t_ciphertext
         * Ciphertext type.
         * @param a_ciphertext
         * Ciphertext.
         * @return
         * Plaintext size.
         */
        template<
            concepts::bytes t_ciphertext
        >
        constexpr auto
        decrypt_size(
            const t_ciphertext& a_ciphertext
        ) noexcept(true) -> size_t
        {
            return decrypt_size(
                a_ciphertext.data(),
                a_ciphertext.size()
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
        ) noexcept(true) -> size_t
        {
            do_finalize();
            
            return m_impl.digest(
                a_digest_ptr,
                a_digest_size
            );
        }
        
        /**
         * This function extracts digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest
         * Digest.
         * @param a_digest_size
         * Digest size.
         * @return
         * Digest size.
         */
        template<
            concepts::bytes t_digest
        >
        constexpr auto
        digest(
            t_digest& a_digest,
            size_t a_digest_size = digest_size
        ) noexcept(concepts::container_static<t_digest>) -> size_t
        {
            do_finalize();
            
            a_digest_size = common::min(
                a_digest_size,
                digest_size
            );
            
            a_digest_size = container::resize(
                a_digest,
                a_digest_size
            );
            
            return digest(
                a_digest.data(),
                a_digest_size
            );
        }
        
        /**
         * This function extracts digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest_size
         * Digest size.
         * @return
         * Digest.
         */
        template<
            concepts::bytes t_digest = container::bytes_dynamic
        >
        constexpr auto
        digest(
            size_t a_digest_size
        ) noexcept(false) -> t_digest
        requires
        requires
        {
            requires concepts::container_dynamic<t_digest>;
        }
        {
            t_digest result;
            
            digest(
                result,
                a_digest_size
            );
            
            return result;
        }
        
        /**
         * This function extracts digest.
         *
         * @tparam t_digest
         * Digest type.
         * @return
         * Digest.
         */
        template<
            concepts::bytes t_digest = container::bytes_static<digest_size>
        >
        constexpr auto
        digest(
        ) noexcept(true) -> t_digest
        requires
        requires
        {
            requires concepts::container_static<t_digest, 1, digest_size>;
        }
        {
            t_digest result;
            
            digest(
                result,
                result.size()
            );
            
            return result;
        }
        
        /**
         * This function verifies digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest_ptr
         * Digest pointer.
         * @param a_digest_size
         * Digest size.
         * @param a_error
         * Error
         */
        template<
            concepts::byte t_digest
        >
        constexpr auto
        verify(
            const t_digest* a_digest_ptr,
            size_t a_digest_size,
            error& a_error
        ) noexcept(true) -> void
        {
            a_error =
                inner::memory_match(
                    digest(),
                    container::view_dynamic<const t_digest>(
                        a_digest_ptr,
                        a_digest_size
                    )
                )
                ? error::none
                : error::aead_verify;
        }
        
        /**
         * This function verifies digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest_ptr
         * Digest pointer.
         * @param a_digest_size
         * Digest size.
         */
        template<
            concepts::byte t_digest
        >
        constexpr auto
        verify(
            const t_digest* a_digest_ptr,
            size_t a_digest_size
        ) noexcept(false) -> void
        {
            error error;
            
            verify(
                a_digest_ptr,
                a_digest_size,
                error
            );
            
            if (error)
            {
                throw error;
            }
        }
        
        /**
         * This function verifies digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest
         * Digest.
         * @param a_error
         * Error
         */
        template<
            concepts::bytes t_digest
        >
        constexpr auto
        verify(
            const t_digest& a_digest,
            error& a_error
        ) noexcept(true) -> void
        {
            verify(
                a_digest.data(),
                a_digest.size(),
                a_error
            );
        }
        
        /**
         * This function verifies digest.
         *
         * @tparam t_digest
         * Digest type.
         * @param a_digest
         * Digest.
         */
        template<
            concepts::bytes t_digest
        >
        constexpr auto
        verify(
            const t_digest& a_digest
        ) noexcept(false) -> void
        {
            verify(
                a_digest.data(),
                a_digest.size()
            );
        }
    };
    
    /**
     * This function performs aead encryption.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_aad
     * Aad type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_digest
     * Digest type.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @param a_iv_ptr
     * Iv pointer.
     * @param a_iv_size
     * Iv size.
     * @param a_aad_ptr
     * Aad pointer.
     * @param a_aad_size
     * Aad size.
     * @param a_plaintext_ptr
     * Plaintext pointer.
     * @param a_plaintext_size
     * Plaintext size.
     * @param a_ciphertext_ptr
     * Ciphertext pointer.
     * @param a_digest_ptr
     * Digest pointer.
     * @return
     * [Ciphertext size, Digest size]
     */
    template<
        concepts::aead t_impl,
        concepts::byte t_key,
        concepts::byte t_iv,
        concepts::byte t_aad,
        concepts::byte t_plaintext,
        concepts::byte t_ciphertext,
        concepts::byte t_digest
    >
    constexpr auto
    encrypt(
        const t_key* a_key_ptr,
        size_t a_key_size,
        const t_iv* a_iv_ptr,
        size_t a_iv_size,
        const t_aad* a_aad_ptr,
        size_t a_aad_size,
        const t_plaintext* a_plaintext_ptr,
        size_t a_plaintext_size,
        t_ciphertext* a_ciphertext_ptr,
        t_digest* a_digest_ptr
    ) noexcept(true) -> container::tuple<size_t, size_t>
    {
        apie<t_impl> apie(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        
        apie.template aad<t_aad>(
            a_aad_ptr,
            a_aad_size
        );
        
        auto ciphertext_size = apie.template encrypt<t_plaintext, t_ciphertext>(
            a_plaintext_ptr,
            a_plaintext_size,
            a_ciphertext_ptr
        );
        
        auto digest_size = apie.template digest<t_digest>(
            a_digest_ptr
        );
        
        return container::tuple<size_t, size_t>(
            ciphertext_size,
            digest_size
        );
    }
    
    /**
     * This function performs aead encryption.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_aad
     * Aad type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_digest
     * Digest type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_aad
     * Aad.
     * @param a_plaintext
     * Plaintext.
     * @param a_ciphertext
     * Ciphertext.
     * @param a_digest
     * Digest.
     * @return
     * [Ciphertext size, Digest size]
     */
    template<
        concepts::aead t_impl,
        concepts::bytes t_key,
        concepts::bytes t_iv,
        concepts::bytes t_aad,
        concepts::bytes t_plaintext,
        concepts::bytes t_ciphertext,
        concepts::bytes t_digest
    >
    constexpr auto
    encrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_aad& a_aad,
        const t_plaintext& a_plaintext,
        t_ciphertext& a_ciphertext,
        t_digest& a_digest
    ) noexcept(
    (
        concepts::container_static<t_ciphertext> &&
        concepts::container_static<t_digest>
    )
    ) -> container::tuple<size_t, size_t>
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        apie.template aad<t_aad>(
            a_aad
        );
        
        auto ciphertext_size = apie.template encrypt<t_plaintext, t_ciphertext>(
            a_plaintext,
            a_ciphertext
        );
        
        auto digest_size = apie.template digest<t_digest>(
            a_digest
        );
        
        return container::tuple<size_t, size_t>(
            ciphertext_size,
            digest_size
        );
    }
    
    /**
     * This function performs aead encryption.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_digest
     * Digest type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_aad
     * Aad type.
     * @tparam t_plaintext
     * Plaintext type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_aad
     * Aad.
     * @param a_plaintext
     * Plaintext.
     * @return
     * [Ciphertext, Digest]
     */
    template<
        concepts::aead t_impl,
        concepts::bytes t_ciphertext = container::bytes_dynamic,
        concepts::bytes t_digest = container::bytes_static<t_impl::digest_size>,
        concepts::bytes t_key,
        concepts::bytes t_iv,
        concepts::bytes t_aad,
        concepts::bytes t_plaintext
    >
    constexpr auto
    encrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_aad& a_aad,
        const t_plaintext& a_plaintext
    ) noexcept(false) -> container::tuple<t_ciphertext, t_digest>
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        apie.template aad<t_aad>(
            a_aad
        );
        
        auto ciphertext = apie.template encrypt<t_ciphertext, t_plaintext>(
            a_plaintext
        );
        
        auto digest = apie.template digest<t_digest>();
        
        return container::tuple<t_ciphertext, t_digest>(
            inner::move(ciphertext),
            inner::move(digest)
        );
    }
    
    /**
     * This function performs aead decryption.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_aad
     * Aad type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_digest
     * Digest type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_error
     * Error type.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @param a_iv_ptr
     * Iv pointer.
     * @param a_iv_size
     * Iv size.
     * @param a_aad_ptr
     * Aad pointer.
     * @param a_aad_size
     * Aad size.
     * @param a_ciphertext_ptr
     * Ciphertext pointer.
     * @param a_ciphertext_size
     * Ciphertext size.
     * @param a_digest_ptr
     * Digest pointer.
     * @param a_digest_size
     * Digest size.
     * @param a_plaintext_ptr
     * Plaintext pointer.
     * @param a_error
     * Error.
     * @return
     * Plaintext size
     */
    template<
        concepts::aead t_impl,
        concepts::byte t_key,
        concepts::byte t_iv,
        concepts::byte t_aad,
        concepts::byte t_ciphertext,
        concepts::byte t_digest,
        concepts::byte t_plaintext,
        typename... t_error
    >
    constexpr auto
    decrypt(
        const t_key* a_key_ptr,
        size_t a_key_size,
        const t_iv* a_iv_ptr,
        size_t a_iv_size,
        const t_aad* a_aad_ptr,
        size_t a_aad_size,
        const t_ciphertext* a_ciphertext_ptr,
        size_t a_ciphertext_size,
        const t_digest* a_digest_ptr,
        size_t a_digest_size,
        t_plaintext* a_plaintext_ptr,
        t_error& ...a_error
    ) noexcept(
    (
        sizeof...(t_error) == 0
    )
    ) -> size_t
    requires
    requires
    {
        requires sizeof...(t_error) <= 1;
    }
    {
        apie<t_impl> apie(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        
        apie.template aad<t_aad>(
            a_aad_ptr,
            a_aad_size
        );
        
        auto plaintext_size = apie.template decrypt<t_ciphertext, t_plaintext>(
            a_ciphertext_ptr,
            a_ciphertext_size,
            a_plaintext_ptr
        );
        
        apie.template verify<t_digest>(
            a_digest_ptr,
            a_digest_size,
            a_error...
        );
        
        return plaintext_size;
    }
    
    /**
     * This function performs aead decryption.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_aad
     * Aad type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_digest
     * Digest type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_error.
     * Error type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_aad
     * Aad.
     * @param a_ciphertext
     * Ciphertext.
     * @param a_digest
     * Digest.
     * @param a_plaintext
     * Plaintext.
     * @param a_error
     * Error.
     * @return
     * Plaintext size.
     */
    template<
        concepts::aead t_impl,
        concepts::bytes t_key,
        concepts::bytes t_iv,
        concepts::bytes t_aad,
        concepts::bytes t_ciphertext,
        concepts::bytes t_digest,
        concepts::bytes t_plaintext,
        typename... t_error
    >
    constexpr auto
    decrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_aad& a_aad,
        const t_ciphertext& a_ciphertext,
        const t_digest& a_digest,
        t_plaintext& a_plaintext,
        t_error& ...a_error
    ) noexcept(
    (
        sizeof...(t_error) == 0 &&
        concepts::container_static<t_plaintext>
    )
    ) -> size_t
    requires
    requires
    {
        requires sizeof...(t_error) <= 1;
    }
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        apie.template aad<t_aad>(
            a_aad
        );
        
        auto plaintext_size = apie.template decrypt<t_ciphertext, t_plaintext>(
            a_ciphertext,
            a_plaintext
        );
        
        apie.template verify<t_digest>(
            a_digest,
            a_error...
        );
        
        return plaintext_size;
    }
    
    /**
     * This function performs aead decryption.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_aad
     * Aad type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_digest
     * Digest type.
     * @tparam t_error.
     * Error type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_aad
     * Aad.
     * @param a_ciphertext
     * Ciphertext.
     * @param a_digest
     * Digest.
     * @param a_error
     * Error.
     * @return
     * Plaintext.
     */
    template<
        concepts::aead t_impl,
        concepts::bytes t_plaintext = container::bytes_dynamic,
        concepts::bytes t_key,
        concepts::bytes t_iv,
        concepts::bytes t_aad,
        concepts::bytes t_ciphertext,
        concepts::bytes t_digest,
        typename... t_error
    >
    constexpr auto
    decrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_aad& a_aad,
        const t_ciphertext& a_ciphertext,
        const t_digest& a_digest,
        t_error& ...a_error
    ) noexcept(false) -> t_plaintext
    requires
    requires
    {
        requires sizeof...(t_error) <= 1;
    }
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        apie.template aad<t_aad>(
            a_aad
        );
        
        auto plaintext = apie.template decrypt<t_plaintext, t_ciphertext>(
            a_ciphertext
        );
        
        apie.template verify<t_digest>(
            a_digest,
            a_error...
        );
        
        return plaintext;
    }
}
