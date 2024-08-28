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
        if (forCreate) root->start = {779.f, 124.f};
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
    updatePos(newRoot, forCreate, forCreate, {779.f, 124.f}, {0.f, 0.f});
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

Node234* Tree234::merge(Node234* &predecessor) {
    if (!predecessor) return nullptr;
    if (!predecessor->parent) {
        this->root = nullptr;
        return nullptr;
    }
    Node234* parent = predecessor->parent;
    int pos = 0;

    for (pos = 0; pos < int(parent->children.size()); ++pos) {
        if (parent->children[pos] == predecessor) break;
    }

    Node234* leftSibling = (pos > 0) ? parent->children[pos - 1] : nullptr;
    Node234* rightSibling = (pos < int(parent->children.size()) - 1) ? parent->children[pos + 1] : nullptr;

    if (leftSibling && leftSibling->keys.size() > 1) {
        predecessor->keys.insert(predecessor->keys.begin(), parent->keys[pos - 1]);
        parent->keys[pos - 1] = leftSibling->keys.back();
        leftSibling->keys.pop_back();

        if (!leftSibling->isLeaf()) {
            predecessor->children.insert(predecessor->children.begin(), leftSibling->children.back());
            leftSibling->children.pop_back();
        }
    }
    else if (rightSibling && rightSibling->keys.size() > 1) {
        // Mượn khóa từ right_sibling
        predecessor->keys.push_back(parent->keys[pos]);
        parent->keys[pos] = rightSibling->keys.front();
        rightSibling->keys.erase(rightSibling->keys.begin());

        if (!rightSibling->isLeaf()) {
            predecessor->children.push_back(rightSibling->children.front());
            rightSibling->children.erase(rightSibling->children.begin());
        }
    }
    else {
        if (leftSibling) {
            leftSibling->keys.push_back(parent->keys[pos - 1]);
            leftSibling->keys.insert(leftSibling->keys.end(), predecessor->keys.begin(), predecessor->keys.end());

            if (!predecessor->isLeaf()) {
                leftSibling->children.insert(leftSibling->children.end(), predecessor->children.begin(), predecessor->children.end());
            }
            parent->keys.erase(parent->keys.begin() + (pos - 1));
            parent->children.erase(parent->children.begin() + pos);
            delete predecessor;
            predecessor = nullptr;
        }
        else if (rightSibling) {
            predecessor->keys.push_back(parent->keys[pos]);
            predecessor->keys.insert(predecessor->keys.end(), rightSibling->keys.begin(), rightSibling->keys.end());

            if (!rightSibling->isLeaf()) {
                predecessor->children.insert(predecessor->children.end(), rightSibling->children.begin(), rightSibling->children.end());
            }
            parent->keys.erase(parent->keys.begin() + pos);
            parent->children.erase(parent->children.begin() + (pos + 1));
            delete rightSibling;
            rightSibling = nullptr;
        }
    }
    if (parent == this->root) {
        root = parent->children[0];
        delete parent;
        parent = nullptr;
    }
    return parent;
}

void Tree234::insert(int key) {
    clearProcess();
    if (!root) {
        root = new Node234(key);
        saveStep(root, -1, 0, {}, "insert at root...", "", false);
        return;
    }
    Node234* current = root;
    while (current) {

        saveStep(current, -1, 0, {}, "checking is_leaf()", "", false);
        if (current->isFull()) {
            saveStep(current, -1, 0, {}, "checking...", "", false);
            current = split(current);
            saveStep(nullptr, -1, 1, {}, "split...", "", false);
        }
        if(current->isLeaf()) {
            saveStep(current, -1, 0, {}, "this node is leaf, insert key", "", false);
            break;
        }
        if (key < current->keys[0].value) {
            current = current->children[0];
        } else if (current->keys.size() == 1 || (current->keys.size() > 1 && key < current->keys[1].value)) {
            current = current->children[1];
        } else {
            current = current->children[2];
        }
        saveStep(current, -1, 0, {}, "go to child...", "", false);
    }
    Node234* tmp = current;

    current->insert(key);
    saveStep(current, -1, 0, {}, "inserting...", "", false);
    saveStep(nullptr, -1, -1, {}, "finish", "", false);
}

