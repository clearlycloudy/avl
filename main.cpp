#include <bits/stdc++.h>
#include "node.h"
#include "tree.h"

using namespace std;

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

template<class T>
ostream & operator<<(ostream &s, vector<T> const & in){
    s << " ";
    for(auto &i: in){
	s << i << " ";
    }
    s << endl;
    return s;
}

template<class T, class S>
ostream & operator<<(ostream &s, pair<T,S> const & i){
    s << i.first << ", " << i.second;
    return s;
}

void test(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 g(seed);
    uniform_int_distribution<int> distr(0,10000);
    int n = 2000;
    Tree t;
    {
	set<int> rec;
	for(int i=0; i<n; ++i){
	    int val = distr(g);
	    t.insert(val);
	    rec.insert(val);
#ifdef DEBUG
	    t.check_balance();
#endif
	}
	
	assert(t.size()==rec.size());
	assert(t.height() < 3 * log2(rec.size()));

	Node * ns = t.begin();
	set<int> contain;
	while(ns){
	    contain.insert(ns->v);
	    ns = ns->successor(t);
	}
	assert(contain==rec);
	
	vector<int> shuf(rec.begin(),rec.end());
	std::shuffle(shuf.begin(), shuf.end(), std::default_random_engine(seed));
	for(auto i: shuf){
	    auto node = t.find(i);
	    assert(node);
	    t.remove(node);
#ifdef DEBUG
	    t.check_balance();
#endif
	}
    }
    assert(t.size()==0);
}
int main(){
    test();
    exit(0);
}
