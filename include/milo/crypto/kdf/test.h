

#pragma once


#include <milo/inner.h>

#include <milo/crypto/kdf/apie.h>


namespace milo::crypto::kdf
{
    /**
     * Kdf testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        meta::crypto::kdf t_impl
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
            meta::bytes t_ikm,
            meta::bytes t_salt,
            meta::bytes t_info,
            meta::bytes t_key
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
            requires meta::crypto::kdf_hkdf<impl_type>;
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
                
                if (!inner::memory_equal(
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
            meta::bytes t_ikm,
            meta::bytes t_salt,
            meta::bytes t_key
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
            requires meta::crypto::kdf_pbkdf_2<impl_type>;
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
                
                if (!inner::memory_equal(
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