bool Tree234::search(int key) {
    clearProcess();
    Node234* current = root;
    while (current != nullptr) {
        saveStep(current, -1, 0, {}, "checking", "", false);
        int i = 0;
        for (; i < int(current->keys.size()); i++) {
            if (key == current->keys[i].value) {
                saveStep(current, i, 0, {}, "FOUND", "", false);

                return true;
            }
            if (key < current->keys[i].value) break;
        }
        if (current->isLeaf()) {
            saveStep(nullptr, -1, -1, {}, "not found", "", false);
            return false;
        }
        current = current->children[i];
        saveStep(current, -1, 0, {}, "go to child", "", false);
    }
    saveStep(nullptr, -1, -1, {}, "not found", "", false);
    return false;
}

bool Tree234::remove(int key) {
    clearProcess();
    if (!root) {
        saveStep(nullptr, -2, -1, {}, "empty tree", "");
        return 0;
    }

    Node234* node = root;
    int i = 0;
    while(node) {
        saveStep(node, -1, 0, {}, "Finding key", "", false);
        for(i = 0; i < node->keys.size() && node->keys[i].value < key; i++);
        if (i < node->keys.size() && node->keys[i].value == key) break;
        if (i < node->children.size()) node = node->children[i];
        else node = nullptr;
    }
    if (!node) {
        saveStep(nullptr, -1, -1, {}, "NOT FOUND", "", false);

        return 0;
    }
    saveStep(node, i, 0, {}, "FOUND, start removing", "", false);

    if (node->isLeaf()) {
        saveStep(node, i, 0, {}, "key in leaf node", "", false);
        node->remove(key);
        if(node->keys.size() == 0) {
            do {
                node = merge(node);
                saveStep(node, -1, 0, {}, "merge", "", false);
            } while (node && node->keys.size() == 0);
        }
        // log_tree(this->root);
        saveStep(nullptr, -1, -1, {}, "finish", "", false);

        
        return 1;
    }

    
    Node234* predecessor = node->children[i];
    while(!predecessor->isLeaf()) {
        predecessor = predecessor->children.back();
    }
    node->keys[i].value = predecessor->keys.back().value;
    predecessor->keys.pop_back();
    saveStep(node, i, 0, {}, "replace by predecessor", "", false);
    if (predecessor->keys.size() == 0) {
        do {
            predecessor = merge(predecessor);
        } while (predecessor && predecessor->keys.size() == 0);
        saveStep(predecessor, -1, 0, {}, "merge", "", false);

    }
    saveStep(nullptr, -1, -1, {}, "finish", "", false);
    return 1;
}

void Tree234::createFromFile(const char* filename) {
    freeTree(this->root);
    this->root = nullptr;
        std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    int key;
    while (file >> key) {
        insert(key);
    }
    clearProcess();
    saveStep(nullptr ,-1, -1, {}, "Creating...", "", true);
    saveStep(nullptr, -1, -1, {}, "Create succesfully", "");
    file.close();
}

void Tree234::createRandom(int n, int range) {
    freeTree(this->root);
    srand((int)time(0));
    for(int i = 0; i < n; i++) {
        insert(rand() % range);
        
    }
    clearProcess();
    saveStep(nullptr, -1, -1, {}, "Creating...", "", true);
    saveStep(nullptr, -1, -1, {}, "Create succesfully", "");
}

