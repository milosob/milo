

#pragma once


#include <milo/primitive/detail/mac/hmac.h>


namespace milo::primitive::mac
{
    template<typename t_hash, typename... t_options>
    using hmac = detail::mac_hmac<t_hash, t_options...>;
}
