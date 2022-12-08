

#pragma once


#include <milo/inner.h>


namespace milo::primitive::mac
{
    /**
     * Mac wrapper that provides access to additional APIs.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        meta::primitive::mac t_impl
        // detail::mac::impl
        // detail::impl_mac
        // detail::concept::impl_mac
        // detail::concept::impl_cipher
        // detail::meta::
    >
    class apie
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        static
        constexpr auto key_size = impl_type::key_size;
        
        static
        constexpr auto digest_size = impl_type::digest_size;
    
    private:
        
        impl_type m_impl;
        
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
         * Constructs object.
         *
         * @tparam t_key
         * Key type.
         * @param a_key_ptr
         * Key pointer.
         * @param a_key_size
         * Key size.
         */
        template<
            meta::byte t_key
        >
        constexpr
        apie(
            const t_key* a_key_ptr,
            size_t a_key_size
        ) noexcept(true)
        {
            initialize(
                a_key_ptr,
                a_key_size
            );
        }
        
        /**
         * Constructs object.
         *
         * @tparam t_key
         * Key type.
         * @param a_key
         * Key.
         */
        template<
            meta::bytes t_key
        >
        constexpr
        apie(
            const t_key& a_key
        ) noexcept(true)
        {
            initialize(
                a_key
            );
        }
    
    private:
        
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
         * @param a_key_ptr
         * Key pointer.
         * @param a_key_size
         * Key size.
         */
        template<
            meta::byte t_key
        >
        constexpr auto
        initialize(
            const t_key* a_key_ptr,
            size_t a_key_size
        ) noexcept(true) -> void
        {
            m_impl.initialize(
                a_key_ptr,
                a_key_size
            );
            
            m_finalized = false;
        }
        
        /**
         * This function initializes context.
         *
         * @tparam t_key
         * Key type.
         * @param a_key
         * Key.
         */
        template<
            meta::bytes t_key
        >
        constexpr auto
        initialize(
            const t_key& a_key
        ) noexcept(true) -> void
        {
            initialize(
                a_key.data(),
                a_key.size()
            );
        }
        
        /**
         * This function updates message.
         *
         * @tparam t_message
         * Message type.
         * @param a_message_ptr
         * Message pointer.
         * @param a_message_size
         * Message size.
         */
        template<
            meta::byte t_message
        >
        constexpr auto
        update(
            const t_message* a_message_ptr,
            size_t a_message_size
        ) noexcept(true) -> void
        {
            m_impl.update(
                a_message_ptr,
                a_message_size
            );
        }
        
        /**
         * This function updates message.
         *
         * @tparam t_message
         * Message type.
         * @param a_message
         * Message.
         */
        template<
            meta::bytes t_message
        >
        constexpr auto
        update(
            const t_message& a_message
        ) noexcept(true) -> void
        {
            update(
                a_message.data(),
                a_message.size()
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
            meta::byte t_digest
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
            meta::bytes t_digest
        >
        constexpr auto
        digest(
            t_digest& a_digest,
            size_t a_digest_size = digest_size
        ) noexcept(meta::container_static<t_digest>) -> size_t
        {
            do_finalize();
            
            a_digest_size = inner::min(
                a_digest_size,
                digest_size
            );
            
            a_digest_size = inner::resize(
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
            meta::bytes t_digest = container::bytes_dynamic
        >
        constexpr auto
        digest(
            size_t a_digest_size
        ) noexcept(false) -> t_digest
        requires
        requires
        {
            requires meta::container_dynamic<t_digest>;
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
            meta::bytes t_digest = container::bytes_static<digest_size>
        >
        constexpr auto
        digest(
        ) noexcept(true) -> t_digest
        requires
        requires
        {
            requires meta::container_static<t_digest, 1, digest_size>;
        }
        {
            t_digest result;
            
            digest(
                result,
                result.size()
            );
            
            return result;
        }
    };
    
    /**
     * This function calculates digest.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_message
     * Message type.
     * @tparam t_digest
     * Digest type.
     * @param a_key_ptr
     * Key pointer.
     * @param a_key_size
     * Key size.
     * @param a_message_ptr
     * Message pointer.
     * @param a_message_size
     * Message size.
     * @param a_digest_ptr
     * Digest pointer.
     * @param a_digest_size
     * Digest size.
     * @return
     * Digest size.
     */
    template<
        meta::primitive::mac t_impl,
        meta::byte t_key,
        meta::byte t_message,
        meta::byte t_digest
    >
    constexpr auto
    digest(
        const t_key* a_key_ptr,
        size_t a_key_size,
        const t_message* a_message_ptr,
        size_t a_message_size,
        t_digest* a_digest_ptr,
        size_t a_digest_size = t_impl::digest_size
    ) noexcept(true) -> size_t
    {
        apie<t_impl> apie(
            a_key_ptr,
            a_key_size
        );
        
        apie.template update<t_message>(
            a_message_ptr,
            a_message_size
        );
        
        return apie.template digest<t_digest>(
            a_digest_ptr,
            a_digest_size
        );
    }
    
    /**
     * This function calculates digest.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_key
     * Key type.
     * @tparam t_message
     * Message type.
     * @tparam t_digest
     * Digest type.
     * @param a_key
     * Key.
     * @param a_message
     * Message.
     * @param a_digest
     * Digest.
     * @param a_digest_size
     * Digest size.
     * @return
     * Digest size.
     */
    template<
        meta::primitive::mac t_impl,
        meta::bytes t_key,
        meta::bytes t_message,
        meta::bytes t_digest
    >
    constexpr auto
    digest(
        const t_key& a_key,
        const t_message& a_message,
        t_digest& a_digest,
        size_t a_digest_size = t_impl::digest_size
    ) noexcept(meta::container_static<t_digest>) -> size_t
    {
        apie<t_impl> apie(
            a_key
        );
        
        apie.template update<t_message>(
            a_message
        );
        
        return apie.template digest<t_digest>(
            a_digest,
            a_digest_size
        );
    }
    
    /**
     * This function calculates digest.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_digest
     * Digest type.
     * @tparam t_key
     * Key type.
     * @tparam t_message
     * Message type.
     * @param a_key
     * Key.
     * @param a_message
     * Message.
     * @param a_digest_size
     * Digest size.
     * @return
     * Digest.
     */
    template<
        meta::primitive::mac t_impl,
        meta::bytes t_digest = container::bytes_dynamic,
        meta::bytes t_key,
        meta::bytes t_message
    >
    constexpr auto
    digest(
        const t_key& a_key,
        const t_message& a_message,
        size_t a_digest_size
    ) noexcept(false) -> t_digest
    {
        apie<t_impl> apie(
            a_key
        );
        
        apie.template update<t_message>(
            a_message
        );
        
        return apie.template digest<t_digest>(
            a_digest_size
        );
    }
    
    /**
     * This function calculates digest.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_digest
     * Digest type.
     * @tparam t_key
     * Key type.
     * @tparam t_message
     * Message type.
     * @param a_key
     * Key.
     * @param a_message
     * Message.
     * @return
     * Digest.
     */
    template<
        meta::primitive::mac t_impl,
        meta::bytes t_digest = container::bytes_static<t_impl::digest_size>,
        meta::bytes t_key,
        meta::bytes t_message
    >
    constexpr auto
    digest(
        const t_key& a_key,
        const t_message& a_message
    ) noexcept(false) -> t_digest
    {
        apie<t_impl> apie(
            a_key
        );
        
        apie.template update<t_message>(
            a_message
        );
        
        return apie.template digest<t_digest>();
    }
}
