#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <iostream>
#include <random>

#include <benchmark/benchmark.h>

namespace bm = benchmark;


static void bm_add(bm::State &state) {
    int32_t a = std::rand(), b = std::rand(), c = 0;
    int32_t i = -1;
    for (auto _ : state) {
      i *= -1;
      c = a + i * b;
    }
}

BENCHMARK(bm_add);


// Extend BENCHMARK_MAIN() with more system info

int main(int argc, char **argv) {
    // Set defaults
    char arg0_default[] = "benchmark";
    char *args_default = arg0_default;
    if (!argv) argc = 1, argv = &args_default;
    bm::Initialize(&argc, argv);
    if (bm::ReportUnrecognizedArguments(argc, argv)) return 1;
    bm::RunSpecifiedBenchmarks();
    bm::Shutdown();

    return 0;
}
