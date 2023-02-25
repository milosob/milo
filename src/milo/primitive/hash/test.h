

#pragma once


#include <milo/internal.h>

#include <milo/primitive/hash/apie.h>


namespace milo::primitive::hash
{
    /**
     * Hash testing interface.
     */
    class test
    {
    public:
        
        /**
         * This function performs digest test.
         *
         * @tparam t_impl
         * Implementation type.
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
            meta::primitive::hash t_impl,
            meta::bytes t_message,
            meta::bytes t_digest
        >
        static
        constexpr auto
        digest(
            const t_message& a_message,
            const t_digest& a_digest
        ) noexcept(true) -> bool
        {
            using impl_type = t_impl;
            
            auto digest = hash::digest<impl_type>(
                a_message
            );
            
            if (!internal::memory_equal(
                digest,
                a_digest
            ))
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
            meta::primitive::hash t_impl_0,
            meta::primitive::hash t_impl_1
        >
        static
        constexpr auto
        differential(
        ) noexcept(true) -> bool
        requires
        requires
        {
            requires t_impl_0::block_size == t_impl_1::block_size;
            requires t_impl_0::digest_size == t_impl_1::digest_size;
        }
        {
            using impl_0_type = t_impl_0;
            using impl_1_type = t_impl_1;
            
            constexpr auto input_size = 64 * 8;
            char input[input_size]{};
            
            apie<impl_0_type> impl_0;
            apie<impl_1_type> impl_1;
            
            auto update_loop = [&]<typename t_impl>(
                t_impl& impl,
                size_t update_size
            )
            {
                size_t offs = 0;
                size_t updates = input_size / update_size;
                
                impl.initialize();
                
                for (size_t j = 0; j < updates; j += 1)
                {
                    impl.update(
                        input + offs,
                        update_size
                    );
                    
                    offs += update_size;
                }
                
                if (offs < input_size)
                {
                    impl.update(
                        input + offs,
                        input_size - offs
                    );
                }
            };
            
            size_t grow_size = 3;
            
            for (size_t i = 0; i < input_size; i += 1)
            {
                size_t impl_0_update_size = 1;
                size_t impl_1_update_size = 1;
                
                while (impl_0_update_size <= input_size + grow_size ||
                       impl_1_update_size <= input_size + grow_size)
                {
                    size_t temp;
                    
                    update_loop(
                        impl_0,
                        impl_0_update_size
                    );
                    
                    update_loop(
                        impl_1,
                        impl_1_update_size
                    );
                    
                    temp = impl_0_update_size;
                    impl_0_update_size = impl_1_update_size + grow_size;
                    impl_1_update_size = temp;
                    
                    if (impl_0.digest() != impl_1.digest())
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
