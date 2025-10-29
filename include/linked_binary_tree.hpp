// linked_binary_tree.hpp
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

template <typename E>
class LinkedBinaryTree {
protected:
    class Node {
    public:
        E elem;
        Node* par{nullptr};
        Node* left{nullptr};
        Node* right{nullptr};

        Node(const E& e = E(), Node* p = nullptr)
            : elem{e}, par{p}, left{nullptr}, right{nullptr} {}
    }; // end of Node class

protected:
    Node* rt{nullptr}; // root
    int sz{0};         // size

public:
    class Position {
    private:
        Node* node; // pointer to the node being wrapped
    public:
        Position(Node* nd = nullptr) : node{nd} {}

        bool operator==(Position other) const {
            return node == other.node;
        }
        bool operator!=(Position other) const {
            return node != other.node;
        }
        bool is_null() const {
            return node == nullptr;
        }
        bool is_root() const {
            return node && node->par == nullptr;
        }
        bool is_external() const {
            return node && node->left == nullptr && node->right == nullptr;
        }
        E& element() {
            return node->elem;
        }
        const E& element() const {
            return node->elem;
        }

        Position parent() const {
            return Position(node ? node->par : nullptr);
        }
        Position left() const {
            return Position(node ? node->left : nullptr);
        }
        Position right() const {
            return Position(node ? node->right : nullptr);
        }

        std::vector<Position> children() const {
            std::vector<Position> v;
            if (node) {
                if (node->left) v.push_back(Position(node->left));
                if (node->right) v.push_back(Position(node->right));
            }
            return v;
        }

        int num_children() const {
            int count = 0;
            if (node && node->left) ++count;
            if (node && node->right) ++count;
            return count;
        }
    }; // end of Position class

    LinkedBinaryTree() = default; // tree with zero nodes

    int size() const {
        return sz;
    }
    bool empty() const {
        return sz == 0;
    }
    Position root() const {
        return Position(rt);
    }

    // preorder traversal helper
private:
    void preorder(Node* n, std::vector<Position>& v) const {
        if (n == nullptr) return;
        v.push_back(Position(n));
        preorder(n->left, v);
        preorder(n->right, v);
    }

public:
    std::vector<Position> positions() const {
        std::vector<Position> v;
        preorder(rt, v);
        return v;
    }

    void add_root(const E& e = E()) {
        if (rt != nullptr)
            throw std::runtime_error("Root already exists");
        rt = new Node(e);
        sz = 1;
    }

    void add_left(Position p, const E& e) {
        Node* n = p.node;
        if (!n)
            throw std::runtime_error("Invalid position");
        if (n->left != nullptr)
            throw std::runtime_error("Left child already exists");
        n->left = new Node(e, n);
        ++sz;
    }

    void add_right(Position p, const E& e) {
        Node* n = p.node;
        if (!n)
            throw std::runtime_error("Invalid position");
        if (n->right != nullptr)
            throw std::runtime_error("Right child already exists");
        n->right = new Node(e, n);
        ++sz;
    }

    void erase(Position p) {
        Node* n = p.node;
        if (!n)
            throw std::runtime_error("Invalid position");
        if (n->left && n->right)
            throw std::runtime_error("Cannot erase node with two children");

        Node* child = (n->left ? n->left : n->right);
        if (child)
            child->par = n->par;

        if (n == rt)
            rt = child;
        else {
            Node* parent = n->par;
            if (parent->left == n)
                parent->left = child;
            else
                parent->right = child;
        }

        delete n;
        --sz;
    }

    void attach(Position p, LinkedBinaryTree& left, LinkedBinaryTree& right) {
        Node* n = p.node;
        if (!n)
            throw std::runtime_error("Invalid position");
        if (n->left || n->right)
            throw std::runtime_error("Node already has children");

        if (!left.empty()) {
            n->left = left.rt;
            left.rt->par = n;
            sz += left.sz;
            left.rt = nullptr;
            left.sz = 0;
        }

        if (!right.empty()) {
            n->right = right.rt;
            right.rt->par = n;
            sz += right.sz;
            right.rt = nullptr;
            right.sz = 0;
        }
    }

private:
    void tear_down(Node* nd) {
        if (nd == nullptr) return;
        tear_down(nd->left);
        tear_down(nd->right);
        delete nd;
    }

    static Node* clone(Node* model) {
        if (!model) return nullptr;
        Node* new_node = new Node(model->elem);
        if (model->left) {
            new_node->left = clone(model->left);
            new_node->left->par = new_node;
        }
        if (model->right) {
            new_node->right = clone(model->right);
            new_node->right->par = new_node;
        }
        return new_node;
    }

public:
    ~LinkedBinaryTree() {
        tear_down(rt);
    }

    LinkedBinaryTree(const LinkedBinaryTree& other) {
        rt = clone(other.rt);
        sz = other.sz;
    }

    LinkedBinaryTree& operator=(const LinkedBinaryTree& other) {
        if (this != &other) {
            tear_down(rt);
            rt = clone(other.rt);
            sz = other.sz;
        }
        return *this;
    }

    LinkedBinaryTree(LinkedBinaryTree&& other) {
        rt = other.rt;
        sz = other.sz;
        other.rt = nullptr;
        other.sz = 0;
    }

    LinkedBinaryTree& operator=(LinkedBinaryTree&& other) {
        if (this != &other) {
            tear_down(rt);
            rt = other.rt;
            sz = other.sz;
            other.rt = nullptr;
            other.sz = 0;
        }
        return *this;
    }

private:
    // recursive helper for count_left_leaves
    int count_left_leaves_rec(Node* n, bool is_left) const {
        if (n == nullptr)
            return 0;
        if (n->left == nullptr && n->right == nullptr)
            return is_left ? 1 : 0;
        int left_count = count_left_leaves_rec(n->left, true);
        int right_count = count_left_leaves_rec(n->right, false);
        return left_count + right_count;
    }

public:
    int count_left_leaves() const {
        return count_left_leaves_rec(rt, false);
    }

    int count_left_leaves_bfs() const {
        if (rt == nullptr)
            return 0;

        std::queue<std::pair<Node*, bool>> q;
        q.push({rt, false});
        int count = 0;

        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            Node* node = current.first;
            bool is_left = current.second;

            if (!node->left && !node->right && is_left)
                ++count;

            if (node->left) q.push({node->left, true});
            if (node->right) q.push({node->right, false});
        }
        return count;
    }
};
