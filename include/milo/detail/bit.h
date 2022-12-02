

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/traits.h>


namespace milo::detail
{
    /*
     * TODO
     *  Replace template width function with trait.
     *  Use std::numeric_limits or CHAR_BIT.
     */
    
    constexpr auto
    bit_width(
        size_t a_size
    ) noexcept(true) -> size_t
    {
        return a_size * 8;
    }
    
    template<
        typename t_value
    >
    constexpr auto
    bit_width(
    ) noexcept(true) -> size_t
    {
        return sizeof(t_value) * 8;
    }
    
    template<
        concepts::integral_u t_value
    >
    constexpr auto
    bit_rotl(
        t_value a_value,
        size_t a_by
    ) noexcept(true) -> t_value
    {
        constexpr auto bits = bit_width<t_value>();
        
        return a_value << a_by | a_value >> (bits - a_by);
    }
    
    template<
        concepts::integral_u t_value
    >
    constexpr auto
    bit_rotr(
        t_value a_value,
        size_t a_by
    ) noexcept(true) -> t_value
    {
        constexpr auto bits = bit_width<t_value>();
        
        return a_value >> a_by | a_value << (bits - a_by);
    }
}
