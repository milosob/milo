project(
        milo_test_internal
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

file(
        GLOB
        MEMORY_TESTS
        memory_*.cpp
)

set(
        TESTS
        ${MEMORY_TESTS}
)

foreach (TEST_SOURCE ${TESTS})
    get_filename_component(
            TEST_NAME
            ${TEST_SOURCE}
            NAME_WE
    )

    add_executable(
            ${TEST_NAME}
            ${TEST_SOURCE}
    )

    target_link_libraries(
            ${TEST_NAME}
            PRIVATE
            milo
    )

    target_compile_options(
            ${TEST_NAME}
            PRIVATE
            -march=native
            $<$<CXX_COMPILER_ID:Clang>:-Wall -fconstexpr-steps=2147483648>
            $<$<CXX_COMPILER_ID:GNU>:-Wall -fconstexpr-ops-limit=2147483648>
            $<$<CXX_COMPILER_ID:MSVC>:/Wall /constexpr:steps2147483648>
    )

    target_compile_definitions(
            ${TEST_NAME}
            PRIVATE
            $<$<BOOL:${MILO_TEST_CONSTEXPR}>:MILO_TEST_CONSTEXPR>
    )

    add_test(
            ${TEST_NAME}
            ${TEST_NAME}
    )

endforeach ()
