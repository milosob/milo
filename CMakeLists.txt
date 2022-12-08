cmake_minimum_required(VERSION 3.15)
project(
        milo
        VERSION 0.1.0.0
        LANGUAGES CXX
)

include(GNUInstallDirs)

option(MILO_TEST "Enable testing." OFF)
option(MILO_TEST_CONSTEXPR "Enable constexpr testing." OFF)
option(MILO_TEST_INTERNAL "Enable internal testing." OFF)
option(MILO_TEST_PRIMITIVE "Enable primitive testing." OFF)

configure_file(
        ${PROJECT_SOURCE_DIR}/cmake/config.h
        ${PROJECT_SOURCE_DIR}/include/milo/internal/config.h
)

add_library(
        ${PROJECT_NAME}
        INTERFACE
)

target_compile_features(
        ${PROJECT_NAME}
        INTERFACE
        cxx_std_20
)

target_include_directories(
        ${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-config
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
        EXPORT ${PROJECT_NAME}-config
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)

if (${MILO_TEST})
    enable_testing()
    add_subdirectory(test)
endif ()
