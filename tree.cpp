#include <cassert>
#include <iostream>

#include "node.h"
#include "tree.h"

Tree::Tree(): root(nullptr) {}

Node * Tree::find(int val){
    if(root)
	return root->find(*this,val);
    else
	return nullptr;
}
void Tree::insert(int val){
    if(root)
	return root->insert(*this,val);
    else
	root = new Node(val);
}
void Tree::remove(Node * n){
    //assumes n belongs to current tree
    if(n){
	assert(root);
	Node * del = n->remove(*this);
	assert(del);
	delete del;
    }
}
int Tree::size(){
    if(root)
	return root->size();
    else
	return 0;
}

int Tree::height(){
    if(root)
	return root->height();
    else
	return 0;
}

Node * Tree::begin(){
    Node * c = root;
    if(!c)
	return nullptr;
    else{
	Node * cc = c->predecessor(*this);
	while(cc){
	    c = cc;
	    cc = cc->predecessor(*this);
	}
	return c;
    }
}
Node * Tree::rbegin(){
    Node * c = root;
    if(!c)
	return nullptr;
    else{
	Node * cc = c->successor(*this);
	while(cc){
	    c = cc;
	    cc = cc->successor(*this);
	}
	return c;
    }
}
#ifdef DEBUG
void Tree::print(){
    if(root) root->print();
}
void Tree::check_balance(){
    if(root) root->check_balance();
}
#endif
