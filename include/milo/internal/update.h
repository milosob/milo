

#pragma once


#include <milo/meta.h>

#include <milo/internal/base.h>
#include <milo/internal/memory.h>
#include <milo/internal/utility.h>


namespace milo::internal
{
    template<
        typename t_impl,
        meta::byte t_buf,
        meta::byte t_src,
        typename... t_args
    >
    constexpr auto
    update_block_soak(
        t_buf* a_buf_ptr,
        size_t a_buf_size,
        const t_src* a_src_ptr,
        size_t a_src_size,
        t_args&& ... a_args
    ) noexcept(true) -> size_t
    {
        using impl_type = t_impl;
        
        constexpr auto block_size = impl_type::block_size;
        
        if (a_buf_size > 0)
        {
            auto left_size = block_size - a_buf_size;
            auto todo_size = min(
                left_size,
                a_src_size
            );
            
            memory_copy(
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
                forward<
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
                forward<
                    t_args
                >(
                    a_args
                )...
            );
        }
        
        if (last_size)
        {
            a_src_ptr += full_size * block_size;
            
            memory_copy(
                a_buf_ptr,
                a_src_ptr,
                last_size
            );
        }
        
        return last_size;
    }
    
    template<
        typename t_impl,
        meta::byte t_buf,
        meta::byte t_dst,
        meta::byte t_src,
        typename... t_args
    >
    constexpr auto
    update_block_prod_xor(
        t_buf* a_buf_ptr,
        size_t a_buf_size,
        t_dst* a_dst_ptr,
        const t_src* a_src_ptr,
        size_t a_src_size,
        t_args&& ... a_args
    ) noexcept(true) -> size_t
    {
        using impl_type = t_impl;
        
        constexpr auto block_size = impl_type::block_size;
        
        if (a_buf_size > 0)
        {
            auto left_size = block_size - a_buf_size;
            auto todo_size = min(
                left_size,
                a_src_size
            );
            
            memory_xor(
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
            impl_type::template invoke<
                0
            >(
                block,
                forward<
                    t_args
                >(
                    a_args
                )...
            );
            
            memory_xor(
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
            impl_type::template invoke<
                0
            >(
                a_buf_ptr,
                forward<
                    t_args
                >(
                    a_args
                )...
            );
            
            memory_xor(
                a_dst_ptr,
                a_src_ptr,
                a_buf_ptr,
                last_size
            );
        }
        
        return last_size;
    }
}
