

#pragma once


#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept prf =
    requires
    {
        requires constructible_noexcept<t_0>;
        requires destructible_noexcept<t_0>;
    } &&
    requires
    {
        typename t_0::type;
        typename t_0::properties::prf;
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
    concept prf_kdf_pbkdf_2 =
    requires
    {
        requires prf<t_0>;
    } &&
    requires
    {
        requires t_0::key_size == 0;
    };
    
    /*
    * @formatter:on
    */
}
