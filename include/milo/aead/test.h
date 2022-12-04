

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/container.h>

#include <milo/aead/apie.h>


namespace milo::aead
{
    /**
     * Aead testing interface.
     *
     * @tparam t_impl
     * Impl type.
     */
    template<
        concepts::aead t_impl
    >
    class test
    {
    public:
        
        using impl_type = t_impl;
    
    public:
        
        /**
         * This function performs encryption test.
         *
         * @tparam t_key
         * Key type.
         * @tparam t_iv
         * Iv type.
         * @tparam t_aad
         * Aad type.
         * @tparam t_plaintext
         * Plaintext type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @tparam t_digest
         * Digest type.
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         * @param a_aad
         * Aad.
         * @param a_plaintext
         * Plaintext.
         * @param a_ciphertext
         * Ciphertext.
         * @param a_digest
         * Digest.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            concepts::bytes t_key,
            concepts::bytes t_iv,
            concepts::bytes t_aad,
            concepts::bytes t_plaintext,
            concepts::bytes t_ciphertext,
            concepts::bytes t_digest
        >
        static
        constexpr auto
        encrypt(
            const t_key& a_key,
            const t_iv& a_iv,
            const t_aad& a_aad,
            const t_plaintext& a_plaintext,
            const t_ciphertext& a_ciphertext,
            const t_digest& a_digest
        ) noexcept(true) -> bool
        {
            try
            {
                auto [ciphertext, digest] = aead::encrypt<impl_type>(
                    a_key,
                    a_iv,
                    a_aad,
                    a_plaintext
                );
    
                if (!memory::equal(
                    ciphertext,
                    a_ciphertext
                ))
                {
                    return false;
                }
    
                if (!memory::match(
                    digest,
                    a_digest
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
         * @tparam t_aad
         * Aad type.
         * @tparam t_ciphertext
         * Ciphertext type.
         * @tparam t_digest
         * Digest type.
         * @tparam t_plaintext
         * Plaintext type.
         * @param a_key
         * Key.
         * @param a_iv
         * Iv.
         * @param a_ciphertext
         * Ciphertext.
         * @param a_digest
         * Digest.
         * @param a_plaintext
         * Plaintext.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            concepts::bytes t_key,
            concepts::bytes t_iv,
            concepts::bytes t_aad,
            concepts::bytes t_ciphertext,
            concepts::bytes t_digest,
            concepts::bytes t_plaintext
        >
        static
        constexpr auto
        decrypt(
            const t_key& a_key,
            const t_iv& a_iv,
            const t_aad& a_aad,
            const t_ciphertext& a_ciphertext,
            const t_digest& a_digest,
            const t_plaintext& a_plaintext
        ) noexcept(true) -> bool
        {
            try
            {
                auto plaintext = aead::decrypt<impl_type>(
                    a_key,
                    a_iv,
                    a_aad,
                    a_ciphertext,
                    a_digest
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
