

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
    
    /**
     * This function calculates bit size of byte size.
     * @param a_size
     * Size.
     * @return
     * Bits.
     */
    constexpr auto
    bit_width(
        size_t a_size
    ) noexcept(true) -> size_t
    {
        return a_size * 8;
    }
    
    /**
     * This function calculates bit size of type.
     *
     * @tparam t_value
     * Value type.
     * @return
     * Bits.
     */
    template<
        typename t_value
    >
    constexpr auto
    bit_width(
    ) noexcept(true) -> size_t
    {
        return sizeof(t_value) * 8;
    }
    
    /**
     * This function performs circular bitwise left rotation.
     *
     * @tparam t_value
     * Value type.
     * @param a_value
     * Value.
     * @param a_by
     * By.
     * @return
     * Value << By | Value >> (width(Value) - By).
     */
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
    
    /**
     * This function performs circular bitwise right rotation.
     *
     * @tparam t_value
     * Value type.
     * @param a_value
     * Value.
     * @param a_by
     * By.
     * @return
     * Value >> By | Value << (width(Value) - By).
     */
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
