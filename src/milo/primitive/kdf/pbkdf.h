

#pragma once


#include <milo/primitive/detail/kdf/pbkdf_2.h>


namespace milo::primitive::kdf
{
    template<typename t_prf, typename... t_options>
    using pbkdf_2 = detail::kdf_pbkdf_2<t_prf, t_options...>;
}
