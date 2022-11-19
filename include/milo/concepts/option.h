

#pragma once


#include <milo/concepts/basic.h>


namespace milo::concepts
{
    /*
     * @formatter:off
     */
    
    template<typename t_0>
    concept option =
    requires
    {
        typename t_0::properties::option_type;
    };
    
    template<typename t_0>
    concept option_ofvalue =
    requires
    {
        requires option<t_0>;
    } &&
    requires
    {
        typename t_0::properties::option_ofvalue_type;
    } &&
    requires
    {
        {
        t_0::value
        };
    };
    
    template<typename t_0>
    concept option_oftype =
    requires
    {
        requires option<t_0>;
    } &&
    requires
    {
        typename t_0::properties::option_oftype_type;
    } &&
    requires
    {
        {
        typename t_0::type()
        };
    };
    
    template<typename t_0>
    concept option_digest_size =
    requires
    {
        requires option_ofvalue<t_0>;
    } &&
    requires
    {
        typename t_0::properties::option_digest_size_type;
    } &&
    requires
    {
        {
        t_0::value
        };
    };
    
    /*
     * @formatter:on
     */
}
