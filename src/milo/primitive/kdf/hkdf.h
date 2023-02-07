

#pragma once


#include <milo/primitive/detail/kdf/hkdf.h>


namespace milo::primitive::kdf
{
    template<typename t_hmac, typename... t_options>
    using hkdf = detail::kdf_hkdf<t_hmac, t_options...>;
}
