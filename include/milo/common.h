

#pragma once


#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>


/*
 * @formatter:off
 */

#define MILO_CONSTEVAL                                              \
    (std::is_constant_evaluated())                                      \
    
/*
 * @formatter:on
 */

namespace milo::common
{
    /**
     * Inheriting from this class disables it.
     */
    struct disabled
    {
        constexpr ~disabled() noexcept(true) = delete;
        
        constexpr auto
        operator =(const disabled&) noexcept(true) -> disabled& = delete;
    };
    
    /**
     * This function selects maximum.
     *
     * @tparam t_value
     * Type of value.
     * @param a_a
     * A.
     * @param a_b
     * B.
     * @return
     * Greater of A and B.
     */
    template<
        typename t_value
    >
    constexpr auto
    max(
        const t_value& a_a,
        const t_value& a_b
    ) noexcept(true) -> const t_value&
    {
        if (a_a < a_b)
        {
            return a_b;
        }
        else
        {
            return a_a;
        }
    }
    
    /**
     * This function selects minimum.
     *
     * @tparam t_value
     * Type of the value.
     * @param a_a
     * A.
     * @param a_b
     * B.
     * @return
     * Lesser of A and B.
     */
    template<
        typename t_value
    >
    constexpr auto
    min(
        const t_value& a_a,
        const t_value& a_b
    ) noexcept(true) -> const t_value&
    {
        if (a_b < a_a)
        {
            return a_b;
        }
        else
        {
            return a_a;
        }
    }
}
