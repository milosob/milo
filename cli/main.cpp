
#include <milocli/run.h>


auto
main(
    int a_argc,
    char** a_argv
) noexcept(false) -> int
{
    return milocli::run(
        a_argc,
        a_argv
    );
}
