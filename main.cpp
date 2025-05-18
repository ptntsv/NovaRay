#include "src/demos.hpp"
#include "tests/benchmark_bhv.hpp"

int main() {
    switch (8) {
        case 1:
            bulky_demo();
            break;
        case 2:
            levitating();
            break;
        case 3:
            metals_demo();
            break;
        case 4:
            planes_demo();
            break;
        case 5:
            basic_perlin_demo();
            break;
        case 6:
            interpolated_perlin_demo();
            break;
        case 7:
            perlin_demo();
            break;
        case 8:
            simple_light();
            break;
        case 9: {
            BVHBenchmark benchmark;
            benchmark.run();
            break;
        }
        default:;
    }
    return 0;
}
