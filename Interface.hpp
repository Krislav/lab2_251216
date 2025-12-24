#pragma once
#include "IDictionary\IDictionary.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

void Menu();
void EnterPageSize();
void EnterText();
std::vector<std::string> SplitStringToWords(const std::string& str);