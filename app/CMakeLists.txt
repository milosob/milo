project(
        milo-app
        VERSION ${PROJECT_VERSION}
        LANGUAGES CXX
)

add_executable(
        ${PROJECT_NAME}
        main.cpp
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME milo
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        include
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        milo
)

target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        -Wall
)
