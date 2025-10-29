//tree_traversal.hpp
#pragma once
#include "linked_binary_tree.hpp"
#include <queue>
#include <deque>


//container must be able to store 
//objects of type Position
template <typename Position, typename Container>
void preorder(Position p, Container& visited) {
    visited.push_back(p);
    for (Position child : p.children())
        preorder(child, visited);
}

template <typename Position, typename Container>
void postorder(Position p, Container& visited) {
    for (Position child : p.children())
        postorder(child, visited);
    visited.push_back(p);
}


template <typename Position, typename Container>
void breadthfirst(Position p, Container& visited) {
    if (!p.is_null()) {
        std::queue<Position> fringe;
        fringe.push(p);
        while (!fringe.empty()) {
            Position curr = fringe.front();
            fringe.pop();
            visited.push_back(curr);
            for (Position c : curr.children())
                fringe.push(c);
        }
    }
}


template <typename Position>
std::queue<Position> breadthfirst_2(Position p) {
    std::queue<Position> result;
    if (!p.is_null()) {
        std::queue<Position> fringe;
        fringe.push(p);                       // start exploring from p
        while (!fringe.empty()) {
            Position p = fringe.front();      // explore from front of queue
            result.push(p);
            fringe.pop();
            for (Position c : p.children())
                fringe.push(c);               // pushed to back of the queue
        }
    }
    return result;
}



template <typename Position, typename Container>
void inorder(Position p, Container& visited) {
    if (!p.is_null()) {
        inorder(p.left(), visited);
        visited.push_back(p);
        inorder(p.right(), visited);
    }
}
