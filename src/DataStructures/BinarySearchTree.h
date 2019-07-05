// Copyright 2019 Cosmin-Razvan Vancea

#ifndef DATASTRUCTURES_BINARYSEARCHTREE_H_
#define DATASTRUCTURES_BINARYSEARCHTREE_H_

#include <vector>
#include <functional>

template <class T, class Compare = std::less<T>>
class BinarySearchTree
{
    struct Node {
        explicit Node(const T& data) : data(data) {
            left = right = nullptr;
        }
        Node* left;
        Node* right;
        T data;
    };

    Node* m_Root;
    size_t m_Size;
    Compare m_CmpFn;

 public:
    BinarySearchTree() {
        m_Root = nullptr;
        m_Size = 0;
    }
    ~BinarySearchTree() {
        cleanUp(m_Root);
    }

    BinarySearchTree(const BinarySearchTree&) = delete;
    void operator= (const BinarySearchTree&) = delete;

 private:
    bool insert(const T& x, Node*& root) {
        if (root == nullptr) {
            root = new Node(x);
            m_Size++;
            return 1;
        } else if (m_CmpFn(x, root->data)) {
            return insert(x, root->left);
        } else if (m_CmpFn(root->data, x)) {
            return insert(x, root->right);
        }
        // nu permit duplicate
        return 0;
    }

    Node* find(const T& x, Node* root) {
        if (root == nullptr) {
            return nullptr;
        }

        if (m_CmpFn(x, root->data)) {
            return find(x, root->left);
        } else if (m_CmpFn(root->data, x)) {
            return find(x, root->right);
        }
        return root;
    }

    bool erase(const T& x, Node*& root) {
        if (root == nullptr) {
            return 0;
        } else if (m_CmpFn(x, root->data)) {
            return erase(x, root->left);
        } else if (m_CmpFn(root->data, x)) {
            return erase(x, root->right);
        } else if (root->left && root->right) {
            Node* min_right = findMinNode(root->right);
            root->data = min_right->data;
            return erase(root->data, root->right);
        } else {
            Node* old_root = root;
            if (root->left == nullptr) {
                root = root->right;
            } else if (root->right == nullptr) {
                root = root->left;
            }

            m_Size--;
            delete old_root;

            return 1;
        }
    }

    Node* findMinNode(Node* root) {
        if (root == nullptr) {
            return nullptr;
        }

        while (root->left != nullptr) {
            root = root->left;
        }
        return root;
    }
    Node* findMaxNode(Node* root) {
        if (root == nullptr) {
            return nullptr;
        }

        while (root->right != nullptr) {
            root = root->right;
        }
        return root;
    }

    void cleanUp(Node* node) {
        if (node == nullptr) {
            return;
        }
        if (node->left) {
            cleanUp(node->left);
        }
        if (node->right) {
            cleanUp(node->right);
        }
        delete node;
    }

 public:
    bool isEmpty() const {
        return (m_Size == 0);
    }

    size_t getSize() const {
        return m_Size;
    }

    bool insert(const T& x) {
        return insert(x, m_Root);
    }
    int insert(const std::vector<T>& keys) {
        int failCount = 0;
        for (const auto& key : keys) {
            failCount += (insert(key) == 0);
        }
        return failCount;
    }

    bool exists(const T& x) {
        return find(x, m_Root) != nullptr;
    }

    bool erase(const T& x) {
        return erase(x, m_Root);
    }

    T findMin() {
        Node* node = findMinNode(m_Root);
        if (node) {
            return node->data;
        }
        return T();
    }

    T findMax() {
        Node* node = findMaxNode(m_Root);
        if (node) {
            return node->data;
        }
        return T();
    }

    std::vector<T> inOrderTraverse() {
        std::vector<T> res;
        res.reserve(m_Size);

        inOrderHelper(m_Root, res);
        return res;
    }

 private:
    void inOrderHelper(Node *root, std::vector<T>& res) {
        if (root == nullptr) {
            return;
        }
        inOrderHelper(root->left, res);
        res.push_back(root->data);
        inOrderHelper(root->right, res);
    }
};

#endif  // DATASTRUCTURES_BINARYSEARCHTREE_H_
