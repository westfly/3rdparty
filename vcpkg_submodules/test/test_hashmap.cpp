#include <benchmark/benchmark.h>
#include <cstring>
#include <fmt/format.h>

static void BM_memcpy(benchmark::State& state) {
    char* src = new char[state.range(0)];
    char* dst = new char[state.range(0)];
    memset(src, 'x', state.range(0));
    for (auto _ : state)
        memcpy(dst, src, state.range(0));
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
    delete[] src;
    delete[] dst;
}
static int snprintf(int num) {
    char buffer[16];
    return snprintf(buffer, sizeof(buffer), "%d", num);
}
static void BM_snprintf(benchmark::State& state) {
    int i  = 0;
    for (auto _ : state) {
        fmt::print("xxx {}", i);
        snprintf(++i);
    }
}

constexpr auto my_pow(const int i)
{
    return i * i;
}

// 使用operator[]读取元素，依次存入1-6的平方
static void bench_array_operator(benchmark::State& state)
{
    constexpr int len = 6;
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _: state) {
        arr[0] = my_pow(i);
        arr[1] = my_pow(i+1);
        arr[2] = my_pow(i+2);
        arr[3] = my_pow(i+3);
        arr[4] = my_pow(i+4);
        arr[5] = my_pow(i+5);
    }
}
BENCHMARK(bench_array_operator);
// Register the function as a benchmark
BENCHMARK(BM_snprintf)->Iterations(1000);
BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1<<10)->Arg(8<<10);
BENCHMARK_MAIN();