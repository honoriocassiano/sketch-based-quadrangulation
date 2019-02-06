#ifndef UTILS_H
#define UTILS_H

#include <string>

struct Status {
    bool result;
    std::string message;

    static Status make(bool r, std::string m) { return Status{r, m}; }

    operator bool() const { return result; }
};

static const Status STATUS_OK = {true, "OK!"};

#endif // UTILS_H
