

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
         * @tparam t_impl_a
         * Implementation a type.
         * @tparam t_impl_b
         * Implementation b type.
         * @return
         * True on success, otherwise returns False.
         */
        template<
            meta::primitive::hash t_impl_a,
            meta::primitive::hash t_impl_b
        >
        static
        constexpr auto
        differential(
        ) noexcept(true) -> bool
        requires
        requires
        {
            requires t_impl_a::block_size == t_impl_b::block_size;
            requires t_impl_a::digest_size == t_impl_b::digest_size;
        }
        {
            using impl_0_type = t_impl_a;
            using impl_1_type = t_impl_b;
            
            constexpr auto block_factor = 4;
            constexpr auto block_size = impl_0_type::block_size;
            constexpr auto message_size = block_size * block_factor;
            
            char message[message_size]{};
            
            apie<impl_0_type> impl_0;
            apie<impl_1_type> impl_1;
            
            auto update_loop = [&]<typename t_impl>(
                t_impl& a_impl,
                size_t a_step
            )
            {
                size_t offs = 0;
                size_t step = a_step;
                size_t updates = message_size / step;
                
                a_impl.initialize();
                
                for (size_t j = 0; j < updates; j += 1)
                {
                    a_impl.update(
                        message + offs,
                        step
                    );
                    
                    offs += step;
                }
                
                if (offs < message_size)
                {
                    a_impl.update(
                        message + offs,
                        message_size - offs
                    );
                }
            };
            
            size_t step_a = 1;
            size_t step_b = 1;
            size_t step_grow = 3;
            
            for (size_t i = 0; i < message_size; i += 1)
            {
                while (step_a <= message_size + step_grow ||
                       step_b <= message_size + step_grow)
                {
                    size_t temp;
                    
                    update_loop(
                        impl_0,
                        step_a
                    );
                    
                    update_loop(
                        impl_1,
                        step_b
                    );
                    
                    temp = step_a;
                    step_a = step_b;
                    step_b = temp;
                    
                    if (impl_0.digest() != impl_1.digest())
                    {
                        return false;
                    }
                    
                    step_a += step_grow;
                }
                
                message[i] = char(i);
            }
            
            return true;
        }
    };
}
