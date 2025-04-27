#pragma once

struct fmt_printable {
    virtual ~fmt_printable() = default;
    virtual void fmt_print(int indent) const {
        std::cout << std::string(indent, ' ');
    }
};