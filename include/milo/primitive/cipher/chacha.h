

#pragma once


#include <milo/primitive/detail/cipher/chacha/chacha_20.h>


namespace milo::primitive::cipher
{
    template<typename... t_options>
    using chacha_20 = detail::cipher_chacha_20<t_options...>;
}
