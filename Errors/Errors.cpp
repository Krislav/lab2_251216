#include "Errors.hpp"

void ErrorsOutput(ErrorCode error) {
    switch (error) {
    case ErrorCode::OK:
        std::cout << "Successful completion\n";
        break;
    case ErrorCode::EMPTY_NAME:
        std::cout << "File name not specified\n";
        break;
    case ErrorCode::COULDNT_OPEN_FILE:
        std::cout << "File can't be opened\n";
        break;
    case ErrorCode::FILE_ISNT_OPENED:
        std::cout << "File is clsoed\n";
        break;
    case ErrorCode::INDEX_OUT_OF_RANGE:
        std::cout << "Index out of range\n";
        break;
    case ErrorCode::SEEK_FAIL:
        std::cout << "Seek failed\n";
        break;
    case ErrorCode::WRITE_FAILED:
        std::cout << "Write failed\n";
        break;
    
    default:
        std::cout << "Unknown error\n";
        break;
    }
}