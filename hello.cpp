#include "hello.h"
#include <iostream>

std::string printHello(const std::string &src) {
    std::string out = "Hello, " + src;
    return out;
}
