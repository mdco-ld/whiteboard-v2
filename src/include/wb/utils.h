#ifndef _UTILS_H_
#define _UTILS_H_

#if defined(DEBUG_MODE)

#include <iostream>

template <typename T>
void _printd(const char *filename, int lineNumber, const char *thingName,
             T thing) {
    std::cerr << "[DEBUG] (" << filename << ":" << lineNumber << ") "
              << thingName << " = " << thing << std::endl;
}

#define PRINT_DBG(x) _printd(__FILE__, __LINE__, #x, x)

#else

#define PRINT_DBG(x)

#endif

#endif