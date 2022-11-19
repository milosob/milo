

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>

#include <milo/hash/apie.h>


namespace milo::hash
{
    /**
     * Hash testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::hash t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        constexpr test() noexcept(true) = delete;
    
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
            concepts::bytes t_message,
            concepts::bytes t_digest
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
    
            if (!memory::equal(
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
