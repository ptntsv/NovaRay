#pragma once
#include <iostream>

struct fmt_printable {
    virtual ~fmt_printable() = default;
    virtual void fmt_print(int indent) const {
        std::cout << std::string(indent, ' ');
    }
};
