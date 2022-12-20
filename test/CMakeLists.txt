project(
        milo-test
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

function(add_tests)
    foreach (TEST_SOURCE ${ARGN})
        get_filename_component(
                TEST_NAME
                ${TEST_SOURCE}
                NAME_WE
        )

        set(
                TEST_NAME
                ${PROJECT_NAME}-${TEST_NAME}
        )

        string(
                REPLACE
                _
                -
                TEST_NAME_TMP
                ${TEST_NAME}
        )

        set(
                TEST_NAME
                ${TEST_NAME_TMP}
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
                $<$<CXX_COMPILER_ID:Clang>:-Wall -fconstexpr-steps=2147483648 -march=native>
                $<$<CXX_COMPILER_ID:GNU>:-Wall -fconstexpr-ops-limit=2147483648 -march=native>
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
endfunction()

if (${MILO_TEST_INTERNAL})
    add_subdirectory(internal)
endif ()

if (${MILO_TEST_PRIMITIVE})
    add_subdirectory(primitive)
endif ()
