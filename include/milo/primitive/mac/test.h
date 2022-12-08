

#pragma once


#include <milo/internal.h>

#include <milo/primitive/mac/apie.h>


namespace milo::primitive::mac
{
    /**
     * Mac testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        meta::primitive::mac t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        /**
         * This function performs digest test.
         *
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
         * @return
         * True on success, otherwise returns False.
         */
        template<
            meta::bytes t_key,
            meta::bytes t_message,
            meta::bytes t_digest
        >
        static
        constexpr auto
        digest(
            const t_key& a_key,
            const t_message& a_message,
            const t_digest& a_digest
        ) noexcept(true) -> bool
        {
            if constexpr (impl_type::key_size != 0)
            {
                if (a_key.size() > impl_type::key_size)
                {
                    return false;
                }
            }
            
            auto digest = mac::digest<impl_type>(
                a_key,
                a_message
            );
            
            if (!internal::memory_equal(
                digest,
                a_digest
            ))
            {
                return false;
            }
            
            return true;
        }
    };
}
