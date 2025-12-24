#pragma once
#include "IDictionary\IDictionary.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <limits>

extern std::string text;
extern int page_size;
extern bool is_size_in_words;
extern IDictionary<std::string, int>* dictionary;

void Menu();
void EnterPageSize();
void EnterText();
std::vector<std::string> SplitStringToWords(const std::string& str);