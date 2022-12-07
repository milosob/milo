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
Instead, each breakthrough change will be listed in the **release notes**
along with information on how to proceed with the update.
This approach will allow to deliver the library of the best possible quality before first **major** release,
after which backward compatibility policy will be revised.

As for branches, try not to use `master` and definitely do not use `develop` as dependency.
These branches are subject to change, with the only difference being that the first is supposed to be stable.
Most likely, you want to use the tagged release version and keeping the upgrading, according to **release notes**.

Possible early development stage changes that may occur and affect library users:

- Simplification of header files names.
- Moving algorithms into namespaces that better suit their application.
- Hiding implementation details by moving them to the `detail` namespace.

If you are still here, know that I am doing my best to release
only correct implementations with a full suite of repeatable tests.
Even so, I am still human and I can make mistakes.

### Basics

The library consists of groups of cryptographic algorithms.
Each group's algorithms are placed in a dedicated namespace.
Currently, the following groups are defined:

- codec - `milo::crypto::codec` - bytes encoding/decoding
- hash - `milo::crypto::hash` - hashing functions
- mac - `milo::crypto::mac` - message authentication codes
- kdf - `milo::crypto::kdf` - key derivation functions
- cipher - `milo::crypto::cipher` - ciphers
- aead - `milo::crypto::aead` - authenticated encryption with associated data

Each group has the following code component hierarchy:

- `impl` - Algorithm implementations.
    - Actual processing logic.
    - No allocations.
    - Always exception safe.
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
auto digest = milo::crypto::hash::digest<milo::crypto::hash::sha_2_256>("message"sv);
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

In the case of objective memory any byte linear container of bytes like type is accepted:

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

For algorithms where the size of the output is known to be limited, e.g. hash, maximum 64 bytes.
Capacity limit will be detected and only as much as it is safe to will be written out.
There are interfaces that offer a compile-time error deduction,
depending on whether the requested size was a template or a method argument.

```c++
/*
 * Valid.
 */
auto digest = milo::crypto::hash::digest<milo::crypto::hash::sha_2_256>("message"sv);

/*
 * Valid.
 */
auto digest = milo::crypto::hash::digest<milo::crypto::hash::sha_2_256, std::array<char, 32>>("message"sv);

/*
 * Compile-time error. The maximum size of the digest is 32.
 */
auto digest = milo::crypto::hash::digest<milo::crypto::hash::sha_2_256, std::array<char, 33>>("message"sv);
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

Each algorithm's test program in the library is generated using the Python application `milo-codegen`,
which sources are placed in the `apps` directory.
Tests are generated based on configuration files, located in the `tests/configs` directory.

Each configuration file contains information about the source of
the vectors or the software with which they were generated.

Automatically generated tests make up about "75%" of the code base.

To regenerate tests:

```shell
pip install --user pipenv
pipenv install
bash tests/configs/generate.sh
```

Tests are detected by CMake when `MILO_BUILD_TESTS` option is set to `ON`.
Testing constexpr cases can be done when `MILO_BUILD_TESTS_CONSTEXPR` option is set `ON`.
It often takes a few minutes, and the evaluation of `pbkdf_2` consumes most of the time.

To understand how to generate custom test programs, analyze
the configuration files and `generate.sh` the script in the library.

### Performance

Without benchmarking programs that would allow to reproduce tests,
making statements and presenting data about algorithms performance would not be serious.

As for so, as an author I will limit my self to general claims.
Compared to the software versions of the implementations from other libraries, the performance is very similar.
Once optimized assembly or specific hardware targeted instructions kick in, ye it's obvious.
However, in cases where the inputs are small, you may experience better
performance than the heavily optimized implementations, because `milo`:

- Can be fully inlined.
- Never allocates memory unless api that does so was used.

The algorithm benchmarking framework is planned in a future release.
When available, a detailed performance report will appear.
