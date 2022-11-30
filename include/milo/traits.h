

#pragma once


#include <array>
#include <type_traits>


namespace milo::traits
{
    template<
        typename t_type,
        bool t_value = t_type::value
    >
    struct boolean
    {
        using type = t_type;
        
        static
        constexpr bool value = t_value;
    };
    
    template<
        typename... t_args
    >
    struct args
    {
    };
    
    template<
        typename t_input,
        template<typename ...> typename t_reader,
        typename... t_reader_args
    >
    struct args_reader
    {
        using type = decltype(
        []<typename... t_args>(
            args<t_args...>
        ) constexpr noexcept(true) -> auto
        {
            return t_reader<
                t_reader_args...,
                t_args...
            >();
        }
            (
                t_input()
            )
        );
    };
    
    template<bool t_cond, typename t_0, typename t_1>
    using conditional = std::conditional<t_cond, t_0, t_1>;
    
    template<typename... t_args>
    using disjunction = std::disjunction<t_args...>;
    
    template<typename t_0>
    using is_assignable_bycopy = std::is_copy_assignable<t_0>;
    
    template<typename t_0>
    constexpr auto is_assignable_bycopy_v = is_assignable_bycopy<t_0>::value;
    
    template<typename t_0>
    using is_assignable_bycopy_noexcept = std::is_nothrow_copy_assignable<t_0>;
    
    template<typename t_0>
    constexpr auto is_assignable_bycopy_noexcept_v = is_assignable_bycopy_noexcept<t_0>::value;
    
    template<typename t_0>
    using is_assignable_bymove = std::is_move_assignable<t_0>;
    
    template<typename t_0>
    constexpr auto is_assignable_bymove_v = is_assignable_bymove<t_0>::value;
    
    template<typename t_0>
    using is_assignable_bymove_noexcept = std::is_nothrow_move_assignable<t_0>;
    
    template<typename t_0>
    constexpr auto is_assignable_bymove_noexcept_v = is_assignable_bymove_noexcept<t_0>::value;
    
    template<typename t_0, typename... t_args>
    using is_constructible = std::is_constructible<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    constexpr auto is_constructible_v = is_constructible<t_0, t_args...>::value;
    
    template<typename t_0, typename... t_args>
    using is_constructible_noexcept = std::is_nothrow_constructible<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    constexpr auto is_constructible_noexcept_v = is_constructible_noexcept<t_0, t_args...>::value;
    
    template<typename t_0>
    using is_constructible_bycopy = std::is_trivially_copyable<t_0>;
    
    template<typename t_0>
    constexpr auto is_constructible_bycopy_v = is_constructible_bycopy<t_0>::value;
    
    template<typename t_0>
    using is_constructible_bycopy_noexcept = std::is_nothrow_copy_constructible<t_0>;
    
    template<typename t_0>
    constexpr auto is_constructible_bycopy_noexcept_v = is_constructible_bycopy_noexcept<t_0>::value;
    
    template<typename t_0>
    using is_constructible_bymove = std::is_trivially_move_constructible<t_0>;
    
    template<typename t_0>
    constexpr auto is_constructible_bymove_v = is_constructible_bymove<t_0>::value;
    
    template<typename t_0>
    using is_constructible_bymove_noexcept = std::is_nothrow_move_assignable<t_0>;
    
    template<typename t_0>
    constexpr auto is_constructible_bymove_noexcept_v = is_constructible_bymove_noexcept<t_0>::value;
    
    template<typename t_0, typename t_1>
    using is_convertible = std::is_convertible<t_0, t_1>;
    
    template<typename t_0, typename t_1>
    constexpr auto is_convertible_v = is_convertible<t_0, t_1>::value;
    
    template<typename t_0>
    using is_destructible = std::is_destructible<t_0>;
    
    template<typename t_0>
    constexpr auto is_destructible_v = is_destructible<t_0>::value;
    
    template<typename t_0>
    using is_destructible_noexcept = std::is_nothrow_destructible<t_0>;
    
    template<typename t_0>
    constexpr auto is_destructible_noexcept_v = is_destructible_noexcept<t_0>::value;
    
