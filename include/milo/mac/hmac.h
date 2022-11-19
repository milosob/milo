

#pragma once


#include <milo/detail/mac/hmac/hmac.h>


namespace milo::mac
{
    template<typename t_hash>
    using hmac = detail::hmac<t_hash>;
}
