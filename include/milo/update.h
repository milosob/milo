

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>
#include <milo/utility.h>


namespace milo::update
{
    /**
     * This function updates with block-soak function.
     *
     * @tparam t_impl
     * Implementation type.
     * @tparam t_buf
     * Buffer type.
     * @tparam t_src
     * Source type.
     * @tparam t_args
     * Args types.
     * @param a_buf_ptr
     * Buffer pointer.
     * @param a_buf_size
     * Buffer size.
     * @param a_src_ptr
     * Source pointer.
     * @param a_src_size
     * Source size.
     * @param a_args
     * Args.
     * @return
     * Buffer size.
     */
    template<
        typename t_impl,
        concepts::byte t_buf,
        concepts::byte t_src,
        typename... t_args
    >
    constexpr auto
    block_soak(
        t_buf* a_buf_ptr,
        size_t a_buf_size,
        const t_src* a_src_ptr,
        size_t a_src_size,
        t_args&& ... a_args
    ) noexcept(true) -> size_t
    requires
    requires
    {
        requires true;
    }
    {
        using impl_type = t_impl;
        
        constexpr auto block_size = impl_type::block_size;
        
        if (a_buf_size > 0)
        {
            auto left_size = block_size - a_buf_size;
            auto todo_size = common::min(
                left_size,
                a_src_size
            );
            
            memory::copy(
                a_buf_ptr + a_buf_size,
                a_src_ptr,
                todo_size
            );
            
            if (todo_size < left_size)
            {
                return a_buf_size + todo_size;
            }
            
            impl_type::template invoke<
                0
            >(
                a_buf_ptr,
                1,
                utility::forward<
                    t_args
                >(
                    a_args
                )...
            );
            
            a_src_ptr += todo_size;
            a_src_size -= todo_size;
        }
        
        auto full_size = a_src_size / block_size;
        auto last_size = a_src_size % block_size;
        
        if (full_size)
        {
            impl_type::template invoke<
                0
            >(
                a_src_ptr,
                full_size,
                utility::forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        if (last_size)
        {
            a_src_ptr += full_size * block_size;
            
            memory::copy(
                a_buf_ptr,
                a_src_ptr,
                last_size
            );
        }
        
        return last_size;
    }
    
    /**
     * This function updates with block-prod-xor function.
     *
     * @tparam t_block
     * Block type.
     * @tparam t_buf
     * Buffer type.
     * @tparam t_dst
     * Destination type.
     * @tparam t_src
     * Source type.
     * @tparam t_args
     * Args types.
     * @param a_buf_ptr
     * Buffer pointer.
     * @param a_buf_size
     * Buffer size.
     * @param a_dst_ptr
     * Destination pointer.
     * @param a_src_ptr
     * Source pointer.
     * @param a_src_size
     * Source size.
     * @param a_args
     * Args.
     * @return
     * Buffer size.
     */
    template<
        typename t_block,
        concepts::byte t_buf,
        concepts::byte t_dst,
        concepts::byte t_src,
        typename... t_args
    >
    constexpr auto
    block_prod_xor(
        t_buf* a_buf_ptr,
        size_t a_buf_size,
        t_dst* a_dst_ptr,
        const t_src* a_src_ptr,
        size_t a_src_size,
        t_args&& ... a_args
    ) noexcept(true) -> size_t
    requires
    requires
    {
        requires concepts::block_prod<t_block, t_args...>;
    }
    {
        using block_type = t_block;
        
        constexpr auto block_size = block_type::block_size;
        
        if (a_buf_size > 0)
        {
            auto left_size = block_size - a_buf_size;
            auto todo_size = common::min(
                left_size,
                a_src_size
            );
    
            memory::xorr(
                a_dst_ptr,
                a_src_ptr,
                a_buf_ptr + a_buf_size,
                todo_size
            );
            
            if (todo_size < left_size)
            {
                return a_buf_size + todo_size;
            }
            
            a_dst_ptr += todo_size;
            a_src_ptr += todo_size;
            a_src_size -= todo_size;
        }
    
        uint8_t block[block_size];
    
        auto full_size = a_src_size / block_size;
        auto last_size = a_src_size % block_size;
    
        for (size_t i = 0; i < full_size; i += 1)
        {
            block_type::process(
                block,
                utility::forward<t_args>(a_args)...
            );
    
            memory::xorr(
                a_dst_ptr,
                a_src_ptr,
                block,
                block_size
            );
            
            a_dst_ptr += block_size;
            a_src_ptr += block_size;
        }
        
        if (last_size)
        {
            block_type::process(
                a_buf_ptr,
                utility::forward<t_args>(a_args)...
            );
            
            memory::xorr(
                a_dst_ptr,
                a_src_ptr,
                a_buf_ptr,
                last_size
            );
        }
        
        return last_size;
    }
}
