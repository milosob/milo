

#pragma once


#include <milo/crypto/cipher/chacha.h>
#include <milo/crypto/mac/poly.h>

#include <milo/crypto/detail/aead/chacha/chacha_20_poly_1305.h>


namespace milo::crypto::aead
{
    using chacha_20_poly_1305 = detail::aead_chacha_20_poly_1305<cipher::chacha_20, mac::poly_1305>;
}
