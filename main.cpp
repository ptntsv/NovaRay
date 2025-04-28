#include "src/demos.hpp"

int main() {
    switch (7) {
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
            perlin_demo();
            break;
        case 6:
            quads();
            break;
        case 7:
            boxes_demo();
            break;
        default:;
    }
    return 0;
}
