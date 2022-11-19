

#pragma once


#include <milo/mac/poly_1305/poly_1305_basic.h>
#include <milo/mac/poly_1305/poly_1305_block.h>


namespace milo::mac
{
    using poly_1305 = poly_1305_basic<poly_1305_block>;
}
