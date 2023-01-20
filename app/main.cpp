

#include <milo/app/run.h>


auto
main(
    int a_argc,
    char** a_argv
) noexcept(false) -> int
{
    return milo::app::run(
        a_argc,
        a_argv
    );
}
