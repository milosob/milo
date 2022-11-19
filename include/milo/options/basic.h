

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/traits.h>


namespace milo::options::detail
{
    template<
        typename t_option
    >
    struct oftype
    {
        struct properties
        {
            using option_type [[maybe_unused]] = oftype;
    
            using option_oftype_type [[maybe_unused]] = oftype;
        };
    
        using type [[maybe_unused]] = t_option;
    };
    
    template<
        typename t_option,
        t_option t_value
    >
    struct ofvalue
    {
        struct properties
        {
            using option_type [[maybe_unused]] = ofvalue;
    
            using option_ofvalue_type [[maybe_unused]] = ofvalue;
        };
    
        static constexpr t_option value = t_value;
    };
}

namespace milo::options::detail
{
    template<typename t_0>
    struct is_option_oftype
        : traits::conditional<
            concepts::option_oftype<t_0>,
            traits::true_type,
            traits::false_type
        >::type
    {
    };
    
    template<typename t_0>
    struct is_option_ofvalue
        : traits::conditional<
            concepts::option_ofvalue<t_0>,
            traits::true_type,
            traits::false_type
        >::type
    {
    };
}
