

#pragma once


#include <milo/app/app.h>
#include <milo/app/dep.h>
#include <milo/app/type.h>


namespace milo::app::command::version
{
    auto
    run(
    ) -> void
    {
        std::cout << MILO_NAME << " " << MILO_VERSION_SEMANTIC << std::endl;
    }
}
