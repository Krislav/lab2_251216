#include "Errors.hpp"

void ErrorsOutput(ErrorCode error) {
    switch (error) {
    case ErrorCode::OK:
        std::cout << "Successful completion\n";
        break;
    case ErrorCode::KEY_NOT_FOUND:
        std::cout << "Key not found\n";
        break;
    
    default:
        std::cout << "Unknown error\n";
        break;
    }
}