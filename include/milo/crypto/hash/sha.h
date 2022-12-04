

#pragma once


#include <milo/crypto/detail/hash/sha/sha_1_160.h>
#include <milo/crypto/detail/hash/sha/sha_2_256.h>
#include <milo/crypto/detail/hash/sha/sha_2_512.h>


namespace milo::crypto::hash
{
    using sha_1_160 = detail::hash_sha_1_160<160>;
    
    using sha_2_224 = detail::hash_sha_2_256<224>;
    
    using sha_2_256 = detail::hash_sha_2_256<256>;
    
    using sha_2_384 = detail::hash_sha_2_512<384>;
    
    using sha_2_512 = detail::hash_sha_2_512<512>;
    
    using sha_2_512_224 = detail::hash_sha_2_512<224>;
    
    using sha_2_512_256 = detail::hash_sha_2_512<256>;
}
