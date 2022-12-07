

#include <iostream>

#include <milo/inner.h>
#include <milo/literal.h>

#include <milo/crypto/codec/base.h>
#include <milo/crypto/codec/test.h>


using namespace milo::literal;

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
                "\x86\x61\x2f\xa5\x5d\x65\x33\x8b\xd7\xc0\xd6\x79\x12\x02\x78\x4b"
                "\x98\x89\x50\xe6\x4b\x8a\x58\x36\x38\x79\xb1\xbd\xd6\x7c\x9e\xa7"_cv,
                .to =
                "hmEvpV1lM4vXwNZ5EgJ4S5iJUOZLilg2OHmxvdZ8nqc="_cv
            },
        test_vector
            {
                .id = 2,
                .from =
                "\x86\x83\x0b\xc7\x33\x32\xb7\xce\x18\x8a\x07\x64\xb4\x1c\xdc\xcf"
                "\x3b\xb7\xc3\x0b\xf5\x48\xf9\x52\x2f\xc2\x0e\xdb\xab\x6d\xef\x98"
                "\x95"_cv,
                .to =
                "hoMLxzMyt84YigdktBzczzu3wwv1SPlSL8IO26tt75iV"_cv
            },
        test_vector
            {
                .id = 3,
                .from =
                "\x56\x8d\x15\x4c\x10\xf3\x62\x00\xdc\x72\xbf\x80\xd4\xc3\x6e\x5a"
                "\x22\x51\xc1\xba\x89\x5a\xcd\x08\xfc\xb2\x68\x2f\x61\x68\xdb\xa8"
                "\x0c\x22"_cv,
                .to =
                "Vo0VTBDzYgDccr+A1MNuWiJRwbqJWs0I/LJoL2Fo26gMIg=="_cv
            },
        test_vector
            {
                .id = 4,
                .from =
                "\x3f\xf5\x29\xa3\x15\x27\x89\xe4\xcc\xcd\xc0\xaf\x3d\xd7\xfe\x48"
                "\x95\xb0\xbd\xce\x39\x29\xa8\xc5\x48\xf3\x6c\xb7\x28\xe9\xac\x0b"
                "\x48\x44\xdb"_cv,
                .to =
                "P/UpoxUnieTMzcCvPdf+SJWwvc45KajFSPNstyjprAtIRNs="_cv
            },
        test_vector
            {
                .id = 5,
                .from =
                "\xd4\x6f\xe9\x74\x90\xf4\xab\xb7\xec\x00\xbe\xea\xa1\xb5\x77\x6b"
                "\x0d\xef\x58\xcb\x93\xb5\x5c\xe5\x2e\x17\x0d\xc1\x64\x7f\xf6\x6d"
                "\x7f\x2d\xf8\xe1"_cv,
                .to =
                "1G/pdJD0q7fsAL7qobV3aw3vWMuTtVzlLhcNwWR/9m1/Lfjh"_cv
            },
        test_vector
            {
                .id = 6,
                .from =
                "\xc5\xa3\xe9\x13\xb5\xda\x5c\x58\xaf\x70\x2c\x96\xce\xf2\x70\x99"
                "\x22\xd6\x1c\x21\x18\x73\xcc\x05\x85\xa0\xe0\x32\x7d\x96\x3e\xbf"
                "\xb2\x29\xab\x33\x71"_cv,
                .to =
                "xaPpE7XaXFivcCyWzvJwmSLWHCEYc8wFhaDgMn2WPr+yKaszcQ=="_cv
            },
        test_vector
            {
                .id = 7,
                .from =
                "\x5f\x00\xfd\x18\x24\xaa\x16\x5d\x21\x80\x15\x95\x31\x67\xa4\xa0"
                "\xbf\xa6\x37\x70\xdf\xde\x7b\x6b\x8f\x39\x7a\xd7\xb0\x8c\x5d\xcc"
                "\xce\x01\x90\x0e\x39\x56"_cv,
                .to =
                "XwD9GCSqFl0hgBWVMWekoL+mN3Df3ntrjzl617CMXczOAZAOOVY="_cv
            },
        test_vector
            {
                .id = 8,
                .from =
                "\x41\xe2\x33\x50\x85\x82\x62\x93\xbb\x62\xc9\xc7\x69\x29\x1d\x21"
                "\x61\x5c\x4c\xf3\x3d\x7b\xcd\x96\xc0\x1b\x31\xf1\xd3\x8a\x63\x45"
                "\xe3\x25\x2e\x8f\x44\xb9\xdd"_cv,
                .to =
                "QeIzUIWCYpO7YsnHaSkdIWFcTPM9e82WwBsx8dOKY0XjJS6PRLnd"_cv
            },
        test_vector
            {
                .id = 9,
                .from =
                "\x01\x15\xa4\x67\x29\x0d\x18\xf0\x98\xe4\xf8\xd2\x95\xbd\xa4\x55"
                "\x6d\xd3\x84\x93\xae\x40\xba\x4c\xc4\x48\xbe\x0e\x2c\x3e\xf9\x63"
                "\xfb\xe8\x4d\xa2\xa5\x67\x23\x45"_cv,
                .to =
                "ARWkZykNGPCY5PjSlb2kVW3ThJOuQLpMxEi+Diw++WP76E2ipWcjRQ=="_cv
            },
        test_vector
            {
                .id = 10,
                .from =
                "\x53\x8e\xe0\x1c\x92\xd2\xb7\x02\x22\xa3\xcf\xfa\x67\x66\x4b\xa2"
                "\xf1\x4b\x4f\xbc\x8f\xcb\x81\x22\xcb\xee\x04\x86\xe4\xf0\xe2\x7e"
                "\xc2\x9f\xb4\x35\x36\x6e\x87\xdf\x88"_cv,
                .to =
                "U47gHJLStwIio8/6Z2ZLovFLT7yPy4Eiy+4EhuTw4n7Cn7Q1Nm6H34g="_cv
            },
        test_vector
            {
                .id = 11,
                .from =
                "\x00\xab\xf1\xc8\x9b\x8d\x4b\x34\x13\xf8\x14\x88\x61\xf6\x06\xd9"
                "\xf1\x74\xa6\xc9\x7b\xa2\x16\xa6\x56\x1b\x2a\x78\x3a\xa5\x99\xe9"
                "\xae\x90\x32\xd7\x40\x39\xf3\x65\x19\x75"_cv,
                .to =
                "AKvxyJuNSzQT+BSIYfYG2fF0psl7ohamVhsqeDqlmemukDLXQDnzZRl1"_cv
            },
        test_vector
            {
                .id = 12,
                .from =
                "\x4b\x82\x9c\xde\xc0\xe6\xd2\x60\xb3\x37\x18\xde\xd7\x8d\x13\xb4"
                "\x29\x33\x5a\x26\x53\xf7\x12\x87\x86\x28\xb1\x16\xc9\x74\x29\xea"
                "\x9f\x30\x3e\x4e\x08\xf4\x31\x4e\x52\x72\xff"_cv,
                .to =
                "S4Kc3sDm0mCzNxje140TtCkzWiZT9xKHhiixFsl0KeqfMD5OCPQxTlJy/w=="_cv
            },
        test_vector
            {
                .id = 13,
                .from =
                "\xa3\x5a\xd3\x19\x5e\x8a\xc5\xf1\xca\xdb\x29\x2b\x3f\x69\x5a\xa6"
                "\x84\x37\xb0\x9d\x04\xd4\x13\xc0\xcd\x7c\xa3\xeb\x24\x4d\x74\x9c"
                "\x39\x32\x17\x61\x4a\x89\x9c\xf5\x80\x4e\x09\xd0"_cv,
                .to =
                "o1rTGV6KxfHK2ykrP2lapoQ3sJ0E1BPAzXyj6yRNdJw5MhdhSomc9YBOCdA="_cv
            },
        test_vector
            {
                .id = 14,
                .from =
                "\x49\x0a\x07\xb0\x98\x3d\x7d\x00\x91\x44\x91\x4b\x6e\x25\xe4\xc2"
                "\x9a\x49\xc2\xd2\x64\x7a\x37\x48\x5d\x75\xa3\x53\x37\x5b\x77\x52"
                "\x5e\x5b\x9f\xc0\x38\x5b\x28\x63\x5a\xb5\x28\xb6\x69"_cv,
                .to =
                "SQoHsJg9fQCRRJFLbiXkwppJwtJkejdIXXWjUzdbd1JeW5/AOFsoY1q1KLZp"_cv
            },
        test_vector
            {
                .id = 15,
                .from =
                "\x8c\x9b\x29\xb9\xef\xcd\x79\x89\xc6\x87\x74\xc0\x8c\x33\x15\x48"
                "\x2b\x98\x1e\xc1\x9b\x70\x52\x9c\x00\xe1\x44\xb5\x43\xd4\x83\xe1"
                "\x8f\x2b\xd7\x8f\xd4\xc0\x1f\xf6\x9d\x9d\x9f\xcc\x0c\xff"_cv,
                .to =
                "jJspue/NeYnGh3TAjDMVSCuYHsGbcFKcAOFEtUPUg+GPK9eP1MAf9p2dn8wM/w=="_cv
            },
        test_vector
            {
                .id = 16,
                .from =
                "\xda\x73\x6c\xc8\x38\x62\x5a\x4a\xec\x4c\xbd\x4f\x4c\x32\xed\x97"
                "\x16\x4f\x48\x7c\x51\xb5\x19\xf8\x2f\x5c\x01\x8a\x0f\xe1\xce\xd6"
                "\xa0\x70\xcd\x2c\x19\x16\xe5\xab\x26\x68\xb1\x17\x53\x75\xda"_cv,
                .to =
                "2nNsyDhiWkrsTL1PTDLtlxZPSHxRtRn4L1wBig/hztagcM0sGRblqyZosRdTddo="_cv
            },
        test_vector
            {
                .id = 17,
                .from =
                "\x83\xc7\x89\x91\x5f\x18\x93\xf1\x11\x4a\xe5\xd4\xd8\xf0\x05\xbe"
                "\x02\x83\xd2\x4a\xdb\xbf\x66\xc3\xf3\x04\x53\x7e\x14\x31\xc4\x13"
                "\xde\x71\xb6\x5a\xea\x8e\x3b\xfc\x98\x22\xe9\x7a\x18\xb0\x0a\x07"_cv,
                .to =
                "g8eJkV8Yk/ERSuXU2PAFvgKD0krbv2bD8wRTfhQxxBPecbZa6o47/Jgi6XoYsAoH"_cv
            },
        test_vector
            {
                .id = 18,
                .from =
                "\x2c\x03\xc1\x68\xbd\xf3\x44\x31\xef\x98\x8b\xcb\x02\xb9\xdb\x23"
                "\x7f\xf3\x96\x8e\xf9\xe0\x19\xe1\xbf\x5d\x52\xbc\xa4\x38\x97\xcf"
                "\x4a\xbf\x79\x09\xdb\x45\x08\x55\x44\x74\x80\x67\x78\x1f\xce\x6b"
                "\x80"_cv,
                .to =
                "LAPBaL3zRDHvmIvLArnbI3/zlo754Bnhv11SvKQ4l89Kv3kJ20UIVUR0gGd4H85r"
                "gA=="_cv
            },
        test_vector
            {
                .id = 19,
                .from =
                "\xa8\x4c\x2d\x1a\xa4\x23\xd5\xa9\x62\x1e\xd3\xa4\x99\x2a\x1b\x0c"
                "\x6a\x57\x62\xdc\x33\x26\xbe\x2c\xf3\x57\x0c\x01\x56\x53\xd3\x72"
                "\x92\x83\x7f\xc8\x2d\xb7\xf3\xc1\x0e\x26\xb3\x88\x9e\x32\x5f\xe0"
                "\x38\xc0"_cv,
                .to =
                "qEwtGqQj1aliHtOkmSobDGpXYtwzJr4s81cMAVZT03KSg3/ILbfzwQ4ms4ieMl/g"
                "OMA="_cv
            },
        test_vector
            {
                .id = 20,
                .from =
                "\xda\xe1\x5e\xbb\x5e\xd5\x55\x9f\x44\xd9\xc7\xb2\xda\x1e\xf6\x93"
                "\x73\x43\xe5\xb8\x46\xd7\x80\x9e\x07\x67\x04\xb6\x45\x2e\x32\x03"
                "\x91\x0d\x3e\x77\xbd\xf5\xd0\xa5\x4c\xe6\x1b\x1b\x1d\x43\x9f\x20"
                "\x48\xb9\xc7"_cv,
                .to =
                "2uFeu17VVZ9E2cey2h72k3ND5bhG14CeB2cEtkUuMgORDT53vfXQpUzmGxsdQ58g"
                "SLnH"_cv
            },
        test_vector
            {
                .id = 21,
                .from =
                "\xa5\x9a\xac\xbc\x22\xdc\x81\xed\x23\xb9\x20\x69\x5f\x20\x66\x3e"
                "\xdf\x28\xa9\xe5\xf7\x51\x02\x9c\x12\xd5\xd0\x50\xd5\x38\xe1\x3d"
                "\x94\xa9\xf1\xf9\x63\xee\x03\x47\x21\xc9\x64\xa9\xf1\x8a\x42\x4a"
                "\x31\x2e\x70\xe1"_cv,
                .to =
                "pZqsvCLcge0juSBpXyBmPt8oqeX3UQKcEtXQUNU44T2UqfH5Y+4DRyHJZKnxikJK"
                "MS5w4Q=="_cv
            },
        test_vector
            {
                .id = 22,
                .from =
                "\x27\xee\x03\x14\xaa\xfe\xd2\x72\x96\xfd\x2a\xd1\xaa\xa3\x2e\x8b"
                "\x78\xa4\x1f\x01\x87\xaa\x87\x91\x87\xa8\x22\xe3\xec\x6b\x6a\xc6"
                "\x6a\x36\x75\x1b\x88\x9e\x72\xcc\x5d\xc8\x1c\xf7\x5c\x87\x3c\x86"
                "\x0a\x88\x28\xcc\x03"_cv,
                .to =
                "J+4DFKr+0nKW/SrRqqMui3ikHwGHqoeRh6gi4+xrasZqNnUbiJ5yzF3IHPdchzyG"
                "CogozAM="_cv
            },
        test_vector
            {
                .id = 23,
                .from =
                "\xf0\x93\xc2\x33\x4f\xdf\xea\x02\x25\xed\x97\x30\x01\xbc\x74\xcd"
                "\xf6\x7a\x64\x67\xb8\x37\x09\xe9\xad\xb3\x03\xf7\x54\x30\xf7\x86"
                "\x4d\xe2\x7b\x2a\x78\x94\xe2\x22\xfa\x7b\x78\xc3\x85\x55\xec\x3c"
                "\x18\xb1\x9e\x97\x20\x82"_cv,
                .to =
                "8JPCM0/f6gIl7ZcwAbx0zfZ6ZGe4NwnprbMD91Qw94ZN4nsqeJTiIvp7eMOFVew8"
                "GLGelyCC"_cv
            },
        test_vector
            {
                .id = 24,
                .from =
                "\x60\x62\xde\x87\x35\x41\x91\xe6\xe9\x14\xc9\xac\x99\xfe\xfb\x90"
                "\xc5\xf6\xf2\xad\x69\xf9\x05\x75\x4b\x2e\x0a\x22\x3d\x46\x02\xd2"
                "\x2e\x68\xa6\x61\x8a\x75\xce\x2a\x70\xcc\xe7\x9b\xab\x62\x3b\x52"
                "\xd1\x54\xb3\xf8\x6c\x77\xd6"_cv,
                .to =
                "YGLehzVBkebpFMmsmf77kMX28q1p+QV1Sy4KIj1GAtIuaKZhinXOKnDM55urYjtS"
                "0VSz+Gx31g=="_cv
            },
        test_vector
            {
                .id = 25,
                .from =
                "\x04\xb7\x94\xa1\x31\x7d\xaa\x22\x7f\x4e\xc6\x02\x35\x21\x32\x7c"
                "\xf4\x6d\xcf\x0b\x69\x1e\x49\x48\xa6\x00\x32\xa7\xab\xe9\xe4\xc2"
                "\xd5\x4a\xff\x2e\x57\x9b\x44\x65\xf6\x68\xa2\x77\xae\x83\xea\xa9"
                "\x78\xb8\x5d\x97\x71\xe3\xd2\x92"_cv,
                .to =
                "BLeUoTF9qiJ/TsYCNSEyfPRtzwtpHklIpgAyp6vp5MLVSv8uV5tEZfZooneug+qp"
                "eLhdl3Hj0pI="_cv
            },
        test_vector
            {
                .id = 26,
                .from =
                "\x08\xe4\xf0\xb2\xd7\xc8\xf1\x14\x04\x06\x11\xb2\xe8\x2e\xcb\x09"
                "\x49\x0c\x5f\xc1\x5f\x3e\x23\xeb\x1a\x9d\x2c\xc0\x48\xab\x06\xcd"
                "\xf8\xf5\x18\x26\x1c\x05\x13\xc2\xe8\xb5\xdf\xb8\x43\x35\x84\x60"
                "\xb1\xbb\x0c\x44\xa5\x1e\xbe\x54\x94"_cv,
                .to =
                "COTwstfI8RQEBhGy6C7LCUkMX8FfPiPrGp0swEirBs349RgmHAUTwui137hDNYRg"
                "sbsMRKUevlSU"_cv
            },
        test_vector
            {
                .id = 27,
                .from =
                "\x95\xdb\xb5\x0f\x01\x3e\xb8\xd3\x40\xd6\xef\xce\x6b\xfd\x28\x63"
                "\x5f\xa3\x0c\x9f\x67\xa7\xb5\x4d\x6b\x83\xc3\xde\xe8\xde\xe2\xb1"
                "\x59\x37\x7a\x5c\xd2\x60\x17\xa0\x00\xe4\xa8\xfb\x5b\xfc\xd4\x1a"
                "\xc6\x1b\x2b\xb4\x4c\x3f\xde\x6c\xa1\x43"_cv,
                .to =
                "ldu1DwE+uNNA1u/Oa/0oY1+jDJ9np7VNa4PD3uje4rFZN3pc0mAXoADkqPtb/NQa"
                "xhsrtEw/3myhQw=="_cv
            },
        test_vector
            {
                .id = 28,
                .from =
                "\xf2\xdd\x6a\x16\x60\xad\xc3\x96\xcd\x45\xcc\x10\x81\x0f\xc7\x9c"
                "\xd1\xbb\xa1\x43\xd3\x58\x69\xbe\x12\xaa\xb4\xc7\x1e\xf9\xa8\x06"
                "\x60\x5d\x05\xdd\x4d\xa0\x3a\x3f\x60\x87\x03\x0a\x6c\xc3\xfa\x02"
                "\x2e\x46\xc0\x7f\x91\xa8\x57\x27\x6b\x13\x91"_cv,
                .to =
                "8t1qFmCtw5bNRcwQgQ/HnNG7oUPTWGm+Eqq0xx75qAZgXQXdTaA6P2CHAwpsw/oC"
                "LkbAf5GoVydrE5E="_cv
            },
        test_vector
            {
                .id = 29,
                .from =
                "\x34\x6a\x8e\x00\xa0\x19\xc7\x70\x95\x7a\xc4\x54\xb1\x14\x88\xb4"
                "\xfc\x13\xb5\x6d\x1a\xf9\x54\x5c\x57\x60\xac\xbd\xa8\x65\xb6\xb2"
                "\xf1\xb3\x55\x05\x82\x4f\x58\xbf\x1e\x5d\x19\x1c\xee\x1d\xc2\xb2"
                "\xa4\x93\xbd\x2c\xbf\x91\x41\x03\xaa\x7f\xb7\x84"_cv,
                .to =
                "NGqOAKAZx3CVesRUsRSItPwTtW0a+VRcV2CsvahltrLxs1UFgk9Yvx5dGRzuHcKy"
                "pJO9LL+RQQOqf7eE"_cv
            },
        test_vector
            {
                .id = 30,
                .from =
                "\x1f\x9e\x7f\xfe\x61\x02\xb5\x18\x3d\x85\x55\xf4\xbb\x30\x64\x93"
                "\x36\xae\xe5\xd7\xa8\x6a\x43\x96\x2c\xec\x55\xc9\xd8\xec\xda\xef"
                "\xc1\x81\x1b\x5a\xe4\x77\x40\x3d\x48\x76\x82\xe5\x07\x15\xad\x74"
                "\xdd\xc7\xa5\xab\x31\x9c\x0f\x4b\xef\xd8\xfe\x30\x21"_cv,
                .to =
                "H55//mECtRg9hVX0uzBkkzau5deoakOWLOxVydjs2u/BgRta5HdAPUh2guUHFa10"
                "3celqzGcD0vv2P4wIQ=="_cv
            },
        test_vector
            {
                .id = 31,
                .from =
                "\x9e\x24\xb4\x3c\xe1\xff\x3f\x41\xc8\xfd\xef\x10\xde\xdf\xba\xc1"
                "\x84\x6a\x3f\xc2\x8f\xe1\x41\x41\x45\x7f\x07\xfd\x9c\x73\x19\x38"
                "\x7f\xbb\x6e\x3c\x44\xa8\x81\xcf\x58\x77\x79\x15\x06\xcc\xcc\x52"
                "\xac\xc5\x92\x5b\xbc\x92\x7a\x66\x1e\x0a\x49\xea\xa1\x08"_cv,
                .to =
                "niS0POH/P0HI/e8Q3t+6wYRqP8KP4UFBRX8H/ZxzGTh/u248RKiBz1h3eRUGzMxS"
                "rMWSW7ySemYeCknqoQg="_cv
            },
        test_vector
            {
                .id = 32,
                .from =
                "\xe8\xca\x46\x46\x84\xaf\x93\xd8\xfc\xd9\x90\x7a\x71\x27\x07\x3c"
                "\x94\x62\xe1\xb0\xf6\xb1\x73\x0d\xf8\x67\xca\x5f\x50\x2b\x78\x7a"
                "\xac\x4d\x84\x05\xc4\x3e\xf6\x01\x31\x78\x0a\x5f\xb5\x5d\xd5\x78"
                "\x73\x90\xa6\x49\x83\xa8\x34\x49\x8c\xbc\xd1\xc4\x49\x91\x94"_cv,
                .to =
                "6MpGRoSvk9j82ZB6cScHPJRi4bD2sXMN+GfKX1AreHqsTYQFxD72ATF4Cl+1XdV4"
                "c5CmSYOoNEmMvNHESZGU"_cv
            },
        test_vector
            {
                .id = 33,
                .from =
                "\x81\x02\x5e\x67\x3c\x31\xfa\xd5\xa6\x49\xe1\xf1\xb6\xae\xa8\x3e"
                "\x27\x55\xf6\xbc\x2f\xaa\x2e\x8a\x42\x36\x5b\x59\xfe\x90\x84\x0d"
                "\x85\xb7\xb8\xaf\x14\x2e\x8e\xe1\xd0\x1e\x66\x4c\xba\x13\x72\x84"
                "\xea\x21\x72\xbe\xf7\x8a\x0d\x7f\x8d\xdd\xc6\x8b\x2d\x52\x83\x14"_cv,
                .to =
                "gQJeZzwx+tWmSeHxtq6oPidV9rwvqi6KQjZbWf6QhA2Ft7ivFC6O4dAeZky6E3KE"
                "6iFyvveKDX+N3caLLVKDFA=="_cv
            },
        test_vector
            {
                .id = 34,
                .from =
                "\x7e\x46\x78\xa1\xca\xa9\x1d\x3e\x10\x55\xe1\xa3\xdd\x4c\xd8\xdd"
                "\xe9\xb2\x7d\x71\x53\x8c\x87\xc2\x8b\x72\xbc\x1f\xb8\xda\xfd\xb2"
                "\x61\x7b\x4f\x5b\xa9\xa3\x2b\xb8\x74\x0e\x63\xaa\x95\xc2\xed\x02"
                "\x07\xbe\x27\x92\x09\x07\x7f\x18\x3f\x85\x15\x0a\x11\x66\xab\x1f"
                "\x62"_cv,
                .to =
                "fkZ4ocqpHT4QVeGj3UzY3emyfXFTjIfCi3K8H7ja/bJhe09bqaMruHQOY6qVwu0C"
                "B74nkgkHfxg/hRUKEWarH2I="_cv
            },
        test_vector
            {
                .id = 35,
                .from =
                "\xc2\x76\xf2\xdf\x0b\xfa\xc5\x6a\xe3\x34\x63\x74\xd6\x07\xcb\xa6"
                "\xc3\xe8\xe3\x48\xac\x92\xc4\x31\x95\x34\x28\xb8\xb9\xbb\x84\xb6"
                "\x92\x4e\xd2\xc7\xcf\x74\x40\xf3\x1b\x8e\xa1\x6f\xcc\x4a\xe8\x53"
                "\x80\x54\xb3\xbe\x7d\xb8\xfe\x2e\x14\xd6\xee\x35\x4e\x7d\xde\x71"
                "\xa2\x7d"_cv,
                .to =
                "wnby3wv6xWrjNGN01gfLpsPo40isksQxlTQouLm7hLaSTtLHz3RA8xuOoW/MSuhT"
                "gFSzvn24/i4U1u41Tn3ecaJ9"_cv
            },
        test_vector
            {
                .id = 36,
                .from =
                "\x38\x52\x27\x19\x62\x5e\x42\xb1\xcf\x77\x23\x5b\xc7\x00\xee\xc6"
                "\x88\xcd\xda\x96\x21\xbe\x7a\x4c\x2e\xff\x0e\x47\x32\x24\xbc\x52"
                "\xb7\x16\x8e\xe1\x42\xe2\x9c\x64\x43\xaa\x7b\x95\x42\xbf\x9b\x46"
                "\x27\x05\x76\x47\x05\x4c\xfd\xe4\xe5\x63\x32\x79\xf8\x8b\x6c\x0d"
                "\x2d\x07\x3f"_cv,
                .to =
                "OFInGWJeQrHPdyNbxwDuxojN2pYhvnpMLv8ORzIkvFK3Fo7hQuKcZEOqe5VCv5tG"
                "JwV2RwVM/eTlYzJ5+ItsDS0HPw=="_cv
            },
        test_vector
            {
                .id = 37,
                .from =
                "\xea\x7d\x84\xf6\x20\x36\xbd\xfa\xb0\x16\x96\x38\x3a\x50\x65\xf0"
                "\x06\xe5\xc0\xa7\x96\x60\x5c\xa4\x2e\x70\xbf\x0f\xa1\x97\x0b\xd6"
                "\x51\xce\xaf\xfa\xc8\x00\xf3\xf5\x2f\xf9\x60\x9f\x5d\x74\xc9\x47"
                "\x9c\x1f\x4c\x9c\x44\x89\xd3\x3f\x08\x07\x44\xb8\xaa\xde\xa4\xc8"
                "\xe9\xd4\xbe\x31"_cv,
                .to =
                "6n2E9iA2vfqwFpY4OlBl8AblwKeWYFykLnC/D6GXC9ZRzq/6yADz9S/5YJ9ddMlH"
                "nB9MnESJ0z8IB0S4qt6kyOnUvjE="_cv
            },
        test_vector
            {
                .id = 38,
                .from =
                "\x9d\xfd\x86\x0f\x7e\x16\xb5\x08\x8a\xfb\x41\x21\xa5\xf5\xa8\x03"
                "\x61\x45\x93\x62\x30\xdf\x65\x64\xf1\x4b\x65\xb4\xc9\x07\xce\xc3"
                "\x30\xb2\x97\x80\xf1\xcf\x01\x1a\x22\xaa\xed\x2c\xad\x07\x95\xee"
                "\x59\x7b\x66\x90\xea\xfe\xa3\x72\x28\xbc\x93\xeb\xe8\xfe\x1f\x35"
                "\x34\xb1\x3e\x6a\x55"_cv,
                .to =
                "nf2GD34WtQiK+0EhpfWoA2FFk2Iw32Vk8UtltMkHzsMwspeA8c8BGiKq7SytB5Xu"
                "WXtmkOr+o3IovJPr6P4fNTSxPmpV"_cv
            },
        test_vector
            {
                .id = 39,
                .from =
                "\x0b\x60\x43\xaa\x01\x48\x15\x02\xfd\x2d\x4d\x06\x98\x64\xe3\x00"
                "\x1d\x07\x89\xd3\xdb\xdb\x0b\x94\x5f\x7a\xad\x40\x87\x0a\xe4\x1d"
                "\xa0\x98\x09\xa2\x25\xa1\x95\x88\x57\xab\xbb\x46\x18\x26\xbd\x80"
                "\xba\x3f\xa2\x9f\x8d\xaf\xe6\xa5\x1b\x04\x8f\x4d\x9e\xc9\x20\xbc"
                "\x28\x9b\x1f\xc2\x0b\x89"_cv,
                .to =
                "C2BDqgFIFQL9LU0GmGTjAB0HidPb2wuUX3qtQIcK5B2gmAmiJaGViFeru0YYJr2A"
                "uj+in42v5qUbBI9NnskgvCibH8ILiQ=="_cv
            },
        test_vector
            {
                .id = 40,
                .from =
                "\xa1\xe6\x5c\x84\x98\x9d\xb3\x2f\x01\x3b\xb4\x3d\x5b\x15\x65\x65"
                "\x03\x75\xf2\xea\xb6\x92\xc3\xad\x14\x81\x4b\xb8\x87\x65\x25\xf2"
                "\x88\x23\x54\x3c\xd5\x7d\xbc\xcd\xf6\x86\x3d\xab\x0c\x29\x7c\x83"
                "\x82\x45\x15\x0d\xf1\x46\x9e\xcf\x45\xf7\x64\x17\x88\x97\xfe\xae"
                "\x79\x13\x0c\xf2\xe0\xb1\xac"_cv,
                .to =
                "oeZchJidsy8BO7Q9WxVlZQN18uq2ksOtFIFLuIdlJfKII1Q81X28zfaGPasMKXyD"
                "gkUVDfFGns9F92QXiJf+rnkTDPLgsaw="_cv
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
        auto result = milo::crypto::codec::test<milo::crypto::codec::base_64>::encode(
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

