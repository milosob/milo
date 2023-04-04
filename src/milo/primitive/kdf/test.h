

#pragma once


#include <milo/internal.h>

#include <milo/primitive/kdf/apie.h>


namespace milo::primitive::kdf
{
    /**
     * Kdf testing interface.
     */
    class test
    {
    public:
    
        /**
         * This function performs derive test.
         *
         * @tparam t_impl
         * Implementation type.
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
            meta::primitive::kdf t_impl,
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
            requires meta::primitive::kdf_hkdf<t_impl>;
        }
        {
            using impl_type = t_impl;
            
            try
            {
                auto key = kdf::derive<impl_type>(
                    a_ikm,
                    a_salt,
                    a_info,
                    a_key.size()
                );
                
                if (!internal::memory_equal(
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
         * @tparam t_impl
         * Implementation type.
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
            meta::primitive::kdf t_impl,
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
            requires meta::primitive::kdf_pbkdf_2<t_impl>;
        }
        {
            using impl_type = t_impl;
            
            try
            {
                auto key = kdf::derive<impl_type>(
                    a_ikm,
                    a_salt,
                    a_iterations,
                    a_key.size()
                );
                
                if (!internal::memory_equal(
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
            meta::primitive::kdf t_impl_0,
            meta::primitive::kdf t_impl_1
        >
        static
        constexpr auto
        differential(
        ) noexcept(true) -> bool
        requires
        requires
        {
            requires meta::primitive::kdf_hkdf<t_impl_0>;
            requires meta::primitive::kdf_hkdf<t_impl_1>;
        }
        {
            using impl_0_type = t_impl_0;
            using impl_1_type = t_impl_1;
        
            constexpr size_t input_size = 64 * 4;
            constexpr size_t limit_size = 64 * 4;
        
            char input[input_size]{};
            char key_0[limit_size]{};
            char key_1[limit_size]{};
        
            apie<impl_0_type> impl_0;
            apie<impl_1_type> impl_1;
        
            for (size_t i = 0; i < input_size; i += 1)
            {
                for (size_t j = 0; j < limit_size; j += 1)
                {
                    impl_0.initialize(
                        input,
                        input_size,
                        input,
                        input_size,
                        input,
                        input_size
                    );
                
                    impl_1.initialize(
                        input,
                        input_size,
                        input,
                        input_size,
                        input,
                        input_size
                    );
                
                    impl_0.derive(
                        key_0,
                        j
                    );
                
                    impl_1.derive(
                        key_1,
                        j
                    );
                
                    if (!internal::memory_equal(
                        key_0,
                        key_1,
                        j
                    ))
                    {
                        return false;
                    }
                }
            
                input[i] = char(i);
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
            meta::primitive::kdf t_impl_0,
            meta::primitive::kdf t_impl_1
        >
        static
        constexpr auto
        differential(
        ) noexcept(true) -> bool
        requires
        requires
        {
            requires meta::primitive::kdf_pbkdf_2<t_impl_0>;
            requires meta::primitive::kdf_pbkdf_2<t_impl_1>;
        }
        {
            using impl_0_type = t_impl_0;
            using impl_1_type = t_impl_1;
        
            constexpr size_t input_size = 64 * 2;
            constexpr size_t limit_size = 64 * 2;
        
            char input[input_size]{};
            char key_0[limit_size]{};
            char key_1[limit_size]{};
        
            apie<impl_0_type> impl_0;
            apie<impl_1_type> impl_1;
        
            for (size_t i = 0; i < input_size; i += 1)
            {
                for (size_t j = 0; j < limit_size; j += 1)
                {
                    impl_0.initialize(
                        input,
                        input_size,
                        input,
                        input_size,
                        uint32_t(i)
                    );
                
                    impl_1.initialize(
                        input,
                        input_size,
                        input,
                        input_size,
                        uint32_t(i)
                    );
                
                    impl_0.derive(
                        key_0,
                        j
                    );
                
                    impl_1.derive(
                        key_1,
                        j
                    );
                
                    if (!internal::memory_equal(
                        key_0,
                        key_1,
                        j
                    ))
                    {
                        return false;
                    }
                }
            
                input[i] = char(i);
            }
        
            return true;
        }
    };
}
