

#pragma once


#include <milo/primitive/detail/mac/hmac.h>


namespace milo::primitive::mac
{
    template<typename t_hash>
    using hmac = detail::mac_hmac<t_hash>;
}
