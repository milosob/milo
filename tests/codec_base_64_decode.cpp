

#include <iostream>

#include <milo/inner.h>
#include <milo/literals.h>

#include <milo/crypto/codec/base.h>
#include <milo/crypto/codec/test.h>


using namespace milo::literals;

struct test_vector
{
    const size_t id;
    
    const milo::container::chars_const_view_dynamic from;
    
    const milo::container::chars_const_view_dynamic to;
};

constexpr milo::container::array<test_vector, 40> test_vectors
    {
        test_vector
            {
                .id = 1,
                .from =
                "KQ++bJlmO2XDD5mX0xXnCAIZryqaxYxFgevacZoGlPQ="_cv,
                .to =
                "\x29\x0f\xbe\x6c\x99\x66\x3b\x65\xc3\x0f\x99\x97\xd3\x15\xe7\x08"
                "\x02\x19\xaf\x2a\x9a\xc5\x8c\x45\x81\xeb\xda\x71\x9a\x06\x94\xf4"_cv
            },
        test_vector
            {
                .id = 2,
                .from =
                "GT4Dy0vwU5gITXrf3pvEFukDODiqXKvbKRGW7VNOu3kO"_cv,
                .to =
                "\x19\x3e\x03\xcb\x4b\xf0\x53\x98\x08\x4d\x7a\xdf\xde\x9b\xc4\x16"
                "\xe9\x03\x38\x38\xaa\x5c\xab\xdb\x29\x11\x96\xed\x53\x4e\xbb\x79"
                "\x0e"_cv
            },
        test_vector
            {
                .id = 3,
                .from =
                "cPfzdCCpyAViaru1DGWiKqGkdHM5psre69JUY86yQXZfKg=="_cv,
                .to =
                "\x70\xf7\xf3\x74\x20\xa9\xc8\x05\x62\x6a\xbb\xb5\x0c\x65\xa2\x2a"
                "\xa1\xa4\x74\x73\x39\xa6\xca\xde\xeb\xd2\x54\x63\xce\xb2\x41\x76"
                "\x5f\x2a"_cv
            },
        test_vector
            {
                .id = 4,
                .from =
                "DWUqlhKjpVDEWvYnKArTKkJS5mk7vSSQarLvU+u3FoeMhwE="_cv,
                .to =
                "\x0d\x65\x2a\x96\x12\xa3\xa5\x50\xc4\x5a\xf6\x27\x28\x0a\xd3\x2a"
                "\x42\x52\xe6\x69\x3b\xbd\x24\x90\x6a\xb2\xef\x53\xeb\xb7\x16\x87"
                "\x8c\x87\x01"_cv
            },
        test_vector
            {
                .id = 5,
                .from =
                "9NYd+ylb4tl4t13oxg8NtcXVgqs1u9eiD56bxXn8GK5gJat/"_cv,
                .to =
                "\xf4\xd6\x1d\xfb\x29\x5b\xe2\xd9\x78\xb7\x5d\xe8\xc6\x0f\x0d\xb5"
                "\xc5\xd5\x82\xab\x35\xbb\xd7\xa2\x0f\x9e\x9b\xc5\x79\xfc\x18\xae"
                "\x60\x25\xab\x7f"_cv
            },
        test_vector
            {
                .id = 6,
                .from =
                "haRpJ7XCpcQkrcPotN5LsE0atgBRMKa1ThQPlxrfEVyc27s1mA=="_cv,
                .to =
                "\x85\xa4\x69\x27\xb5\xc2\xa5\xc4\x24\xad\xc3\xe8\xb4\xde\x4b\xb0"
                "\x4d\x1a\xb6\x00\x51\x30\xa6\xb5\x4e\x14\x0f\x97\x1a\xdf\x11\x5c"
                "\x9c\xdb\xbb\x35\x98"_cv
            },
        test_vector
            {
                .id = 7,
                .from =
                "UXUyXkT7dbCOjE9kR0w1xUl2+bd7vWrmcDC5rtphnbD8gKf0rs0="_cv,
                .to =
                "\x51\x75\x32\x5e\x44\xfb\x75\xb0\x8e\x8c\x4f\x64\x47\x4c\x35\xc5"
                "\x49\x76\xf9\xb7\x7b\xbd\x6a\xe6\x70\x30\xb9\xae\xda\x61\x9d\xb0"
                "\xfc\x80\xa7\xf4\xae\xcd"_cv
            },
        test_vector
            {
                .id = 8,
                .from =
                "gmM6JaaXZbNv6Ypyb2d6geRUnfKDiepb5uKVp7YiNnZbk6TO80vr"_cv,
                .to =
                "\x82\x63\x3a\x25\xa6\x97\x65\xb3\x6f\xe9\x8a\x72\x6f\x67\x7a\x81"
                "\xe4\x54\x9d\xf2\x83\x89\xea\x5b\xe6\xe2\x95\xa7\xb6\x22\x36\x76"
                "\x5b\x93\xa4\xce\xf3\x4b\xeb"_cv
            },
        test_vector
            {
                .id = 9,
                .from =
                "yz9NSEv/0UifRYKieh99fW8aXsHjopmrG1WMaf3d2TRpGDKjH5uA6w=="_cv,
                .to =
                "\xcb\x3f\x4d\x48\x4b\xff\xd1\x48\x9f\x45\x82\xa2\x7a\x1f\x7d\x7d"
                "\x6f\x1a\x5e\xc1\xe3\xa2\x99\xab\x1b\x55\x8c\x69\xfd\xdd\xd9\x34"
                "\x69\x18\x32\xa3\x1f\x9b\x80\xeb"_cv
            },
        test_vector
            {
                .id = 10,
                .from =
                "O0uoXB26P13OhmwUSRgLP1vfKJbeDtGsYfN4jGID3LfkWTBKpS0h+OE="_cv,
                .to =
                "\x3b\x4b\xa8\x5c\x1d\xba\x3f\x5d\xce\x86\x6c\x14\x49\x18\x0b\x3f"
                "\x5b\xdf\x28\x96\xde\x0e\xd1\xac\x61\xf3\x78\x8c\x62\x03\xdc\xb7"
                "\xe4\x59\x30\x4a\xa5\x2d\x21\xf8\xe1"_cv
            },
        test_vector
            {
                .id = 11,
                .from =
                "jnuFvGMQZC5gD6LSngCqHg562ZG1c5l58I8l5NfAb0sHzUKT1fMvsQ7n"_cv,
                .to =
                "\x8e\x7b\x85\xbc\x63\x10\x64\x2e\x60\x0f\xa2\xd2\x9e\x00\xaa\x1e"
                "\x0e\x7a\xd9\x91\xb5\x73\x99\x79\xf0\x8f\x25\xe4\xd7\xc0\x6f\x4b"
                "\x07\xcd\x42\x93\xd5\xf3\x2f\xb1\x0e\xe7"_cv
            },
        test_vector
            {
                .id = 12,
                .from =
                "qqO5Vhoa+SNwrv1ySwzO4p00SYPXTpnPHfUQYZHip1WskKYzbuE5wDmj5A=="_cv,
                .to =
                "\xaa\xa3\xb9\x56\x1a\x1a\xf9\x23\x70\xae\xfd\x72\x4b\x0c\xce\xe2"
                "\x9d\x34\x49\x83\xd7\x4e\x99\xcf\x1d\xf5\x10\x61\x91\xe2\xa7\x55"
                "\xac\x90\xa6\x33\x6e\xe1\x39\xc0\x39\xa3\xe4"_cv
            },
        test_vector
            {
                .id = 13,
                .from =
                "Q07Ky+3MiZDkGZAEMxGsLdLh7J5aXFRsD5Mpr9zaLYKasTGsMCUe45cdv44="_cv,
                .to =
                "\x43\x4e\xca\xcb\xed\xcc\x89\x90\xe4\x19\x90\x04\x33\x11\xac\x2d"
                "\xd2\xe1\xec\x9e\x5a\x5c\x54\x6c\x0f\x93\x29\xaf\xdc\xda\x2d\x82"
                "\x9a\xb1\x31\xac\x30\x25\x1e\xe3\x97\x1d\xbf\x8e"_cv
            },
        test_vector
            {
                .id = 14,
                .from =
                "qr4Kechms08Pdcggi3zw/KLtzM4wKPGTQpkOfKtz6bgO8X1Bc6OBonTXQAIi"_cv,
                .to =
                "\xaa\xbe\x0a\x79\xc8\x66\xb3\x4f\x0f\x75\xc8\x20\x8b\x7c\xf0\xfc"
                "\xa2\xed\xcc\xce\x30\x28\xf1\x93\x42\x99\x0e\x7c\xab\x73\xe9\xb8"
                "\x0e\xf1\x7d\x41\x73\xa3\x81\xa2\x74\xd7\x40\x02\x22"_cv
            },
        test_vector
            {
                .id = 15,
                .from =
                "b78ChsFqJkgidjOXm8UWilf+fDkZvkrgrkFp7NqgmaHKpJ+cmgJ7yzR7Y0Lpnw=="_cv,
                .to =
                "\x6f\xbf\x02\x86\xc1\x6a\x26\x48\x22\x76\x33\x97\x9b\xc5\x16\x8a"
                "\x57\xfe\x7c\x39\x19\xbe\x4a\xe0\xae\x41\x69\xec\xda\xa0\x99\xa1"
                "\xca\xa4\x9f\x9c\x9a\x02\x7b\xcb\x34\x7b\x63\x42\xe9\x9f"_cv
            },
        test_vector
            {
                .id = 16,
                .from =
                "L/57AnqgmK6C8WoFGfOiPfYML8jiHgzTt09cPBHkaH7j3nwbTF8MdqqrEXi1U50="_cv,
                .to =
                "\x2f\xfe\x7b\x02\x7a\xa0\x98\xae\x82\xf1\x6a\x05\x19\xf3\xa2\x3d"
                "\xf6\x0c\x2f\xc8\xe2\x1e\x0c\xd3\xb7\x4f\x5c\x3c\x11\xe4\x68\x7e"
                "\xe3\xde\x7c\x1b\x4c\x5f\x0c\x76\xaa\xab\x11\x78\xb5\x53\x9d"_cv
            },
        test_vector
            {
                .id = 17,
                .from =
                "7KrtPVccqld+FdShSrGRP2U3CP82t+NgBc5ntZnFv3LaDd0mSLpjyn3Im7WYo7/L"_cv,
                .to =
                "\xec\xaa\xed\x3d\x57\x1c\xaa\x57\x7e\x15\xd4\xa1\x4a\xb1\x91\x3f"
                "\x65\x37\x08\xff\x36\xb7\xe3\x60\x05\xce\x67\xb5\x99\xc5\xbf\x72"
                "\xda\x0d\xdd\x26\x48\xba\x63\xca\x7d\xc8\x9b\xb5\x98\xa3\xbf\xcb"_cv
            },
        test_vector
            {
                .id = 18,
                .from =
                "mQmOiHxZW8Jd0t5bfJQWEpcAuRy/Pxj3YSn8yN1Qgx6guty5cK+AjVol6pXfPb53"
                "7g=="_cv,
                .to =
                "\x99\x09\x8e\x88\x7c\x59\x5b\xc2\x5d\xd2\xde\x5b\x7c\x94\x16\x12"
                "\x97\x00\xb9\x1c\xbf\x3f\x18\xf7\x61\x29\xfc\xc8\xdd\x50\x83\x1e"
                "\xa0\xba\xdc\xb9\x70\xaf\x80\x8d\x5a\x25\xea\x95\xdf\x3d\xbe\x77"
                "\xee"_cv
            },
        test_vector
            {
                .id = 19,
                .from =
                "NQY1576hVnvi3ztUxOKRJigPmyMOu5L0pcLPfB/h/0v8U4c9tp54qqYWeIN/3Mdw"
                "PH4="_cv,
                .to =
                "\x35\x06\x35\xe7\xbe\xa1\x56\x7b\xe2\xdf\x3b\x54\xc4\xe2\x91\x26"
                "\x28\x0f\x9b\x23\x0e\xbb\x92\xf4\xa5\xc2\xcf\x7c\x1f\xe1\xff\x4b"
                "\xfc\x53\x87\x3d\xb6\x9e\x78\xaa\xa6\x16\x78\x83\x7f\xdc\xc7\x70"
                "\x3c\x7e"_cv
            },
        test_vector
            {
                .id = 20,
                .from =
                "E/i5efm0KZ14qWXjB3o4T+B1o5LVzR2XI0Ra5l6tHMZwmruHqdouC5vtH/Wybg75"
                "WYK5"_cv,
                .to =
                "\x13\xf8\xb9\x79\xf9\xb4\x29\x9d\x78\xa9\x65\xe3\x07\x7a\x38\x4f"
                "\xe0\x75\xa3\x92\xd5\xcd\x1d\x97\x23\x44\x5a\xe6\x5e\xad\x1c\xc6"
                "\x70\x9a\xbb\x87\xa9\xda\x2e\x0b\x9b\xed\x1f\xf5\xb2\x6e\x0e\xf9"
                "\x59\x82\xb9"_cv
            },
        test_vector
            {
                .id = 21,
                .from =
                "tHYEaBZtvlHiw5eZzYTjZWn6TwSf3Btd/mqqi6EyBnIkZI3VFRbCkkmWcdKJDUqY"
                "GhRGvA=="_cv,
                .to =
                "\xb4\x76\x04\x68\x16\x6d\xbe\x51\xe2\xc3\x97\x99\xcd\x84\xe3\x65"
                "\x69\xfa\x4f\x04\x9f\xdc\x1b\x5d\xfe\x6a\xaa\x8b\xa1\x32\x06\x72"
                "\x24\x64\x8d\xd5\x15\x16\xc2\x92\x49\x96\x71\xd2\x89\x0d\x4a\x98"
                "\x1a\x14\x46\xbc"_cv
            },
        test_vector
            {
                .id = 22,
                .from =
                "7/g3yEv3IdVYfHEH+u0t3f92lVFqTHba4WkAPya6aeMECDT6DOYlEXAJSoz/E4hs"
                "vn8+Sbk="_cv,
                .to =
                "\xef\xf8\x37\xc8\x4b\xf7\x21\xd5\x58\x7c\x71\x07\xfa\xed\x2d\xdd"
                "\xff\x76\x95\x51\x6a\x4c\x76\xda\xe1\x69\x00\x3f\x26\xba\x69\xe3"
                "\x04\x08\x34\xfa\x0c\xe6\x25\x11\x70\x09\x4a\x8c\xff\x13\x88\x6c"
                "\xbe\x7f\x3e\x49\xb9"_cv
            },
        test_vector
            {
                .id = 23,
                .from =
                "zcCIk3mM+DwAKsoDN6rZWesIMlzxO7ezTWnuRpoDEmRkU1KY37rgUSWLMTtgFsJe"
                "ZDNR83s+"_cv,
                .to =
                "\xcd\xc0\x88\x93\x79\x8c\xf8\x3c\x00\x2a\xca\x03\x37\xaa\xd9\x59"
                "\xeb\x08\x32\x5c\xf1\x3b\xb7\xb3\x4d\x69\xee\x46\x9a\x03\x12\x64"
                "\x64\x53\x52\x98\xdf\xba\xe0\x51\x25\x8b\x31\x3b\x60\x16\xc2\x5e"
                "\x64\x33\x51\xf3\x7b\x3e"_cv
            },
        test_vector
            {
                .id = 24,
                .from =
                "Z9egdeSpjhL6DEGrTHDHn2nbASLjy1boUHR2jaER5ZsUMhi1cX6mup6OCNPc6m00"
                "JOL9LDvOXg=="_cv,
                .to =
                "\x67\xd7\xa0\x75\xe4\xa9\x8e\x12\xfa\x0c\x41\xab\x4c\x70\xc7\x9f"
                "\x69\xdb\x01\x22\xe3\xcb\x56\xe8\x50\x74\x76\x8d\xa1\x11\xe5\x9b"
                "\x14\x32\x18\xb5\x71\x7e\xa6\xba\x9e\x8e\x08\xd3\xdc\xea\x6d\x34"
                "\x24\xe2\xfd\x2c\x3b\xce\x5e"_cv
            },
        test_vector
            {
                .id = 25,
                .from =
                "UzpXeW1tBCnXY9pduidRTT0980X1JkRCDUoXoXdfC6wUH4sXLopcrcMDgBb5ehef"
                "o8O97mUl5xE="_cv,
                .to =
                "\x53\x3a\x57\x79\x6d\x6d\x04\x29\xd7\x63\xda\x5d\xba\x27\x51\x4d"
                "\x3d\x3d\xf3\x45\xf5\x26\x44\x42\x0d\x4a\x17\xa1\x77\x5f\x0b\xac"
                "\x14\x1f\x8b\x17\x2e\x8a\x5c\xad\xc3\x03\x80\x16\xf9\x7a\x17\x9f"
                "\xa3\xc3\xbd\xee\x65\x25\xe7\x11"_cv
            },
        test_vector
            {
                .id = 26,
                .from =
                "f+HdnsszbWkHjDCmiccSqZtBTmLuD1jSpDP/h1/MSAjlnYtX+hEFZC981uefp1Ls"
                "riTXJCq2Rpvs"_cv,
                .to =
                "\x7f\xe1\xdd\x9e\xcb\x33\x6d\x69\x07\x8c\x30\xa6\x89\xc7\x12\xa9"
                "\x9b\x41\x4e\x62\xee\x0f\x58\xd2\xa4\x33\xff\x87\x5f\xcc\x48\x08"
                "\xe5\x9d\x8b\x57\xfa\x11\x05\x64\x2f\x7c\xd6\xe7\x9f\xa7\x52\xec"
                "\xae\x24\xd7\x24\x2a\xb6\x46\x9b\xec"_cv
            },
        test_vector
            {
                .id = 27,
                .from =
                "97bW12URxpmwuvxlRpXg/X+6Ny/12lA/G0slAymDYYdCBmDhcP0uxpl2dKAjeETt"
                "49OmZaILpJdI1w=="_cv,
                .to =
                "\xf7\xb6\xd6\xd7\x65\x11\xc6\x99\xb0\xba\xfc\x65\x46\x95\xe0\xfd"
                "\x7f\xba\x37\x2f\xf5\xda\x50\x3f\x1b\x4b\x25\x03\x29\x83\x61\x87"
                "\x42\x06\x60\xe1\x70\xfd\x2e\xc6\x99\x76\x74\xa0\x23\x78\x44\xed"
                "\xe3\xd3\xa6\x65\xa2\x0b\xa4\x97\x48\xd7"_cv
            },
        test_vector
            {
                .id = 28,
                .from =
                "qvgp3tXEEv8PgB19aZNZGiTp7reHZfkGsTX9YhZZo963fvMRDZxTsSkxb8YiLKKm"
                "ZYNCjPkzGvIfljg="_cv,
                .to =
                "\xaa\xf8\x29\xde\xd5\xc4\x12\xff\x0f\x80\x1d\x7d\x69\x93\x59\x1a"
                "\x24\xe9\xee\xb7\x87\x65\xf9\x06\xb1\x35\xfd\x62\x16\x59\xa3\xde"
                "\xb7\x7e\xf3\x11\x0d\x9c\x53\xb1\x29\x31\x6f\xc6\x22\x2c\xa2\xa6"
                "\x65\x83\x42\x8c\xf9\x33\x1a\xf2\x1f\x96\x38"_cv
            },
        test_vector
            {
                .id = 29,
                .from =
                "pOd5H4I/9YWwgUnuXIzzOzbItn7pc4YO0hVGd4KKJxPLclYfscHRPSV1vTZ1/ReN"
                "LODC9dWAiS6a+Oof"_cv,
                .to =
                "\xa4\xe7\x79\x1f\x82\x3f\xf5\x85\xb0\x81\x49\xee\x5c\x8c\xf3\x3b"
                "\x36\xc8\xb6\x7e\xe9\x73\x86\x0e\xd2\x15\x46\x77\x82\x8a\x27\x13"
                "\xcb\x72\x56\x1f\xb1\xc1\xd1\x3d\x25\x75\xbd\x36\x75\xfd\x17\x8d"
                "\x2c\xe0\xc2\xf5\xd5\x80\x89\x2e\x9a\xf8\xea\x1f"_cv
            },
        test_vector
            {
                .id = 30,
                .from =
                "pbsmHt5CQrF87cxMrCBwohFp63bhHHdscDMXEBu9QwTi/dVbmhhN7ZC74qB+ae1y"
                "HlHRJQnVz0Z1ALzihw=="_cv,
                .to =
                "\xa5\xbb\x26\x1e\xde\x42\x42\xb1\x7c\xed\xcc\x4c\xac\x20\x70\xa2"
                "\x11\x69\xeb\x76\xe1\x1c\x77\x6c\x70\x33\x17\x10\x1b\xbd\x43\x04"
                "\xe2\xfd\xd5\x5b\x9a\x18\x4d\xed\x90\xbb\xe2\xa0\x7e\x69\xed\x72"
                "\x1e\x51\xd1\x25\x09\xd5\xcf\x46\x75\x00\xbc\xe2\x87"_cv
            },
        test_vector
            {
                .id = 31,
                .from =
                "OKlnHc+5DS/IhU0Ls/xYQX2Mnzkx2TPmFgUilTFGFIQ/US0br7yt7dEf8LRdCD3B"
                "ptMCMGs3pCAYke12NE4="_cv,
                .to =
                "\x38\xa9\x67\x1d\xcf\xb9\x0d\x2f\xc8\x85\x4d\x0b\xb3\xfc\x58\x41"
                "\x7d\x8c\x9f\x39\x31\xd9\x33\xe6\x16\x05\x22\x95\x31\x46\x14\x84"
                "\x3f\x51\x2d\x1b\xaf\xbc\xad\xed\xd1\x1f\xf0\xb4\x5d\x08\x3d\xc1"
                "\xa6\xd3\x02\x30\x6b\x37\xa4\x20\x18\x91\xed\x76\x34\x4e"_cv
            },
        test_vector
            {
                .id = 32,
                .from =
                "dyRFzQZadQnhavtTCSJhKn1cUvjdwCHr9d4e7EceP9xrz5IoXdfVdjl93hN+ihcO"
                "V85C5QeiDHEOIBM4yHL1"_cv,
                .to =
                "\x77\x24\x45\xcd\x06\x5a\x75\x09\xe1\x6a\xfb\x53\x09\x22\x61\x2a"
                "\x7d\x5c\x52\xf8\xdd\xc0\x21\xeb\xf5\xde\x1e\xec\x47\x1e\x3f\xdc"
                "\x6b\xcf\x92\x28\x5d\xd7\xd5\x76\x39\x7d\xde\x13\x7e\x8a\x17\x0e"
                "\x57\xce\x42\xe5\x07\xa2\x0c\x71\x0e\x20\x13\x38\xc8\x72\xf5"_cv
            },
        test_vector
            {
                .id = 33,
                .from =
                "zQPjJ7BmjRWKwm2sqFAEpdqG95t0vpbxvOiE4CrTtNEtoni81o4si9e26fvqvCJz"
                "snf+JD1Pcu0PLPSAGiGRpw=="_cv,
                .to =
                "\xcd\x03\xe3\x27\xb0\x66\x8d\x15\x8a\xc2\x6d\xac\xa8\x50\x04\xa5"
                "\xda\x86\xf7\x9b\x74\xbe\x96\xf1\xbc\xe8\x84\xe0\x2a\xd3\xb4\xd1"
                "\x2d\xa2\x78\xbc\xd6\x8e\x2c\x8b\xd7\xb6\xe9\xfb\xea\xbc\x22\x73"
                "\xb2\x77\xfe\x24\x3d\x4f\x72\xed\x0f\x2c\xf4\x80\x1a\x21\x91\xa7"_cv
            },
        test_vector
            {
                .id = 34,
                .from =
                "WxbWym04AWtKLkQQI+LMux9rfCfGHmdx/PqzQd5bXLyGscUQnTKyK9wHMC9S4aJG"
                "Pm+AeG86tnK3na9sK7LF0IY="_cv,
                .to =
                "\x5b\x16\xd6\xca\x6d\x38\x01\x6b\x4a\x2e\x44\x10\x23\xe2\xcc\xbb"
                "\x1f\x6b\x7c\x27\xc6\x1e\x67\x71\xfc\xfa\xb3\x41\xde\x5b\x5c\xbc"
                "\x86\xb1\xc5\x10\x9d\x32\xb2\x2b\xdc\x07\x30\x2f\x52\xe1\xa2\x46"
                "\x3e\x6f\x80\x78\x6f\x3a\xb6\x72\xb7\x9d\xaf\x6c\x2b\xb2\xc5\xd0"
                "\x86"_cv
            },
        test_vector
            {
                .id = 35,
                .from =
                "sG7zAYfep/Tlr+iIO35jrv9tbGYhedyLwmMYWE4qpLeCaM12Y9Dk2h18y16FVtgU"
                "etK2CBXkFazHzd+HUN1YoGFu"_cv,
                .to =
                "\xb0\x6e\xf3\x01\x87\xde\xa7\xf4\xe5\xaf\xe8\x88\x3b\x7e\x63\xae"
                "\xff\x6d\x6c\x66\x21\x79\xdc\x8b\xc2\x63\x18\x58\x4e\x2a\xa4\xb7"
                "\x82\x68\xcd\x76\x63\xd0\xe4\xda\x1d\x7c\xcb\x5e\x85\x56\xd8\x14"
                "\x7a\xd2\xb6\x08\x15\xe4\x15\xac\xc7\xcd\xdf\x87\x50\xdd\x58\xa0"
                "\x61\x6e"_cv
            },
        test_vector
            {
                .id = 36,
                .from =
                "X5y7dLQcjVVH+Ko3CoF/fEni8IhWem63RRzHShYHt6GgeYi7YSbhb4Q5Ob87AG9J"
                "NUZL27mj+GhR/I1H75IY5Wj4lA=="_cv,
                .to =
                "\x5f\x9c\xbb\x74\xb4\x1c\x8d\x55\x47\xf8\xaa\x37\x0a\x81\x7f\x7c"
                "\x49\xe2\xf0\x88\x56\x7a\x6e\xb7\x45\x1c\xc7\x4a\x16\x07\xb7\xa1"
                "\xa0\x79\x88\xbb\x61\x26\xe1\x6f\x84\x39\x39\xbf\x3b\x00\x6f\x49"
                "\x35\x46\x4b\xdb\xb9\xa3\xf8\x68\x51\xfc\x8d\x47\xef\x92\x18\xe5"
                "\x68\xf8\x94"_cv
            },
        test_vector
            {
                .id = 37,
                .from =
                "b6EZ79/kIidEp8BQOOvg3/5d+IdwEh7KOC+/YbPGZN88Fp5OHW/kGixLZLaAtwqP"
                "qj9P+QcApFBy+duCnJBElufvjG4="_cv,
                .to =
                "\x6f\xa1\x19\xef\xdf\xe4\x22\x27\x44\xa7\xc0\x50\x38\xeb\xe0\xdf"
                "\xfe\x5d\xf8\x87\x70\x12\x1e\xca\x38\x2f\xbf\x61\xb3\xc6\x64\xdf"
                "\x3c\x16\x9e\x4e\x1d\x6f\xe4\x1a\x2c\x4b\x64\xb6\x80\xb7\x0a\x8f"
                "\xaa\x3f\x4f\xf9\x07\x00\xa4\x50\x72\xf9\xdb\x82\x9c\x90\x44\x96"
                "\xe7\xef\x8c\x6e"_cv
            },
        test_vector
            {
                .id = 38,
                .from =
                "De5/8H4MBN5BaS02tXpc+etHqUT9T6U0uf6MlatDe19ynp4u9iWMN5TrppAGXN6b"
                "pjRmHk1g4lR73wDDoWubfIVE3xCz"_cv,
                .to =
                "\x0d\xee\x7f\xf0\x7e\x0c\x04\xde\x41\x69\x2d\x36\xb5\x7a\x5c\xf9"
                "\xeb\x47\xa9\x44\xfd\x4f\xa5\x34\xb9\xfe\x8c\x95\xab\x43\x7b\x5f"
                "\x72\x9e\x9e\x2e\xf6\x25\x8c\x37\x94\xeb\xa6\x90\x06\x5c\xde\x9b"
                "\xa6\x34\x66\x1e\x4d\x60\xe2\x54\x7b\xdf\x00\xc3\xa1\x6b\x9b\x7c"
                "\x85\x44\xdf\x10\xb3"_cv
            },
        test_vector
            {
                .id = 39,
                .from =
                "6o4mdOJb9XaAZHNAMljSp36GGcTbS1bCwb7c3Ra54jUPHU0Louuk/5yYX64mI/ao"
                "V/aflmHuVlWs/9TX3Y/pGP7QzQC7EA=="_cv,
                .to =
                "\xea\x8e\x26\x74\xe2\x5b\xf5\x76\x80\x64\x73\x40\x32\x58\xd2\xa7"
                "\x7e\x86\x19\xc4\xdb\x4b\x56\xc2\xc1\xbe\xdc\xdd\x16\xb9\xe2\x35"
                "\x0f\x1d\x4d\x0b\xa2\xeb\xa4\xff\x9c\x98\x5f\xae\x26\x23\xf6\xa8"
                "\x57\xf6\x9f\x96\x61\xee\x56\x55\xac\xff\xd4\xd7\xdd\x8f\xe9\x18"
                "\xfe\xd0\xcd\x00\xbb\x10"_cv
            },
        test_vector
            {
                .id = 40,
                .from =
                "afWmB0O7PtDNZB5wsedUvtoBQwtADyBMcjWn0l4QTkFrq71Kt7DGGtRqmqALuIjO"
                "4RIMb+UUtpGfC6sCz85KLwTI51gd5P8="_cv,
                .to =
                "\x69\xf5\xa6\x07\x43\xbb\x3e\xd0\xcd\x64\x1e\x70\xb1\xe7\x54\xbe"
                "\xda\x01\x43\x0b\x40\x0f\x20\x4c\x72\x35\xa7\xd2\x5e\x10\x4e\x41"
                "\x6b\xab\xbd\x4a\xb7\xb0\xc6\x1a\xd4\x6a\x9a\xa0\x0b\xb8\x88\xce"
                "\xe1\x12\x0c\x6f\xe5\x14\xb6\x91\x9f\x0b\xab\x02\xcf\xce\x4a\x2f"
                "\x04\xc8\xe7\x58\x1d\xe4\xff"_cv
            },
    };

constexpr auto
test(
) noexcept(true) -> size_t
{
    size_t test_vectors_size = test_vectors.size();
    size_t test_vectors_failed = 0;
    
    for (size_t i = 0; i < test_vectors_size; i += 1)
    {
        auto result = milo::crypto::codec::test<milo::crypto::codec::base_64>::decode(
            test_vectors[i].from,
            test_vectors[i].to
        );
        
        if (result)
        {
            continue;
        }
        
        if MILO_CONSTEVAL
        {
            return test_vectors[i].id;
        }
        else
        {
            if (test_vectors_failed == 0)
            {
                std::cerr << "Tests that failed:\n";
            }
            
            std::cerr << "  - " << test_vectors[i].id << "\n";
        }
        
        test_vectors_failed += 1;
    }
    
    return test_vectors_failed;
}

auto
main(
) noexcept(false) -> int
{
    #ifdef MILO_TESTS_CONSTEXPR
    static_assert(test() == 0);
    #endif
    
    volatile auto test_cb = test;
    
    return test_cb() > 0;
}

