#pragma once
#include <iostream>

void TestResultPrint(int result_of_test); 
// коды результата теста: 0 - успех теста, 1 - провал теста, 2 - неожиданная ошибка
int NextTest();
int ResetTestNumber();
int NextGroup();