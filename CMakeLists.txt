cmake_minimum_required(VERSION 3.23)
project(
        milo
        VERSION 0.2.0
        LANGUAGES CXX
)

include(CheckLanguage)
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

add_library(
        ${PROJECT_NAME}
)

# Resolve sources.
set(MILO_HDR_FILES "")
set(MILO_SRC_FILES "")
set(MILO_ASM_FILES "")
set(MILO_ASM_FOUND FALSE)

file(GLOB_RECURSE MILO_HDR_FILES CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE MILO_SRC_FILES CONFIGURE_DEPENDS include/*.cpp)

if (${MILO_ASM})
    if (
            ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR
            ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64"
    )
        check_language(ASM_NASM)
        if (CMAKE_ASM_NASM_COMPILER)
            enable_language(ASM_NASM)
            set(MILO_ASM_FOUND TRUE)
            set(CMAKE_ASM_NASM_FLAGS "-i ${PROJECT_SOURCE_DIR}/include/milo/internal/arch/x86_64")
            file(
                    GLOB_RECURSE MILO_ASM_FILES
                    CONFIGURE_DEPENDS
                    include/milo/internal/arch/x86_64/*.asm
                    include/*x86_64.asm
            )
            message(STATUS "[${PROJECT_NAME}] Assembler for the current target was found, NASM is available")
        else ()
            message(STATUS "[${PROJECT_NAME}] Assembler for the current target was not found, NASM is missing")
        endif ()
    else ()
        message(STATUS "[${PROJECT_NAME}] Assembler for the current target is not used")
    endif ()

    if (${MILO_ASM_FOUND})
        message(STATUS "[${PROJECT_NAME}] Assembler configuration done")
    else ()
        message(STATUS "[${PROJECT_NAME}] Assembler configuration skipped")
    endif ()
endif ()

target_include_directories(
        ${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

target_sources(
        ${PROJECT_NAME}
        PUBLIC
        FILE_SET HEADERS BASE_DIRS include FILES ${MILO_HDR_FILES}
        PRIVATE
        ${MILO_SRC_FILES}
        ${MILO_ASM_FILES}
)

target_compile_definitions(
        ${PROJECT_NAME}
        PUBLIC
        $<$<BOOL:${MILO_ASM_FOUND}>:MILO_INTERNAL_ASM=true>
        $<$<IN_LIST:$<LOWER_CASE:${CMAKE_SYSTEM_NAME}>,linux>:MILO_INTERNAL_ABI_UNIX=true>
        $<$<IN_LIST:$<LOWER_CASE:${CMAKE_SYSTEM_NAME}>,windows>:MILO_INTERNAL_ABI_WIND=true>
)

target_compile_features(
        ${PROJECT_NAME}
        PUBLIC
        cxx_std_20
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
