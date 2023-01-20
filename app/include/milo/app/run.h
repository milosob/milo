
#pragma once


#include <milo/app/app.h>
#include <milo/app/command/benchmark.h>
#include <milo/app/command/version.h>
#include <milo/app/type.h>


namespace milo::app
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
        catch (const std::exception& error)
        {
            std::cerr << error.what() << "\n";
            return 1;
        }
        catch (...)
        {
            std::cerr << "Error. Unexpected behavior." << "\n";
            return 2;
        }
        
        return 0;
    }
}
