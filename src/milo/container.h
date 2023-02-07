

#pragma once


#include <cstdint>
#include <cstddef>
#include <array>
#include <span>
#include <vector>


namespace milo::container
{
    using std::array;
    using std::vector;
    
    template<typename t_value, size_t t_size>
    using view_static = std::span<t_value, t_size>;
    
    template<typename t_value>
    using view_dynamic = std::span<t_value, std::dynamic_extent>;
    
    template<size_t t_size>
    using bytes_static = array<uint8_t, t_size>;
    
    using bytes_dynamic = vector<uint8_t>;
    
    template<size_t t_size>
    using bytes_view_static = view_static<uint8_t, t_size>;
    
    using bytes_view_dynamic = view_dynamic<uint8_t>;
    
    template<size_t t_size>
    using bytes_const_view_static = view_static<const uint8_t, t_size>;
    
    using bytes_const_view_dynamic = view_dynamic<const uint8_t>;
    
    template<size_t t_size>
    using chars_static = array<char, t_size>;
    
    using chars_dynamic = vector<char>;
    
    template<size_t t_size>
    using chars_view_static = view_static<char, t_size>;
    
    using chars_view_dynamic = view_dynamic<char>;
    
    template<size_t t_size>
    using chars_const_view_static = view_static<const char, t_size>;
    
    using chars_const_view_dynamic = view_dynamic<const char>;
    
    template<
        typename... t_args
    >
    struct tuple;
    
    template<>
    struct tuple<>
    {
    };
    
    template<
        typename t_0
    >
    struct tuple<
        t_0
    >
    {
        t_0 item_0;
    };
    
    template<
        typename t_0,
        typename t_1
    >
    struct tuple<
        t_0,
        t_1
    >
    {
        t_0 item_0;
        t_1 item_1;
    };
    
    template<
        typename t_0,
        typename t_1,
        typename t_2
    >
    struct tuple<
        t_0,
        t_1,
        t_2
    >
    {
        t_0 item_0;
        t_1 item_1;
        t_2 item_2;
    };
    
    template<
        typename t_0,
        typename t_1,
        typename t_2,
        typename t_3
    >
    struct tuple<
        t_0,
        t_1,
        t_2,
        t_3
    >
    {
        t_0 item_0;
        t_1 item_1;
        t_2 item_2;
        t_3 item_3;
    };
    
    template<
        typename... t_args
    >
    tuple(
        t_args&& ... a_args
    ) -> tuple<t_args...>;
}
