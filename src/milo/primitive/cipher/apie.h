

#pragma once


#include <milo/internal.h>


namespace milo::primitive::cipher
{
    /**
     * Cipher wrapper that provides access to additional APIs.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        meta::primitive::cipher t_impl
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
        constexpr auto block_size = impl_type::block_size;
    
    private:
        
        impl_type m_impl;
    
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
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         */
        template<
            meta::bytes t_key,
            meta::bytes t_iv
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
            m_impl.initialize(
                a_key_ptr,
                a_key_size,
                a_iv_ptr,
                a_iv_size
            );
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
            meta::bytes t_key,
            meta::bytes t_iv
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
            meta::bytes t_plaintext,
            meta::bytes t_ciphertext
        >
        constexpr auto
        encrypt(
            const t_plaintext& a_plaintext,
            t_ciphertext& a_ciphertext
        ) noexcept(meta::container_static<t_ciphertext>) -> size_t
        {
            internal::resize(
                a_ciphertext,
                encrypt_size(a_plaintext)
            );
            
            auto size = encrypt(
                a_plaintext.data(),
                a_plaintext.size(),
                a_ciphertext.data()
            );
            
            internal::resize(
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
            meta::bytes t_ciphertext,
            meta::bytes t_plaintext
        >
        constexpr auto
        encrypt(
            const t_plaintext& a_plaintext
        ) noexcept(false) -> t_ciphertext
        requires
        requires
        {
            requires meta::container_dynamic<t_ciphertext>;
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
            meta::byte t_plaintext
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
            meta::bytes t_plaintext
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
            meta::bytes t_ciphertext,
            meta::bytes t_plaintext
        >
        constexpr auto
        decrypt(
            const t_ciphertext& a_ciphertext,
            t_plaintext& a_plaintext
        ) noexcept(meta::container_static<t_plaintext>) -> size_t
        {
            internal::resize(
                a_plaintext,
                decrypt_size(a_ciphertext)
            );
            
            auto size = decrypt(
                a_ciphertext.data(),
                a_ciphertext.size(),
                a_plaintext.data()
            );
            
            internal::resize(
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
            meta::bytes t_plaintext,
            meta::bytes t_ciphertext
        >
        constexpr auto
        decrypt(
            const t_ciphertext& a_ciphertext
        ) noexcept(false) -> t_plaintext
        requires
        requires
        {
            requires meta::container_dynamic<t_plaintext>;
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
            meta::byte t_ciphertext
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
            meta::bytes t_ciphertext
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
    };
    
    /**
     * This function encrypts plaintext.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @param a_iv_ptr
     * Iv pointer.
     * @param a_iv_size
     * Iv size.
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
        meta::primitive::cipher t_impl,
        meta::byte t_key,
        meta::byte t_iv,
        meta::byte t_plaintext,
        meta::byte t_ciphertext
    >
    constexpr auto
    encrypt(
        const t_key* a_key_ptr,
        size_t a_key_size,
        const t_iv* a_iv_ptr,
        size_t a_iv_size,
        const t_plaintext* a_plaintext_ptr,
        size_t a_plaintext_size,
        t_ciphertext* a_ciphertext_ptr
    ) noexcept(true) -> size_t
    {
        apie<t_impl> apie(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        
        return apie.template encrypt<t_plaintext, t_ciphertext>(
            a_plaintext_ptr,
            a_plaintext_size,
            a_ciphertext_ptr
        );
    }
    
    /**
     * This function encrypts plaintext.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_plaintext
     * Plaintext.
     * @param a_ciphertext
     * Ciphertext.
     * @return
     * Ciphertext size.
     */
    template<
        meta::primitive::cipher t_impl,
        meta::bytes t_key,
        meta::bytes t_iv,
        meta::bytes t_plaintext,
        meta::bytes t_ciphertext
    >
    constexpr auto
    encrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_plaintext& a_plaintext,
        t_ciphertext& a_ciphertext
    ) noexcept(meta::container_static<t_ciphertext>) -> size_t
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        return apie.template encrypt<t_plaintext, t_ciphertext>(
            a_plaintext,
            a_ciphertext
        );
    }
    
    /**
     *
     * This function encrypts plaintext.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_plaintext
     * Plaintext type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_plaintext
     * Plaintext.
     * @return
     * Ciphertext
     */
    template<
        meta::primitive::cipher t_impl,
        meta::bytes t_ciphertext = container::bytes_dynamic,
        meta::bytes t_key,
        meta::bytes t_iv,
        meta::bytes t_plaintext
    >
    constexpr auto
    encrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_plaintext& a_plaintext
    ) noexcept(false) -> t_ciphertext
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        return apie.template encrypt<t_ciphertext, t_plaintext>(
            a_plaintext
        );
    }
    
    /**
     * This function decrypts ciphertext.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_plaintext
     * Plaintext type.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @param a_iv_ptr
     * Iv pointer.
     * @param a_iv_size
     * Iv size.
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
        meta::primitive::cipher t_impl,
        meta::byte t_key,
        meta::byte t_iv,
        meta::byte t_ciphertext,
        meta::byte t_plaintext
    >
    constexpr auto
    decrypt(
        const t_key* a_key_ptr,
        size_t a_key_size,
        const t_iv* a_iv_ptr,
        size_t a_iv_size,
        const t_ciphertext* a_ciphertext_ptr,
        size_t a_ciphertext_size,
        t_plaintext* a_plaintext_ptr
    ) noexcept(true) -> size_t
    {
        apie<t_impl> apie(
            a_key_ptr,
            a_key_size,
            a_iv_ptr,
            a_iv_size
        );
        
        return apie.template decrypt<t_ciphertext, t_plaintext>(
            a_ciphertext_ptr,
            a_ciphertext_size,
            a_plaintext_ptr
        );
    }
    
    /**
     * This function decrypts ciphertext.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @tparam t_plaintext
     * Plaintext type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_ciphertext
     * Ciphertext.
     * @param a_plaintext
     * Plaintext.
     * @return
     * Plaintext size.
     */
    template<
        meta::primitive::cipher t_impl,
        meta::bytes t_key,
        meta::bytes t_iv,
        meta::bytes t_ciphertext,
        meta::bytes t_plaintext
    >
    constexpr auto
    decrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_ciphertext& a_ciphertext,
        t_plaintext& a_plaintext
    ) noexcept(meta::container_static<t_plaintext>) -> size_t
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        return apie.template decrypt<t_ciphertext, t_plaintext>(
            a_ciphertext,
            a_plaintext
        );
    }
    
    /**
     * This function decrypts ciphertext.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_plaintext
     * Plaintext type.
     * @tparam t_key
     * Key type.
     * @tparam t_iv
     * Iv type.
     * @tparam t_ciphertext
     * Ciphertext type.
     * @param a_key
     * Key.
     * @param a_iv
     * Iv.
     * @param a_ciphertext
     * Ciphertext.
     * @return
     * Plaintext.
     */
    template<
        meta::primitive::cipher t_impl,
        meta::bytes t_plaintext = container::bytes_dynamic,
        meta::bytes t_key,
        meta::bytes t_iv,
        meta::bytes t_ciphertext
    >
    constexpr auto
    decrypt(
        const t_key& a_key,
        const t_iv& a_iv,
        const t_ciphertext& a_ciphertext
    ) noexcept(false) -> t_plaintext
    {
        apie<t_impl> apie(
            a_key,
            a_iv
        );
        
        return apie.template decrypt<t_plaintext, t_ciphertext>(
            a_ciphertext
        );
    }
}
