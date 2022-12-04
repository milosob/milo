

#include <milo/concepts.h>
#include <milo/container.h>

#include <milo/inner/memory.h>


template<
    milo::concepts::integral t_stor,
    milo::concepts::integral t_data,
    size_t t_size
>
constexpr auto
test_case(
) noexcept(true)
{
    using stor_type = t_stor;
    using data_type = t_data;
    
    constexpr auto size = t_size;
    constexpr auto stor_size = sizeof(stor_type);
    constexpr auto data_size = sizeof(data_type);
    constexpr auto stors_size = size / stor_size;
    constexpr auto datas_size = size / data_size;
    
    static_assert(size / data_size < 256);
    static_assert(size % data_size == 0);
    
    milo::container::array<milo::container::array<data_type, datas_size>, (stor_size + 1 + 1) * 2> datas{};
    
    for (size_t i = 0, c = 0; i < stors_size; i += 1)
    {
        stor_type stor = 0;
        
        for (size_t j = 0; j < stor_size; j += 1)
        {
            if constexpr (stor_size > 1)
            {
                stor <<= 8;
            }
    
            stor |= stor_type(c & 0xff);
    
            c += 1;
        }
    
        milo::inner::memory_stor_le<stor_type>(
            datas[0].data(),
            i,
            stor
        );
    
        milo::inner::memory_stor_be<stor_type>(
            datas[1].data(),
            i,
            stor
        );
    
        for (size_t j = 0; j < stor_size + 1; j += 1)
        {
            milo::inner::memory_stor_le<stor_type>(
                datas[2 + j * 2].data(),
                i,
                stor,
                j
            );
        
            milo::inner::memory_stor_be<stor_type>(
                datas[2 + j * 2 + 1].data(),
                i,
                stor,
                j
            );
        }
    }
    
    return datas;
}

template<
    typename t_stor,
    typename t_data
>
auto
test_do(
) noexcept(false) -> void
{
    constexpr auto case_constexpr = test_case<t_stor, t_data, 32>();
    
    volatile auto stors_runtime_cb = test_case<t_stor, t_data, 32>;
    auto case_runtime = stors_runtime_cb();
    
    if (case_constexpr != case_runtime)
    {
        throw int(sizeof(t_stor) << 8 | sizeof(t_data));
    }
}

auto
test(
) noexcept(true) -> size_t
{
    try
    {
        test_do<uint8_t, uint8_t>();
        test_do<uint8_t, uint16_t>();
        test_do<uint8_t, uint32_t>();
        test_do<uint8_t, uint64_t>();
        
        test_do<uint16_t, uint8_t>();
        test_do<uint16_t, uint16_t>();
        test_do<uint16_t, uint32_t>();
        test_do<uint16_t, uint64_t>();
        
        test_do<uint32_t, uint8_t>();
        test_do<uint32_t, uint16_t>();
        test_do<uint32_t, uint32_t>();
        test_do<uint32_t, uint64_t>();
        
        test_do<uint64_t, uint8_t>();
        test_do<uint64_t, uint16_t>();
        test_do<uint64_t, uint32_t>();
        test_do<uint64_t, uint64_t>();
    }
    catch (int error)
    {
        return error;
    }
    
    return 0;
}

auto
main(
) noexcept(false) -> int
{
    return test() > 0;
}
