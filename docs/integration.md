## Integration

This file describes how to integrate the library into your project.

### Index

- [Index](#index)
- [FetchContent](#fetchcontent)
- [Installation](#installation)

### FetchContent

Link the library to the target directly with `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
        milo
        GIT_REPOSITORY https://github.com/milosob/milo.git
        GIT_TAG 0.1.0
)

FetchContent_MakeAvailable(
        milo
)

target_link_libraries(
        target
        PRIVATE
        milo
)
```

### Installation

Follow the instructions to install the project:

```shell
git clone --branch 0.1.0 https://github.com/milosob/milo.git milo
```

```shell
mkdir milo-build
```

```shell
cd milo-build
```

```shell
cmake ../milo -DMILO_BUILD_TESTS=ON -DMILO_BUILD_TESTS_CONSTEXPR=ON && make && ctest --test-dir tests
```

Skip `-DMILO_BUILD_TESTS_CONSTEXPR=ON` if testing for `constexpr` correctness is not necessary.
Testing constexpr cases can take up to several minutes.

```shell
# Using sudo may be required if installing to /usr/local.
cmake --install .
```

Link the installed library to the target in CMake:

```cmake
find_package(
        milo
        CONFIG REQUIRED
)

target_link_libraries(
        target
        PRIVATE
        milo
)
```
