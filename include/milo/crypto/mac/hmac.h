

#pragma once


#include <milo/crypto/detail/mac/hmac.h>


namespace milo::crypto::mac
{
    template<typename t_hash>
    using hmac = detail::mac_hmac<t_hash>;
}
