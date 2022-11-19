

#pragma once


#include <milo/detail/option/basic.h>


namespace milo::detail
{
    template<
        size_t t_value
    >
    struct option_digest_size
        : option_ofvalue<
            size_t,
            t_value
        >
    {
        struct properties
            : option_ofvalue<
                size_t,
                t_value
            >::properties
        {
            using option_digest_size = int;
        };
    };
    
    template<
        typename t_0
    >
    struct option_digest_size_is
        : traits::boolean<
            t_0,
            (
                requires
                {
                    typename t_0::option_digest_size;
                    requires option_ofvalue_is<t_0>::value;
                }
            )
        >
    {
    };
    
    template<
        typename t_default,
        typename... t_options
    >
    struct option_digest_size_query
        : traits::disjunction<
            option_digest_size_is<t_options>...,
            option_digest_size_is<t_default>
        >::type
    {
    };
}
