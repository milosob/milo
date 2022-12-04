

#pragma once


#include <milo/common.h>
#include <milo/traits.h>


namespace milo::inner
{
    template<
        typename t_arg
    >
    constexpr auto
    move(
        t_arg&& a_arg
    ) noexcept(true) -> traits::del_reference_t<t_arg>&&
    {
        return static_cast<traits::del_reference_t<t_arg>&&>(a_arg);
    }
}
