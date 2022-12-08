project(
        milo_test
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

if (${MILO_TEST_INTERNAL})
    add_subdirectory(internal)
endif ()

if (${MILO_TEST_PRIMITIVE})
    add_subdirectory(primitive)
endif ()
