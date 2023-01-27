

#pragma once


#include <milo/primitive/detail/cipher/chacha/chacha.h>


namespace milo::primitive::cipher
{
    template<typename... t_options>
    using chacha_20 = detail::cipher_chacha<t_options...>;
}
