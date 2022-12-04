

#pragma once


#include <array>
#include <span>
#include <vector>
#include <tuple>

#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/inner/memory.h>


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
    
    /**
     * This function resizes container.
     *
     * @tparam t_container
     * Container type.
     * @param a_container
     * Container.
     * @param a_size
     * Size.
     * @return
     * Size.
     */
    template<
        typename t_container
    >
    constexpr auto
    resize(
        t_container& a_container,
        size_t a_size
    ) noexcept(concepts::container_static<t_container>) -> size_t
    {
        if constexpr (concepts::container_dynamic<t_container>)
        {
            a_container.resize(
                a_size,
                typename t_container::value_type()
            );
            
            return a_size;
        }
        else
        {
            return common::min(
                a_size,
                a_container.size()
            );
        }
    }
}
