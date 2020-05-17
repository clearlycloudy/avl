#include "node.h"
#include "tree.h"
#include <algorithm>
#include <cassert>
#include <iostream>

Node::Node(int val): p(nullptr), l(nullptr), r(nullptr), bal(0), v(val) {}

//dfs traversal O(n)
int Node::size(){
    return 1 + (l ? l->size() : 0) + (r ? r->size() : 0);
}
int Node::height(){
    return 1 + std::max( (l ? l->height() : 0), (r ? r->height() : 0) );
}
//log(n)
Node * Node::find(Tree & t, int val){
    if(val<v){
	if(l) return l->find(t, val);
	else return nullptr;
    }else if(val>v){
	if(r) return r->find(t, val);
	else return nullptr;
    }else{
	return this;
    }
}
void Node::insert(Tree & t, int val){
    if(val<v){
	if(l) l->insert(t, val);
	else{
	    l = new Node(val);
	    l->p = this;
	    l->insert_rebalance(t);
	}
    }else if(val>v){
	if(r) r->insert(t, val);
	else{
	    r = new Node(val);
	    r->p = this;
	    r->insert_rebalance(t);
	}
    }else{
	v = val; //overwrite current value
    }
}
Node * Node::successor(Tree & t){
    Node * c = r;
    if(c){
	while(c->l)
	    c = c->l;
	return c;
    }else{
	c = this;
	while(c->p){
	    if(c->p->l==c){
		return c->p;
	    }
	    c = c->p;
	}
	return nullptr;
    }
}
Node * Node::predecessor(Tree & t){
    Node * c = l;
    if(c){
	while(c->r)
	    c = c->r;
	return c;
    }else{
	c = this;
	while(c->p){
	    if(c->p->r==c){
		return c->p;
	    }
	    c = c->p;
	}
	return nullptr;
    }
}
Node * Node::remove(Tree & t){
    //returns node to be recycled

    //node to recycle at the end
    Node * del = nullptr;
    if(!l && !r){ // no children
	// std::cout<< "remove 0 children case" << std::endl;
	del = this;
	if(this->p){
	    this->remove_rebalance(t);
	    unlink_parent(del); //unlink current
	}else{
	    t.root = nullptr;
	}
    }else if(l && !r){ //left
	// std::cout<< "remove left child case" << std::endl;
	del = this;
	Node * ret = replace_with_l(del); //link parent with child
	ret->remove_rebalance(t);
    }else if(r && !l){ //right
	// std::cout<< "remove right child case" << std::endl;
	del = this;
	Node * ret = replace_with_r(del); //link parent with child
	ret->remove_rebalance(t);
    }else{ //2 children
	// std::cout<< "remove 2 children case" << std::endl;
	Node * suc = this->successor(t);
	assert(suc);
	assert(!suc->l);
	this->v = suc->v; //copy value
	del = suc; //remove successor node
	if(suc->r){
	    Node * ret = replace_with_r(suc); //link parent with child
	    ret->remove_rebalance(t);
	}else{
	    suc->remove_rebalance(t);
	    unlink_parent(del); //unlink current
	}
    }
    assert(del);
    return del;
}
void Node::insert_rebalance(Tree & t){
    //current node is assume to have increased height by 1
    Node * par = this->p;
    if(par){
	Node * pp = par->p;	
	Node * n = nullptr;
	if(par->r==this){ //right branch
	    if(par->bal>0){
		if(this->bal<0){//right left case
		    n = rotate_right_left(par,this);
		}else{ //right right
		    n = rotate_left(par,this);
		}
	    }else if(par->bal<0){
		par->bal = 0;
		return;
	    }else{ //bal == 0
		par->bal = 1;
		return par->insert_rebalance(t); //go up a level, recursive
	    }
	}else{ //left branch
	    // std::cout << "left branch" << std::endl;
	    if(par->bal<0){
		if(this->bal>0){//left right case
		    n = rotate_left_right(par,this);
		}else{ //left left
		    n = rotate_right(par,this);
		}
	    }else if(par->bal>0){
		par->bal = 0;
		return;
	    }else{ //bal == 0
		par->bal = -1;
		return par->insert_rebalance(t); //go up a level, recursive
	    }
	}
	assert(n);
	n->p = pp;
	if(pp){
	    if(pp->l == par)
		pp->l = n;
	    else
		pp->r = n;
	}else
	    t.root = n;
    }
}

Node * Node::rotate_left(Node * par, Node * cur, bool apply_rebalance){
    //assumes cur subtree now has 2 more height than sibling
    assert(par);
    assert(cur);
    assert(par->r==cur);
    par->r = cur->l;
    if(cur->l)
	cur->l->p = par;
    par->p = cur;
    cur->l = par;

    if(apply_rebalance){
	//adjust balance
	if(cur->bal==0){
	    par->bal = 1;
	    cur->bal = -1;
	}else{
	    par->bal = 0;
	    cur->bal = 0;
	}
    }

    return cur;
}

Node * Node::rotate_right(Node * par, Node * cur, bool apply_rebalance){
    //assumes cur subtree now has 2 more height than sibling
    assert(par);
    assert(cur);
    assert(par->l==cur);
    par->l = cur->r;
    if(cur->r)
	cur->r->p = par;
    par->p = cur;
    cur->r = par;

    if(apply_rebalance){
	//adjust balance
	if(cur->bal==0){
	    par->bal = -1;
	    cur->bal = 1;
	}else{
	    par->bal = 0;
	    cur->bal = 0;
	}
    }
    
    return cur;
}

