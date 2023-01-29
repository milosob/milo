

#pragma once


/*
 * This file contains all primitives aliases.
 * Currently indented for project development use only.
 */

#include <milo/option.h>
#include <milo/primitive/aead/chacha.h>
#include <milo/primitive/cipher/chacha.h>
#include <milo/primitive/codec/base.h>
#include <milo/primitive/hash/sha.h>
#include <milo/primitive/kdf/hkdf.h>
#include <milo/primitive/kdf/pbkdf.h>
#include <milo/primitive/mac/hmac.h>
#include <milo/primitive/mac/poly.h>


namespace milo::primitive
{
    /*
     * @formatter:off
     */
    
    /*
     * codec
     */
    
    using codec_any                               = codec::base_16<>;
    using codec_base_16                           = codec::base_16<>;
    using codec_base_64                           = codec::base_64<>;
    
    /*
     * hash
     */
    
    using hash_any                                = hash::sha_1_160<>;
    using hash_sha_1_160                          = hash::sha_1_160<>;
    using hash_sha_2_224                          = hash::sha_2_224<>;
    using hash_sha_2_256                          = hash::sha_2_256<>;
    using hash_sha_2_384                          = hash::sha_2_384<>;
    using hash_sha_2_512                          = hash::sha_2_512<>;
    using hash_sha_2_512_224                      = hash::sha_2_512_224<>;
    using hash_sha_2_512_256                      = hash::sha_2_512_256<>;
    
    using hash_sha_1_160_sw                       = hash::sha_1_160<option::impl_runtime<detail::hash_sha_1_160_impl_sw>>;
    using hash_sha_2_224_sw                       = hash::sha_2_224<option::impl_runtime<detail::hash_sha_2_256_impl_sw>>;
    using hash_sha_2_256_sw                       = hash::sha_2_256<option::impl_runtime<detail::hash_sha_2_256_impl_sw>>;
    using hash_sha_2_384_sw                       = hash::sha_2_384<option::impl_runtime<detail::hash_sha_2_512_impl_sw>>;
    using hash_sha_2_512_sw                       = hash::sha_2_512<option::impl_runtime<detail::hash_sha_2_512_impl_sw>>;
    using hash_sha_2_512_224_sw                   = hash::sha_2_512_224<option::impl_runtime<detail::hash_sha_2_512_impl_sw>>;
    using hash_sha_2_512_256_sw                   = hash::sha_2_512_256<option::impl_runtime<detail::hash_sha_2_512_impl_sw>>;
    
    using hash_sha_1_160_hw_x86_64_ni             = hash::sha_1_160<option::impl_runtime<detail::hash_sha_1_160_impl_hw_x86_64_ni>>;
    using hash_sha_2_224_hw_x86_64_ni             = hash::sha_2_224<option::impl_runtime<detail::hash_sha_2_256_impl_hw_x86_64_ni>>;
    using hash_sha_2_256_hw_x86_64_ni             = hash::sha_2_256<option::impl_runtime<detail::hash_sha_2_256_impl_hw_x86_64_ni>>;
    
    /*
     * mac
     */
    
    using mac_any                                 = mac::hmac<hash_any>;
    using mac_hmac_sha_1_160                      = mac::hmac<hash_sha_1_160>;
    using mac_hmac_sha_2_224                      = mac::hmac<hash_sha_2_224>;
    using mac_hmac_sha_2_256                      = mac::hmac<hash_sha_2_256>;
    using mac_hmac_sha_2_384                      = mac::hmac<hash_sha_2_384>;
    using mac_hmac_sha_2_512                      = mac::hmac<hash_sha_2_512>;
    using mac_hmac_sha_2_512_224                  = mac::hmac<hash_sha_2_512_224>;
    using mac_hmac_sha_2_512_256                  = mac::hmac<hash_sha_2_512_256>;
    using mac_poly_1305                           = mac::poly_1305<>;
    
    using mac_hmac_sha_1_160_sw                   = mac::hmac<hash_sha_1_160_sw>;
    using mac_hmac_sha_2_224_sw                   = mac::hmac<hash_sha_2_224_sw>;
    using mac_hmac_sha_2_256_sw                   = mac::hmac<hash_sha_2_256_sw>;
    using mac_hmac_sha_2_384_sw                   = mac::hmac<hash_sha_2_384_sw>;
    using mac_hmac_sha_2_512_sw                   = mac::hmac<hash_sha_2_512_sw>;
    using mac_hmac_sha_2_512_224_sw               = mac::hmac<hash_sha_2_512_224_sw>;
    using mac_hmac_sha_2_512_256_sw               = mac::hmac<hash_sha_2_512_256_sw>;
    using mac_poly_1305_sw                        = mac::poly_1305<option::impl_runtime<detail::mac_poly_1305_impl_sw>>;
    
    using mac_hmac_sha_1_160_hw_x86_64_ni         = mac::hmac<hash_sha_1_160_hw_x86_64_ni>;
    using mac_hmac_sha_2_224_hw_x86_64_ni         = mac::hmac<hash_sha_2_224_hw_x86_64_ni>;
    using mac_hmac_sha_2_256_hw_x86_64_ni         = mac::hmac<hash_sha_2_256_hw_x86_64_ni>;
    
    /*
     * kdf-hkdf
     */
    
