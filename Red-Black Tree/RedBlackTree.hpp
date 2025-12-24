#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include "..\Errors\Errors.hpp"

template <class Tkey, class TElement>
class RedBlackTree {
public:
    struct Node {
        Tkey key;
        TElement data;
        bool color; // false - черный, true - красный
        Node* left;
        Node* right;
        Node* parent;

        Node(const Tkey& k, const TElement& v, Node* par = nullptr, bool col = true) 
            : key(k), data(v), color(col), left(nullptr), right(nullptr), parent(par) {}
    };

private:
    Node* root;
    Node* nil;

    void leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != nil) y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != nil) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr) root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node* k) {
        Node* u;
        while (k->parent != nullptr && k->parent->color == true) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u != nil && u->color == true) {
                    u->color = false;
                    k->parent->color = false;
                    k->parent->parent->color = true;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = false;
                    k->parent->parent->color = true;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;
                if (u != nil && u->color == true) {
                    u->color = false;
                    k->parent->color = false;
                    k->parent->parent->color = true;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = false;
                    k->parent->parent->color = true;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) break;
        }
        root->color = false;
    }

    void fixDelete(Node* x) {
        Node* s;
        while (x != root && x->color == false) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == true) {
                    s->color = false;
                    x->parent->color = true;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == false && s->right->color == false) {
                    s->color = true;
                    x = x->parent;
                } else {
                    if (s->right->color == false) {
                        s->left->color = false;
                        s->color = true;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    s->color = x->parent->color;
                    x->parent->color = false;
                    s->right->color = false;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == true) {
                    s->color = false;
                    x->parent->color = true;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == false && s->left->color == false) {
                    s->color = true;
                    x = x->parent;
                } else {
                    if (s->left->color == false) {
                        s->right->color = false;
                        s->color = true;
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    s->color = x->parent->color;
                    x->parent->color = false;
                    s->left->color = false;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = false;
    }

    void rbTransplant(Node* u, Node* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    Node* minimum(Node* node) {
        while (node->left != nil) node = node->left;
        return node;
    }

    void deleteNode(Node*& root, const Tkey& key) {
        Node* z = root;
        Node* x;
        Node* y;
        
        while (z != nil && z->key != key) {
            if (key < z->key) z = z->left;
            else z = z->right;
        }
        
        if (z == nil) return;

        y = z;
        bool yOriginalColor = y->color;
        
        if (z->left == nil) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == nil) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            
            if (y->parent == z) {
                if (x != nil) x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            
            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        
        delete z;
        
        if (yOriginalColor == false) {
            fixDelete(x);
        }
        
        if (x == nil) {
            Node* newRoot = root;
            while (newRoot->parent != nil) newRoot = newRoot->parent;
            root = newRoot;
        }
    }

    void clearTree(Node* node) {
        if (node != nil) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    Node* copyTree(Node* node, Node* parent) {
        if (node == nil) return nil;
        Node* newNode = new Node(node->key, node->data, parent, node->color);
        newNode->left = copyTree(node->left, newNode);
        newNode->right = copyTree(node->right, newNode);
        return newNode;
    }

    void InOrderTraversalHelper(Node* node, std::vector<std::pair<Tkey, TElement>>& result) const {
        if (node != nil) {
            InOrderTraversalHelper(node->left, result);
            result.emplace_back(node->key, node->data);
            InOrderTraversalHelper(node->right, result);
        }
    }

public:
    RedBlackTree() {
        nil = new Node(Tkey(), TElement(), nullptr, false);
        root = nil;
    }

    ~RedBlackTree() {
        clearTree(root);
        delete nil;
    }

    RedBlackTree(const RedBlackTree& other) {
        nil = new Node(Tkey(), TElement(), nullptr, false);
        root = copyTree(other.root, nullptr);
        if (root != nil) root->parent = nullptr;
    }

    void Insert(const Tkey& key, const TElement& value) {
        Node* newNode = new Node(key, value, nullptr, true);
        newNode->left = nil;
        newNode->right = nil;

        Node* y = nullptr;
        Node* x = root;

        while (x != nil) {
            y = x;
            if (newNode->key < x->key) x = x->left;
            else if (newNode->key > x->key) x = x->right;
            else {
                x->data = value;
                delete newNode;
                return;
            }
        }

        newNode->parent = y;
        if (y == nullptr) root = newNode;
        else if (newNode->key < y->key) y->left = newNode;
        else y->right = newNode;

        if (newNode->parent == nullptr) {
            newNode->color = false;
            return;
        }

        if (newNode->parent->parent == nullptr) return;

        fixInsert(newNode);
    }

    void Remove(const Tkey& key) {
        deleteNode(root, key);
    }

    TElement* Search(const Tkey& key) {
        Node* current = root;
        while (current != nil) {
            if (key == current->key) return &(current->data);
            else if (key < current->key) current = current->left;
            else current = current->right;
        }
        return nullptr;
    }

    bool Contains(const Tkey& key) {
        return Search(key) != nullptr;
    }

    Node* GetRoot() const {
        return (root != nil) ? root : nullptr;
    }

    bool IsEmpty() const {
        return root == nil;
    }

    std::vector<std::pair<Tkey, TElement>> InOrderTraversal() const {
        std::vector<std::pair<Tkey, TElement>> result;
        InOrderTraversalHelper(root, result);
        return result;
    }
};