project(
        milo_test_primitive
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

file(
        GLOB
        HASH_TESTS
        hash_*.cpp
)

file(
        GLOB
        MAC_TESTS
        mac_*.cpp
)

file(
        GLOB
        KDF_TESTS
        kdf_*.cpp
)

file(
        GLOB
        CIPHER_TESTS
        cipher_*.cpp
)

file(
        GLOB
        AEAD_TESTS
        aead_*.cpp
)

file(
        GLOB
        CODEC_TEST
        codec_*.cpp
)

set(
        TESTS
        ${HASH_TESTS}
        ${MAC_TESTS}
        ${KDF_TESTS}
        ${CIPHER_TESTS}
        ${AEAD_TESTS}
        ${CODEC_TESTS}
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
