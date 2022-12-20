project(
        milo-test-internal
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

file(
        GLOB
        MEMORY_TESTS
        memory_*.cpp
)

add_tests(
        ${MEMORY_TESTS}
)
