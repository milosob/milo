

#pragma once


#include <milo/primitive/detail/codec/base_16.h>
#include <milo/primitive/detail/codec/base_64.h>


namespace milo::primitive::codec
{
    template<typename... t_options>
    using base_16 = detail::codec_base_16<t_options...>;
    
    template<typename... t_options>
    using base_64 = detail::codec_base_64<t_options...>;
}
