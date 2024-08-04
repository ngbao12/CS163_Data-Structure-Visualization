#ifndef Tree234_hpp
#define Tree234_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Node234 {
    vector <int> key;
    vector <Node234*> child;
    bool isLeaf;
    
    Node234(bool leaf);
    ~Node234();
};

class Tree234 {
private:
    Node234* root;
    
    void splitChild(Node234* parent, int index, Node234* child);
    void insertNonFull(Node234* node, int key);
    void remove(Node234* node, int index);
    void removeFromLeaf(Node234* node, int index);
    void removeFromNonLeaf(Node234* node, int indx);
    int getPredecessor(Node234* node, int index);
    int getSuccessor(Node234* node, int index);
    void fill(Node234* node, int index);
    void borrowFromPrev( Node234* node, int index);
    void borrowFromNext(Node234* node, int index);
    void merge(Node234* node, int index);
    bool search(Node234* node, int key);
    
public:
    Tree234();
    ~Tree234();
    
    void createRandom(int numElements);
    void createFromFile(const string& filename);
    void insert(int key);
    void remove(int key);
    bool search(int key);
    void traverse();
    
};

#endif /* Tree234_hpp */
