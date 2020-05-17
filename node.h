#ifndef NODE_H
#define NODE_H

#include "tree.h"

#define ON_BALANCE

class Node {
public:
    
    int v;
    
    Node(int val);
    Node * find(Tree & t, int val);
    void insert(Tree & t, int val);
    Node * successor(Tree & t);
    Node * predecessor(Tree & t);
    Node * remove(Tree & t);
    int size();
    int height();
#ifdef DEBUG
    void print();
    void check_balance();
#endif
private:
    static void unlink_parent(Node *);
    static Node * replace_with_l(Node *);
    static Node * replace_with_r(Node *);

    static Node * rotate_right_left(Node * par, Node * cur);
    static Node * rotate_left_right(Node * par, Node * cur);
    static Node * rotate_right(Node * par, Node * cur, bool apply_rebalance = true);
    static Node * rotate_left(Node * par, Node * cur, bool apply_rebalance = true);
    
    void remove_rebalance(Tree & t);
    void insert_rebalance(Tree & t);

    Node * p;
    Node * l;
    Node * r;
    int bal;
};

#endif
