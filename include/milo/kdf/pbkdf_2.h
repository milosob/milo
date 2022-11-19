

#pragma once


#include <milo/kdf/pbkdf_2/pbkdf_2_basic.h>


namespace milo::kdf
{
    template<typename t_prf>
    using pbkdf_2 = pbkdf_2_basic<t_prf>;
}
