

#pragma once


#include <milo/options/basic.h>


namespace milo::options
{
    template<size_t t_value>
    struct digest_size
        : detail::ofvalue<size_t, t_value>
    {
        struct properties
            : detail::ofvalue<size_t, t_value>::properties
        {
            using option_digest_size_type = digest_size;
        };
    };
}

namespace milo::options::detail
{
    template<typename t_0>
    struct is_option_digest_size
        : traits::conditional<
            concepts::option_digest_size<t_0>,
            traits::true_type,
            traits::false_type
        >::type
    {
        using type = t_0;
    };
}

namespace milo::options::query
{
    template<
        typename t_default,
        typename... t_options
    >
    constexpr size_t digest_size = traits::disjunction<
        detail::is_option_digest_size<t_options>...,
        detail::is_option_digest_size<t_default>
    >::type::value;
}
