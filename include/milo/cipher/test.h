

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>

#include <milo/cipher/apie.h>


namespace milo::cipher
{
    /**
     * Cipher testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::cipher t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        constexpr test() noexcept(true) = delete;
    
    public:
        
        /**
         * This function performs encryption test.
         *
         * @tparam t_key
         * Key type.
         * @tparam t_iv
         * Iv type.
         * @tparam t_plaintext
         * Plaintext type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         * @param a_plaintext
         * Plaintext.
         * @param a_ciphertext
         * Ciphertext.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            concepts::bytes t_key,
            concepts::bytes t_iv,
            concepts::bytes t_plaintext,
            concepts::bytes t_ciphertext
        >
        static
        constexpr auto
        encrypt(
            const t_key& a_key,
            const t_iv& a_iv,
            const t_plaintext& a_plaintext,
            const t_ciphertext& a_ciphertext
        ) noexcept(true) -> bool
        {
            try
            {
                auto ciphertext = cipher::encrypt<impl_type>(
                    a_key,
                    a_iv,
                    a_plaintext
                );
    
                if (!memory::equal(
                    ciphertext,
                    a_ciphertext
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
         * This function performs decryption test.
         *
         * @tparam t_key
         * Key type.
         * @tparam t_iv
         * Iv type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         * @param a_ciphertext
         * Ciphertext.
         * @param a_plaintext
         * Plaintext.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            concepts::bytes t_key,
            concepts::bytes t_iv,
            concepts::bytes t_ciphertext,
            concepts::bytes t_plaintext
        >
        static
        constexpr auto
        decrypt(
            const t_key& a_key,
            const t_iv& a_iv,
            const t_ciphertext& a_ciphertext,
            const t_plaintext& a_plaintext
        ) noexcept(true) -> bool
        {
            try
            {
                auto plaintext = cipher::decrypt<impl_type>(
                    a_key,
                    a_iv,
                    a_ciphertext
                );
    
                if (!memory::equal(
                    plaintext,
                    a_plaintext
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
