#include <assert.h>

#include "src/utils/camera.hpp"
#include "src/utils/utilities.hpp"
#include "src/utils/world.hpp"

int main() {
    hittable_list objlist;
    camera cam{};
    objlist.add(new sphere{point3{0, 0, -1}, 0.5});
    objlist.add(new sphere{point3{0, -101, -1}, 100});
    cam.render(&objlist);
}
