class Tree {
private:
    Node * root;
public:
    Node * find(int val);
    void insert(int val);
    void remove(Node * n);
    int size();
};
