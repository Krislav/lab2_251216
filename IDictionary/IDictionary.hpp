#pragma once
#include "..\Red-Black Tree\RedBlackTree.hpp"
#include "..\Errors\Errors.hpp"

template <class Tkey, class TElement>
class IDictionary {
private:
    RedBlackTree<Tkey, TElement> tree;
    int count;

public:
    IDictionary() : count(0) {}
    ~IDictionary() = default;
    
    int GetCount() {
        return count;
    }
    
    TElement Get(Tkey key) {
        TElement* result = tree.Search(key);
        if (result == nullptr) {
            throw ErrorCode::KEY_NOT_FOUND;
        }
        return *result;
    }
    
    bool ContainsKey(Tkey key) {
        return tree.Contains(key);
    }
    
    void Add(Tkey key, TElement element) {
        if (!ContainsKey(key)) {
            count++;
        }
        tree.Insert(key, element);
    }
    
    void Remove(Tkey key) {
        if (ContainsKey(key)) {
            count--;
            tree.Remove(key);
        } else {
            throw ErrorCode::KEY_NOT_FOUND;
        }
    }

    std::vector<std::pair<Tkey, TElement>> GetFullDictionary() { //Возвращает вектор содержащий элементы словоря по возрастанию ключей
        return tree.InOrderTraversal();
    }
};