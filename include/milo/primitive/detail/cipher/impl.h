

#pragma once


#include <milo/internal.h>


namespace milo::primitive::detail
{
    struct cipher_impl_chooser_stream
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
            return container::tuple{
                /*
                 * TODO
                 *  Find another way to return constants.
                 *  Some dedicated call.
                 */
                t_impl::block_size * t_impl::block_ratio,
                t_impl::template transform<
                    uint8_t,
                    uint8_t
                >,
                t_impl::template generate<
                    uint8_t
                >
            };
        }
    };
    
    struct cipher_impl_invoker_stream
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
            if constexpr (t_id == 0)
            {
                return t_impl::block_size * t_impl::block_ratio;
            }
            
            if constexpr (t_id == 1)
            {
                if MILO_INTERNAL_CONSTEVAL
                {
                    return t_impl::transform(
                        internal::forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
                else
                {
                    return t_impl::transform(
                        internal::forward<
                            t_args,
                            meta::type_map_char_ptr_to_unsigned_char_ptr,
                            meta::type_map_const_char_ptr_to_const_unsigned_char_ptr
                        >(
                            a_args
                        )...
                    );
                }
            }
            
            if constexpr (t_id == 2)
            {
                if MILO_INTERNAL_CONSTEVAL
                {
                    return t_impl::generate(
                        internal::forward<
                            t_args
                        >(
                            a_args
                        )...
                    );
                }
                else
                {
                    return t_impl::generate(
                        internal::forward<
                            t_args,
                            meta::type_map_char_ptr_to_unsigned_char_ptr,
                            meta::type_map_const_char_ptr_to_const_unsigned_char_ptr
                        >(
                            a_args
                        )...
                    );
                }
            }
            
            static_assert(
                t_id == 0 ||
                t_id == 1 ||
                t_id == 2
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
            t_impl&& a_impl,
            t_args&& ...a_args
        ) noexcept(true)
        {
            if constexpr (t_id == 0)
            {
                return a_impl.item_0;
            }
            
            if constexpr (t_id == 1)
            {
                return a_impl.item_1(
                    internal::forward<
                        t_args,
                        meta::type_map_char_ptr_to_unsigned_char_ptr,
                        meta::type_map_const_char_ptr_to_const_unsigned_char_ptr
                    >(
                        a_args
                    )...
                );
            }
            
            if constexpr (t_id == 2)
            {
                return a_impl.item_2(
                    internal::forward<
                        t_args,
                        meta::type_map_char_ptr_to_unsigned_char_ptr,
                        meta::type_map_const_char_ptr_to_const_unsigned_char_ptr
                    >(
                        a_args
                    )...
                );
            }
            
            static_assert(
                t_id == 0 ||
                t_id == 1 ||
                t_id == 2
            );
        }
    };
}
