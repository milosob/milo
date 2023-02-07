

#pragma once


#include <milo/meta.h>

#include <milo/internal/base.h>


namespace milo::internal
{
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
    
    template<
        typename t_arg
    >
    constexpr auto
    forward(
        meta::del_reference<t_arg>& a_arg
    ) noexcept(true) -> t_arg&&
    {
        return static_cast<t_arg&&>(a_arg);
    }
    
    template<
        typename t_arg
    >
    constexpr auto
    forward(
        meta::del_reference<t_arg>&& a_arg
    ) noexcept(true) -> t_arg&&
    {
        return static_cast<t_arg&&>(a_arg);
    }
    
    template<
        typename t_arg,
        template<typename> typename t_map,
        template<typename> typename ... t_maps
    >
    constexpr auto
    forward(
        meta::del_reference<t_arg>& a_arg
    ) noexcept(true)
    {
        using arg_type = meta::del_reference<t_arg>;
        using map_type = t_map<arg_type>;
        using map_to_type = typename map_type::type;
        
        if constexpr (map_type::value)
        {
            if constexpr (meta::same<arg_type, map_to_type>)
            {
                return forward<t_arg>(a_arg);
            }
            else
            {
                return reinterpret_cast<map_to_type&&>(a_arg);
            }
        }
        else
        {
            return forward<t_arg, t_maps...>(a_arg);
        }
    }
    
    template<
        typename t_arg,
        template<typename> typename t_map,
        template<typename> typename ... t_maps
    >
    constexpr auto
    forward(
        meta::del_reference<t_arg>&& a_arg
    ) noexcept(true)
    {
        using arg_type = meta::del_reference<t_arg>;
        using map_type = t_map<arg_type>;
        using map_to_type = typename map_type::type;
        
        if constexpr (map_type::value)
        {
            static_assert(!map_type::value);
            
            if constexpr (meta::same<arg_type, map_to_type>)
            {
                return forward<t_arg>(a_arg);
            }
            else
            {
                return reinterpret_cast<map_to_type&&>(a_arg);
            }
        }
        else
        {
            return forward<t_arg, t_maps...>(a_arg);
        }
    }
    
    template<
        typename t_arg
    >
    constexpr auto
    move(
        t_arg&& a_arg
    ) noexcept(true) -> meta::del_reference<t_arg>&&
    {
        return static_cast<meta::del_reference<t_arg>&&>(a_arg);
    }
    
    template<
        typename t_container
    >
    constexpr auto
    resize(
        t_container& a_container,
        size_t a_size
    ) noexcept(meta::container_static<t_container>) -> size_t
    {
        if constexpr (meta::container_dynamic<t_container>)
        {
            a_container.resize(
                a_size,
                typename t_container::value_type()
            );
            
            return a_size;
        }
        else
        {
            return min(
                a_size,
                a_container.size()
            );
        }
    }
}
