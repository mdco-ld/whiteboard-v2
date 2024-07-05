#ifndef _UTILS_H_
#define _UTILS_H_

#include <ostream>
#include <vector>
#if defined(DEBUG_MODE)

#include <iostream>

template <typename T>
std::ostream &operator<<(std::ostream &out, std::vector<T> &v) {
    if (v.empty()) {
        return out << "{}";
    }
    out << "{" << v.front();
    for (std::size_t i = 1; i < v.size(); i++) {
        out << ", " << v[i];
    }
    return out << "}";
}

template <typename T>
void _printd(const char *filename, int lineNumber, const char *thingName,
             T thing) {
    std::cerr << "[DEBUG] (" << filename << ":" << lineNumber << ") "
              << thingName << " = " << thing << std::endl;
}

#define PRINT_DBG(x) _printd(__FILE__, __LINE__, #x, x)

#define DEBUG_ONLY(...) __VA_ARGS__

#else

#define PRINT_DBG(x)
#define DEBUG_ONLY(...)

#endif

#endif
