#include <iostream>
#include <stdexcept>

void fatalError(const std::string& message)
{
    std::cerr << message << std::endl;
    throw std::runtime_error("Fatal error.");
}
