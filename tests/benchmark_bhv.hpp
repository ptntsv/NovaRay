#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <memory>
#include "../src/hittable_list.hpp"
#include "../src/hittable.hpp"
#include "../src/bvh.hpp"
#include "../src/material.hpp"
#include "../src/primitives/sphere.hpp"

class BVHBenchmark {
    hittable_list world;

public:
    void run() {
        std::vector<size_t> scene_sizes = {100, 1000, 5000, 10000, 20000};

        for (size_t size : scene_sizes) {
            std::cout << "\n=== Benchmark for scene with " << size << " objects ===\n";
            create_random_scene(size);

            // without BVH
            auto start = std::chrono::high_resolution_clock::now();
            render_without_bvh();
            auto end = std::chrono::high_resolution_clock::now();
            auto witout_bvh_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            // with BVH
            start = std::chrono::high_resolution_clock::now();
            auto* bvh = new bvh_node(world.items());
            end = std::chrono::high_resolution_clock::now();
            auto build_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            render_with_bhv(*bvh);
            end = std::chrono::high_resolution_clock::now();
            auto bvh_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            std::cout << "Without BVH render time: " << witout_bvh_time << " ms\n";
            std::cout << "BVH build time: " << build_time << " ms\n";
            std::cout << "BVH render time: " << bvh_time << " ms\n";
            std::cout << "Total BVH time: " << (build_time + bvh_time) << " ms\n";
            std::cout << "Speedup (including build): "
                      << (double)witout_bvh_time / (build_time + bvh_time) << "x\n";
            delete bvh;
            world = hittable_list();
        }
    }

private:
    void render_without_bvh() {
        std::vector<ray> test_rays = generate_rays(1000);

        for (const auto& r : test_rays) {
            hit_record rec;
            for (auto obj : world.items()) {
                obj->hit(r, interval(0.001, 1000), rec);
            }
        }
    }

    void render_with_bhv(const bvh_node& bvh) {
        std::vector<ray> test_rays = generate_rays(1000);

        for (const auto& r : test_rays) {
            hit_record rec;
            bvh.hit(r, interval(0.001, 1000), rec);
        }
    }

    std::vector<ray> generate_rays(size_t count) {
        std::vector<ray> rays;
        for (size_t i = 0; i < count; ++i) {
            point3 start = vec::random(-10, 10);
            point3 end = vec::random(-10, 10);
            rays.emplace_back(start, end - start);
        }
        return rays;
    }

    void create_random_scene(size_t count) {
        auto material = std::make_shared<lambertian_reflectance>(color(0.5, 0.5, 0.5));
        for (size_t i = 0; i < count; ++i) {
            point3 center = vec::random(-10, 10);
            double radius = utility::random_double(0.1, 2.0);
            auto* sphere = new ::sphere(center, radius, material);
            world.add(sphere);
        }
    }
};