Tree234Visualize::Tree234Visualize(Font font) {
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    this->isInsertChosen = false;
    this->isDeleteChosen = false;
    this->isSearchChosen = false;
    this->stepIndex = 0;
    this->frame = 0;
    this->type = false;
    this->numFrameOfAnimation = FPS;

    this->createButton = Button({8, 415, 110, 30}, "Create", -1, BLACK, 20, font);
    this->randomButton = Button({156.5, 449.3, 110, 30}, "Random", -1, BLACK, 20, font);
    this->loadFileButton = Button({156.5, 520.6, 110, 30}, "Load File", -1, BLACK, 20, font);
    this->insertButton = Button({8, 458, 110, 30}, "Insert", -1, BLACK, 20, font);
    this->deleteButton = Button({8, 504, 110, 30}, "Delete", -1, BLACK, 20, font);
    this->inputNumber = InputStr(156.5, 449.3, 110, 30, "", 20, this->font);
    this->playButton = Button({173, 492, 70, 30}, "Play", -1, BLACK, 20, font);
    this->searchButton = Button({8, 545, 110, 30}, "Search", -1, BLACK, 20, font);

}

void Tree234Visualize::updateStep(int index) {
    this->step = this->tree.getProcess()[index];
    this->frame = 0;
    this->stepIndex = index;
}

