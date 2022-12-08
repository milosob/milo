

#pragma once


#include <milo/primitive/detail/kdf/hkdf.h>


namespace milo::primitive::kdf
{
    template<typename t_hmac>
    using hkdf = detail::kdf_hkdf<t_hmac>;
}
