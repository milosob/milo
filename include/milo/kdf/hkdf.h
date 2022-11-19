

#pragma once


#include <milo/kdf/hkdf/hkdf_basic.h>


namespace milo::kdf
{
    template<typename t_hmac>
    using hkdf = hkdf_basic<t_hmac>;
}
