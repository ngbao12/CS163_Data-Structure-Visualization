#include "Tree234.hpp"

Node234::Node234(bool leaf) : isLeaf(leaf) {
    key.reserve(3);
    child.reserve(4);
}

Node234::~Node234() {
    for (Node234* node : child) {
        delete node;
    }
}

Tree234::Tree234() {
    root = new Node234(true);
}

Tree234::~Tree234() {
    delete root;
}

void Tree234::createRandom(int numElements) {
    srand((int)time(0));
    
    delete root;
    root = new Node234(true);
    
    for (int i=0; i < numElements; i++) {
        int key = rand() % 100;
        insert(key);
    }
}

void Tree234::createFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        delete root;
        root = new Node234(true);
        
        int key;
        while(file >> key) {
            insert(key);
        }
        file.close();
    }
}

void Tree234::splitChild(Node234* parent, int index, Node234* child) {
    Node234* newChild = new Node234(child->isLeaf);
    
    int middleKey = child->key[1];
    newChild->key.assign(child->key.begin() + 2, child->key.end());
    child->key.resize(1);
    
    if (!child->isLeaf) {
        newChild->child.assign(child->child.begin() + 2, child->child.end());
        child->child.resize(2);
    }
    
    parent->child.insert(parent->child.begin() + index + 1, newChild);
    parent->key.insert(parent->key.begin() + index, middleKey);
}

void Tree234::insert(int key) {
    if (root->key.size() == 3) {
        Node234* newRoot = new Node234(false);
        newRoot->child.push_back(root);
        splitChild(newRoot, 0, root);
        root = newRoot;
    }
    insertNonFull(root, key);
}

void Tree234::insertNonFull(Node234*node, int key) {
    int i =(int) node->key.size() - 1;
    if (root->isLeaf) {
        node->key.push_back(0);
        while (i >= 0 && node->key[i] > key) {
            node->key[i+1] = node->key[i];
            i--;
        }
        node->key[i+1] = key;
    }
    else {
        while (i >= 0 && node->key[i] > key) i--;
        i++;
        if (node->child.size() == 3) {
            splitChild(node, i, node->child[i]);
            if (node->key[i] < key) i++;
        }
        insertNonFull(node->child[i], key);
    }
}

void Tree234::remove(Node234* node, int key) {
    int index = 0;
    while (index < node->key.size() && node->key[index] < key)
        index ++;
    
    if (index < node->key.size() && node->key[index] == key) {
        if (node->isLeaf)
            removeFromLeaf(node, index);
        else
            removeFromNonLeaf(node, index);
    }
    else {
        if (node->isLeaf)
            return;
        bool flag = (index == node->key.size());
        
        if (node->child[index]->key.size() < 2)
            fill(node, index);
        
        if (flag && index > node->key.size())
            remove(node->child[index - 1], key);
        else
            remove(node->child[index], key);
    }
}

void Tree234::removeFromLeaf(Node234* node, int index) {
    node->key.erase(node->key.begin() + index);
}

void Tree234::removeFromNonLeaf(Node234* node, int index) {
    int key = node->key[index];
    
    if (node->child[index]->key.size() > 1) {
        int pred = getPredecessor(node, index);
        node->key[index] = pred;
        remove(node->child[index], pred);
    }
    else if (node->child[index + 1]->key.size() > 1) {
        int suc = getSuccessor(node, index);
        node->key[index] = suc;
        remove(node->child[index + 1], suc);
    }
    else {
        if (index < node->key.size()) {
            merge(node, index);
            remove(node->child[index], key);
        }
        else {
            merge(node, index - 1);
            remove(node->child[index - 1], key);
        }
    }
}

// find the maximum of left childs
int Tree234::getPredecessor(Node234* node, int index) {
    Node234* cur = node->child[index];
    while (!cur->isLeaf) {
        cur = cur->child[cur->key.size()];
    }
    return cur->key[cur->key.size()-1];
}

// find the minimum of right childs
int Tree234::getSuccessor(Node234* node, int index) {
    Node234* cur = node->child[index+1];
    while (!cur->isLeaf) {
        cur = cur->child[0];
    }
    return cur->key[0];
}

// borrow left
void Tree234::borrowFromPrev(Node234* node, int index) {
    Node234* child = node->child[index];
    Node234* sibling = node->child[index - 1];
    
    child->key.insert(child->key.begin(), node->key[index - 1]);
    if (!child->isLeaf) {
        child->child.insert(child->child.begin(), sibling->child.back());
        sibling->child.pop_back();
    }
    
    node->key[index - 1] = sibling->key.back();
    sibling->key.pop_back();
}

// borrow right
void Tree234::borrowFromNext(Node234* node, int index) {
    Node234* child = node->child[index];
    Node234* sibling = node->child[index+1];
    
    child->key.push_back(node->key[index]);
    if (!child->isLeaf) {
        child->child.push_back(sibling->child[0]);
        sibling->child.erase(sibling->child.begin());
    }
    
    node->key[index] = sibling->key[0];
    sibling->key.erase(sibling->key.begin());
}

void Tree234::merge(Node234* node, int index) {
    Node234* child = node->child[index];
    Node234* sibling = node->child[index+1];
    
    child->key.push_back(node->key[index]);
    for (int i = 0; i < sibling->key.size(); i++) {
        child->key.push_back(sibling->key[i]);
    }
    
    if (!child->isLeaf) {
        for (int i = 0; i < sibling->child.size(); i++) {
            child->child.push_back(sibling->child[i]);
        }
    }
    
    node->key.erase(node->key.begin() + index);
    node->child.erase(node->child.begin() + index + 1);
    delete sibling;
}

void Tree234::fill(Node234* node, int index) {
    if (index != 0 && node->child[index - 1]->key.size() > 1) {
        borrowFromPrev(node, index);
    }
    else if (index != node->key.size() && node->child[index + 1]->key.size() > 1) {
        borrowFromNext(node, index);
    }
    else {
        if (index != node->key.size())
            merge(node, index);
        else
            merge(node, index - 1);
    }
}

bool Tree234::search(int key) {
    return search(root, key);
}

bool Tree234::search(Node234* node, int key) {
    int index = 0;
    while (index < node->key.size() && node->key[index] < key)
        index++;
    
    if (index < node->key.size() && node->key[index] == key)
        return true;
    if (node->isLeaf)
        return false;
    
    return search(node->child[index], key);
}
