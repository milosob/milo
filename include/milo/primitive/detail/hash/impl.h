

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    struct hash_impl_chooser
    {
        template<
            typename t_impl
        >
        using type = t_impl;
        
        template<
            typename t_impl
        >
        static
        constexpr auto
        hook(
        ) noexcept(true)
        {
            return t_impl::template process<uint8_t>;
        }
    };
    
    struct hash_impl_invoker
    {
        template<
            unsigned t_id,
            typename t_impl,
            typename... t_args
        >
        static
        constexpr auto
        type(
            t_args&& ...a_args
        ) noexcept(true)
        {
            return t_impl::process(
                internal::forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        template<
            unsigned t_id,
            typename t_impl,
            typename... t_args
        >
        static
        constexpr auto
        hook(
            const t_impl& a_impl,
            t_args&& ...a_args
        ) noexcept(true)
        {
            return a_impl(
                internal::forward<
                    t_args,
                    meta::type_map_char_ptr_to_unsigned_char_ptr,
                    meta::type_map_const_char_ptr_to_const_unsigned_char_ptr
                >(
                    a_args
                )...
            );
        }
    };
}
