

#pragma once


#include <milo/concepts/basic.h>
#include <milo/concepts/cipher.h>
#include <milo/concepts/mac.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept aead =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::type;
        typename t_0::properties;
        typename t_0::properties::aead_type;
    } &&
    requires
    {
        typename t_0::cipher_type;
        typename t_0::mac_type;
    } &&
    requires
    {
        {
        typename t_0::cipher_type()
        } noexcept -> cipher;
        {
        typename t_0::mac_type()
        } noexcept -> mac;
    } &&
    requires
    {
        t_0::key_size;
        t_0::iv_size;
        t_0::digest_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.aad(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.aad(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.aadover()
        } noexcept -> same<void>;
        {
        impl.encrypt(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.encrypt_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt_size(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.decrypt_size(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.finalize()
        } noexcept -> same<void>;
        {
        impl.digest(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<char*>(nullptr)
        )
        } noexcept -> same<size_t>;
        {
        impl.digest(
            static_cast<unsigned char*>(nullptr)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept aead_chacha_20_poly_1305 =
    requires
    {
        requires aead<t_0>;
    } &&
    requires
    {
        typename t_0::properties::aead_chacha_20_poly_1305_type;
    } &&
    requires
    {
        {
        typename t_0::ciph_type()
        } noexcept -> cipher_chacha_20;
        {
        typename t_0::mac_type()
        } noexcept -> mac_poly_1305;
    } &&
    requires
    {
        requires t_0::key_size == 32;
        requires t_0::iv_size == 12;
        requires t_0::digest_size == 16;
    };
    
    /*
     * @formatter:on
     */
}
