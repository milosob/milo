

#pragma once


#include <milo/cipher/chacha_20/chacha_20_basic.h>
#include <milo/cipher/chacha_20/chacha_20_block.h>


namespace milo::cipher
{
    using chacha_20 = chacha_20_basic<chacha_20_block>;
}
