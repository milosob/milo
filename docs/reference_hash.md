## Hash

This file describes hashing algorithms.

### Index

- [Index](#index)
- [Algorithms](#algorithms)
    - [Sha-2](#algorithms-sha-2)
- [Hash computation](#hash-computation)
    - [Using pointers](#hash-computation-using-pointers)
    - [Using containers](#hash-computation-using-containers)
    - [Using oneliners](#hash-computation-using-oneliners)
- [More](#more)

### Algorithms

Supported:

- `sha-1-160`
- `sha-2-224`
- `sha-2-256`
- `sha-2-384`
- `sha-2-512`
- `sha-2-512-224`
- `sha-2-512-256`

Headers and types:

- `<milo/crypto/hash/apie.h>`
  - `milo::crypto::hash::apie<hash_type>`
- `<milo/crypto/hash/sha.h>`
  - `milo::crypto::hash::sha_1_160`
  - `milo::crypto::hash::sha_2_224`
  - `milo::crypto::hash::sha_2_256`
  - `milo::crypto::hash::sha_2_384`
  - `milo::crypto::hash::sha_2_512`
  - `milo::crypto::hash::sha_2_512_224`
  - `milo::crypto::hash::sha_2_512_256`

All implementations of the algorithms share the same interface.

Differences that may occur:

- Instance declaration.
- Resulting digest.

Constants:

```c++
using alg_type = ...;

/*
 * Size of the digest.
 */
constexpr auto digest_size = alg_type::digest_size;

/*
 * Size of the block.
 */
constexpr auto block_size = alg_type::block_size;
```

#### Algorithms Sha-2

```c++
#include <milo/crypto/hash/sha.h>

milo::crypto::hash::sha_1_160       sha_1_160;
milo::crypto::hash::sha_2_224       sha_2_224;
milo::crypto::hash::sha_2_256       sha_2_256;
milo::crypto::hash::sha_2_384       sha_2_384;
milo::crypto::hash::sha_2_512       sha_2_512;
milo::crypto::hash::sha_2_512_224   sha_2_512_224;
milo::crypto::hash::sha_2_512_256   sha_2_512_256;
```

### Hash computation

#### Hash computation using pointers

```c++
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include <milo/crypto/codec/apie.h>
#include <milo/crypto/codec/base.h>
#include <milo/crypto/hash/sha.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare alias.
     */
    using hash_type = milo::crypto::hash::sha_2_256;
    
    auto message = "message"sv;
    
    /*
     * Declare fixed size buffers.
     * The size of the digest is available in hash_type::digest_size constant.
     */
    char digest_a[hash_type::digest_size]{};
    char digest_b[hash_type::digest_size]{};
    char digest_c[hash_type::digest_size]{};
    char digest_d[hash_type::digest_size]{};
    
    hash_type hash;
    
    /*
     * Initialize.
     */
    hash.initialize();
    
    /*
     * Update the message.
     * Works as concatenation.
     * Can be called multiple times.
     */
    hash.update(message.data(), message.size());
    hash.update(message.data(), message.size());
    
    /*
     * Finalize.
     */
    hash.finalize();
    
    /*
     * Get the digest.
     * Can be called multiple times.
     * Returns the size of the stored digest.
     */
    auto digest_a_size = hash.digest(digest_a);
    auto digest_b_size = hash.digest(digest_b);
    
    /*
     * Get the digest again, but this time shorten it.
     * Returns 16.
     */
    auto digest_c_size = hash.digest(digest_c, 16);
    
    /*
     * Get the digest again, but this time ask for more than algorithm can output.
     * There is no overflow.
     * Returns 32.
     */
    auto digest_d_size = hash.digest(digest_d, 100);
    
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(std::span(digest_a, digest_a_size)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(std::span(digest_b, digest_b_size)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(std::span(digest_c, digest_c_size)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(std::span(digest_d, digest_d_size)) << "\n";
    
    /*
     * Must be re-initialized before reuse.
     */
    hash.initialize();
    
    return 0;
}
```

#### Hash computation using containers

```c++
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <milo/crypto/codec/apie.h>
#include <milo/crypto/codec/base.h>
#include <milo/crypto/hash/apie.h>
#include <milo/crypto/hash/sha.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare alias via apie wrapper.
     */
    using hash_type = milo::crypto::hash::apie<milo::crypto::hash::sha_2_256>;
    
    auto message = "message"sv;
    
    char digest_a[hash_type::digest_size]{};
    
    /*
     * Construct instance.
     * Initialization takes place in the constructor.
     */
    hash_type hash;
    
    /*
     * Update the message.
     * Can be done with objective memory or pointers.
     */
    hash.update(message);
    hash.update(message.data(), message.size());
    
    /*
     * Get the digest.
     * Can be called multiple times.
     * Finalization is not required.
     */
    auto digest_a_size = hash.digest(digest_a);
    
    /*
     * Get the digest by value.
     * Returns std::array of bytes with size of hash_type::digest_size.
     */
    auto digest_returned_by_value_1 = hash.digest();
    
    /*
     * Get the digest by value.
     * Returns a byte array of requested size.
     */
    auto digest_returned_by_value_2 = hash.digest<std::array<char, 16>>();
    
    /*
     * Get the digest by value.
     * Returns a byte vector of the requested size or truncated to the hash limit.
     */
    auto digest_returned_by_value_3 = hash.digest(16);
    
    /*
     * Get the digest by value.
     * Returns a byte vector of the requested size or truncated to the hash limit.
     * Notice requesting size of 5000.
     */
    auto digest_returned_by_value_4 = hash.digest(5000);
    
    std::vector<char>    digest_returned_by_reference_1;
    std::array<char, 12> digest_returned_by_reference_2{};
    
    /*
     * Get the digest by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     * 
     * Syntax is hash.digest(target, size = digest_size).
     * Can supply size parameter to get the truncated digest.
     */
    auto digest_returned_by_reference_1_size = hash.digest(digest_returned_by_reference_1);
    
    /*
     * Get the digest by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     * Digest was truncated to 12 due to fixed target.
     * 
     * Syntax is hash.digest(target, size = digest_size).
     * Can supply size parameter to get the truncated digest.
     */
    auto digest_returned_by_reference_2_size = hash.digest(digest_returned_by_reference_2);
    
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(std::span(digest_a, digest_a_size)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(digest_returned_by_value_1) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(digest_returned_by_value_2) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(digest_returned_by_value_3) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(digest_returned_by_value_4) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(digest_returned_by_reference_1) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(digest_returned_by_reference_2) << "\n";
    
    /*
     * Must be re-initialized before reuse.
     */
    hash.initialize();
    
    return 0;
}
```

#### Hash computation using oneliners

```c++
#include <iostream>
#include <string_view>

#include <milo/crypto/codec/apie.h>
#include <milo/crypto/codec/base.h>
#include <milo/crypto/hash/apie.h>
#include <milo/crypto/hash/sha.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare alias.
     */
    using hash_type = milo::crypto::hash::sha_2_256;
    
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(milo::crypto::hash::digest<hash_type>("message"sv)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(milo::crypto::hash::digest<hash_type>("message"sv, 16)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(milo::crypto::hash::digest<hash_type>("hello world"sv)) << "\n";
    std::cout << milo::crypto::codec::encode<milo::crypto::codec::base_16, std::string>(milo::crypto::hash::digest<hash_type>("hello world"sv, 8)) << "\n";
    
    return 0;
}
```

### More

Most likely there are more overloads as needed.
All available options are listed in the header files in the examples above.
All methods to be used come with documentation in code.
