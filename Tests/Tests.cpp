#include "Tests.hpp"

void TestRedBlackTree(){ //GROUP 1 тесты методов красно-черного дерева
    ResetTestNumber();
    int result_of_test = 1;

    result_of_test = 1; //TEST 1.1 Проверка пустого дерева
    RedBlackTree<std::string, int> tree1_1;
    try {
        if (tree1_1.IsEmpty()) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 0; //TEST 1.2 Проверка вставки
    try {
        tree1_1.Insert("word", 1);
    }
    catch(ErrorCode ex) {
        result_of_test = 1;
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.3 Проверка Contains
    try {
        if (tree1_1.Contains("word")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 1.4 Проверка Search
    try {
        if (*tree1_1.Search("word") == 1) result_of_test = 0;
    }
    catch(ErrorCode ex) {
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 0; //TEST 1.5 Проверка InOrderTraversal
    try {
        tree1_1.Insert("abra", 1);
        tree1_1.Insert("kadabra", 2);
        tree1_1.Insert("boom", 1);
        std::vector<std::pair<std::string, int>> check = tree1_1.InOrderTraversal();
    }
    catch(ErrorCode ex) {
    }
    TestResultPrint(result_of_test);
    NextTest();
}

void TestIDictionary(){ //GROUP 2 тесты методов IDictionary
    NextGroup();
    ResetTestNumber();
    int result_of_test = 1;

    result_of_test = 1; //TEST 2.1 Проверка пустого
    IDictionary<std::string, int> dic1_1;
    try {
        if (dic1_1.GetCount() == 0) result_of_test = 0;
    }
    catch(ErrorCode ex) {
        ErrorsOutput(ex);
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 0; //TEST 2.2 Проверка вставки
    try {
        dic1_1.Add("word", 1);
    }
    catch(ErrorCode ex) {
        result_of_test = 1;
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 2.3 Проверка ContainsKey
    try {
        if (dic1_1.ContainsKey("word")) result_of_test = 0;
    }
    catch(ErrorCode ex) {
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 1; //TEST 2.4 Проверка Get
    try {
        if (dic1_1.Get("word") == 1) result_of_test = 0;
    }
    catch(ErrorCode ex) {
    }
    TestResultPrint(result_of_test);
    NextTest();

    result_of_test = 0; //TEST 2.5 Проверка GetFullDictionary
    try {
        dic1_1.Add("abra", 1);
        dic1_1.Add("kadabra", 2);
        dic1_1.Add("boom", 1);
        std::vector<std::pair<std::string, int>> check = dic1_1.GetFullDictionary();
    }
    catch(ErrorCode ex) {
    }
    TestResultPrint(result_of_test);
    NextTest();
}

void TestAll() {
    TestRedBlackTree();
    TestIDictionary();
}