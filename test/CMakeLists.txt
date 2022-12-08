project(
        milo_test
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

if (${MILO_TEST_INNER})
    add_subdirectory(inner)
endif ()

if (${MILO_TEST_PRIMITIVE})
    add_subdirectory(primitive)
endif ()
