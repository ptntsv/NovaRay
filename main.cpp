#include "src/demos.hpp"

int main() {
    switch (4) {
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
        default:;
    }
    return 0;
}
