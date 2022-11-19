

#pragma once


#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept cipher =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::type;
        typename t_0::properties::cipher_type;
    } &&
    requires
    {
        t_0::key_size;
        t_0::iv_size;
        t_0::block_size;
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
    };
    
    template<typename t_0>
    concept cipher_block =
    requires
    {
        requires cipher<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher_block_type;
    };
    
    template<typename t_0>
    concept cipher_stream =
    requires
    {
        requires cipher<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher_stream_type;
    };
    
    template<typename t_0>
    concept cipher_chacha_20 =
    requires
    {
        requires cipher_stream<t_0>;
    } &&
    requires
    {
        typename t_0::properties::cipher_chacha_20_type;
    } &&
    requires
    {
        requires t_0::key_size == 32;
        requires t_0::iv_size == 16;
    };
    
    /*
     * @formatter:on
     */
}
