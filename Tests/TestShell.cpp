#include "TestShell.hpp"
int test_group_number = 1;
int test_number = 1;

void TestResultPrint(int result_of_test){
    switch (result_of_test)
    {
    case 0:
        std::cout << "Test " << test_group_number << "." << test_number << " passed [O]\n";
        break;
    case 1:
        std::cout << "Test " << test_group_number << "." << test_number << " failed [X]\n";
        break;
    case 2:
        std::cout << "Test " << test_group_number << "." << test_number << " ended with unexpected error [?]\n";
        break;
    default:
        std::cout << "Test " << test_group_number << "." << test_number << " uncorrect\n";
        break;
    }
}

int NextTest() {
    test_number++;
    return test_number;
}

int ResetTestNumber(){
    test_number = 1;
    return test_number;
}

int NextGroup() {
    test_group_number++;
    return test_group_number;
}