#include <gtest/gtest.h>

#include "../src/utils/sphere.hpp"
#include "../src/world.hpp"

TEST(BvhTest, PrimitiveHitbox) {
    sphere* s1{new sphere(point3{-1, 0, -1}, 5, nullptr)};
    EXPECT_EQ(s1->hitbox.xs(), interval(-6, 4));
    EXPECT_EQ(s1->hitbox.ys(), interval(-5, 5));
    EXPECT_EQ(s1->hitbox.zs(), interval(-6, 4));
    delete s1;
}
TEST(BvhTest, OverlappingSpheresBox) {
    sphere* s1{new sphere(point3{-2, 0, -4}, 5, nullptr)};
    sphere* s2{new sphere(point3{0, 0, 0}, 5, nullptr)};
    hittable_list list;
    list.add(s1);
    EXPECT_EQ(list.hitbox,
              aabb(interval(-7, 3), interval(-5, 5), interval(-9, 1)));
    list.add(s2);
    EXPECT_EQ(list.hitbox,
              aabb(interval(-7, 5), interval(-5, 5), interval(-9, 5)));
}

TEST(BvhTest, NotOverlappingSpheresBox) {
    sphere* s1{new sphere(point3{-2, 0, -4}, 5, nullptr)};
    sphere* s2{new sphere(point3{10, 10, 10}, 5, nullptr)};
    hittable_list list;
    list.add(s1);
    list.add(s2);
    EXPECT_EQ(list.hitbox,
              aabb(interval(-8, 15), interval(-5, 15), interval(-9, 15)));
}
