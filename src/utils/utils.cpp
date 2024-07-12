#include "tsompcc/utils.h"
#include <iostream>

namespace tsompcc {

void printError(const char* message) {
    std::cerr << "Error: " << message << std::endl;
}

} // namespace tsompcc