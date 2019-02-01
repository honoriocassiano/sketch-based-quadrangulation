#ifndef UTILS_H
#define UTILS_H

#include <string>

struct Status {
    bool result;
    std::string message;

    operator bool() const { return result; }
};

static const Status STATUS_OK = {true, "OK!"};

#endif // UTILS_H
