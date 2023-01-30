## Reference

This file describes the basics of library.

### Index

- [Index](#index)
- [Intro](#intro)
- [Basics](#basics)
- [Inputs and outputs](#inputs-and-outputs)
- [Endianness](#endianness)
- [Errors](#errors)
- [Constexpr](#constexpr)
- [Testing](#testing)
- [Performance](#performance)
- [Algorithms](#index)
    - [Codec](reference_codec.md)
    - [Hash functions](reference_hash.md)
    - [Message authentication codes](reference_mac.md)
    - [Key derivation functions](reference_kdf.md)
    - [Ciphers](reference_cipher.md)
    - [Authenticated encryption with associated data](reference_aead.md)

### Intro

**Please read before proceeding.**

Library is in early development stage, and changes may occur, changes that could be breaking.
During this phase of development, backward compatibility will not be maintained.
Doing so would severely limit the possibilities of correcting design mistakes.
This approach will allow to deliver the library of the best possible quality before first **major** release,
after which backward compatibility policy will be revised.

Possible early development stage changes that may occur and affect library users:

- Simplification of header files names.
- Moving algorithms into namespaces that better suit their application.
- Hiding implementation details by moving them to the `detail` namespace.

As for branches, definitely do not use `develop` as dependency.
If stability is main concern use the latest tagged release version.
If features and performance is what one seeks for use `master`.

If you are still here, know that I am doing my best to release
only correct implementations with a full suite of repeatable tests.
Even so, I am still human and I can make mistakes.

### Basics

The library defines several sub-namespaces that group a common type of logic:

- `milo::internal` - internal
- `milo::primitive` - algorithms
- `milo::meta` - metaprogramming
- `milo::container` - containers
- `milo::literal` - literals
- `milo::option` - options

The namespace `milo::primitive` is further subdivided into groups:

- codec - `milo::primitive::codec` - bytes encoding/decoding
- hash - `milo::primitive::hash` - hashing functions
- mac - `milo::primitive::mac` - message authentication codes
- kdf - `milo::primitive::kdf` - key derivation functions
- cipher - `milo::primitive::cipher` - ciphers
- aead - `milo::primitive::aead` - authenticated encryption with associated data
- detail - `milo::primitive::detail` - implementation details

Each group except `detail` has the following code component hierarchy:

- `impl` - Algorithm implementations.
    - Actual processing logic.
    - No allocations.
    - Always exception safe.
    - Backend selection and dispatching.
- `apie` - Application programing interface extensions.
    - Wrapper over `impl` types.
    - Sometimes removes necessity of calling certain methods, like `finalize`, `initialize`.
    - Provides constructor overloads for `impl` initialization methods.
    - Single definition per group.
    - Solves the differences if there are any.
    - Propagates constants.
    - Focuses on providing translations from an objective memory, such as `array`, `strings`, `vectors` to pointers.
    - Provides additional interfaces that support:
        - Allocation.
        - Returning by value and reference.
        - Throwing if this is desired way to handle errors.
    - All the common logic is here, significantly reducing potential code repetition.
- `oneliners` - Functions that allow to get work done within a single call.
    - Based on apie.
    - Often much shorter syntax.
    - Modern, e.g. calculate message digest in one call without a single allocation.

Calculate message digest in one call without a single allocation:

```c++
auto digest = milo::primitive::hash::digest<milo::primitive::hash::sha_2_256<>>("message"sv);
```

Which not only looks better but is more handy and less error-prone.
The digest by default is array of bytes, most-likely copy elided by any decent compiler.

All algorithm instances are designed to be copy and move both constructible and assignable.

### Inputs and outputs

The kinds of parameters that the library accepts as inputs or outputs are:

- pointers
- objective memory

When talking about inputs and outputs, the two most used concepts throughout the library have to be introduced:

- `milo::meta::byte` - signed or unsigned char.
- `milo::meta::bytes` - objective memory with `data() -> milo::meta::byte*` and `size() -> size_t` methods.

There are no direct overloads for char pointers, unsigned char pointers, strings, vectors of byte, etc.
It would be hard to maintain and would introduce a lot of redundant code, instead concepts are used.
With concepts, template parameters are constrained and only limited set of types are being accepted.
This way defining a pointer API that limits types to char and unsigned char provides cast free API, same for objective memory.

In the case of objective memory any linear container of bytes like type is accepted:

- `std::string`
- `std::vector<char>`
- `std::vector<unsinged char>`
- `std::array<char, 16>`
- `std::array<unsigned char, 16>`
- `std::span<char>`
- `std::span<char, 16>`
- etc.

Custom containers are also supported as long as they meet the requirements of the `bytes` concept.
The user is free to use the container he likes, even implemented by him.

There is one more important topic to discuss, behavior of the library when the output container is resizable or not.
In case where the type of the `output|result` container is resizable, it will be resized to the correct size.
Otherwise, an opportunistic approach is taken, that callee supplied buffers can hold the result of the operation.

For algorithms where the size of the output is known to be limited, e.g. hash, e.g. maximum 64 bytes.
Capacity limit will be detected and only as much as it is safe to will be written out.
There are interfaces that offer a compile-time error deduction,
depending on whether the requested size was a template or a method argument.

```c++
/*
 * Valid.
 */
auto digest = milo::primitive::hash::digest<milo::primitive::hash::sha_2_256<>>("message"sv);

/*
 * Valid.
 */
auto digest = milo::primitive::hash::digest<milo::primitive::hash::sha_2_256<>, std::array<char, 32>>("message"sv);

/*
 * Compile-time error. The maximum size of the digest is 32.
 */
auto digest = milo::primitive::hash::digest<milo::primitive::hash::sha_2_256<>, std::array<char, 33>>("message"sv);
```

### Endianness

Library was designed to work under little and big endian architectures.
However, so far it has only been tested on little endian platforms.

### Errors

All errors that library can report during execution are defined in `<milo/error.h>` header file.
Errors are reported in two ways:

- Returned by reference.
- Thrown with instance of `milo::error`.

The `milo::error` class does not inherit from `std::exception` or exception types that inherit from it.
The reason for this is that `std::exception` is not usable in `constexpr` contexts.
Whenever objetive memory is used, that can be resized and result with allocation error,
implementation defined exception is thrown, no exception translation occurs.

Both exception safe and throwing API are available.
Only a limited number of operations can cause an error.

Working with the API is similar to this piece of pseudocode:

```c++
milo::error error;

/*
 * Library won't throw.
 * It can still throw parameter-dependent exceptions during a resize that results with an allocation.
 */
operation(paremters..., error);

/*
 * Library can throw.
 */
operation(paremters...);
```

The operation that may result in an error specified by the library will either:

- Take argument of `milo::error` type as the last and return by reference.
- Potentially throw when `milo::error` argument is omitted.

### Constexpr

At the moment, each algorithm supports the constexpr compile-time evaluation.

### Testing

Each algorithm's test program in the library is generated using the Python application `codegen`,
which sources are placed in the `tool/codegen` directory.
Tests are generated based on configuration files.
Some configuration files are located in the `test/primitive/config` directory.

To regenerate tests:

```shell
pip install --user pipenv
pipenv install
pipenv shell
cd test/primitive/config
bash generate.sh
```

Tests are detected by CMake when `MILO_TEST` option is set to `ON`.
Testing constexpr cases can be done when `MILO_TEST_CONSTEXPR` option is set to `ON`.

To understand how to generate custom test programs, analyze the configuration files and `generate.sh` script in the library.
