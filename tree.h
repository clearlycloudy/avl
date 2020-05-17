#ifndef TREE_H
#define TREE_H

class Node;

class Tree {
    friend class Node;
private:
    Node * root;
public:
    Tree();
    Node * find(int val);
    void insert(int val);
    void remove(Node * n);
    int size();
    int height();
    Node * begin();
    Node * rbegin();
#ifdef DEBUG
    void print();
    void check_balance();
#endif
};

#endif
