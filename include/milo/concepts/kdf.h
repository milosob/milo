

#pragma once


#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept kdf =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::type;
        typename t_0::properties::kdf_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.derive(
            static_cast<char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
        {
        impl.derive(
            static_cast<unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<size_t>;
    };
    
    template<typename t_0>
    concept kdf_pbkdf_2 =
    requires
    {
        requires kdf<t_0>;
    } &&
    requires
    {
        typename t_0::properties::kdf_pbkdf_2_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
            static_cast<const char*>(nullptr),
            size_t(0),
            uint32_t(0)
        )
        } noexcept -> same<void>;
        {
        impl.initialize(
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0),
            uint32_t(0)
        )
        } noexcept -> same<void>;
    };
    
    template<typename t_0>
    concept kdf_hkdf =
    requires
    {
        requires kdf<t_0>;
    } &&
    requires
    {
        typename t_0::properties::kdf_hkdf_type;
    } &&
    requires(t_0 impl)
    {
        {
        impl.initialize(
            static_cast<const char*>(nullptr),
            size_t(0),
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
            size_t(0),
            static_cast<const unsigned char*>(nullptr),
            size_t(0)
        )
        } noexcept -> same<void>;
    };

    /*
    * @formatter:on
    */
}
