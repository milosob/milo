

#pragma once


#include <milo/common.h>
#include <milo/traits.h>


namespace milo::detail
{
    template<
        typename t_option
    >
    struct option_oftype
        : private common::disabled
    {
        struct properties
        {
            using option [[maybe_unused]] = int;
            
            using option_oftype [[maybe_unused]] = int;
        };
        
        using type [[maybe_unused]] = t_option;
    };
    
    template<
        typename t_0
    >
    struct option_oftype_is
        : traits::boolean<
            t_0,
            (
                requires
                {
                    typename t_0::properties::option;
                    typename t_0::properties::option_oftype;
                    typename t_0::type;
                }
            )
        >
    {
    };
    
    template<
        typename t_option,
        t_option t_value
    >
    struct option_ofvalue
        : private common::disabled
    {
        struct properties
        {
            using option [[maybe_unused]] = int;
            
            using option_ofvalue [[maybe_unused]] = int;
        };
        
        static
        constexpr t_option value = t_value;
    };
    
    template<
        typename t_0
    >
    struct option_ofvalue_is
        : traits::boolean<
            t_0,
            (
                requires
                {
                    typename t_0::properties::option;
                    typename t_0::properties::option_ofvalue;
                    t_0::value;
                }
            )
        >
    {
    };
}
