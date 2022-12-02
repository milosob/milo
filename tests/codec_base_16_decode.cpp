

#include <iostream>

#include <milo/concepts.h>
#include <milo/container.h>
#include <milo/literals.h>

#include <milo/codec/base.h>
#include <milo/codec/test.h>


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
                "68cc4eb7f3590827b38c256f90bb8facdc9ec65627c0504da6cc6f950ca9281c"_cv,
                .to =
                "\x68\xcc\x4e\xb7\xf3\x59\x08\x27\xb3\x8c\x25\x6f\x90\xbb\x8f\xac"
                "\xdc\x9e\xc6\x56\x27\xc0\x50\x4d\xa6\xcc\x6f\x95\x0c\xa9\x28\x1c"_cv
            },
        test_vector
            {
                .id = 2,
                .from =
                "2519d5a1c36fe4b0695b68f0296ce53612284935b2de6bf05999af961a10ebcc"
                "ba"_cv,
                .to =
                "\x25\x19\xd5\xa1\xc3\x6f\xe4\xb0\x69\x5b\x68\xf0\x29\x6c\xe5\x36"
                "\x12\x28\x49\x35\xb2\xde\x6b\xf0\x59\x99\xaf\x96\x1a\x10\xeb\xcc"
                "\xba"_cv
            },
        test_vector
            {
                .id = 3,
                .from =
                "d3f3093236b48660dc81fa9b3089e272648b677cd46bf182d89551acb2fa490f"
                "0446"_cv,
                .to =
                "\xd3\xf3\x09\x32\x36\xb4\x86\x60\xdc\x81\xfa\x9b\x30\x89\xe2\x72"
                "\x64\x8b\x67\x7c\xd4\x6b\xf1\x82\xd8\x95\x51\xac\xb2\xfa\x49\x0f"
                "\x04\x46"_cv
            },
        test_vector
            {
                .id = 4,
                .from =
                "4afef7235135f057d940e57029756bc0a4364ab9c2a72419b797ce72eaa450ba"
                "fa1406"_cv,
                .to =
                "\x4a\xfe\xf7\x23\x51\x35\xf0\x57\xd9\x40\xe5\x70\x29\x75\x6b\xc0"
                "\xa4\x36\x4a\xb9\xc2\xa7\x24\x19\xb7\x97\xce\x72\xea\xa4\x50\xba"
                "\xfa\x14\x06"_cv
            },
        test_vector
            {
                .id = 5,
                .from =
                "0e97369e7f30be6c65ebf082fbe009c94e70a3acac2762688fd1d1c9ac647af7"
                "8046b2ca"_cv,
                .to =
                "\x0e\x97\x36\x9e\x7f\x30\xbe\x6c\x65\xeb\xf0\x82\xfb\xe0\x09\xc9"
                "\x4e\x70\xa3\xac\xac\x27\x62\x68\x8f\xd1\xd1\xc9\xac\x64\x7a\xf7"
                "\x80\x46\xb2\xca"_cv
            },
        test_vector
            {
                .id = 6,
                .from =
                "70d38017e799d7611cdada99394e860a9169e4095682ae010f72e4a85b858ca7"
                "6fe9cd1c5e"_cv,
                .to =
                "\x70\xd3\x80\x17\xe7\x99\xd7\x61\x1c\xda\xda\x99\x39\x4e\x86\x0a"
                "\x91\x69\xe4\x09\x56\x82\xae\x01\x0f\x72\xe4\xa8\x5b\x85\x8c\xa7"
                "\x6f\xe9\xcd\x1c\x5e"_cv
            },
        test_vector
            {
                .id = 7,
                .from =
                "dc7e02e8b8a97466feb9a4743034f5e7e50d906062a16982a23c0fec61339d4f"
                "ae6756ac5e57"_cv,
                .to =
                "\xdc\x7e\x02\xe8\xb8\xa9\x74\x66\xfe\xb9\xa4\x74\x30\x34\xf5\xe7"
                "\xe5\x0d\x90\x60\x62\xa1\x69\x82\xa2\x3c\x0f\xec\x61\x33\x9d\x4f"
                "\xae\x67\x56\xac\x5e\x57"_cv
            },
        test_vector
            {
                .id = 8,
                .from =
                "ce24807612529d4604c7c5e50c1b751ee56ecfd16a0ed52b00e02c262413eaae"
                "25bbfbb61f4253"_cv,
                .to =
                "\xce\x24\x80\x76\x12\x52\x9d\x46\x04\xc7\xc5\xe5\x0c\x1b\x75\x1e"
                "\xe5\x6e\xcf\xd1\x6a\x0e\xd5\x2b\x00\xe0\x2c\x26\x24\x13\xea\xae"
                "\x25\xbb\xfb\xb6\x1f\x42\x53"_cv
            },
        test_vector
            {
                .id = 9,
                .from =
                "8a37eb53feb44ea5723a25fcc6282d618f3525f0cc4ff26942d974a8b5940200"
                "ffad18c212eb2e57"_cv,
                .to =
                "\x8a\x37\xeb\x53\xfe\xb4\x4e\xa5\x72\x3a\x25\xfc\xc6\x28\x2d\x61"
                "\x8f\x35\x25\xf0\xcc\x4f\xf2\x69\x42\xd9\x74\xa8\xb5\x94\x02\x00"
                "\xff\xad\x18\xc2\x12\xeb\x2e\x57"_cv
            },
        test_vector
            {
                .id = 10,
                .from =
                "9a9cd0fb18d1cb85042ded5c6c21191ba52e3c2fd115c4c54e4209ecb769f6cb"
                "b26a28a16223f83e2b"_cv,
                .to =
                "\x9a\x9c\xd0\xfb\x18\xd1\xcb\x85\x04\x2d\xed\x5c\x6c\x21\x19\x1b"
                "\xa5\x2e\x3c\x2f\xd1\x15\xc4\xc5\x4e\x42\x09\xec\xb7\x69\xf6\xcb"
                "\xb2\x6a\x28\xa1\x62\x23\xf8\x3e\x2b"_cv
            },
        test_vector
            {
                .id = 11,
                .from =
                "44a99efd17f10cfaa2e9266f4a11a74af08636f741220041ddb73fd3027240ef"
                "96501d4f821d84b31f1c"_cv,
                .to =
                "\x44\xa9\x9e\xfd\x17\xf1\x0c\xfa\xa2\xe9\x26\x6f\x4a\x11\xa7\x4a"
                "\xf0\x86\x36\xf7\x41\x22\x00\x41\xdd\xb7\x3f\xd3\x02\x72\x40\xef"
                "\x96\x50\x1d\x4f\x82\x1d\x84\xb3\x1f\x1c"_cv
            },
        test_vector
            {
                .id = 12,
                .from =
                "293d072cc3e8cc18c9a3fa224c1921f4c2171bbeb79501bc33519bcdfdde190c"
                "72bc0df9fbb308e6dfa829"_cv,
                .to =
                "\x29\x3d\x07\x2c\xc3\xe8\xcc\x18\xc9\xa3\xfa\x22\x4c\x19\x21\xf4"
                "\xc2\x17\x1b\xbe\xb7\x95\x01\xbc\x33\x51\x9b\xcd\xfd\xde\x19\x0c"
                "\x72\xbc\x0d\xf9\xfb\xb3\x08\xe6\xdf\xa8\x29"_cv
            },
        test_vector
            {
                .id = 13,
                .from =
                "b35ecf8bf7de3315f1dc5a096e79d58493a686411de796e59e2212cebe42b8c1"
                "22bc7008bae410838bb20c08"_cv,
                .to =
                "\xb3\x5e\xcf\x8b\xf7\xde\x33\x15\xf1\xdc\x5a\x09\x6e\x79\xd5\x84"
                "\x93\xa6\x86\x41\x1d\xe7\x96\xe5\x9e\x22\x12\xce\xbe\x42\xb8\xc1"
                "\x22\xbc\x70\x08\xba\xe4\x10\x83\x8b\xb2\x0c\x08"_cv
            },
        test_vector
            {
                .id = 14,
                .from =
                "bdfdf9f6a8f7f5ddbeb1d3d4b9ddedd514e1eed6d2f53311b66f596ac34059c9"
                "defa7fac1b530250f533acf2f5"_cv,
                .to =
                "\xbd\xfd\xf9\xf6\xa8\xf7\xf5\xdd\xbe\xb1\xd3\xd4\xb9\xdd\xed\xd5"
                "\x14\xe1\xee\xd6\xd2\xf5\x33\x11\xb6\x6f\x59\x6a\xc3\x40\x59\xc9"
                "\xde\xfa\x7f\xac\x1b\x53\x02\x50\xf5\x33\xac\xf2\xf5"_cv
            },
        test_vector
            {
                .id = 15,
                .from =
                "f4befcd94f29d32fef7654eb8a6119a6960b58f67cfdf1e216f7b951cee6d389"
                "38c465b6654c0d5c81743bc1c18d"_cv,
                .to =
                "\xf4\xbe\xfc\xd9\x4f\x29\xd3\x2f\xef\x76\x54\xeb\x8a\x61\x19\xa6"
                "\x96\x0b\x58\xf6\x7c\xfd\xf1\xe2\x16\xf7\xb9\x51\xce\xe6\xd3\x89"
                "\x38\xc4\x65\xb6\x65\x4c\x0d\x5c\x81\x74\x3b\xc1\xc1\x8d"_cv
            },
        test_vector
            {
                .id = 16,
                .from =
                "b8fb71d85f741e9c1a4a9c74880213fdc61c4b13c8914e18ebea8ec374082271"
                "d6d331b651d5f95618096d8d77f9b8"_cv,
                .to =
                "\xb8\xfb\x71\xd8\x5f\x74\x1e\x9c\x1a\x4a\x9c\x74\x88\x02\x13\xfd"
                "\xc6\x1c\x4b\x13\xc8\x91\x4e\x18\xeb\xea\x8e\xc3\x74\x08\x22\x71"
                "\xd6\xd3\x31\xb6\x51\xd5\xf9\x56\x18\x09\x6d\x8d\x77\xf9\xb8"_cv
            },
        test_vector
            {
                .id = 17,
                .from =
                "0fe451f124e27572a8a897523c82e78df9b293b25cf0de5d11a7eef40e47df46"
                "47f47690810dc8a7f8fb5c39d7d0ad60"_cv,
                .to =
                "\x0f\xe4\x51\xf1\x24\xe2\x75\x72\xa8\xa8\x97\x52\x3c\x82\xe7\x8d"
                "\xf9\xb2\x93\xb2\x5c\xf0\xde\x5d\x11\xa7\xee\xf4\x0e\x47\xdf\x46"
                "\x47\xf4\x76\x90\x81\x0d\xc8\xa7\xf8\xfb\x5c\x39\xd7\xd0\xad\x60"_cv
            },
        test_vector
            {
                .id = 18,
                .from =
                "57bdd4f49c72436e30b6cd77ad5e3c2000c606689c7b450cb6c53a0fd091e298"
                "24a92a14908867f3b62213561b3806cd5d"_cv,
                .to =
                "\x57\xbd\xd4\xf4\x9c\x72\x43\x6e\x30\xb6\xcd\x77\xad\x5e\x3c\x20"
                "\x00\xc6\x06\x68\x9c\x7b\x45\x0c\xb6\xc5\x3a\x0f\xd0\x91\xe2\x98"
                "\x24\xa9\x2a\x14\x90\x88\x67\xf3\xb6\x22\x13\x56\x1b\x38\x06\xcd"
                "\x5d"_cv
            },
        test_vector
            {
                .id = 19,
                .from =
                "09b3704e88ae7f61dadf44e164be60ffc286c92eef22d4cbf38376246c347610"
                "1a3f48c0b5b3ec4fafee816d7c253e2ec4c2"_cv,
                .to =
                "\x09\xb3\x70\x4e\x88\xae\x7f\x61\xda\xdf\x44\xe1\x64\xbe\x60\xff"
                "\xc2\x86\xc9\x2e\xef\x22\xd4\xcb\xf3\x83\x76\x24\x6c\x34\x76\x10"
                "\x1a\x3f\x48\xc0\xb5\xb3\xec\x4f\xaf\xee\x81\x6d\x7c\x25\x3e\x2e"
                "\xc4\xc2"_cv
            },
        test_vector
            {
                .id = 20,
                .from =
                "85c45f37d3d88fa6f74d028cdd94174abfe063002b7dce699227a36a3e5097a5"
                "2ae72388bc41a3deb68733421a5d18d4432c5e"_cv,
                .to =
                "\x85\xc4\x5f\x37\xd3\xd8\x8f\xa6\xf7\x4d\x02\x8c\xdd\x94\x17\x4a"
                "\xbf\xe0\x63\x00\x2b\x7d\xce\x69\x92\x27\xa3\x6a\x3e\x50\x97\xa5"
                "\x2a\xe7\x23\x88\xbc\x41\xa3\xde\xb6\x87\x33\x42\x1a\x5d\x18\xd4"
                "\x43\x2c\x5e"_cv
            },
        test_vector
            {
                .id = 21,
                .from =
                "5b243daac0ad8971a424263731efee2cc71f33877f77a3dccc741778716c6193"
                "2656627899d8612aa1cbada65e3d71c6c54a50ea"_cv,
                .to =
                "\x5b\x24\x3d\xaa\xc0\xad\x89\x71\xa4\x24\x26\x37\x31\xef\xee\x2c"
                "\xc7\x1f\x33\x87\x7f\x77\xa3\xdc\xcc\x74\x17\x78\x71\x6c\x61\x93"
                "\x26\x56\x62\x78\x99\xd8\x61\x2a\xa1\xcb\xad\xa6\x5e\x3d\x71\xc6"
                "\xc5\x4a\x50\xea"_cv
            },
        test_vector
            {
                .id = 22,
                .from =
                "0bb431874b8996474f95650c515788052d6a82bc322a05a93ee467726115e91c"
                "74109a9b960a6f1aaacf0b83738fdd4c2e939e7ba1"_cv,
                .to =
                "\x0b\xb4\x31\x87\x4b\x89\x96\x47\x4f\x95\x65\x0c\x51\x57\x88\x05"
                "\x2d\x6a\x82\xbc\x32\x2a\x05\xa9\x3e\xe4\x67\x72\x61\x15\xe9\x1c"
                "\x74\x10\x9a\x9b\x96\x0a\x6f\x1a\xaa\xcf\x0b\x83\x73\x8f\xdd\x4c"
                "\x2e\x93\x9e\x7b\xa1"_cv
            },
        test_vector
            {
                .id = 23,
                .from =
                "3aca467ade76f31a998a8392890fa8184a69bd217a069d25ffde636cf3ee96b3"
                "e23b504547690c5e3b3a31c86794da4f65f077c4ddb3"_cv,
                .to =
                "\x3a\xca\x46\x7a\xde\x76\xf3\x1a\x99\x8a\x83\x92\x89\x0f\xa8\x18"
                "\x4a\x69\xbd\x21\x7a\x06\x9d\x25\xff\xde\x63\x6c\xf3\xee\x96\xb3"
                "\xe2\x3b\x50\x45\x47\x69\x0c\x5e\x3b\x3a\x31\xc8\x67\x94\xda\x4f"
                "\x65\xf0\x77\xc4\xdd\xb3"_cv
            },
        test_vector
            {
                .id = 24,
                .from =
                "dbee254a1657f5b1f911117b7db2b01866fa1c3ca860e29d35cc7548cce4b204"
                "a4c10cbc641dfabea03baefcc4aed64260716febe8a614"_cv,
                .to =
                "\xdb\xee\x25\x4a\x16\x57\xf5\xb1\xf9\x11\x11\x7b\x7d\xb2\xb0\x18"
                "\x66\xfa\x1c\x3c\xa8\x60\xe2\x9d\x35\xcc\x75\x48\xcc\xe4\xb2\x04"
                "\xa4\xc1\x0c\xbc\x64\x1d\xfa\xbe\xa0\x3b\xae\xfc\xc4\xae\xd6\x42"
                "\x60\x71\x6f\xeb\xe8\xa6\x14"_cv
            },
        test_vector
            {
                .id = 25,
                .from =
                "2f1006bdab890b4f59668ea0974ffce6ddbebeeb815be102c796b84ac69e3dcd"
                "9f712a531ee02ea8d876a5d0d2ec90fba0c634c59d9c32f4"_cv,
                .to =
                "\x2f\x10\x06\xbd\xab\x89\x0b\x4f\x59\x66\x8e\xa0\x97\x4f\xfc\xe6"
                "\xdd\xbe\xbe\xeb\x81\x5b\xe1\x02\xc7\x96\xb8\x4a\xc6\x9e\x3d\xcd"
                "\x9f\x71\x2a\x53\x1e\xe0\x2e\xa8\xd8\x76\xa5\xd0\xd2\xec\x90\xfb"
                "\xa0\xc6\x34\xc5\x9d\x9c\x32\xf4"_cv
            },
        test_vector
            {
                .id = 26,
                .from =
                "ae41d00fc543541d1f8cac03c2dbc3170fc530a3d3f6ec88cd741741b6dd2151"
                "d68209562f3b793d8b188ff887a18c78e3093521815ac735fa"_cv,
                .to =
                "\xae\x41\xd0\x0f\xc5\x43\x54\x1d\x1f\x8c\xac\x03\xc2\xdb\xc3\x17"
                "\x0f\xc5\x30\xa3\xd3\xf6\xec\x88\xcd\x74\x17\x41\xb6\xdd\x21\x51"
                "\xd6\x82\x09\x56\x2f\x3b\x79\x3d\x8b\x18\x8f\xf8\x87\xa1\x8c\x78"
                "\xe3\x09\x35\x21\x81\x5a\xc7\x35\xfa"_cv
            },
        test_vector
            {
                .id = 27,
                .from =
                "dd8770b1e969777ec162e8526f4bf96c617b6ba66d3aec2e8b8dc82821a9f1b8"
                "cf198c0f014b5d000d6e08e4ff98f4211bbe2a10802feff85159"_cv,
                .to =
                "\xdd\x87\x70\xb1\xe9\x69\x77\x7e\xc1\x62\xe8\x52\x6f\x4b\xf9\x6c"
                "\x61\x7b\x6b\xa6\x6d\x3a\xec\x2e\x8b\x8d\xc8\x28\x21\xa9\xf1\xb8"
                "\xcf\x19\x8c\x0f\x01\x4b\x5d\x00\x0d\x6e\x08\xe4\xff\x98\xf4\x21"
                "\x1b\xbe\x2a\x10\x80\x2f\xef\xf8\x51\x59"_cv
            },
        test_vector
            {
                .id = 28,
                .from =
                "a122f2aa95643522e6b203d37912d49f821028ba2d8e8cac074a1daebb01bae7"
                "71214c86a81bd035e0fe9961f54456df03b9417cf106315331221a"_cv,
                .to =
                "\xa1\x22\xf2\xaa\x95\x64\x35\x22\xe6\xb2\x03\xd3\x79\x12\xd4\x9f"
                "\x82\x10\x28\xba\x2d\x8e\x8c\xac\x07\x4a\x1d\xae\xbb\x01\xba\xe7"
                "\x71\x21\x4c\x86\xa8\x1b\xd0\x35\xe0\xfe\x99\x61\xf5\x44\x56\xdf"
                "\x03\xb9\x41\x7c\xf1\x06\x31\x53\x31\x22\x1a"_cv
            },
        test_vector
            {
                .id = 29,
                .from =
                "fc8adbe1c337ad708dfe0e9126c48091d2a8e0044b8ed5d9f2be2b4133d4e7b4"
                "dceece01d146cf900c07ac49e33887192835c063d2bc255e92ad80bc"_cv,
                .to =
                "\xfc\x8a\xdb\xe1\xc3\x37\xad\x70\x8d\xfe\x0e\x91\x26\xc4\x80\x91"
                "\xd2\xa8\xe0\x04\x4b\x8e\xd5\xd9\xf2\xbe\x2b\x41\x33\xd4\xe7\xb4"
                "\xdc\xee\xce\x01\xd1\x46\xcf\x90\x0c\x07\xac\x49\xe3\x38\x87\x19"
                "\x28\x35\xc0\x63\xd2\xbc\x25\x5e\x92\xad\x80\xbc"_cv
            },
        test_vector
            {
                .id = 30,
                .from =
                "b2ee742cba0fe55e5f944544f4dd584f49d839fa4b0fb3f7fa90678413493fea"
                "da1a7e65b1c462bd7f93f17a848dceaf0053b64aad50ad8c6e34df5fba"_cv,
                .to =
                "\xb2\xee\x74\x2c\xba\x0f\xe5\x5e\x5f\x94\x45\x44\xf4\xdd\x58\x4f"
                "\x49\xd8\x39\xfa\x4b\x0f\xb3\xf7\xfa\x90\x67\x84\x13\x49\x3f\xea"
                "\xda\x1a\x7e\x65\xb1\xc4\x62\xbd\x7f\x93\xf1\x7a\x84\x8d\xce\xaf"
                "\x00\x53\xb6\x4a\xad\x50\xad\x8c\x6e\x34\xdf\x5f\xba"_cv
            },
        test_vector
            {
                .id = 31,
                .from =
                "ea0a52ed27ae820570d2561e04dd992347dfe2ff27f363a0d88104d1c4137ed5"
                "5e41b8c8a78730d9bcf25cb050314e19c657c622f4c399b2ca54fbb2361a"_cv,
                .to =
                "\xea\x0a\x52\xed\x27\xae\x82\x05\x70\xd2\x56\x1e\x04\xdd\x99\x23"
                "\x47\xdf\xe2\xff\x27\xf3\x63\xa0\xd8\x81\x04\xd1\xc4\x13\x7e\xd5"
                "\x5e\x41\xb8\xc8\xa7\x87\x30\xd9\xbc\xf2\x5c\xb0\x50\x31\x4e\x19"
                "\xc6\x57\xc6\x22\xf4\xc3\x99\xb2\xca\x54\xfb\xb2\x36\x1a"_cv
            },
        test_vector
            {
                .id = 32,
                .from =
                "72bde146bf94bdacda5bae481e60a44439d833e9061910e4ff39de6bc9500652"
                "39dd37358aa801f06ccd93f0baf997a45abee4e7c14d4334bca4f171b7c027"_cv,
                .to =
                "\x72\xbd\xe1\x46\xbf\x94\xbd\xac\xda\x5b\xae\x48\x1e\x60\xa4\x44"
                "\x39\xd8\x33\xe9\x06\x19\x10\xe4\xff\x39\xde\x6b\xc9\x50\x06\x52"
                "\x39\xdd\x37\x35\x8a\xa8\x01\xf0\x6c\xcd\x93\xf0\xba\xf9\x97\xa4"
                "\x5a\xbe\xe4\xe7\xc1\x4d\x43\x34\xbc\xa4\xf1\x71\xb7\xc0\x27"_cv
            },
        test_vector
            {
                .id = 33,
                .from =
                "7aacca05b53157b73c7831dea4fa708fc0312f62283a19c6cbde1a2c978a28e3"
                "cd99486b6982e57ffbb9c733a7e8eb5aba12115df2ae31d820ba73f0cc72ec1b"_cv,
                .to =
                "\x7a\xac\xca\x05\xb5\x31\x57\xb7\x3c\x78\x31\xde\xa4\xfa\x70\x8f"
                "\xc0\x31\x2f\x62\x28\x3a\x19\xc6\xcb\xde\x1a\x2c\x97\x8a\x28\xe3"
                "\xcd\x99\x48\x6b\x69\x82\xe5\x7f\xfb\xb9\xc7\x33\xa7\xe8\xeb\x5a"
                "\xba\x12\x11\x5d\xf2\xae\x31\xd8\x20\xba\x73\xf0\xcc\x72\xec\x1b"_cv
            },
        test_vector
            {
                .id = 34,
                .from =
                "7771009439d36baa2ccf8fd2a4ba2647de92fe884ae9bef520f12fff31c885f2"
                "4beb2fa249a9f9b25bd99eb57c094ecd98b145535d92d1cd2f69bfb4e290db1d"
                "f4"_cv,
                .to =
                "\x77\x71\x00\x94\x39\xd3\x6b\xaa\x2c\xcf\x8f\xd2\xa4\xba\x26\x47"
                "\xde\x92\xfe\x88\x4a\xe9\xbe\xf5\x20\xf1\x2f\xff\x31\xc8\x85\xf2"
                "\x4b\xeb\x2f\xa2\x49\xa9\xf9\xb2\x5b\xd9\x9e\xb5\x7c\x09\x4e\xcd"
                "\x98\xb1\x45\x53\x5d\x92\xd1\xcd\x2f\x69\xbf\xb4\xe2\x90\xdb\x1d"
                "\xf4"_cv
            },
        test_vector
            {
                .id = 35,
                .from =
                "dc7aad926e9e98893fc168bcd5217bc7c099fd3fe3ea8afefd460617dc0e2ab1"
                "dc527872fd9e04c0557aaeccf86c15d2b49411ff6f0d237d246571a22b788537"
                "7875"_cv,
                .to =
                "\xdc\x7a\xad\x92\x6e\x9e\x98\x89\x3f\xc1\x68\xbc\xd5\x21\x7b\xc7"
                "\xc0\x99\xfd\x3f\xe3\xea\x8a\xfe\xfd\x46\x06\x17\xdc\x0e\x2a\xb1"
                "\xdc\x52\x78\x72\xfd\x9e\x04\xc0\x55\x7a\xae\xcc\xf8\x6c\x15\xd2"
                "\xb4\x94\x11\xff\x6f\x0d\x23\x7d\x24\x65\x71\xa2\x2b\x78\x85\x37"
                "\x78\x75"_cv
            },
        test_vector
            {
                .id = 36,
                .from =
                "7cefadeab6fe33a196a9e9da8ccdd6e9c128aa6a333553c391d2da4360f67fbd"
                "4cb98053ec528275d4f39d9632d546ed46922d432fda7c8baec6e4fba32c70af"
                "1774a8"_cv,
                .to =
                "\x7c\xef\xad\xea\xb6\xfe\x33\xa1\x96\xa9\xe9\xda\x8c\xcd\xd6\xe9"
                "\xc1\x28\xaa\x6a\x33\x35\x53\xc3\x91\xd2\xda\x43\x60\xf6\x7f\xbd"
                "\x4c\xb9\x80\x53\xec\x52\x82\x75\xd4\xf3\x9d\x96\x32\xd5\x46\xed"
                "\x46\x92\x2d\x43\x2f\xda\x7c\x8b\xae\xc6\xe4\xfb\xa3\x2c\x70\xaf"
                "\x17\x74\xa8"_cv
            },
        test_vector
            {
                .id = 37,
                .from =
                "170990369d1c07af6b46322b86b7946020cbd01c4c39384ae6bf0c3787e5573e"
                "efd728783778f76ad22961d80299db81d384d3933d7199ddb4159dd0bf25bf43"
                "5ca3a689"_cv,
                .to =
                "\x17\x09\x90\x36\x9d\x1c\x07\xaf\x6b\x46\x32\x2b\x86\xb7\x94\x60"
                "\x20\xcb\xd0\x1c\x4c\x39\x38\x4a\xe6\xbf\x0c\x37\x87\xe5\x57\x3e"
                "\xef\xd7\x28\x78\x37\x78\xf7\x6a\xd2\x29\x61\xd8\x02\x99\xdb\x81"
                "\xd3\x84\xd3\x93\x3d\x71\x99\xdd\xb4\x15\x9d\xd0\xbf\x25\xbf\x43"
                "\x5c\xa3\xa6\x89"_cv
            },
        test_vector
            {
                .id = 38,
                .from =
                "d2ce3b9148a70897097bed9a37a3aa71e2aee516e94deef995a30de128f0e375"
                "545f14b0e537636581a5f88310aac7dc82adac96a839cdddb21a09436184c080"
                "5c0e831c90"_cv,
                .to =
                "\xd2\xce\x3b\x91\x48\xa7\x08\x97\x09\x7b\xed\x9a\x37\xa3\xaa\x71"
                "\xe2\xae\xe5\x16\xe9\x4d\xee\xf9\x95\xa3\x0d\xe1\x28\xf0\xe3\x75"
                "\x54\x5f\x14\xb0\xe5\x37\x63\x65\x81\xa5\xf8\x83\x10\xaa\xc7\xdc"
                "\x82\xad\xac\x96\xa8\x39\xcd\xdd\xb2\x1a\x09\x43\x61\x84\xc0\x80"
                "\x5c\x0e\x83\x1c\x90"_cv
            },
        test_vector
            {
                .id = 39,
                .from =
                "ca2975b86f5b59b7eb30fa03514077f492f36e5786d99936e917515fa2ba428b"
                "37c68ff5c97e08e629b759e004d69253f07150ced9b32d791214e61c2e889782"
                "be5b64b465b2"_cv,
                .to =
                "\xca\x29\x75\xb8\x6f\x5b\x59\xb7\xeb\x30\xfa\x03\x51\x40\x77\xf4"
                "\x92\xf3\x6e\x57\x86\xd9\x99\x36\xe9\x17\x51\x5f\xa2\xba\x42\x8b"
                "\x37\xc6\x8f\xf5\xc9\x7e\x08\xe6\x29\xb7\x59\xe0\x04\xd6\x92\x53"
                "\xf0\x71\x50\xce\xd9\xb3\x2d\x79\x12\x14\xe6\x1c\x2e\x88\x97\x82"
                "\xbe\x5b\x64\xb4\x65\xb2"_cv
            },
        test_vector
            {
                .id = 40,
                .from =
                "3d3de1323bfb9a0c0ef57b6d733b7f121817ddd7e3d2e36c5d2bcd8e988c9397"
                "740808e70db18d2e6671e0772b75cf383c6906f188619441fc205a66ad9ce004"
                "a83f1daf763387"_cv,
                .to =
                "\x3d\x3d\xe1\x32\x3b\xfb\x9a\x0c\x0e\xf5\x7b\x6d\x73\x3b\x7f\x12"
                "\x18\x17\xdd\xd7\xe3\xd2\xe3\x6c\x5d\x2b\xcd\x8e\x98\x8c\x93\x97"
                "\x74\x08\x08\xe7\x0d\xb1\x8d\x2e\x66\x71\xe0\x77\x2b\x75\xcf\x38"
                "\x3c\x69\x06\xf1\x88\x61\x94\x41\xfc\x20\x5a\x66\xad\x9c\xe0\x04"
                "\xa8\x3f\x1d\xaf\x76\x33\x87"_cv
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
        auto result = milo::codec::test<milo::codec::base_16>::decode(
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

