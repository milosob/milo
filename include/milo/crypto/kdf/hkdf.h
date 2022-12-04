

#pragma once


#include <milo/crypto/detail/kdf/hkdf.h>


namespace milo::crypto::kdf
{
    template<typename t_hmac>
    using hkdf = detail::kdf_hkdf<t_hmac>;
}
