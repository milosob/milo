

#pragma once


#include <array>
#include <span>
#include <vector>
#include <tuple>

#include <milo/common.h>
#include <milo/memory.h>


namespace milo::container
{
    using std::array;
    using std::vector;
    using std::tuple;
    
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
}
