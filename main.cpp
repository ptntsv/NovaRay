#include <assert.h>

#include "src/utils/utilities.hpp"
#include "src/utils/world.hpp"

int main() {
    world w{};
    w.add_obj(new sphere{point3{0, 0, -1}, 1});
    w.render();
}
