

#pragma once


#include <iostream>

#include <milocli/app.h>
#include <milocli/dep.h>
#include <milocli/type.h>


namespace milocli::command::version
{
    auto
    run(
    ) -> void
    {
        std::cout << MILO_NAME << " " << MILO_VERSION_SEMANTIC << std::endl;
    }
}
