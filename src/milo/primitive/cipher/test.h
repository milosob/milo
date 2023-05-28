

#pragma once


#include <milo/internal.h>

#include <milo/primitive/cipher/apie.h>


namespace milo::primitive::cipher
{
    /**
     * Cipher testing interface.
     */
    class test
    {
    public:
        
        /**
         * This function performs encryption test.
         *
         * @tparam t_impl
         * Implementation type.
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
            meta::primitive::cipher t_impl,
            meta::bytes t_key,
            meta::bytes t_iv,
            meta::bytes t_plaintext,
            meta::bytes t_ciphertext
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
            using impl_type = t_impl;
            
            try
            {
                auto ciphertext = cipher::encrypt<impl_type>(
                    a_key,
                    a_iv,
                    a_plaintext
                );
                
                if (!internal::memory_equal(
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
         * @tparam t_impl
         * Implementation type.
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
            meta::primitive::cipher t_impl,
            meta::bytes t_key,
            meta::bytes t_iv,
            meta::bytes t_ciphertext,
            meta::bytes t_plaintext
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
            using impl_type = t_impl;
            
            try
            {
                auto plaintext = cipher::decrypt<impl_type>(
                    a_key,
                    a_iv,
                    a_ciphertext
                );
                
                if (!internal::memory_equal(
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
        
        /**
         * This function performs differential test.
         * Two implementations are compared to check if they yield the same results.
         *
         * @tparam t_impl_0
         * Implementation 0 type.
         * @tparam t_impl_1
         * Implementation 1 type.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            meta::primitive::cipher t_impl_0,
            meta::primitive::cipher t_impl_1
        >
        static
        constexpr auto
        differential(
        ) noexcept(true) -> bool
        {
            using impl_0_type = t_impl_0;
            using impl_1_type = t_impl_1;
            
            apie<impl_0_type> impl_0;
            apie<impl_1_type> impl_1;
            
            constexpr auto iv_size = impl_0_type::iv_size;
            constexpr auto key_size = impl_0_type::key_size;
            constexpr auto blk_size = impl_0_type::block_size;
            constexpr auto data_size = size_t(blk_size * 64);
            
            char iv[iv_size]{};
            char key[key_size]{};
            
            container::bytes_dynamic data;
            container::bytes_dynamic impl_0_encrypted;
            container::bytes_dynamic impl_0_decrypted;
            container::bytes_dynamic impl_1_encrypted;
            container::bytes_dynamic impl_1_decrypted;
            
            try
            {
                /*
                 * @formatter:off
                 */
        
                data.resize(data_size, 0);
                impl_0_encrypted.resize(data_size, 0);
                impl_0_decrypted.resize(data_size, 0);
                impl_1_encrypted.resize(data_size, 0);
                impl_1_decrypted.resize(data_size, 0);
        
                /*
                 * @formatter:on
                 */
            }
            catch (...)
            {
                return false;
            }
            
            auto encrypt = [&](
                auto& impl,
                auto update,
                auto src,
                auto size,
                auto dst
            )
            {
                size_t offs = 0;
                size_t updates = size / update;
                
                impl.initialize(
                    key,
                    key_size,
                    iv,
                    iv_size
                );
                
                for (size_t j = 0; j < updates; j += 1)
                {
                    impl.encrypt(
                        src + offs,
                        update,
                        dst + offs
                    );
                    
                    offs += update;
                }
                
                if (offs < size)
                {
                    impl.encrypt(
                        src + offs,
                        size - offs,
                        dst + offs
                    );
                }
            };
            
            auto decrypt = [&](
                auto& impl,
                auto update,
                auto src,
                auto size,
                auto dst
            )
            {
                size_t offs = 0;
                size_t updates = size / update;
                
                impl.initialize(
                    key,
                    key_size,
                    iv,
                    iv_size
                );
                
                for (size_t j = 0; j < updates; j += 1)
                {
                    impl.decrypt(
                        src + offs,
                        update,
                        dst + offs
                    );
                    
                    offs += update;
                }
                
                if (offs < size)
                {
                    impl.decrypt(
                        src + offs,
                        size - offs,
                        dst + offs
                    );
                }
            };
            
            size_t grow_size = 1;
            
            for (size_t i = 0; i < data_size; i += 1)
            {
                size_t impl_0_update_size = 1;
                size_t impl_1_update_size = 1;
                
                while (impl_0_update_size <= data_size + grow_size ||
                       impl_1_update_size <= data_size + grow_size)
                {
                    size_t temp;
                    
                    encrypt(
                        impl_0,
                        impl_0_update_size,
                        data.data(),
                        data_size,
                        impl_0_encrypted.data()
                    );
                    
                    encrypt(
                        impl_1,
                        impl_1_update_size,
                        data.data(),
                        data_size,
                        impl_1_encrypted.data()
                    );
                    
                    if (!internal::memory_equal(
                        impl_0_encrypted,
                        impl_1_encrypted
                    ))
                    {
                        return false;
                    }
                    
                    decrypt(
                        impl_0,
                        impl_0_update_size,
                        impl_1_encrypted.data(),
                        data_size,
                        impl_0_decrypted.data()
                    );
                    
                    decrypt(
                        impl_1,
                        impl_1_update_size,
                        impl_0_encrypted.data(),
                        data_size,
                        impl_1_decrypted.data()
                    );
                    
                    if (!internal::memory_equal(
                        data,
                        impl_0_decrypted
                    ))
                    {
                        return false;
                    }
                    
                    if (!internal::memory_equal(
                        data,
                        impl_1_decrypted
                    ))
                    {
                        return false;
                    }
                    
                    if (!internal::memory_equal(
                        impl_0_decrypted,
                        impl_1_decrypted
                    ))
                    {
                        return false;
                    }
                    
                    temp = impl_0_update_size;
                    impl_0_update_size = impl_1_update_size + grow_size;
                    impl_1_update_size = temp;
                }
                
                key[i % key_size] = char(i);
                iv[i % iv_size] = char(i);
                data[i] = char(i);
                grow_size += 1;
            }
            
            return true;
        }
    };
}
