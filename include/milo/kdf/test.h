

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>

#include <milo/kdf/apie.h>


namespace milo::kdf
{
    /**
     * Kdf testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::kdf t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        /**
         * This function performs derive test.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @tparam t_info
         * Info type.
         * @tparam t_key
         * Key type.
         * @param a_ikm
         * Ikm.
         * @param a_salt
         * Salt.
         * @param a_info
         * Info.
         * @param a_key
         * Key.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            concepts::bytes t_ikm,
            concepts::bytes t_salt,
            concepts::bytes t_info,
            concepts::bytes t_key
        >
        static
        constexpr auto
        derive(
            const t_ikm& a_ikm,
            const t_salt& a_salt,
            const t_info& a_info,
            const t_key& a_key
        ) noexcept(true) -> bool
        requires
        requires
        {
            requires concepts::kdf_hkdf<impl_type>;
        }
        {
            try
            {
                auto key = kdf::derive<impl_type>(
                    a_ikm,
                    a_salt,
                    a_info,
                    a_key.size()
                );
    
                if (!memory::equal(
                    key,
                    a_key
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
         * This function performs derive test.
         *
         * @tparam t_ikm
         * Ikm type.
         * @tparam t_salt
         * Salt type.
         * @tparam t_key
         * Key type.
         * @param a_ikm
         * Ikm.
         * @param a_salt
         * Salt.
         * @param a_iterations
         * Iterations.
         * @param a_key.
         * Key.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            concepts::bytes t_ikm,
            concepts::bytes t_salt,
            concepts::bytes t_key
        >
        static
        constexpr auto
        derive(
            const t_ikm& a_ikm,
            const t_salt& a_salt,
            uint32_t a_iterations,
            const t_key& a_key
        ) noexcept(true) -> bool
        requires
        requires
        {
            requires concepts::kdf_pbkdf_2<impl_type>;
        }
        {
            try
            {
                auto key = kdf::derive<impl_type>(
                    a_ikm,
                    a_salt,
                    a_iterations,
                    a_key.size()
                );
    
                if (!memory::equal(
                    key,
                    a_key
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
