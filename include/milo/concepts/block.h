

#pragma once


#include <milo/utility.h>

#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0, typename... t_args>
    concept block_soak =
    requires
    {
        t_0::block_size;
    } &&
    requires(
        const uint8_t* src_ptr,
        size_t blocks,
        t_args&& ... args
    )
    {
        {
        t_0::process(
            src_ptr,
            blocks,
            utility::forward<t_args>(args)...
        )
        } noexcept -> same<void>;
    };
    
    template<typename t_0, typename... t_args>
    concept block_prod =
    requires
    {
        t_0::block_size;
    } &&
    requires(
        uint8_t* dst_ptr,
        t_args&& ... args
    )
    {
        {
        t_0::process(
            dst_ptr,
            utility::forward<t_args>(args)...
        )
        } noexcept -> same<void>;
    };
    
    /*
     * @formatter:on
     */
}
