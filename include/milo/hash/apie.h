

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>
#include <milo/error.h>
#include <milo/utility.h>


namespace milo::hash
{
    /**
     * Hash wrapper that provides access to additional APIs.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::hash t_impl
    >
    class apie
    {
    public:
        
        using type = apie;
        
        using impl_type = t_impl;
    
    public:
        
        static
        constexpr auto block_size = impl_type::block_size;
        
        static
        constexpr auto digest_size = impl_type::digest_size;
    
    private:
        
        impl_type m_impl;
        
        bool m_finalized;
    
    public:
        
        constexpr apie() noexcept(true)
        {
            initialize();
        }
        
        constexpr apie(type&& object) noexcept(true) = default;
        
        constexpr apie(const type& object) noexcept(true) = default;
        
        constexpr ~apie() noexcept(true) = default;
    
    public:
        
        constexpr auto
        operator =(const type& object) noexcept(true) -> type& = default;
    
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
         */
        constexpr auto
        initialize(
        ) noexcept(true) -> void
        {
            m_impl.initialize();
            
            m_finalized = false;
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
            concepts::byte t_message
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
            concepts::bytes t_message
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
    
            a_digest_size = utility::resize(
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
    };
    
    /**
     * This function calculates digest.
     *
     * @tparam t_impl
     * Impl type.
     * @tparam t_message
     * Message type.
     * @tparam t_digest
     * Digest type.
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
        concepts::hash t_impl,
        concepts::byte t_message,
        concepts::byte t_digest
    >
    constexpr auto
    digest(
        const t_message* a_message_ptr,
        size_t a_message_size,
        t_digest* a_digest_ptr,
        size_t a_digest_size = t_impl::digest_size
    ) noexcept(true) -> size_t
    {
        apie<t_impl> apie;
    
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
     * @tparam t_message
     * Message type.
     * @tparam t_digest
     * Digest type.
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
        concepts::hash t_impl,
        concepts::bytes t_message,
        concepts::bytes t_digest
    >
    constexpr auto
    digest(
        const t_message& a_message,
        t_digest& a_digest,
        size_t a_digest_size = t_impl::digest_size
    ) noexcept(concepts::container_static<t_digest>) -> size_t
    {
        apie<t_impl> apie;
    
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
     * @tparam t_message
     * Message type.
     * @param a_message
     * Message
     * @param a_digest_size
     * Digest size.
     * @return
     * Digest.
     */
    template<
        concepts::hash t_impl,
        concepts::bytes t_digest = container::bytes_dynamic,
        concepts::bytes t_message
    >
    constexpr auto
    digest(
        const t_message& a_message,
        size_t a_digest_size
    ) noexcept(false) -> t_digest
    {
        apie<t_impl> apie;
    
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
     * @tparam t_message
     * Message type.
     * @param a_message
     * Message
     * @return
     * Digest.
     */
    template<
        concepts::hash t_impl,
        concepts::bytes t_digest = container::bytes_static<t_impl::digest_size>,
        concepts::bytes t_message
    >
    constexpr auto
    digest(
        const t_message& a_message
    ) noexcept(true) -> t_digest
    {
        apie<t_impl> apie;
    
        apie.template update<t_message>(
            a_message
        );
        
        return apie.template digest<t_digest>();
    }
}