    template<typename t_0, typename... t_args>
    using is_invocable = std::is_invocable<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    constexpr auto is_invocable_v = is_invocable<t_0, t_args...>::value;
    
    template<typename t_0, typename... t_args>
    using is_invocable_noexcept = std::is_nothrow_invocable<t_0, t_args...>;
    
    template<typename t_0, typename... t_args>
    constexpr auto is_invocable_noexcept_v = is_invocable_noexcept<t_0, t_args...>::value;
    
    template<typename t_0>
    using is_integral = std::is_integral<t_0>;
    
    template<typename t_0>
    constexpr auto is_integral_v = is_integral<t_0>::value;
    
    template<typename t_0>
    using is_integral_signed = std::is_signed<t_0>;
    
    template<typename t_0>
    constexpr auto is_integral_signed_v = is_integral_signed<t_0>::value;
    
    template<typename t_0>
    using is_integral_unsigned = std::is_unsigned<t_0>;
    
    template<typename t_0>
    constexpr auto is_integral_unsigned_v = is_integral_unsigned<t_0>::value;
    
    template<typename t_0, typename... t_args>
    using is_same = std::disjunction<std::is_same<t_0, t_args>...>;
    
    template<typename t_0, typename... t_args>
    constexpr auto is_same_v = is_same<t_0, t_args...>::value;
    
    template<typename t_0>
    using is_class = std::is_class<t_0>;
    
    template<typename t_0>
    constexpr auto is_class_v = is_class<t_0>::value;
    
    template<typename t_0>
    using add_constant = std::add_const<t_0>;
    
    template<typename t_0>
    using add_constant_t = typename add_constant<t_0>::type;
    
    template<typename t_0>
    using add_constant_volatile = std::add_cv<t_0>;
    
    template<typename t_0>
    using add_constant_volatile_t = typename add_constant_volatile<t_0>::type;
    
    template<typename t_0>
    using add_volatile = std::add_volatile<t_0>;
    
    template<typename t_0>
    using add_volatile_t = typename add_volatile<t_0>::type;
    
    template<typename t_0>
    using del_constant = std::remove_const<t_0>;
    
    template<typename t_0>
    using del_constant_t = typename del_constant<t_0>::type;
    
    template<typename t_0>
    using del_constant_volatile = std::remove_cv<t_0>;
    
    template<typename t_0>
    using del_constant_volatile_t = typename del_constant_volatile<t_0>::type;
    
    template<typename t_0>
    using del_volatile = std::remove_volatile<t_0>;
    
    template<typename t_0>
    using del_volatile_t = typename del_volatile<t_0>::type;
    
    template<typename t_0>
    using del_reference = std::remove_reference<t_0>;
    
    template<typename t_0>
    using del_reference_t = typename del_reference<t_0>::type;
    
    template<typename t_0>
    using get_container_static_size = std::tuple_size<t_0>;
    
    template<typename t_0>
    constexpr auto get_container_static_size_v = get_container_static_size<t_0>::value;
    
    template<typename t_0>
    using to_integral_signed = std::make_signed<t_0>;
    
    template<typename t_0>
    using to_integral_signed_t = typename to_integral_signed<t_0>::type;
    
    template<typename t_0>
    using to_integral_unsigned = std::make_unsigned<t_0>;
    
    template<typename t_0>
    using to_integral_unsigned_t = typename to_integral_unsigned<t_0>::type;
    
    template<
        typename t_arg,
        typename t_to,
        typename... t_args
    >
    struct type_map
        : traits::boolean<
            t_to,
            traits::is_same_v<
                t_arg,
                t_args...
            >
        >
    {
    };
    
    template<
        typename t_arg
    >
    using type_map_char_ptr_to_unsigned_char_ptr =
        type_map<
            t_arg,
            unsigned char*,
            char*
        >;
    
    template<
        typename t_arg
    >
    using type_map_const_char_ptr_to_const_unsigned_char_ptr =
        type_map<
            t_arg,
            const unsigned char*,
            const char*
        >;
}
