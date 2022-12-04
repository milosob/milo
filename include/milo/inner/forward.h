

#pragma once


#include <milo/common.h>
#include <milo/traits.h>


namespace milo::inner
{
    template<
        typename t_arg
    >
    constexpr auto
    forward(
        traits::del_reference_t<t_arg>& a_arg
    ) noexcept(true) -> t_arg&&
    {
        return static_cast<t_arg&&>(a_arg);
    }
    
    template<
        typename t_arg
    >
    constexpr auto
    forward(
        traits::del_reference_t<t_arg>&& a_arg
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
        traits::del_reference_t<t_arg>& a_arg
    ) noexcept(true)
    {
        using arg_type = traits::del_reference_t<t_arg>;
        using map_type = t_map<arg_type>;
        using map_to_type = typename map_type::type;
        
        if constexpr (map_type::value)
        {
            if constexpr (traits::is_same_v<arg_type, map_to_type>)
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
        traits::del_reference_t<t_arg>&& a_arg
    ) noexcept(true)
    {
        using arg_type = traits::del_reference_t<t_arg>;
        using map_type = t_map<arg_type>;
        using map_to_type = typename map_type::type;
        
        if constexpr (map_type::value)
        {
            static_assert(!map_type::value);
            
            if constexpr (traits::is_same_v<arg_type, map_to_type>)
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
}
