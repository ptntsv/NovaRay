#pragma once

#include "quad.h"

class box {
    std::vector<quad*> quads;

public:
    ~box() {
        for (auto& q : quads) delete q;
    }
    box(const point3& p0, const point3& p1, material* mat) {
    }
};
