// src/main.cpp
#include <iostream>
#include "linked_binary_tree.hpp"

int main() {
    LinkedBinaryTree<int> T;
    T.add_root(10);
    auto r = T.root();
    T.add_left(r, 5);
    T.add_right(r, 20);
    T.add_left(r.left(), 3);
    T.add_right(r.left(), 7);
    T.add_left(r.right(), 30);

    //modify above code to try different trees before submission

    std::cout << "Left Nodes: " << T.count_left_leaves_bfs() << "\n"; //2
    std::cout << "Left Nodes: " << T.count_left_leaves() << "\n"; //2

    return 0;
}
