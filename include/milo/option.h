

#pragma once


#include <milo/detail/option.h>


namespace milo::option
{
    template<size_t t_value>
    using digest_size = milo::detail::option_digest_size<t_value>;
}
