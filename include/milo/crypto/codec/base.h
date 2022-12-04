

#pragma once


#include <milo/crypto/detail/codec/base_16.h>
#include <milo/crypto/detail/codec/base_64.h>


namespace milo::crypto::codec
{
    /**
     * Base16 coder.
     */
    using base_16 = detail::codec_base_16<>;
    
    /**
     * Base64 coder.
     */
    using base_64 = detail::codec_base_64<>;
}
