## Codec

This file describes codec algorithms.

### Index

- [Index](#index)
- [Algorithms](#algorithms)
    - [Base-16](#algorithms-base-16)
    - [Base-64](#algorithms-base-64)
- [Encoding](#encoding)
    - [Using pointers](#encoding-using-pointers)
    - [Using containers](#encoding-using-containers)
    - [Using oneliners](#encoding-using-oneliners)
- [Decoding](#decoding)
    - [Using pointers](#decoding-using-pointers)
    - [Using containers](#decoding-using-containers)
    - [Using oneliners](#decoding-using-oneliners)
- [More](#more)

### Algorithms

Supported:

- `base-16`
- `base-64`

Headers and types:

- `<milo/primitive/codec/apie.h>`
    - `milo::primitive::codec::apie<codec_type>`
- `<milo/primitive/codec/base.h>`
    - `milo::primitive::codec::base_16`
    - `milo::primitive::codec::base_64`

All implementations of the algorithms share the same interface.

Differences that may occur:

- Resulting encoding.
- Resulting decoding.

Constants:

```c++
using alg_type = ...;

/*
 * Bytes ratio.
 * How many bytes make up the chars chunk.
 */
constexpr auto ratio_bytes_size = alg_type::ratio_bytes_size;

/*
 * Chars ratio
 * How many chars make up the bytes chunk.
 */
constexpr auto ratio_chars_size = alg_type::ratio_chars_size;
```

#### Algorithms Base-16

```c++
#include <milo/primitive/codec/base.h>

using codec_type = milo::primitive::codec::base_16;
```

#### Algorithms Base-64

```c++
#include <milo/primitive/codec/base.h>

using codec_type = milo::primitive::codec::base_64;
```

### Encoding

Encoding is an operation that transforms a given input into a different representation.

#### Encoding using pointers

```c++
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/base.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare codec alias.
     */
    using codec_type = milo::primitive::codec::base_64;
    
    auto message = "message"sv;
    auto encoded = std::string();
    
    /*
     * Get the size of the buffer to allocate.
     */
    auto encoded_size = codec_type::encode_size(message.data(), message.size());
    
    /*
     * Allocate memory.
     */
    encoded.resize(encoded_size);
    
    /*
     * Encode. 
     * Returns size of the result.
     */
    encoded_size = codec_type::encode(message.data(), message.size(), encoded.data());
    
    /*
     * Resize the buffer to the correct size.
     */
    encoded.resize(encoded_size);
    
    /*
     * Prints "bWVzc2FnZQ==".
     */
    std::cout << encoded << "\n";
    
    return 0;
}
```

#### Encoding using containers

```c++
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare codec alias via apie wrapper.
     */
    using codec_type = milo::primitive::codec::apie<milo::primitive::codec::base_64>;
    
    auto message = "message"sv;
    auto encoded = std::string();
    
    /*
     * Get the result by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     */
    auto encoded_size = codec_type::encode(message, encoded);
    
    /*
     * Prints "bWVzc2FnZQ==".
     */
    std::cout << encoded << "\n";
    
    /*
     * Get the result by value.
     * When retrieving the result by value, pass the resulting container type as a template parameter.
     * Only dynamic containers are supported.
     * Prints "bWVzc2FnZQ==".
     */
    std::cout << codec_type::encode<std::string>(message) << "\n";
    
    return 0;
}
```

#### Encoding using oneliners

```c++
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare codec alias.
     */
    using codec_type = milo::primitive::codec::base_64;
    
    auto message = "message"sv;
    auto encoded = std::string();
    
    /*
     * With oneliners codec_type is passed as the first template parameter to the method.
     */
    
    /*
     * Get the result by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     */
    auto encoded_size = milo::primitive::codec::encode<codec_type>(message, encoded);
    
    /*
     * Prints "bWVzc2FnZQ==".
     */
    std::cout << encoded << "\n";
    
    /*
     * Get the result by value.
     * When retrieving the result by value, pass the resulting container type as a template parameter.
     * Only dynamic containers are supported.
     * Prints "bWVzc2FnZQ==".
     */
    std::cout << milo::primitive::codec::encode<codec_type, std::string>(message) << "\n";
    
    return 0;
}
```

### Decoding

#### Decoding using pointers

```c++
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/base.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare codec alias.
     */
    using codec_type = milo::primitive::codec::base_64;
    
    auto message = "bWVzc2FnZQ=="sv;
    auto decoded = std::string();
    auto error = milo::error();
    
    /*
     * Get the size of the buffer to allocate.
     */
    auto decoded_size = codec_type::decode_size(message.data(), message.size());
    
    /*
     * Allocate memory.
     */
    decoded.resize(decoded_size);
    
    /*
     * Decode. 
     * Returns size of the result.
     */
    decoded_size = codec_type::decode(message.data(), message.size(), decoded.data(), error);
    
    /*
     * May fail if the value or size of the input is invalid.
     * Exception throwing interfaces are available through the apie wrapper.
     */
    if (error)
    {
        std::cerr << "Unexpected decoding error.\n";
        return -1;
    }
    
    /*
     * Resize the buffer to the correct size.
     */
    decoded.resize(decoded_size);
    
    /*
     * Prints "message".
     */
    std::cout << decoded << "\n";
    
    return 0;
}
```

#### Decoding using containers

```c++
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare codec alias via apie wrapper.
     */
    using codec_type = milo::primitive::codec::apie<milo::primitive::codec::base_64>;
    
    auto message = "bWVzc2FnZQ=="sv;
    auto message_invalid = "bWVzABC$c2FnZQ=="sv;
    auto decoded = std::string();
    auto error = milo::error();
    
    /*
     * Get the result by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     * Error is cleared on success, set on failure.
     */
    auto decoded_size = codec_type::decode(message, decoded, error);
    
    /*
     * May fail if the value or size of the input is invalid.
     */
    if (error)
    {
        std::cout << "Unexpected decoding error.\n";
        return -1;
    }
    else
    {
        /*
         * Prints "message".
         */
        std::cout << decoded << "\n";
    }
    
    try
    {
        /*
         * When the error parameter is omitted, an exception will be thrown on error.
         */
        codec_type::decode(message_invalid, decoded);
    }
    catch (const milo::error& e)
    {
        /*
         * At this point, the value of the decoded variable may be different.
         */
        std::cout << "Expected decoding error.\n";
    }
    
    try
    {
        /*
         * Get the result by value.
         * When retrieving the result by value, pass the resulting container type as a template parameter.
         * Only dynamic containers are supported.
         * Signalizing error by reference is not supported.
         * Prints "message".
         */
        std::cout << codec_type::decode<std::string>(message) << "\n";
    }
    catch (const milo::error& e)
    {
        std::cout << "Unexpected decoding error.\n";
        return -1;
    }
    
    return 0;
}
```

#### Decoding using oneliners

```c++
#include <iostream>
#include <string>
#include <string_view>

#include <milo/primitive/codec/apie.h>
#include <milo/primitive/codec/base.h>

int main()
{
    using namespace std::literals;
    
    /*
     * Declare codec alias.
     */
    using codec_type = milo::primitive::codec::base_64;
    
    auto message = "bWVzc2FnZQ=="sv;
    auto message_invalid = "bWVzABC$c2FnZQ=="sv;
    auto decoded = std::string();
    
    /*
     * Get the result by reference.
     * A fixed target buffer is supported.
     * Correctly resized if the target container supports resizing.
     * Returns the size of the result, but can be ignored.
     * This can be useful when the target buffer is of a fixed size.
     */
    auto decoded_size = milo::primitive::codec::decode<codec_type>(message, decoded);
    
    /*
     * Prints "message".
     */
    std::cout << decoded << "\n";
    
    /*
     * Get the result by value.
     * When retrieving the result by value, pass the resulting container type as a template parameter.
     * Only dynamic containers are supported.
     * Prints "message".
     */
    std::cout << milo::primitive::codec::decode<codec_type, std::string>(message) << "\n";
    
    try
    {
        /*
         * Get the result by value.
         * When retrieving the result by value, pass the resulting container type as a template parameter.
         * Only dynamic containers are supported.
         * Signalizing error by reference is not supported.
         */
        std::cout << milo::primitive::codec::decode<codec_type, std::string>(message_invalid) << "\n";
    }
    catch (const milo::error& e)
    {
        std::cout << "Expected decoding error.\n";
        return -1;
    }
    
    return 0;
}
```

### More

Most likely there are more overloads as needed.
All available options are listed in the header files in the examples above.
All methods to be used come with documentation in code.
