## Description

Cryptography library for modern C++.

Features:

- Constexpr.
- Oneliners.
- Assembly optimized.
- Backend dispatching based on CPU capabilities.
- Throwing and non-throwing overloads.
- Allocation and allocation-free interfaces.
- Support for the majority of container types as inputs and outputs.
- Support for user defined container types.
- Overloads that remove playing with pointers.
- Pointer type casting free api, casting from `char*` to `unsigned char*` is not necessary.
- Returns a value of the type the user needs, specified by template parameter.
- Swapping algorithm by swapping the type.
- Zeroing all sensitive data.

Requirements:

- C++ 20 Compiler
- C++ 20 Standard library

Optional:

- Netwide Assembler

Tested with:

- `x86-64-linux`
    - `clang version 14.0.6`
    - `gcc version 12.2.0`
    - `nasm version 2.16.01`
- `x86-64-windows-11`
    - `msvc version 14.00.24245`
    - `nasm version 2.16.01`

## Motivation

The library aims to provide modern and user-friendly interfaces:

```c++
auto digest = milo::primitive::hash::digest<milo::primitive::hash::sha_2_256<>>("message"sv);
```

```c++
try
{
    using namespace milo::literal;
    
    using aead = milo::primitive::aead::chacha_20_poly_1305<>;
    
    auto key = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"_base_16;
    auto iv  = "000102030405060708090a0b"_base_16;
    
    auto [ciphertext, mac] = milo::primitive::aead::encrypt<aead, std::string>(key, iv, "aad"_cv, "message"_cv);
    auto plaintext         = milo::primitive::aead::decrypt<aead, std::string>(key, iv, "aad"_cv, ciphertext, mac);
}
catch (const milo::error& error)
{
    
}
```

More algorithms and interfaces are available, including stateful streaming interfaces.
Information on how to use them can be found in the [reference](doc/reference.md).

## Integration

Integration guide is [here](doc/integration.md).

## Reference

Reference guide is [here](doc/reference.md).

## Algorithms

Algorithms index is [here](doc/algorithms.md).

## Benchmarking

Benchmarking guide is [here](doc/benchmarking.md).
