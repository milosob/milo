

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>

#include <milo/codec/apie.h>


namespace milo::codec
{
    /**
     * Codec testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::codec t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        /**
         * This function performs encoding test.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from
         * From.
         * @param a_to
         * To.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            typename t_from,
            typename t_to
        >
        static
        constexpr auto
        encode(
            const t_from& a_from,
            const t_to& a_to
        ) noexcept(true) -> bool
        {
            try
            {
                auto to = codec::encode<impl_type>(
                    a_from
                );
    
                if (!memory::equal(
                    to,
                    a_to
                ))
                {
                    return false;
                }
            }
            catch (...)
            {
                return false;
            }
    
            return true;
        }
        
        /**
         * This function performs decoding test.
         *
         * @tparam t_from
         * From type.
         * @tparam t_to
         * To type.
         * @param a_from
         * From.
         * @param a_to
         * To.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            typename t_from,
            typename t_to
        >
        static
        constexpr auto
        decode(
            const t_from& a_from,
            const t_to& a_to
        ) noexcept(true) -> bool
        {
            try
            {
                auto to = codec::decode<impl_type>(
                    a_from
                );
    
                if (!memory::equal(
                    to,
                    a_to
                ))
                {
                    return false;
                }
            }
            catch (...)
            {
                return false;
            }
    
            return true;
        }
    };
}
