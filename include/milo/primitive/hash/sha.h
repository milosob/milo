

#pragma once


#include <milo/primitive/detail/hash/sha/sha_1_160.h>
#include <milo/primitive/detail/hash/sha/sha_2_256.h>
#include <milo/primitive/detail/hash/sha/sha_2_512.h>


namespace milo::primitive::hash
{
    template<typename... t_options>
    using sha_1_160 = detail::hash_sha_1_160<160, t_options...>;
    
    template<typename... t_options>
    using sha_2_224 = detail::hash_sha_2_256<224, t_options...>;
    
    template<typename... t_options>
    using sha_2_256 = detail::hash_sha_2_256<256, t_options...>;
    
    template<typename... t_options>
    using sha_2_384 = detail::hash_sha_2_512<384, t_options...>;
    
    template<typename... t_options>
    using sha_2_512 = detail::hash_sha_2_512<512, t_options...>;
    
    template<typename... t_options>
    using sha_2_512_224 = detail::hash_sha_2_512<224, t_options...>;
    
    template<typename... t_options>
    using sha_2_512_256 = detail::hash_sha_2_512<256, t_options...>;
}