    using kdf_hkdf_any                            = kdf::hkdf<mac_hmac_sha_1_160>;
    using kdf_hkdf_hmac_sha_1_160                 = kdf::hkdf<mac_hmac_sha_1_160>;
    using kdf_hkdf_hmac_sha_2_224                 = kdf::hkdf<mac_hmac_sha_2_224>;
    using kdf_hkdf_hmac_sha_2_256                 = kdf::hkdf<mac_hmac_sha_2_256>;
    using kdf_hkdf_hmac_sha_2_384                 = kdf::hkdf<mac_hmac_sha_2_384>;
    using kdf_hkdf_hmac_sha_2_512                 = kdf::hkdf<mac_hmac_sha_2_512>;
    using kdf_hkdf_hmac_sha_2_512_224             = kdf::hkdf<mac_hmac_sha_2_512_224>;
    using kdf_hkdf_hmac_sha_2_512_256             = kdf::hkdf<mac_hmac_sha_2_512_256>;
    
    using kdf_hkdf_hmac_sha_1_160_sw              = kdf::hkdf<mac_hmac_sha_1_160_sw>;
    using kdf_hkdf_hmac_sha_2_224_sw              = kdf::hkdf<mac_hmac_sha_2_224_sw>;
    using kdf_hkdf_hmac_sha_2_256_sw              = kdf::hkdf<mac_hmac_sha_2_256_sw>;
    using kdf_hkdf_hmac_sha_2_384_sw              = kdf::hkdf<mac_hmac_sha_2_384_sw>;
    using kdf_hkdf_hmac_sha_2_512_sw              = kdf::hkdf<mac_hmac_sha_2_512_sw>;
    using kdf_hkdf_hmac_sha_2_512_224_sw          = kdf::hkdf<mac_hmac_sha_2_512_224_sw>;
    using kdf_hkdf_hmac_sha_2_512_256_sw          = kdf::hkdf<mac_hmac_sha_2_512_256_sw>;
    
    using kdf_hkdf_hmac_sha_1_160_hw_x86_64_ni    = kdf::hkdf<mac_hmac_sha_1_160_hw_x86_64_ni>;
    using kdf_hkdf_hmac_sha_2_224_hw_x86_64_ni    = kdf::hkdf<mac_hmac_sha_2_224_hw_x86_64_ni>;
    using kdf_hkdf_hmac_sha_2_256_hw_x86_64_ni    = kdf::hkdf<mac_hmac_sha_2_256_hw_x86_64_ni>;
    
    /*
     * kdf-pbkdf-2
     */
    
    using kdf_pbkdf_2_any                         = kdf::pbkdf_2<mac_hmac_sha_1_160>;
    using kdf_pbkdf_2_hmac_sha_1_160              = kdf::pbkdf_2<mac_hmac_sha_1_160>;
    using kdf_pbkdf_2_hmac_sha_2_224              = kdf::pbkdf_2<mac_hmac_sha_2_224>;
    using kdf_pbkdf_2_hmac_sha_2_256              = kdf::pbkdf_2<mac_hmac_sha_2_256>;
    using kdf_pbkdf_2_hmac_sha_2_384              = kdf::pbkdf_2<mac_hmac_sha_2_384>;
    using kdf_pbkdf_2_hmac_sha_2_512              = kdf::pbkdf_2<mac_hmac_sha_2_512>;
    using kdf_pbkdf_2_hmac_sha_2_512_224          = kdf::pbkdf_2<mac_hmac_sha_2_512_224>;
    using kdf_pbkdf_2_hmac_sha_2_512_256          = kdf::pbkdf_2<mac_hmac_sha_2_512_256>;
    
    using kdf_pbkdf_2_hmac_sha_1_160_sw           = kdf::pbkdf_2<mac_hmac_sha_1_160_sw>;
    using kdf_pbkdf_2_hmac_sha_2_224_sw           = kdf::pbkdf_2<mac_hmac_sha_2_224_sw>;
    using kdf_pbkdf_2_hmac_sha_2_256_sw           = kdf::pbkdf_2<mac_hmac_sha_2_256_sw>;
    using kdf_pbkdf_2_hmac_sha_2_384_sw           = kdf::pbkdf_2<mac_hmac_sha_2_384_sw>;
    using kdf_pbkdf_2_hmac_sha_2_512_sw           = kdf::pbkdf_2<mac_hmac_sha_2_512_sw>;
    using kdf_pbkdf_2_hmac_sha_2_512_224_sw       = kdf::pbkdf_2<mac_hmac_sha_2_512_224_sw>;
    using kdf_pbkdf_2_hmac_sha_2_512_256_sw       = kdf::pbkdf_2<mac_hmac_sha_2_512_256_sw>;
    
    using kdf_pbkdf_2_hmac_sha_1_160_hw_x86_64_ni = kdf::pbkdf_2<mac_hmac_sha_1_160_hw_x86_64_ni>;
    using kdf_pbkdf_2_hmac_sha_2_224_hw_x86_64_ni = kdf::pbkdf_2<mac_hmac_sha_2_224_hw_x86_64_ni>;
    using kdf_pbkdf_2_hmac_sha_2_256_hw_x86_64_ni = kdf::pbkdf_2<mac_hmac_sha_2_256_hw_x86_64_ni>;
    
    /*
     * cipher
     */
    
    using cipher_any                              = cipher::chacha_20<>;
    using cipher_chacha_20                        = cipher::chacha_20<>;
    using cipher_chacha_20_sw                     = cipher::chacha_20<option::impl_runtime<detail::cipher_chacha_impl_sw_ietf>>;
    using cipher_chacha_20_hw_x86_64_ssse_3       = cipher::chacha_20<option::impl_runtime<detail::cipher_chacha_impl_hw_x86_64_ssse_3_ietf>>;

    /*
     * aead
     */
    
    using aead_any                                = aead::chacha_20_poly_1305<>;
    using aead_chacha_20_poly_1305                = aead::chacha_20_poly_1305<>;
    using aead_chacha_20_sw_poly_1305_sw          = detail::aead_chacha_20_poly_1305<cipher_chacha_20_sw, mac_poly_1305_sw>;
    
    /*
     * @formatter:on
     */
}
