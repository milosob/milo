cmake_minimum_required(VERSION 3.23)
project(
        milo
        VERSION 0.1.0.0
        LANGUAGES CXX
)

include(GNUInstallDirs)

# Testing options.
option(MILO_TEST "Enable testing." OFF)
option(MILO_TEST_CONSTEXPR "Enable constexpr testing." OFF)
option(MILO_TEST_INTERNAL "Enable internal testing." OFF)
option(MILO_TEST_PRIMITIVE "Enable primitive testing." OFF)

# Assembly options.
option(MILO_ASM "Enable assembly." ON)

# Application options.
option(MILO_APP "Enable application." OFF)

configure_file(
        ${PROJECT_SOURCE_DIR}/cmake/config.h
        ${PROJECT_SOURCE_DIR}/include/milo/internal/config.h
)

add_library(
        ${PROJECT_NAME}
)

# Resolve sources.
set(MILO_HDR_FILES "")
set(MILO_SRC_FILES "")
set(MILO_ASM_FILES "")

file(GLOB_RECURSE MILO_HDR_FILES CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE MILO_SRC_FILES CONFIGURE_DEPENDS include/*.cpp)

if (${MILO_ASM})
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        enable_language(ASM_NASM)
        file(GLOB_RECURSE MILO_ASM_FILES CONFIGURE_DEPENDS include/*_x86_64.asm)
        # TODO Portability.
    endif ()

    target_compile_definitions(
            ${PROJECT_NAME}
            PUBLIC
            MILO_INTERNAL_ASM=true
    )
endif ()

target_sources(
        ${PROJECT_NAME}
        PUBLIC
        FILE_SET HEADERS BASE_DIRS include FILES ${MILO_HDR_FILES}
        PRIVATE
        ${MILO_SRC_FILES}
        ${MILO_ASM_FILES}
)

target_compile_features(
        ${PROJECT_NAME}
        PUBLIC
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
        FILE_SET HEADERS
)

install(
        EXPORT ${PROJECT_NAME}-config
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake
)

if (${MILO_TEST})
    enable_testing()
    add_subdirectory(test)
endif ()

if (${MILO_APP})
    add_subdirectory(app)
endif ()
