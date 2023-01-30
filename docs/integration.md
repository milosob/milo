## Integration

This file describes how to integrate the library into your project.

### Index

- [Index](#index)
- [Configuration](#configuration)
- [FetchContent](#fetchcontent)
- [Installation](#installation)

### Configuration

Library configuration table.
To override the defaults, provide `-D` prefixed parameter to `cmake` during configuration step.

| Parameter             | Type     | Default | Description                         |
|-----------------------|----------|---------|-------------------------------------|
| `MILO_APP`            | `option` | `OFF`   | Enable building `milo` application. |
| `MILO_ASM`            | `option` | `ON`    | Enable assembly.                    |
| `MILO_TEST`           | `option` | `OFF`   | Enable testing.                     |
| `MILO_TEST_CONSTEXPR` | `option` | `OFF`   | Enable constexpr testing.           |
| `MILO_TEST_INTERNAL`  | `option` | `OFF`   | Enable internal testing.            |
| `MILO_TEST_PRIMITIVE` | `option` | `OFF`   | Enable primitive testing.           |

### FetchContent

Link the library to the target directly with `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
        milo
        GIT_REPOSITORY https://github.com/milosob/milo.git
        GIT_TAG master
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
git clone --branch master https://github.com/milosob/milo.git milo
```

```shell
mkdir milo-build
```

```shell
cd milo-build
```

```shell
cmake ../milo -DMILO_TEST=ON -DMILO_TEST_CONSTEXPR=ON -DMILO_TEST_PRIMITIVE=ON -DMILO_APP=ON && make && ctest
```

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
