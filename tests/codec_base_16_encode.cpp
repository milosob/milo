

#include <iostream>

#include <milo/inner.h>
#include <milo/literal.h>

#include <milo/primitive/codec/base.h>
#include <milo/primitive/codec/test.h>


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
                "\x04\x58\xf3\x99\xe9\xd3\x76\xe1\x51\xed\xc8\x7d\xbd\x72\x85\x6e"
                "\x2f\x56\x63\xc2\x45\x0c\x11\x8c\xa0\x94\xd6\xce\xa2\xbb\x6f\x0f"_cv,
                .to =
                "0458f399e9d376e151edc87dbd72856e2f5663c2450c118ca094d6cea2bb6f0f"_cv
            },
        test_vector
            {
                .id = 2,
                .from =
                "\x37\xf3\x8f\xf5\xda\x79\xc3\xf4\x45\x2c\x05\x99\x25\x01\x13\x3f"
                "\x91\xa9\x42\xb3\x3c\x81\x01\xc0\x3c\x5b\xc0\x3e\xfa\xef\xc3\xec"
                "\x13"_cv,
                .to =
                "37f38ff5da79c3f4452c05992501133f91a942b33c8101c03c5bc03efaefc3ec"
                "13"_cv
            },
        test_vector
            {
                .id = 3,
                .from =
                "\xae\xfb\xd3\x51\x54\xd6\xbd\x1d\x7d\x46\x9b\x2c\xb2\x52\x26\x50"
                "\xc3\xce\x18\xd6\xe7\x8c\xd8\x84\x0e\xdf\x28\x15\xbf\xf5\x1d\x22"
                "\x78\xaa"_cv,
                .to =
                "aefbd35154d6bd1d7d469b2cb2522650c3ce18d6e78cd8840edf2815bff51d22"
                "78aa"_cv
            },
        test_vector
            {
                .id = 4,
                .from =
                "\x42\xd5\x6b\x9b\x60\x6a\x6e\xf1\x0a\x97\x36\x6a\x72\x14\xa4\xf4"
                "\x51\xfa\xb8\xb5\xb6\xce\x33\x65\xdb\xe4\x9d\xfc\xbc\x34\x2f\xa0"
                "\xf0\xd9\xf3"_cv,
                .to =
                "42d56b9b606a6ef10a97366a7214a4f451fab8b5b6ce3365dbe49dfcbc342fa0"
                "f0d9f3"_cv
            },
        test_vector
            {
                .id = 5,
                .from =
                "\xe6\x05\x7f\xdf\xd0\x32\x5f\x08\xb1\xc0\xa8\xe6\x06\x0c\xd1\x2e"
                "\xe9\x74\x97\x71\x32\x9e\x1e\x74\x65\x51\x5a\x5e\x04\xdf\x4f\x94"
                "\xd3\x6b\x45\x31"_cv,
                .to =
                "e6057fdfd0325f08b1c0a8e6060cd12ee9749771329e1e7465515a5e04df4f94"
                "d36b4531"_cv
            },
        test_vector
            {
                .id = 6,
                .from =
                "\xd5\xec\xb0\x6a\x17\xd1\x54\x5d\x77\xce\xc2\xe3\x1c\x83\x18\xe0"
                "\x02\xdd\x18\x80\x09\xf3\x27\x4b\x39\x22\xea\x1f\x94\x6d\x0c\x18"
                "\x50\x31\x5e\x01\xd2"_cv,
                .to =
                "d5ecb06a17d1545d77cec2e31c8318e002dd188009f3274b3922ea1f946d0c18"
                "50315e01d2"_cv
            },
        test_vector
            {
                .id = 7,
                .from =
                "\xf9\x13\xee\xbe\x65\xd9\x48\xd7\x3e\x87\x9b\xae\xb9\x69\x7f\xf5"
                "\xd4\xc2\xaf\x78\xef\xf1\xe1\x70\x81\xa2\x5d\xbc\xc7\x05\x70\xf4"
                "\x35\xd5\x82\x6d\x1d\x54"_cv,
                .to =
                "f913eebe65d948d73e879baeb9697ff5d4c2af78eff1e17081a25dbcc70570f4"
                "35d5826d1d54"_cv
            },
        test_vector
            {
                .id = 8,
                .from =
                "\x71\xee\x72\x60\xf7\xf5\x63\x1f\xed\x7d\x72\xe7\xdb\xaa\x4b\x26"
                "\xfc\x93\xf4\x03\xad\x9c\x56\x66\xfc\xa7\x59\x52\xd2\x5c\xf7\xc5"
                "\x1e\xfa\xb1\x60\xb5\x88\x52"_cv,
                .to =
                "71ee7260f7f5631fed7d72e7dbaa4b26fc93f403ad9c5666fca75952d25cf7c5"
                "1efab160b58852"_cv
            },
        test_vector
            {
                .id = 9,
                .from =
                "\x2d\xc8\xe8\x12\x55\x91\x09\xca\x66\x64\x72\x29\x54\xc5\x08\x5d"
                "\x2e\x5d\x10\xd1\x99\x72\x9f\x21\x09\x92\x6a\x68\xa0\x8a\x12\x20"
                "\x6c\xe6\x30\x10\xf2\x33\xa2\x87"_cv,
                .to =
                "2dc8e812559109ca6664722954c5085d2e5d10d199729f2109926a68a08a1220"
                "6ce63010f233a287"_cv
            },
        test_vector
            {
                .id = 10,
                .from =
                "\xc3\xa1\x07\x91\xe6\xaf\x1f\xd2\xb9\x47\x79\x0c\x22\x97\xe5\x68"
                "\x56\x92\xe0\x3c\x2c\xcb\x0d\x7c\x30\x9b\xa0\x34\xf1\x07\x36\x80"
                "\x91\xa8\x37\x97\x9f\xa5\x8a\x0f\x90"_cv,
                .to =
                "c3a10791e6af1fd2b947790c2297e5685692e03c2ccb0d7c309ba034f1073680"
                "91a837979fa58a0f90"_cv
            },
        test_vector
            {
                .id = 11,
                .from =
                "\x8d\xcc\x85\x47\xef\x80\x78\x33\x16\x7d\x42\xc2\xc4\xef\xc6\x2f"
                "\xa8\x68\xef\xf3\xe5\x92\xe4\xfb\x71\x7c\x04\xf6\x98\xe9\xf3\x47"
                "\x27\x95\x64\x3f\x43\x87\x4b\x37\x92\xe9"_cv,
                .to =
                "8dcc8547ef807833167d42c2c4efc62fa868eff3e592e4fb717c04f698e9f347"
                "2795643f43874b3792e9"_cv
            },
        test_vector
            {
                .id = 12,
                .from =
                "\x68\x5f\xde\xd6\x11\xba\x9e\x74\x13\x8d\x3d\x50\x92\x23\x9d\x14"
                "\xea\x04\x30\x72\x80\x15\xcf\xc6\xd0\x91\x23\x43\xa7\x8a\x94\x85"
                "\x62\xa7\x44\x3c\x37\x7f\x47\xc2\xc9\x57\x35"_cv,
                .to =
                "685fded611ba9e74138d3d5092239d14ea0430728015cfc6d0912343a78a9485"
                "62a7443c377f47c2c95735"_cv
            },
        test_vector
            {
                .id = 13,
                .from =
                "\x2a\xa4\x7a\x2c\xec\xd0\xb3\xe3\xa0\x8e\xd8\x9e\x1a\x73\x87\x8d"
                "\x2c\x02\x47\xde\x81\x5a\x32\xb1\xf8\xf4\x44\x42\x8c\x00\x04\x4f"
                "\x6e\xaf\xf9\xb1\x5f\x9a\xa2\xf3\x6f\x03\xc4\x67"_cv,
                .to =
                "2aa47a2cecd0b3e3a08ed89e1a73878d2c0247de815a32b1f8f444428c00044f"
                "6eaff9b15f9aa2f36f03c467"_cv
            },
        test_vector
            {
                .id = 14,
                .from =
                "\xaf\x68\xb4\x9a\xdc\xff\x66\xb5\xc3\x07\xdd\xc6\xbe\x5c\xcf\x08"
                "\x42\x23\x04\xef\x31\xab\xc4\x8b\xce\xb6\x3d\x87\xfb\x30\x42\x10"
                "\xf0\xc1\x51\x2d\xf1\xd3\x13\x8f\xec\x18\xaa\x77\x9c"_cv,
                .to =
                "af68b49adcff66b5c307ddc6be5ccf08422304ef31abc48bceb63d87fb304210"
                "f0c1512df1d3138fec18aa779c"_cv
            },
        test_vector
            {
                .id = 15,
                .from =
                "\x0b\x1f\xbf\xa8\xb3\xea\x9c\x84\x3a\xf5\x40\x0b\xb0\xc7\x68\x86"
                "\xbb\x71\x81\xff\x4e\x10\x5e\x14\xac\x6e\x03\xd7\x92\x7e\xb1\xb8"
                "\xb2\xf4\xed\xcd\x37\x11\x31\xd2\x9f\xa0\xbe\x96\x40\x12"_cv,
                .to =
                "0b1fbfa8b3ea9c843af5400bb0c76886bb7181ff4e105e14ac6e03d7927eb1b8"
                "b2f4edcd371131d29fa0be964012"_cv
            },
        test_vector
            {
                .id = 16,
                .from =
                "\xf4\xab\x6b\xd0\x84\xaa\x42\x5d\xa3\xe2\x5d\xa1\xb3\x28\x46\x81"
                "\xcd\x5a\x4e\x83\x89\x35\x8f\xef\x76\x04\x63\x4e\x61\x48\x6b\x24"
                "\x87\x56\x88\x33\x60\xbc\xdb\x8b\x4d\xe3\x86\xc7\x41\x47\xd8"_cv,
                .to =
                "f4ab6bd084aa425da3e25da1b3284681cd5a4e8389358fef7604634e61486b24"
                "8756883360bcdb8b4de386c74147d8"_cv
            },
        test_vector
            {
                .id = 17,
                .from =
                "\x37\x6a\x9a\x4b\x05\x96\x19\x21\x9b\xa5\xd5\x0c\x8d\xa1\x83\x62"
                "\x5d\x59\xb5\x94\xb4\x47\x20\x08\xb0\xa3\x5d\x95\xc2\x3f\xe7\xc6"
                "\x0c\xd7\xbe\xf0\xd9\xd5\xa5\x7e\x32\x3c\xa0\x87\xaa\x81\xdb\x2d"_cv,
                .to =
                "376a9a4b059619219ba5d50c8da183625d59b594b4472008b0a35d95c23fe7c6"
                "0cd7bef0d9d5a57e323ca087aa81db2d"_cv
            },
        test_vector
            {
                .id = 18,
                .from =
                "\xd5\x88\x76\xbe\x59\x39\x76\x76\x9a\x49\xbf\x11\xe9\x78\x6a\x4d"
                "\xf2\xac\x52\xd4\x9d\x9d\x82\xc9\x15\x53\xa9\x95\x63\x9e\x3f\x52"
                "\xdb\xaf\xc8\x84\xe2\x47\x90\x89\x29\x65\xaf\xfa\x2c\xf0\x59\x08"
                "\x62"_cv,
                .to =
                "d58876be593976769a49bf11e9786a4df2ac52d49d9d82c91553a995639e3f52"
                "dbafc884e24790892965affa2cf0590862"_cv
            },
        test_vector
            {
                .id = 19,
                .from =
                "\x7b\x1b\x0e\xfc\x02\xc3\x44\x15\x26\x32\x9a\xb9\xb8\x33\xd8\xba"
                "\xd9\x9f\xd0\xca\x00\x6f\xe0\xb0\xe8\x7a\x1e\x79\xb8\x67\xf7\xa4"
                "\xa7\x62\x96\x4d\x15\xf1\x4a\xd1\xe9\x1b\x4a\x69\x63\x9c\x3c\x6a"
                "\x95\x04"_cv,
                .to =
                "7b1b0efc02c3441526329ab9b833d8bad99fd0ca006fe0b0e87a1e79b867f7a4"
                "a762964d15f14ad1e91b4a69639c3c6a9504"_cv
            },
        test_vector
            {
                .id = 20,
                .from =
                "\x9c\x7e\xca\x40\xdb\x79\xf2\x60\x96\x59\x34\x14\x2a\x9d\x9d\x13"
                "\xe3\x3d\xbd\x9a\x1f\xb5\x8a\x84\x20\x23\x19\x8e\xe7\x90\x78\x19"
                "\x12\xe9\x59\x71\xef\x74\xe8\x81\x5d\xef\xfb\xc9\xdf\x57\x96\xb3"
                "\xe5\xcf\x63"_cv,
                .to =
                "9c7eca40db79f260965934142a9d9d13e33dbd9a1fb58a842023198ee7907819"
                "12e95971ef74e8815deffbc9df5796b3e5cf63"_cv
            },
        test_vector
            {
                .id = 21,
                .from =
                "\xcc\x8a\xdd\xea\x22\xff\x7b\xde\xaa\xc3\xf7\x54\x9a\xb9\x1e\xd2"
                "\x66\x50\xb2\x52\x97\xbf\x3d\xfd\x89\x51\x99\xad\x68\x1e\x20\x84"
                "\x5b\x1c\x0d\x0c\x68\xd4\xe9\xdb\x70\x21\x5d\x67\x84\x33\xc5\x57"
                "\xd8\x11\x82\xfa"_cv,
                .to =
                "cc8addea22ff7bdeaac3f7549ab91ed26650b25297bf3dfd895199ad681e2084"
                "5b1c0d0c68d4e9db70215d678433c557d81182fa"_cv
            },
        test_vector
            {
                .id = 22,
                .from =
                "\xf6\x1c\x28\x83\x4b\x8e\xf1\x00\x6e\x86\xa8\xcf\x6e\xe1\x77\x88"
                "\xf9\x66\xa8\xab\xc1\xe4\x7a\x2d\x65\x58\x7d\xf4\x00\x82\x16\x33"
                "\xf8\xf1\xb3\x55\x98\xa2\xc3\x8b\xab\x36\x2a\x21\x48\xf2\xd1\xc8"
                "\x36\x7f\xdb\xa9\xc0"_cv,
                .to =
                "f61c28834b8ef1006e86a8cf6ee17788f966a8abc1e47a2d65587df400821633"
                "f8f1b35598a2c38bab362a2148f2d1c8367fdba9c0"_cv
            },
        test_vector
            {
                .id = 23,
                .from =
                "\x25\x63\x1c\x8b\xed\x9a\x15\x1e\xee\x7c\x9a\xac\xc7\x0e\xf1\x53"
                "\xb8\x48\xbe\xd9\x10\x6a\x11\x45\x5f\x86\xbb\xc0\x7a\xbd\x31\xc5"
                "\x20\x80\x4c\xfd\x5e\x52\x6c\x13\x1e\xef\x69\x11\x83\xa6\x1c\xdb"
                "\x5a\x92\x27\x0a\x5c\xb5"_cv,
                .to =
                "25631c8bed9a151eee7c9aacc70ef153b848bed9106a11455f86bbc07abd31c5"
                "20804cfd5e526c131eef691183a61cdb5a92270a5cb5"_cv
            },
        test_vector
            {
                .id = 24,
                .from =
                "\x2f\x76\x99\x5a\x1c\x83\xca\x8c\x89\xd4\xf0\x6f\x89\x9e\xf0\x68"
                "\xb4\x7c\x29\xe9\xb3\xf0\x48\xc7\x87\x3c\xf7\xfd\x02\xd2\xc3\x92"
                "\x0f\x5b\x5f\x08\x5d\xc6\x32\xe4\xe4\xd2\xde\x53\x19\xb1\x19\x0b"
                "\x7e\xb1\xd2\x8d\x51\x59\x6d"_cv,
                .to =
                "2f76995a1c83ca8c89d4f06f899ef068b47c29e9b3f048c7873cf7fd02d2c392"
                "0f5b5f085dc632e4e4d2de5319b1190b7eb1d28d51596d"_cv
            },
        test_vector
            {
                .id = 25,
                .from =
                "\x27\x75\xe7\x82\xf0\xcb\xb6\xa9\x64\x81\x1f\xc8\x85\x3f\x9d\x9b"
                "\xc0\xa2\xe7\x8f\xfd\xb0\x9b\x5f\x56\x03\x2d\x28\xef\xe8\xba\x85"
                "\x12\xfd\xce\x67\x50\x94\x7e\xc3\x47\x97\xf8\xdd\x74\x3e\x77\xf8"
                "\x1b\x0f\xbb\x0f\xb0\x21\xd8\xf2"_cv,
                .to =
                "2775e782f0cbb6a964811fc8853f9d9bc0a2e78ffdb09b5f56032d28efe8ba85"
                "12fdce6750947ec34797f8dd743e77f81b0fbb0fb021d8f2"_cv
            },
        test_vector
            {
                .id = 26,
                .from =
                "\x82\xdf\x08\x2d\x62\x6f\xd7\x31\x6c\x9a\xbd\x1e\x5e\x50\x0e\xe5"
                "\x68\x18\x26\xe8\x13\x17\x15\xbb\x7d\xa6\xe7\x0b\xda\x9e\xcb\x1d"
                "\x6b\x8e\x97\x63\x63\x88\x29\x5e\xf2\xa5\x0b\x87\xed\xd6\x56\xa6"
                "\xc7\xdd\x04\x94\x41\xeb\xab\x16\x50"_cv,
                .to =
                "82df082d626fd7316c9abd1e5e500ee5681826e8131715bb7da6e70bda9ecb1d"
                "6b8e97636388295ef2a50b87edd656a6c7dd049441ebab1650"_cv
            },
        test_vector
            {
                .id = 27,
                .from =
                "\xb1\x75\x01\x7c\x6b\xdb\xdc\x2b\x0a\xd3\x00\x43\xe3\x00\x40\x26"
                "\xd8\xe2\x1e\x02\x9a\x0d\x48\x1b\x46\x97\x2d\xe9\x25\xd0\xc1\xc0"
                "\x4b\x21\x2b\x1d\x98\x25\xbb\x18\xf7\x82\x3f\xdd\x67\xee\xa7\x86"
                "\x78\x7f\x4b\xc8\xfc\x19\x99\xac\x30\xa2"_cv,
                .to =
                "b175017c6bdbdc2b0ad30043e3004026d8e21e029a0d481b46972de925d0c1c0"
                "4b212b1d9825bb18f7823fdd67eea786787f4bc8fc1999ac30a2"_cv
            },
        test_vector
            {
                .id = 28,
                .from =
                "\xc2\x82\x65\xfe\x56\x91\x73\xae\x1d\x6d\xca\x06\x4d\xd7\xb3\x82"
                "\x38\x09\x50\xf8\x7a\xb5\x67\xb5\x8d\xd4\x84\xd6\x28\x02\x08\x61"
                "\xf0\x34\xb4\xa6\x7e\x5e\xce\xe9\x2b\x1b\x15\x38\x83\xe7\xe1\x1e"
                "\xd1\xc2\x57\xd3\x10\x8f\x09\x70\x9c\x3d\x8a"_cv,
                .to =
                "c28265fe569173ae1d6dca064dd7b382380950f87ab567b58dd484d628020861"
                "f034b4a67e5ecee92b1b153883e7e11ed1c257d3108f09709c3d8a"_cv
            },
        test_vector
            {
                .id = 29,
                .from =
                "\x3d\x21\x44\x22\x6e\xfc\xe4\x1f\x08\xad\xfa\x74\x21\x9f\x69\x08"
                "\xf6\x62\x02\x33\x58\xad\xe3\x39\x44\xd4\x5d\x7a\xe0\xba\x42\xf0"
                "\xec\x64\x27\x04\x6c\x01\xe4\x50\x83\x45\x49\xe7\x4f\x8c\xc4\x8c"
                "\xac\x41\x40\xaa\x80\xc2\x50\x04\x5f\x74\x1c\x5e"_cv,
                .to =
                "3d2144226efce41f08adfa74219f6908f662023358ade33944d45d7ae0ba42f0"
                "ec6427046c01e450834549e74f8cc48cac4140aa80c250045f741c5e"_cv
            },
        test_vector
            {
                .id = 30,
                .from =
                "\x72\xef\xd9\xf9\x09\x17\xf2\x36\x06\xea\x31\xb8\xeb\x73\xd3\x80"
                "\x2b\x7b\xf5\x93\xa0\x67\x87\x81\xaa\x70\xc3\xdb\x11\xff\x70\x4f"
                "\x5e\x5c\x85\x6c\x35\x35\xa3\xcb\xd0\x96\x21\x69\x40\xd4\x4a\x2f"
                "\x04\x5e\xac\x6a\xea\x9e\x23\x96\xc9\xcd\x19\x27\x12"_cv,
                .to =
                "72efd9f90917f23606ea31b8eb73d3802b7bf593a0678781aa70c3db11ff704f"
                "5e5c856c3535a3cbd096216940d44a2f045eac6aea9e2396c9cd192712"_cv
            },
        test_vector
            {
                .id = 31,
                .from =
                "\xd4\xdc\x95\xd5\x6a\x8f\xbe\xb0\x80\xa9\x82\x8b\xa2\x3d\x99\x66"
                "\x62\xeb\x7d\x52\xac\x0c\xdc\xfd\x09\xe0\xd5\x85\xd0\x41\x22\xf0"
                "\x6e\xce\x50\xd6\xff\x89\x06\x99\x64\xa5\xf5\xbc\x99\xcf\x74\xc8"
                "\xd4\x46\x82\x27\x68\xd0\x9c\xa9\x62\x80\xb5\xad\xd5\x84"_cv,
                .to =
                "d4dc95d56a8fbeb080a9828ba23d996662eb7d52ac0cdcfd09e0d585d04122f0"
                "6ece50d6ff89069964a5f5bc99cf74c8d446822768d09ca96280b5add584"_cv
            },
        test_vector
            {
                .id = 32,
                .from =
                "\xfb\xb8\x90\x01\xc1\x10\x2e\xf7\x8c\x0d\xb4\x98\x25\xf5\xb5\xac"
                "\x59\x2a\x7e\x75\xb0\xca\xec\xfe\xfd\x67\xd5\x34\x41\xc8\x8c\x52"
                "\x8b\xd3\x69\x9f\xab\x37\x7e\x68\x39\x1f\x8b\x9f\xd2\x4c\x19\x0b"
                "\x7a\x7e\x46\x4d\xb4\x82\xe5\x4e\xc1\xb8\xdf\x27\xd6\x0b\x2d"_cv,
                .to =
                "fbb89001c1102ef78c0db49825f5b5ac592a7e75b0caecfefd67d53441c88c52"
                "8bd3699fab377e68391f8b9fd24c190b7a7e464db482e54ec1b8df27d60b2d"_cv
            },
        test_vector
            {
                .id = 33,
                .from =
                "\x3a\x71\xc3\xaf\x84\x73\x9f\x09\xd9\x16\x15\x8d\xb9\x57\xa7\x2c"
                "\x8b\x5b\x85\x20\x04\xf0\x8f\x83\x94\x93\x5d\x41\x15\xa9\x6c\xe0"
                "\x5d\xed\x3e\xc0\x08\x45\x7f\x9c\xc1\x27\x4b\x36\x35\x39\x32\x11"
                "\x6b\x1b\x77\x23\x43\x66\xc1\x5d\xe8\x60\x03\x9e\x6e\x51\xd4\x09"_cv,
                .to =
                "3a71c3af84739f09d916158db957a72c8b5b852004f08f8394935d4115a96ce0"
                "5ded3ec008457f9cc1274b36353932116b1b77234366c15de860039e6e51d409"_cv
            },
        test_vector
            {
                .id = 34,
                .from =
                "\xfa\x0a\x43\xbf\xf2\xcc\x77\xf8\xe3\xef\x60\x4b\x6f\xbd\x75\x57"
                "\x7c\xf7\x47\xe2\x47\x18\x2c\x71\xab\x0d\x4d\xa3\x96\x33\x26\x24"
                "\xb9\xb7\x76\xa4\x76\x6c\xec\x2b\xa4\x09\x06\xea\xe8\x77\x97\x1d"
                "\xd3\x6c\xe7\x3e\xdb\xf1\x40\xae\xd7\x09\xe5\x71\x7f\xb5\x98\xa8"
                "\x56"_cv,
                .to =
                "fa0a43bff2cc77f8e3ef604b6fbd75577cf747e247182c71ab0d4da396332624"
                "b9b776a4766cec2ba40906eae877971dd36ce73edbf140aed709e5717fb598a8"
                "56"_cv
            },
        test_vector
            {
                .id = 35,
                .from =
                "\x3c\xf5\xc6\x1c\x66\xa4\xd9\xa1\x70\x33\x11\xec\x38\xf1\x5a\xd3"
                "\xda\x87\x9b\xfc\x51\x74\xba\xc2\xc2\x04\x81\x3e\x93\xda\x41\xbb"
                "\x15\x9c\x57\x83\xa5\xfd\xa0\x96\x64\x62\xcd\x69\x90\x31\x5b\xad"
                "\x6d\x28\x73\xec\x5e\xa8\x2b\xab\x2d\x1a\x3a\xe8\x1a\xc8\xba\xf7"
                "\xdd\x86"_cv,
                .to =
                "3cf5c61c66a4d9a1703311ec38f15ad3da879bfc5174bac2c204813e93da41bb"
                "159c5783a5fda0966462cd6990315bad6d2873ec5ea82bab2d1a3ae81ac8baf7"
                "dd86"_cv
            },
        test_vector
            {
                .id = 36,
                .from =
                "\xd9\x92\x24\x05\xaf\x46\x03\x22\xf7\x7f\xd5\x13\x5e\xb4\x59\x7c"
                "\xae\x18\x95\x54\xd0\x62\x67\xf3\x99\x7b\x84\xad\xe0\x06\x8d\x50"
                "\x30\x1c\xcc\x07\xe8\x4c\x2d\xec\x6e\xcc\x1e\x09\x9d\xdd\x60\xdc"
                "\x95\x86\x42\xa4\xc7\xa7\x71\xa4\xf3\xa9\xa9\xbd\xc6\xc9\x88\x19"
                "\x04\xf6\x15"_cv,
                .to =
                "d9922405af460322f77fd5135eb4597cae189554d06267f3997b84ade0068d50"
                "301ccc07e84c2dec6ecc1e099ddd60dc958642a4c7a771a4f3a9a9bdc6c98819"
                "04f615"_cv
            },
        test_vector
            {
                .id = 37,
                .from =
                "\xac\x68\x36\x91\x81\x66\x94\x40\x4d\x60\xae\xdd\x51\x07\xd6\x78"
                "\x0c\xbd\x82\x18\x36\x69\xe9\x0d\xf9\x9b\xb8\xd1\x3a\xa9\x21\xa1"
                "\x30\xe7\x0b\xac\xaf\x8d\x27\xd6\x70\xd5\x34\x5e\xbd\x47\xcc\x4e"
                "\x74\xc4\x2e\x79\x31\x79\xa0\xf5\xd9\x32\x75\x07\x04\xb4\xed\x78"
                "\x29\xd5\x2f\xdb"_cv,
                .to =
                "ac683691816694404d60aedd5107d6780cbd82183669e90df99bb8d13aa921a1"
                "30e70bacaf8d27d670d5345ebd47cc4e74c42e793179a0f5d932750704b4ed78"
                "29d52fdb"_cv
            },
        test_vector
            {
                .id = 38,
                .from =
                "\x00\xa8\xed\xac\xd8\x4b\xf2\x6e\x2d\x18\x31\xb0\x79\x4e\x92\xd8"
                "\x7c\x2d\x59\xae\xcc\xab\x56\xb6\x70\xb7\xf2\xef\x69\x28\x51\x1d"
                "\x2b\x9e\xc9\x1f\x99\xc6\x3d\x32\xcb\x95\x6f\xff\x2f\xd6\xe2\xd1"
                "\xd1\x52\x89\x96\xa9\x0f\x02\xcc\xee\x84\x46\xc5\x38\xe0\xbf\xc9"
                "\x72\x7a\x26\x2c\x37"_cv,
                .to =
                "00a8edacd84bf26e2d1831b0794e92d87c2d59aeccab56b670b7f2ef6928511d"
                "2b9ec91f99c63d32cb956fff2fd6e2d1d1528996a90f02ccee8446c538e0bfc9"
                "727a262c37"_cv
            },
        test_vector
            {
                .id = 39,
                .from =
                "\x78\x66\xb1\x46\x5c\xba\x7c\x09\x3a\x30\xb2\x90\x92\x03\x0a\x7c"
                "\x28\x15\xb0\xd6\xde\xf7\xec\x02\x99\x50\x4f\x2e\x81\x0b\xc0\xeb"
                "\x1e\x95\xf9\x1f\xf6\xf1\xee\x12\x78\x17\xfb\xf3\xa8\x46\xa4\x32"
                "\xcc\x0f\xaa\x66\x73\x1e\x27\xa0\x10\xa0\x68\x74\xc0\xf9\xd6\x6d"
                "\xef\x67\x7f\xb4\x60\x57"_cv,
                .to =
                "7866b1465cba7c093a30b29092030a7c2815b0d6def7ec0299504f2e810bc0eb"
                "1e95f91ff6f1ee127817fbf3a846a432cc0faa66731e27a010a06874c0f9d66d"
                "ef677fb46057"_cv
            },
        test_vector
            {
                .id = 40,
                .from =
                "\xf3\xf4\x7f\x2a\x12\xfc\x91\x40\x17\x22\xb9\x91\x6f\xa9\xdc\x65"
                "\xe5\xe4\x1a\xcb\xb7\xd9\xd7\xbd\x71\x6f\x42\x0e\xcc\x38\x24\x97"
                "\xd7\xdd\x50\xa0\x83\x8e\x0a\x12\x62\xd9\xf8\x72\xff\x06\x68\x7d"
                "\x75\xda\xea\x35\x6f\x34\x18\x34\x71\x0f\xdb\xb3\xae\x8d\xb1\x49"
                "\x8d\x95\xfb\xdd\xfa\x0b\x2b"_cv,
                .to =
                "f3f47f2a12fc91401722b9916fa9dc65e5e41acbb7d9d7bd716f420ecc382497"
                "d7dd50a0838e0a1262d9f872ff06687d75daea356f341834710fdbb3ae8db149"
                "8d95fbddfa0b2b"_cv
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
        auto result = milo::primitive::codec::test<milo::primitive::codec::base_16>::encode(
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

