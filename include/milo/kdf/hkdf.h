

#pragma once


#include <milo/detail/kdf/hkdf.h>


namespace milo::kdf
{
    template<typename t_hmac>
    using hkdf = detail::kdf_hkdf<t_hmac>;
}
