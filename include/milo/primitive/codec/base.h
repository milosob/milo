

#pragma once


#include <milo/primitive/detail/codec/base_16.h>
#include <milo/primitive/detail/codec/base_64.h>


namespace milo::primitive::codec
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
