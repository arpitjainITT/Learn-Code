#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class BookNotFoundException : public std::runtime_error {
public:
    explicit BookNotFoundException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif
