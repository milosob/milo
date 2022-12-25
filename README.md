## Description

C++ Cryptographic Library.

Features:

- Header only.
- Software only.
- Portable.
- Constexpr evaluation.
- Throwing and non-throwing overloads.
- Allocation and allocation-free interfaces, depending on the type of parameter.
- Support for the majority of containers implementations as inputs and outputs.
- Type casting free api, no need to cast from `char*` to `unsigned char*`.
- Can return by value the type callee wants, specified by template parameter, as long as its `char` or `unsigned char` container.
- Swapping algorithm by swapping the type.
- Zeroing all sensitive data when destroying an algorithm instance.

Requirements:

- C++ 20 Compiler
- C++ 20 Standard library

Tested compilers:

- `clang version 14.0.6`-`x86-64-linux`
- `gcc version 12.2.0`-`x86-64-linux`

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
Information on how to use them can be found in the [reference](docs/reference.md).

## Integration

Integration guide is [here](docs/integration.md).

## Reference

Reference guide is [here](docs/reference.md).

## Algorithms

Algorithms index is [here](docs/algorithms.md).
