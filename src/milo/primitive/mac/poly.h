

#pragma once


#include <milo/primitive/detail/mac/poly.h>


namespace milo::primitive::mac
{
    template<typename... t_options>
    using poly_1305 = detail::mac_poly_1305<t_options...>;
}
