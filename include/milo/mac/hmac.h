

#pragma once


#include <milo/mac/hmac/hmac_basic.h>


namespace milo::mac
{
    template<typename t_hash>
    using hmac = hmac_basic<t_hash>;
}
