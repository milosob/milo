

#pragma once


#include <milo/detail/kdf/pbkdf_2.h>


namespace milo::kdf
{
    template<typename t_prf>
    using pbkdf_2 = detail::kdf_pbkdf_2<t_prf>;
}
