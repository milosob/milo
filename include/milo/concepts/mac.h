

#pragma once


#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept mac =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::properties::mac_type;
    } &&
    requires
    {
        t_0::key_size;
        t_0::digest_size;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.update(
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
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
    concept mac_hmac =
    requires
    {
        requires mac<t_0>;
    } &&
    requires
    {
        typename t_0::properties::mac_hmac_type;
    };
    
    template<typename t_0>
    concept mac_poly_1305 =
    requires
    {
        requires mac<t_0>;
    } &&
    requires
    {
        typename t_0::properties::mac_poly_1305_type;
    };
    
    /*
     * @formatter:on
     */
}
