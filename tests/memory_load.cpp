

#include <milo/concepts.h>
#include <milo/container.h>
#include <milo/memory.h>


template<
    milo::concepts::integral t_load,
    milo::concepts::integral t_data,
    size_t t_size
>
constexpr auto
test_case(
) noexcept(true)
{
    using load_type = t_load;
    using data_type = t_data;
    
    constexpr auto size = t_size;
    constexpr auto load_size = sizeof(load_type);
    constexpr auto data_size = sizeof(data_type);
    constexpr auto loads_size = size / load_size;
    constexpr auto datas_size = size / data_size;
    
    static_assert(size / data_size < 256);
    static_assert(size % data_size == 0);
    
    milo::container::array<load_type, loads_size * (load_size + 1 + 1) * 2> loads{};
    milo::container::array<data_type, datas_size> datas{};
    
    for (size_t i = 0, c = 0; i < datas_size; i += 1)
    {
        data_type data = 0;
        
        for (size_t j = 0; j < data_size; j += 1)
        {
            if constexpr (data_size > 1)
            {
                data <<= 8;
            }
            
            data |= data_type(c & 0xff);
            
            c += 1;
        }
        
        datas[i] = milo::detail::endian_bigof(data);
    }
    
    for (size_t i = 0, c = 0; i < loads_size; i += 1)
    {
        loads[c] = milo::memory::load_le<load_type>(
            datas.data(),
            i
        );
    
        c += 1;
    
        loads[c] = milo::memory::load_be<load_type>(
            datas.data(),
            i
        );
    
        c += 1;
    
        for (size_t j = 0; j < load_size + 1; j += 1)
        {
            loads[c] = milo::memory::load_le<load_type>(
                datas.data(),
                i,
                j
            );
        
            c += 1;
        
            loads[c] = milo::memory::load_be<load_type>(
                datas.data(),
                i,
                j
            );
            
            c += 1;
        }
    }
    
    return loads;
}

template<
    typename t_load,
    typename t_data
>
auto
test_do(
) noexcept(false) -> void
{
    constexpr auto case_constexpr = test_case<t_load, t_data, 32>();
    
    volatile auto loads_runtime_cb = test_case<t_load, t_data, 32>;
    auto case_runtime = loads_runtime_cb();
    
    if (case_constexpr != case_runtime)
    {
        throw int(sizeof(t_load) << 8 | sizeof(t_data));
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
