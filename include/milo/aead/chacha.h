

#pragma once


#include <milo/common.h>
#include <milo/concepts.h>
#include <milo/memory.h>

#include <milo/cipher/chacha.h>
#include <milo/mac/poly.h>

#include <milo/detail/aead/chacha/chacha_20_poly_1305.h>


namespace milo::aead
{
    using chacha_20_poly_1305 = detail::aead_chacha_20_poly_1305<cipher::chacha_20, mac::poly_1305>;
}
