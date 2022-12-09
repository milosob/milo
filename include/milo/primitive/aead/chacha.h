

#pragma once


#include <milo/primitive/cipher/chacha.h>
#include <milo/primitive/mac/poly.h>

#include <milo/primitive/detail/aead/chacha/chacha_20_poly_1305.h>


namespace milo::primitive::aead
{
    template<typename... t_options>
    using chacha_20_poly_1305 = detail::aead_chacha_20_poly_1305<cipher::chacha_20<>, mac::poly_1305<>, t_options...>;
}
