#include "Tree234.hpp"

Node234::Node234(int key, Vector2 start, Vector2 end, Node234* parent) : parent(parent), width(1), start(start), end(end) {
    this->keys.push_back({key});
}

bool Node234::isLeaf() const {
    return children.empty();
}

bool Node234::isFull() const {
    return keys.size() > 2;
}

int Node234::insert(int k) {
    int i = 0;
    for (; i < int(this->keys.size()); i++) {
        if (keys[i].value > k) break;
    }

    Key234 new_key;
    if (i == 0) new_key = Key234(k, keys[i].start, keys[i].end);
    else new_key = Key234(k, keys[i-1].start, keys[i-1].end);

    this->keys.insert(keys.begin() + i, new_key);

    return i;
}

void Node234::remove(int key) {
    int i = 0;
    for(; i < this->keys.size() && this->keys[i].value != key; i++);
    keys.erase(keys.begin() + i);
}

void Node234::logKeys() {
    printf("log_key --> pointer = %p | width = %d | keys = (", this, this->width);
    if (this->keys.empty()) {
        printf("EMPTY)\n");
        return;
    }
    for (auto i : this->keys) {
        printf("%d ", i.value);
    }
    printf(")");
    printf(" | pos: ");
    for(int i = 0; i < keys.size(); i++) {
        printf("%d - st: (%.1f, %.1f) | end: (%.1f, %.1f) | hl: %d -||- ", keys[i].start.x, keys[i].start.y, keys[i].end.x, keys[i].end.y, keys[i].highlight);
    }
    printf("\n");
}

void Tree234::freeTree(Node234* &node) {
    if(!node) return;
    node->keys.clear();
    for (int i = 0; i < node->children.size(); i++) {
        freeTree(node->children[i]);
    }
    delete node;
    node = nullptr;
}

void Tree234::freeStep(Step234 &st) {
    freeTree(st.root);
    st.line.clear();
}

void Tree234::clearProcess() {
    for (auto it = this->process.begin(); it != this->process.end(); ++it) {
        freeStep(*it);
    }
    this->process.clear();
}

int Tree234::estimateWidth(Node234* root) {
    if(!root) return 0;
    if(root->children.size() == 0) {
        root->width =(int) root->keys.size();
        return root->width;
    }
    int width = 0;
    for(int i = 0; i < root->children.size(); i++) {
        width += estimateWidth(root->children[i]);
    }
    root->width = width;
    return width;
}

void Tree234::updatePos(Node234* root, bool updateStart, bool forCreate, Vector2 parentPos, Vector2 delta) {
    if (!root) return;
    float width = root->keys.size() * 30;
    float pos = 0;

    if (updateStart) {
        if (forCreate) root->start = {1013.f, 100.f};
        else root->start = Vector2Add(parentPos, delta);
        for(int i = 0; i < root->keys.size(); i++) {
            root->keys[i].start.x = pos + root->start.x - width/2;
            root->keys[i].start.y = root->start.y;
            pos += 30;
        }
    }
    pos = 0;
    root->end = Vector2Add(parentPos, delta);
    for(int i = 0; i < root->keys.size(); i++) {
        root->keys[i].end.x = pos + root->end.x - width/2;
        root->keys[i].end.y = root->end.y;
        pos += 30;
    }
    
    pos = 0;
    width = root->width*30.f + (root->children.size() - 1) * 10.f;
    for (int i = 0; i < root->children.size(); i++) {
        float childWidth = 30.f*root->children[i]->width + root->children[i]->children.size()*10.f;
        updatePos(root->children[i], updateStart, forCreate, root->end, {(pos + childWidth/2 - width/2), 100.f});
        pos += childWidth + 20;
    }
}

Node234* Tree234::cloneTree(Node234* root, Node234* highlight, int index) {
    if(!root) return nullptr;
    Node234* newNode = new Node234();
    for(auto key : root->keys) {
        newNode->keys.push_back(key);
        newNode->keys.back().highlight = (root == highlight && index == -1);
    }
    if(root == highlight && index > -1) {
        newNode->keys[index].highlight  = true;
    }
    newNode->parent = root->parent;
    newNode->width = root->width;
    newNode->start = root->start;
    newNode->end = root->end;
    for(auto child : root->children) {
        newNode->children.push_back(cloneTree(child, highlight, index));
    }

    return newNode;
}

void Tree234::saveStep(Node234* highlight, int indexHighlight, int type, std::vector<int> lines, const std::string infor, const std::string code, bool forCreate) {
    Node234* newRoot = cloneTree(this->root, highlight, indexHighlight);
    estimateWidth(newRoot);
    updatePos(newRoot, forCreate, forCreate, {1013.f, 100.f}, {0.f, 0.f});
    this->process.push_back({newRoot, lines, infor, code, type});
    estimateWidth(this->root);
    updatePos(this->root, true, false);
}

Node234* Tree234::split(Node234* &node) {
    Key234 middle = node->keys[1];
    Node234* leftNode = new Node234();
    leftNode->keys.push_back(node->keys[0]);
    Node234* rightNode = new Node234();
    rightNode->keys.push_back(node->keys[2]);
    if (!node->isLeaf()) {
        leftNode->children.push_back(node->children[0]);
        leftNode->children.push_back(node->children[1]);

        rightNode->children.push_back(node->children[2]);
        rightNode->children.push_back(node->children[3]);

        leftNode->children[0]->parent = leftNode;
        leftNode->children[1]->parent = leftNode;

        rightNode->children[0]->parent = rightNode;
        rightNode->children[1]->parent = rightNode;
    }
    if (node->parent == nullptr) {
        Node234* newRoot = new Node234();
        newRoot->keys.push_back(middle);
        newRoot->children.push_back(leftNode);
        newRoot->children.push_back(rightNode);
        delete root;
        root = newRoot;
        leftNode->parent = root;
        rightNode->parent = root;
        return root;
    }

    if(node == root) {
        return root;
    }
    int index = node->parent->insert(middle.value);
    node->parent->children[index] = leftNode;
    node->parent->children.insert(node->parent->children.begin() + index + 1, rightNode);
    leftNode->parent = node->parent;
    rightNode->parent = node->parent;
    Node234* returnNode = node->parent;
    delete node;
    node = nullptr;
    if (returnNode != nullptr && returnNode->isFull()) {
        returnNode = split(returnNode);
    }
    return returnNode;
}


