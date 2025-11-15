#pragma once
#include <iostream>
#include <string>


class CustomAssert {
public:
    static void handle_failure(const std::string& expression, 
                              const std::string& message, 
                              const std::string& file, 
                              int line, 
                              const std::string& function) {
        std::cerr << "=== ASSERT FAILED! === " << std::endl << std::endl;
        std::cerr << expression << std::endl;
        if (!message.empty()) {
            std::cerr << "Message: " << message << std::endl;
        }
        std::cerr << "File: " << file << ", Line: " << line << ", Function: " << function << std::endl;

		std::abort();
    }
};

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            CustomAssert::handle_failure(#condition, message, __FILE__, __LINE__, __func__); \
        } \
    } while(0)

