

#pragma once


#include <milo/common.h>
#include <milo/error.h>
#include <milo/traits.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */

    template<typename t_0>
    concept assignable_bymove = traits::is_assignable_bymove_v<t_0>;
    
    template<typename t_0>
    concept assignable_bymove_noexcept = traits::is_assignable_bymove_noexcept_v<t_0>;
    
    template<typename t_0>
    concept assignable_bycopy = traits::is_assignable_bycopy_v<t_0>;
    
    template<typename t_0>
    concept assignable_bycopy_noexcept = traits::is_assignable_bycopy_noexcept_v<t_0>;
    
    template<typename t_0, typename... t_args>
    concept constructible = traits::is_constructible_v<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    concept constructible_noexcept = traits::is_constructible_noexcept_v<t_0, t_args...>;
    
    template<typename t_0>
    concept constructible_bycopy = traits::is_constructible_bycopy_v<t_0>;
    
    template<typename t_0>
    concept constructible_bycopy_noexcept = traits::is_constructible_bycopy_noexcept_v<t_0>;
    
    template<typename t_0>
    concept constructible_bymove = traits::is_constructible_bymove_v<t_0>;
    
    template<typename t_0>
    concept constructible_bymove_noexcept = traits::is_constructible_bymove_noexcept_v<t_0>;
    
    template<typename t_0, typename t_1>
    concept convertible = traits::is_convertible_v<t_0, t_1>;
    
    template<typename t_0>
    concept destructible = traits::is_destructible_v<t_0>;
    
    template<typename t_0>
    concept destructible_noexcept = traits::is_destructible_noexcept_v<t_0>;
    
    template<typename t_0, typename... t_args>
    concept invocable = traits::is_invocable_v<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    concept invocable_noexcept = traits::is_invocable_noexcept_v<t_0, t_args...>;
    
    template<typename t_0>
    concept integral = traits::is_integral_v<t_0>;
    
    template<typename t_0>
    concept integral_s = traits::is_integral_signed_v<t_0>;
    
    template<typename t_0>
    concept integral_u = traits::is_integral_unsigned_v<t_0>;
    
    template<typename t_0, typename... t_args>
    concept same = traits::is_same_v<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    concept differ = !traits::is_same_v<t_0, t_args...>;
    
    template<typename t_0>
    concept byte = same<t_0, char, signed char, unsigned char>;
    
    template<typename t_0>
    concept bytes =
    requires
    {
        typename t_0::value_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<const char*, const signed char*, const unsigned char*, char*, signed char*, unsigned char*>;
        {
        impl.size()
        } noexcept -> same<size_t>;
    } &&
    requires(const t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<const char*, const signed char*, const unsigned char*, char*, signed char*, unsigned char*>;
        {
        impl.size()
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept container =
    requires
    {
        typename t_0::value_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.data()
        } noexcept -> same<const typename t_0::value_type*, typename t_0::value_type*>;
        {
        impl.size()
        } noexcept -> same<size_t>;
    } &&
    requires(const t_0 impl)
    {
          {
          impl.data()
          } noexcept -> same<const typename t_0::value_type*>;
          {
          impl.size()
          } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept container_dynamic =
    requires
    {
        requires container<t_0>;
    } &&
    requires(t_0 impl)
    {
        {
        impl.reserve(size_t(0))
        } -> same<void>;
        {
        impl.resize(size_t(0), typename t_0::value_type())
        } -> same<void>;
        {
        impl.capacity()
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0, size_t t_min = size_t(0), size_t t_max = size_t(-1)>
    concept container_static =
    requires
    {
        requires container<t_0>;
    } &&
    requires
    {
        {
        /*
         * This will fail if type deduction results with an incomplete type, like vector.
         */
        traits::get_container_static_size<t_0>()
        };
        requires traits::get_container_static_size_v<t_0> >= t_min;
        requires traits::get_container_static_size_v<t_0> <= t_max;
    };
    
    /*
     * @formatter:on
     */
}
