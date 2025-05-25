#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include "utils/color.hpp"

class GaussianBlur {
public:
    static std::vector<color> apply(const std::vector<color>& img, int width, int height, double sigma = 5.0, int kernel_size = 5) {
        std::vector<color> result(width * height);
        std::vector<double> kernel(kernel_size);
        double sum = 0.0;
        int radius = kernel_size / 2;

        for (int i = 0; i < kernel_size; ++i) {
            int x = i - radius;
            kernel[i] = exp(-(x * x) / (2 * sigma * sigma));
            sum += kernel[i];
        }

        for (double& k : kernel) k /= sum;

        std::vector<color> temp(width * height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                color sum_color(0);
                double weight_sum = 0.0;

                for (int k = 0; k < kernel_size; ++k) {
                    int px = x + k - radius;
                    if (px < 0 || px >= width) continue;

                    sum_color += img[y * width + px] * kernel[k];
                    weight_sum += kernel[k];
                }
                temp[y * width + x] = sum_color / weight_sum;
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                color sum_color(0);
                double weight_sum = 0.0;

                for (int k = 0; k < kernel_size; ++k) {
                    int py = y + k - radius;
                    if (py < 0 || py >= height) continue;

                    sum_color += temp[py * width + x] * kernel[k];
                    weight_sum += kernel[k];
                }
                result[y * width + x] = sum_color / weight_sum;
            }
        }
        return result;
    }
};