Node * Node::rotate_right_left(Node * par, Node * cur){
    //assumes cur subtree has 2 more height than sibling
    assert(par);
    assert(cur);
    assert(cur->l);
    assert(par->r==cur);
    Node * a = rotate_right(cur, cur->l, false);
    assert(a);
    a->p = par;
    par->r = a;
    Node * b = rotate_left(par,a, false);

    //balance accounting
    if(b->bal==0){
	b->l->bal = 0;
	b->r->bal = 0;
    }else if(b->bal>0){
	b->l->bal = -1;
	b->r->bal = 0;
    }else{
	b->l->bal = 0;
	b->r->bal = 1;
    }
    b->bal = 0;
    return b;
}

Node * Node::rotate_left_right(Node * par, Node * cur){
    //assumes cur subtree has 2 more height than sibling
    assert(par);
    assert(cur);
    assert(cur->r);
    assert(par->l==cur);
    Node * a = rotate_left(cur, cur->r, false);
    assert(a);
    a->p = par;
    par->l = a;
    Node * b = rotate_right(par,a, false);

    //balance accounting
    if(b->bal==0){
	b->l->bal = 0;
	b->r->bal = 0;
    }else if(b->bal>0){
	b->l->bal = -1;
	b->r->bal = 0;
    }else{
	b->l->bal = 0;
	b->r->bal = 1;
    }
    b->bal = 0;
    return b;
}

void Node::remove_rebalance(Tree & t){
    //assumes current subtree has height decreased by 1

    if(p){
	Node * pp = p->p;

	if(p->l==this){ //left child
	    if(p->bal > 0){ //parent is right heavy
		assert(p->r);
		int b = p->r->bal;
		Node * ret = nullptr;
		if(b<0){ //right-left heavy; 2 rotations
		    ret = rotate_right_left(p, p->r);
		}else{ //right-right heavy; 1 rotation
		    ret = rotate_left(p, p->r);
		}
		assert(ret);
		if(pp){
		    if(pp->l == p) pp->l = ret;
		    else pp->r = ret;
		    ret->p = pp;
		}else{
		    ret->p = nullptr;
		    t.root = ret;
		}
		if(b==0) return; //overall balance at new subtree rooted at ret == as before at p
		else ret->remove_rebalance(t); //recursive; up one level
	    }else if(p->bal == 0){
		p->bal = 1; //increment balance
		return; //end
	    }else{ // <0
		p->bal = 0; //neutralize balance
		p->remove_rebalance(t); //recursive; up one level
	    }
	}else{ //right child
	    if(p->bal < 0){ //parent is left heavy
		assert(p->l);
		int b = p->l->bal;
		Node * ret = nullptr;
		if(b>0){ //left-right heavy; 2 rotations
		    ret = rotate_left_right(p, p->l);
		}else{ //left-left heavy; 1 rotation
		    ret = rotate_right(p, p->l);
		}
		assert(ret);
		if(pp){
		    if(pp->l == p) pp->l = ret;
		    else pp->r = ret;
		    ret->p = pp;
		}else{
		    ret->p = nullptr;
		    t.root = ret;
		}
		if(b==0) return; //overall balance at new subtree rooted at ret == as before at p
		else ret->remove_rebalance(t); //recursive; up one level
	    }else if(p->bal == 0){
		p->bal = -1; //increment balance
		return; //end
	    }else{ // >0
		p->bal = 0; //neutralize balance
		p->remove_rebalance(t); //recursive; up one level
	    }
	}
    }else{
	t.root = this;
    }
}

void Node::unlink_parent(Node * n){
    if(n){
	if(n->p){
	    if(n->p->l == n)
		n->p->l = nullptr;
	    else{
		assert(n->p->r == n);
		n->p->r = nullptr;
	    }
	}
    }
}

Node * Node::replace_with_l(Node * n){
    if(n){
	if(n->l)
	    n->l->p = n->p;
	
	if(n->p){
	    if(n->p->l == n)
		n->p->l = n->l;
	    else
		n->p->r = n->l;
	}
	return n->l;
    }else{
	return nullptr;
    }
}

Node * Node::replace_with_r(Node * n){
    if(n){
	if(n->r)
	    n->r->p = n->p;
	
	if(n->p){
	    if(n->p->l == n)
		n->p->l = n->r;
	    else
		n->p->r = n->r;
	}
	return n->r;
    }else{
	return nullptr;
    }
}
#ifdef DEBUG
void Node::print(){
    std::cout << v << ", bal: " << bal << ", children: ";
    if(l)
	std::cout << "l: " << l->v <<" ";
    if(r)
	std::cout << "r: " << r->v <<" ";
    std::cout << std::endl;
    if(l) l->print();
    if(r) r->print();
}
void Node::check_balance(){
    if(l) l->check_balance();
    if(r) r->check_balance();

    int h_l = l ? l->height() : 0;
    int h_r = r ? r->height() : 0;
    int diff = h_r-h_l;
    if(diff!=bal){
	std::cout << "assert: " << v << ", bal: " << bal << ", expected bal: " << diff << std::endl;
	assert(false);
    }
    if(abs(bal)>=2){
	std::cout << "assert: " << v << ", bal: " << bal << ", over balance" << std::endl;
	assert(false);
    }
}
#endif
