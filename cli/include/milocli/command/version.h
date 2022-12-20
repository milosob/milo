

#pragma once


#include <iostream>

#include <milocli/app.h>
#include <milocli/dep.h>
#include <milocli/type.h>


namespace milocli::command
{
    auto
    version(
        app_args& a_args,
        app_context& a_context
    ) noexcept(false) -> void
    {
        std::cout << MILO_NAME << " " << MILO_VERSION_SEMANTIC << std::endl;
    }
}
