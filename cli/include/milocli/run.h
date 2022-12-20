
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
        auto args = app_args{a_argc, a_argv};
        auto context = app_context{
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
                     {"dump", args.raw()},
                     {"args", args.args()},
                 }},
             {
                 "config",
                 {
                     {"verbose", args.option("verbose")}
                 }},
            }
        };
        
        auto commands = app_callmap<void(
            app_args&,
            app_context&
        )>{
            {"benchmark", command::benchmark},
            {"version",   command::version},
        };
        
        try
        {
            commands.at(args.command())(
                args,
                context
            );
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
