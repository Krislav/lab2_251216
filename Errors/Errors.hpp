#pragma once
#include <iostream>

enum class ErrorCode {
    OK,
    KEY_NOT_FOUND
};

void ErrorsOutput(ErrorCode error);