void Tree234Visualize::createFromFile() {
    auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                           { "Text Files (.txt .text)", "*.txt *.text",
                               "All Files", "*" },
                           pfd::opt::force_path);
    if (f.result().empty()) {
        return ;
    }
       
    auto path = f.result().back();
    this->tree.createFromFile(path.c_str());
    if(this->tree.getProcess().empty()) return;
    this->numFrameOfAnimation = 10/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void Tree234Visualize::createWithRandomizedData(int n, int range) {
    this->tree.createRandom(n, range);
    if(this->tree.getProcess().empty()) return;
    this->numFrameOfAnimation = 10/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void Tree234Visualize::insert() {
    this->tree.insert(std::stoi(this->inputNumber.getText()));
    if(this->tree.getProcess().empty()) return;

    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void Tree234Visualize::deleteNode() {
    this->tree.remove(std::stoi(this->inputNumber.getText()));
    if(this->tree.getProcess().empty()) return;
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void Tree234Visualize::search() {
    this->tree.search(std::stoi(this->inputNumber.getText()));
    if(this->tree.getProcess().empty()) return;
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void Tree234Visualize::drawNode(Node234 *root, int frame, int numFrame, Font font, bool isNotification) {
    if (!root) return;
    if (root->keys.size() == 0) {
        Vector2 pos = Vector2Lerp(root->start, root->end, float(frame)/numFrame);
        DrawRectangleV(Vector2Add(pos, (Vector2){-15.f, 0.f}), {30.f, 30.f}, THEME.HIGHLIGHT_NODE_1);
        DrawLineEx(Vector2Add(pos, (Vector2){-0.f, 30.f}), Vector2Lerp(root->children[0]->start, root->children[0]->end, float(frame)/numFrame), 2, THEME.LINE);
    } else {
        for (int i = 0; i < root->children.size(); i++) {
            if(root->children[i]) {
                Vector2 linePos;
                if (i >= root->keys.size()) linePos = Vector2Add(Vector2Lerp(root->keys.back().start, root->keys.back().end, float(frame)/numFrame) , (Vector2){30.f, 30.f});
                else linePos = Vector2Add(Vector2Lerp(root->keys[i].start, root->keys[i].end, float(frame)/numFrame), (Vector2){0.f, 30.f});
                DrawLineEx(linePos, Vector2Lerp(root->children[i]->start, root->children[i]->end, float(frame)/numFrame), 2, THEME.LINE);
            }
        }
    }
    for (int i = 0; i < root->keys.size(); i++) {
        Color color = root->keys[i].highlight ? THEME.HIGHLIGHT_NODE_1 : THEME.NODE;
        Vector2 keyPos = Vector2Lerp(root->keys[i].start, root->keys[i].end, float(frame)/numFrame);
        DrawRectangleV(keyPos, {30.f, 30.f}, color);
        Vector2 textSize = MeasureTextEx(font, TextFormat("%d", root->keys[i].value), CODE_SIZE, 0);
        DrawTextPro(font, TextFormat("%d", root->keys[i].value), {keyPos.x + 15 - textSize.x/2, keyPos.y + 15 - textSize.y/2}, {0.f, 0.f}, 0, CODE_SIZE, 0, BLACK);
    }
    for (int i = 0; i < root->children.size(); i++) {
        if(root->children[i]) {
            drawNode(root->children[i], frame, numFrame, font, isNotification);
        }
    }
}

void Tree234Visualize::drawTree() {
    if (this->tree.getProcess().empty()) return;
    drawNode(this->step.root, this->frame, this->numFrameOfAnimation, this->font, this->step.type == -1);
    if (this->type == 2 || this->type == 0) return;
    this->frame++;
    if(this->frame >= this->numFrameOfAnimation && !this->tree.getProcess().empty()) {
        if (stepIndex == this->tree.getProcess().size() - 1) {
            this->type = 2;
            return;
        }
        updateStep(this->stepIndex + 1);
        this->progressBar.updateStep(1);
    }

}

void Tree234Visualize::drawButtons() {
    this->createButton.draw(50);
    this->deleteButton.draw(50);
    this->insertButton.draw(50);
    this->searchButton.draw(50);

    if(this->isCreateChosen) {
        this->randomButton.draw(50);
        this->loadFileButton.draw(50);
    }

    if(this->isDeleteChosen || this->isInsertChosen || this->isSearchChosen) {
        this->inputNumber.draw();
        this->inputNumber.update();
        this->playButton.draw();
    }
}

int Tree234Visualize::handle() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (this->createButton.getIsHovered()) {
            this->isCreateChosen = true;
            this->isDeleteChosen = false;
            this->isInsertChosen = false;
            this->isSearchChosen = false;
        }
        if (this->deleteButton.getIsHovered()) {
            this->isCreateChosen = false;
            this->isDeleteChosen = true;
            this->isInsertChosen = false;
            this->isSearchChosen = false;
        }
        if (this->insertButton.getIsHovered()) {
            this->isCreateChosen = false;
            this->isDeleteChosen = false;
            this->isInsertChosen = true;
            this->isSearchChosen = false;
        }
        if (this->searchButton.getIsHovered()) {
            this->isCreateChosen = false;
            this->isDeleteChosen = false;
            this->isInsertChosen = false;
            this->isSearchChosen = true;
        }
    }

    if (this->playButton.handle()) {
        if (this->isInsertChosen) {
            insert();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 2;
        }
        if (this->isDeleteChosen) {
            deleteNode();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 3;
        }
        if (this->isSearchChosen) {
            search();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 4;
        }
    }

    if (this->randomButton.handle()) {
        createWithRandomizedData();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    if (this->loadFileButton.handle()) {
        createFromFile();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }

    int flag = this->progressBar.handle();
    if (flag != 10) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    switch (flag)
    {
        case -2:
            if (this->progressBar.getMaxStep() == 0) break;
            this->progressBar.updateStep(-2);
            updateStep(0);
            break;

        case -1:
            if (this->progressBar.getMaxStep() == 0) break;
            this->progressBar.updateStep(-1);
            if (stepIndex == 0) return 0;
            updateStep(this->stepIndex - 1);
            break;

        case 1:
            if (this->progressBar.getMaxStep() == 0) break;
            if(stepIndex == this->tree.getProcess().size() - 1) break;
            this->progressBar.updateStep(1);
            updateStep(this->stepIndex + 1);
            break;

        case 2:
            if (this->progressBar.getMaxStep() == 0) break;
            updateStep((int)this->tree.getProcess().size() - 1);
            this->progressBar.updateStep(2);
            this->type = 2;
            break;

        case 0:
            if (this->type == 2) {
                if (this->progressBar.getMaxStep() == 0) break;
                this->progressBar.updateStep(-2);
                updateStep(0);
                this->type = 1;
                break;
            }
            if (this->type == 1) {
                this->type = 0;
                break;
            }
            if (this->type == 0 && this->step.root) {
                this->type = 1;
                break;
            }

        case 3: case -3:
            this->numFrameOfAnimation = FPS/this->progressBar.getSpeed();

            break;

        default:
            break;
    }

    return 0;
}

void Tree234Visualize::draw() {
    drawSideBar(this->type, this->step.code, this->step.line, this->step.infor, this->progressBar, this->font);
    drawButtons();
    drawTree();
}
