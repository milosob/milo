## Message authentication codes

This file describes message authentication code algorithms.

### Index

- [Index](#index)
- [Algorithms](#algorithms)
    - [Hmac](#algorithms-hmac)
    - [Poly-1305](#algorithms-poly-1305)
- [Mac computation](#mac-computation)
    - [Using pointers](#mac-computation-using-pointers)
    - [Using containers](#mac-computation-using-containers)
    - [Using oneliners](#mac-computation-using-oneliners)
- [More](#more)

### Algorithms

Supported:

- `hmac`
- `poly-1305`

Headers and types:

- `<milo/mac/apie.h>`
    - `milo::mac::apie<mac_type>`
- `<milo/mac/hmac.h>`
    - `milo::mac::hmac<hash_type>`
- `<milo/mac/poly.h>`
    - `milo::mac::poly_1305`

All implementations of the algorithms share the same interface.

Differences that may occur:

- Instance declaration.
- Supported key lengths.
- Resulting digest.

Mac algorithms offer almost exactly the same interface as hashing
algorithms with difference in initialization where key parameter is expected.

Constants:

```c++
using alg_type = ...;

/*
 * Size of the digest.
 */
constexpr auto digest_size = alg_type::digest_size;

/*
 * If this constant is zero, a key of any size is accepted.
 * If this constant value is greater than zero, providing a key whose size:
 *  - Is larger than the constant causes redundant bytes to be ignored.
 *  - Is less than the constant causes missing bytes to be padded with zero.
 */
constexpr auto key_size = alg_type::key_size;
```

#### Algorithms Hmac

```c++
#include <milo/hash/sha.h>
#include <milo/mac/hmac.h>

/*
 * Hmac class template requires a hash type as the template parameter.
 */
milo::mac::hmac<milo::hash::sha_2_256> hmac;
```

#### Algorithms Poly-1305

```c++
#include <milo/mac/poly.h>
    
milo::mac::poly_1305 poly_1305;
```

### Mac computation

#### Mac computation using pointers

```c++
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include <milo/codec/apie.h>
#include <milo/codec/base.h>
#include <milo/hash/sha.h>
#include <milo/mac/hmac.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare alias.
     */
    using mac_type = milo::mac::hmac<milo::hash::sha_2_256>;
    
    auto key     = "key"sv;
    auto message = "message"sv;
    
    /*
     * Declare fixed size buffers.
     * The size of the digest is available in mac_type::digest_size constant.
     */
    char digest_a[mac_type::digest_size]{};
    char digest_b[mac_type::digest_size]{};
    char digest_c[mac_type::digest_size]{};
    char digest_d[mac_type::digest_size]{};
    
    mac_type mac;
    
    /*
     * Initialize with key.
     */
    mac.initialize(key.data(), key.size());
    
    /*
     * Update the message.
     * Works as concatenation.
     * Can be called multiple times.
     */
    mac.update(message.data(), message.size());
    mac.update(message.data(), message.size());
    
    /*
     * Finalize.
     */
    mac.finalize();
    
    /*
     * Get the digest.
     * Can be called multiple times.
     * Returns the size of the stored digest.
     */
    auto digest_a_size = mac.digest(digest_a);
    auto digest_b_size = mac.digest(digest_b);
    
    /*
     * Get the digest again, but this time shorten it.
     * Returns 16.
     */
    auto digest_c_size = mac.digest(digest_c, 16);
    
    /*
     * Get the digest again, but this time ask for more than algorithm can output.
     * There is no overflow.
     * Returns 32.
     */
    auto digest_d_size = mac.digest(digest_d, 100);
    
    
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(std::span(digest_a, digest_a_size)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(std::span(digest_b, digest_b_size)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(std::span(digest_c, digest_c_size)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(std::span(digest_d, digest_d_size)) << "\n";
    
    /*
     * Must be re-initialized before reuse.
     */
    mac.initialize(key.data(), key.size());
    
    return 0;
}
```

#### Mac computation using containers

```c++
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <milo/codec/apie.h>
#include <milo/codec/base.h>
#include <milo/mac/apie.h>
#include <milo/mac/poly.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare alias via apie wrapper.
     */
    using mac_type = milo::mac::apie<milo::mac::poly_1305>;
    
    auto key     = "key"sv;
    auto message = "message"sv;
    
    char digest_a[mac_type::digest_size]{};
    
    /*
     * Construct instance.
     * Initialization takes place in the constructor.
     */
    mac_type mac(key);
    
    /*
     * Update the message.
     * Can be done with objective memory or pointers.
     */
    mac.update(message);
    mac.update(message.data(), message.size());
    
    /*
     * Get the digest.
     * Can be called multiple times.
     * Finalization is not required.
     */
    auto digest_a_size = mac.digest(digest_a);
    
    /*
     * Get the digest by value.
     * Returns std::array of bytes with size of mac_type::digest_size.
     */
    auto digest_returned_by_value_1 = mac.digest();
    
    /*
     * Get the digest by value.
     * Returns a byte array of requested size.
     */
    auto digest_returned_by_value_2 = mac.digest<std::array<char, 12>>();
    
    /*
     * Get the digest by value.
     * Returns a byte vector of the requested size or truncated to the mac limit.
     */
    auto digest_returned_by_value_3 = mac.digest(12);
    
    /*
     * Get the digest by value.
     * Returns a byte vector of the requested size or truncated to the mac limit.
     * Notice requesting size of 5000.
     */
    auto digest_returned_by_value_4 = mac.digest(5000);
    
    std::vector<char>    digest_returned_by_reference_1;
    std::array<char, 12> digest_returned_by_reference_2{};
    
    /*
     * Get the digest by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     *
     * Syntax is mac.digest(target, size = digest_size).
     * Can supply size parameter to get the truncated digest.
     */
    auto digest_returned_by_reference_1_size = mac.digest(digest_returned_by_reference_1);
    
    /*
     * Get the digest by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     * Digest was truncated to 12 due to fixed target.
     *
     * Syntax is mac.digest(target, size = digest_size).
     * Can supply size parameter to get the truncated digest.
     */
    auto digest_returned_by_reference_2_size = mac.digest(digest_returned_by_reference_2);
    
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(std::span(digest_a, digest_a_size)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(digest_returned_by_value_1) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(digest_returned_by_value_2) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(digest_returned_by_value_3) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(digest_returned_by_value_4) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(digest_returned_by_reference_1) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(digest_returned_by_reference_2) << "\n";
    
    /*
     * Must be re-initialized before reuse.
     */
    mac.initialize(key);
    
    return 0;
}
```

#### Mac computation using oneliners

```c++
#include <iostream>
#include <string_view>

#include <milo/codec/apie.h>
#include <milo/codec/base.h>
#include <milo/hash/sha.h>
#include <milo/mac/apie.h>
#include <milo/mac/hmac.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare alias.
     */
    using mac_type = milo::mac::hmac<milo::hash::sha_2_384>;
    
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(milo::mac::digest<mac_type>("first key"sv,"message"sv)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(milo::mac::digest<mac_type>("first key"sv,"message"sv, 16)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(milo::mac::digest<mac_type>("second key"sv, "hello world"sv)) << "\n";
    std::cout << milo::codec::encode<milo::codec::base_16, std::string>(milo::mac::digest<mac_type>("second key"sv, "hello world"sv, 8)) << "\n";
    
    return 0;
}
```

### More

Most likely there are more overloads as needed.
All available options are listed in the header files in the examples above.
All methods to be used come with documentation in code.
