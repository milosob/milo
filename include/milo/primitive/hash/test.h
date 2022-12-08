

#pragma once


#include <milo/inner.h>

#include <milo/primitive/hash/apie.h>


namespace milo::primitive::hash
{
    /**
     * Hash testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        meta::primitive::hash t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        /**
         * This function performs digest test.
         *
         * @tparam t_message
         * Message type.
         * @tparam t_digest
         * Digest type.
         * @param a_message
         * Message.
         * @param a_digest
         * Digest.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            meta::bytes t_message,
            meta::bytes t_digest
        >
        static
        constexpr auto
        digest(
            const t_message& a_message,
            const t_digest& a_digest
        ) noexcept(true) -> bool
        {
            auto digest = hash::digest<impl_type>(
                a_message
            );
            
            if (!inner::memory_equal(
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
