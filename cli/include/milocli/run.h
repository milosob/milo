
#pragma once


#include <milocli/app.h>
#include <milocli/command/benchmark.h>
#include <milocli/command/version.h>
#include <milocli/type.h>


namespace milocli
{
    auto
    run(
        int a_argc,
        char** a_argv
    ) noexcept(false) -> int
    {
        app::args = {
            a_argc,
            a_argv
        };
        app::context = {
            {{
                 "compiler",
                 {
                     {"name", MILO_INTERNAL_COMPILER_NAME},
                     {"version", MILO_INTERNAL_COMPILER_VERSION}
                 }},
             {
                 "project",
                 {
                     {"name", MILO_NAME},
                     {"version", MILO_VERSION_SEMANTIC},
                 }},
             {
                 "command",
                 {
                     {"dump", app::args.dump()},
                     {"args", app::args.args()},
                 }},
             {
                 "result",
                 {
                 }},
            }
        };
        app::options = {
            .verbose = app::args.option("verbose"),
            .advanced = app::args.option("advanced"),
            .suppress = false
        };
        
        auto commands = app_callmap<void()>{
            {"benchmark", command::benchmark::run},
            {"version",   command::version::run},
        };
        
        try
        {
            commands.at(app::args.command())();
        }
        catch (const error& error)
        {
            // TODO
            return 2;
        }
        catch (...)
        {
            // TODO
            return 1;
        }
        
        return 0;
    }
}
