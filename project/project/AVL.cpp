#include "AVL.hpp"

AVLNode* AVLTree::cloneTree(AVLNode* node) {
    if (!node) return nullptr;
    AVLNode* newNode = new AVLNode(node->key, node->start, node->end, node->height);
    newNode->left = cloneTree(node->left);
    newNode->right = cloneTree(node->right);
    return newNode;
}

void AVLTree::freeTree(AVLNode* &root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
    root = nullptr;
}

void AVLTree::freeStep(AVLStep &step) {
    freeTree(step.root);
    step.line.clear();
}

void AVLTree::clearProcess() {
    for (auto it = this->process.rbegin(); it != this->process.rend(); ++it) {
        freeStep(*it);
    }
    this->process.clear();
}

void AVLTree::updateStandardPos(AVLNode* root, Vector2 parentPos, Vector2 delta) {
    if (!root) return;
    root->start = Vector2Add(parentPos, delta);
    root->end = Vector2Add(parentPos, delta);
    updateStandardPos(root->left, root->end, root->left ? (Vector2){-float(pow(2,root->left->height))*10, 50.f} : (Vector2){0.f, 0.f});
    updateStandardPos(root->right, root->end, root->right ? (Vector2){float(pow(2,root->right->height))*10, 50.f} : (Vector2){0.f, 0.f});
}

void AVLTree::updateTargetPos(AVLNode* root, Vector2 parentPos, Vector2 delta, bool forCreate) {
    if (!root) return;
    if (forCreate) root->start = {1013.f, 100.f};
    root->end = Vector2Add(parentPos, delta);
    updateTargetPos(root->left, root->end, root->left ? (Vector2){-float(pow(2,root->left->height))*10, 50.f} : (Vector2){0.f, 0.f}, forCreate);
    updateTargetPos(root->right, root->end, root->right ? (Vector2){float(pow(2,root->right->height))*10, 50.f} : (Vector2){0.f, 0.f}, forCreate);
}

int AVLTree::updateAllHeight(AVLNode* node) {
    if (!node) return 0;
    node->height = 1 + std::max(updateAllHeight(node->left), updateAllHeight(node->right));
    return node->height;
}

void AVLTree::saveStep(int specialValue, int type, std::vector<int> line, const std::string infor, const std::string code, bool forCreate) {
    AVLNode* newRoot = cloneTree(this->root);
    updateAllHeight(newRoot);
    updateTargetPos(newRoot, {1013.f, 100.f}, {0.f, 0.f}, forCreate);
    this->process.push_back({newRoot, line, specialValue, infor, code, type});
    updateAllHeight(this->root);
    updateStandardPos(this->root);
}

AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    y->left = x->right;
    x->right = y;

    updateHeight(y);
    updateHeight(x);

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    x->right = y->left;
    y->left = x;

    updateHeight(x);
    updateHeight(y);
    return y;
}

std::pair<AVLNode*, int> AVLTree::balance(AVLNode* node, const std::string code) {
    int flag = 0; // -2: LL, -1: LR; 1: RL, 2: RR
    updateHeight(node);
    int balanceFactor = getBalance(node);
    // printf("checking- --- >>> %d\n", balanceFactor);
    if (balanceFactor > 1) {
        if (getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            saveStep(node->left->key, 3, {3}, "L.rotate node->left", code);
            flag = -1;
        } else flag = -2;
        return std::make_pair(rightRotate(node), flag);
    }
    if (balanceFactor < -1) {
        if (getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            saveStep(node->right->key, 3, {5}, "R.rotate node->right", code);
            flag = 1;
        } else flag = 2;
        return std::make_pair(leftRotate(node), flag);
    }
    return std::make_pair(node, flag);
}

void AVLTree::insert(int key, Vector2 start, Vector2 end) {
    clearProcess();
    if (!this->root) {
        this->root = new AVLNode(key, {1013, 150}, {1013, 150});
        saveStep(key, 1, {0}, "Insert at Root.", AVL_INSERT);
        saveStep(-1, -1, {}, "Finish Insert", AVL_INSERT);
        return;
    }

    AVLNode* node = this->root;
    AVLNode* parent = nullptr;
    std::vector<AVLNode*> path;

    while (node) {
        parent = node;
        saveStep(node->key, 0, {0}, "Finding exactly pos...", AVL_INSERT);
        path.push_back(node);
        if (key < node->key) {
            node = node->left;
        } else if (key > node->key) {
            node = node->right;
        } else {
            saveStep(node->key, 0, {0}, TextFormat("%d is already exist in Tree.", key), AVL_INSERT);
            saveStep(-1, -1, {}, "Finish Insert", AVL_INSERT);
            return; // Duplicate key, do nothing
        }
    }
    // printf("--------->%d\n", node->key);
    if (key < parent->key) {
        parent->left = new AVLNode(key, parent->end, Vector2Add(parent->end, (Vector2){-10.f, 50.f}));
        updateHeight(parent);
        saveStep(key, 1, {0}, TextFormat("Insert %d as BST...", key), AVL_INSERT);
    } else {
        parent->right = new AVLNode(key, parent->end,Vector2Add( parent->end, (Vector2){10.f, 50.f}));
        updateHeight(parent);
        saveStep(key, 1, {0}, TextFormat("Insert %d as BST...", key), AVL_INSERT);
    }

    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        AVLNode* current = *it;
        saveStep(current->key, 0, {1}, "Checking balance...", AVL_INSERT);
        updateHeight(current);
        std::pair<AVLNode*, int> balancedNode = balance(current, AVL_INSERT);
        // printf("Step %d -->> %d:\n", path.size(), balancedNode.second);
        // log_node(this->Root);
        // printf("\n");
        if (balancedNode.second != 0 && it + 1 != path.rend()) {
            AVLNode* parent = *(it + 1);
            if (parent->left == current) {
                parent->left = balancedNode.first;
            } else if (parent->right == current) {
                parent->right = balancedNode.first;
            }
        } else if (balancedNode.second != 0 && it + 1 == path.rend()) {
            this->root = balancedNode.first;
        }
        switch (balancedNode.second)
        {
            case -2:
                saveStep(balancedNode.first->key, 3, {2}, "R.rotate node", AVL_INSERT);
                break;
            case -1:
                saveStep(balancedNode.first->key, 3, {3}, "R.rotate node", AVL_INSERT);
                break;
            case 1:
                saveStep(balancedNode.first->key, 3, {5}, "L.rotate node", AVL_INSERT);
                break;
            case 2:
                saveStep(balancedNode.first->key, 3, {4}, "L.rotate node", AVL_INSERT);
                break;
            default:
                break;
        }
    }
    // log_process(this->Process);
    saveStep(-1, -1, {}, "Finish Insert", AVL_INSERT);
}

void AVLTree::createFromFile(const char* filename) {
    freeTree(this->root);
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
    saveStep(-1, -1, {}, "Creating...", "", true);
    saveStep(-1, -1, {}, "Create succesfully", "");
    file.close();
}

void AVLTree::createWithRandomizedData(int n, int range) {
    freeTree(this->root);
    srand((int)time(0));
    for(int i = 0; i < n; i++) {
        insert(rand() % range);
    }
    clearProcess();
    saveStep(-1, -1, {}, "Creating...", "", true);
    saveStep(-1, -1, {}, "Create succesfully", "");
}

void AVLTree::deleteNode(int key) {
    clearProcess();
    if (!this->root) {
        saveStep(0, -1, {0}, "Tree have 0 node!!!", AVL_DELETE);
        saveStep(-1, -1, {}, "Finish Delete", AVL_DELETE);

        return;
    }

    AVLNode* node = this->root;
    AVLNode* parent = nullptr;
    std::vector<AVLNode*> path;

    while (node && node->key != key) {
        parent = node;
        printf("Reaching\n");
        saveStep(node->key, 0, {0}, TextFormat("Finding pos of %d...", key), AVL_DELETE);
        path.push_back(node);
        if (key < node->key) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    if (!node) {
        saveStep(0, -1, {0}, TextFormat("Cannot find %d in tree", key), AVL_DELETE);
        saveStep(-1, -1, {}, "Finish Delete", AVL_DELETE);
        return; // Key not found
    }
    saveStep(node->key, 0, {0}, TextFormat("%d was found", key), AVL_DELETE);

    // case 1: one child
    if (!node->left || !node->right) {
        AVLNode* temp = node->left ? node->left : node->right;
        saveStep(node->key, 2, {0}, "Remove Node From Tree", AVL_DELETE);
        
        if (!parent) {
            this->root = temp;
            saveStep(root->key, 0, {0}, "Remove Node From Tree", AVL_DELETE);
            
        } else if (node == parent->left) {
            parent->left = temp;
            saveStep(key, 0, {0}, "Remove Node From Tree", AVL_DELETE);
        } else if (node == parent->right) {
            parent->right = temp;
            saveStep(key, 0, {0}, "Remove Node From Tree", AVL_DELETE);
        }
        delete node;
    }
    // case 2: two children
    else {
        AVLNode* successorParent = node;
        AVLNode* successor = node->right;

        saveStep(successor->key, 0, {0}, "Find replace value", AVL_DELETE);
        while (successor->left) {
            successorParent = successor;
            path.push_back(successor);
            successor = successor->left;
            saveStep(successor->key, 0, {0}, "Find replace value", AVL_DELETE);
        }

        successor->left = node->left;
        if (successor != node->right) {
            successorParent->left = successor->right;
            successor->right = node->right;
        }
        if (!parent) {
            this->root = successor;
            path.insert(path.begin(), successor);
            saveStep(root->key, 0, {0}, "Swap...", AVL_DELETE);
        }
        else if (node == parent->left) {
            parent->left = successor;
            saveStep(parent->left->key, 0, {0}, "Swap", AVL_DELETE);
        }
        else {
            parent->right = successor;
            saveStep(parent->right->key, 0, {0}, "swap", AVL_DELETE);
        }
        delete node;
    }
    
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        AVLNode* current = *it;
        saveStep(current->key, 0, {1}, "Checking balance...", AVL_DELETE);
        std::pair<AVLNode*, int> balancedNode = balance(current, AVL_DELETE);
        
        if (balancedNode.second != 0 && it + 1 != path.rend()) {
            AVLNode* parent = *(it + 1);
            if (parent->left == current) {
                parent->left = balancedNode.first;
            } else if (parent->right == current) {
                parent->right = balancedNode.first;
            }
        } else if (balancedNode.second != 0 && it + 1 == path.rend()) {
            this->root = balancedNode.first;
        }
        switch (balancedNode.second)
        {
            case -2:
                saveStep(balancedNode.first->key, 3, {2}, "R.rotate node", AVL_DELETE);
                break;
            case -1:
                saveStep(balancedNode.first->key, 3, {3}, "R.rotate node", AVL_DELETE);
                break;
            case 1:
                saveStep(balancedNode.first->key, 3, {5}, "L.rotate node", AVL_DELETE);
                break;
            case 2:
                saveStep(balancedNode.first->key, 3, {4}, "L.rotate node", AVL_DELETE);
                break;
            default:
                break;
        }
    }
    saveStep(-1, -1, {}, "Finish Delete", AVL_INSERT);
}

bool AVLTree::search(int key) {
    clearProcess();
    AVLNode* node = this->root;

    while (node) {
        saveStep(node->key, 0, {2}, TextFormat("Checking..."), AVL_SEARCH);
        //printf("key -> %d\n", node->key);
        if (key == node->key) {
            saveStep(key, 0, {2,3}, TextFormat("%d was found.", key), AVL_SEARCH);
            return true;
        }
        else if (key < node->key) {
            saveStep(node->key, 0, {4,5}, TextFormat("%d < %d: node = node->left", key, node->key), AVL_SEARCH);
            node = node->left;
        } else
        {
            saveStep(node->key, 0, {6,7}, TextFormat("%d > %d: node = node->right", key, node->key), AVL_SEARCH);
            node = node->right;
        }
    }

    saveStep(-1,-1, {0,1}, TextFormat("Tree does not have %d", key), AVL_SEARCH);
    return false;
}


void AVLTree::printInOrder() {
    if (!root) return;

    AVLNode* node = root;
    std::vector<AVLNode*> stack;

    while (node || !stack.empty()) {
        while (node) {
            stack.push_back(node);
            node = node->left;
        }

        node = stack.back();
        stack.pop_back();

        std::cout << node->key << " ";

        node = node->right;
    }
}

AVLTreeVisualize::AVLTreeVisualize(Font font) {
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    this->isInsertChosen = false;
    this->isDeleteChosen = false;
    this->isSearchChosen = false;
    this->stepIndex = 0;
    this->frame = 0;
    this->numFrameOfAnimation = FPS;

    this->createButton = Button({8, 415, 110, 30}, "Create", -1, BLACK, 20, font);
    this->randomButton = Button({156.5, 449.3, 110, 30}, "Random", -1, BLACK, 20, font);
    this->loadFileButton = Button({156.5, 520.6, 110, 30}, "Load File", -1, BLACK, 20, font);
    this->insertButton = Button({8, 458, 110, 30}, "Push", -1, BLACK, 20, font);
    this->deleteButton = Button({8, 504, 110, 30}, "Delete", -1, BLACK, 20, font);
    srand((int)time(0));
    this->inputNumber = InputStr(225, 565, 145, 25, TextFormat("%d", rand() % 100), 20, this->font);
    this->playButton = Button({235, 610, 125, 30}, "Play", -1, BLACK, 20, font);
    this->searchButton = Button({8, 545, 110, 30}, "Top", -1, BLACK, 20, font);
}

void AVLTreeVisualize::updateStep(int index) {
    this->step = this->tree.getProcess()[index];
    this->frame = 0;
    this->stepIndex = index;
    // update_animation();
}

void AVLTreeVisualize::createFromFile() {
    this->tree.createFromFile("Data/data.txt");
    this->numFrameOfAnimation = 5/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void AVLTreeVisualize::createWithRandomizedData(int n, int range) {
    n = 15;
    this->tree.createWithRandomizedData(n, range);
    this->numFrameOfAnimation = 5/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void AVLTreeVisualize::insert() {
    this->tree.insert(std::stoi(this->inputNumber.getText()));
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void AVLTreeVisualize::deleteNode() {
    this->tree.deleteNode(std::stoi(this->inputNumber.getText()));
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void AVLTreeVisualize::search() {
    this->tree.search(std::stoi(this->inputNumber.getText()));
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void drawNode(AVLNode *root, int specialValue, int frame, int numFrame, Font font, bool isNotification = false) {
    if (!root) return;
    Vector2 root_pos = Vector2Lerp(root->start, root->end, float(frame)/numFrame);
    if (root->left) DrawLineEx(root_pos, Vector2Lerp(root->left->start, root->left->end, float(frame)/numFrame), 2, NODE_COLOR);
    if (root->right) DrawLineEx(root_pos, Vector2Lerp(root->right->start, root->right->end, float(frame)/numFrame), 2, NODE_COLOR);
    DrawCircle(root_pos.x, root_pos.y, NODE_RADIUS, (root->key == specialValue && isNotification) ? HIGHLIGHT_NODE_COLOR_1 : NODE_COLOR);
    Vector2 text_size = MeasureTextEx(font, TextFormat("%d", root->key), CODE_SIZE, 0);
    DrawTextPro(font, TextFormat("%d", root->key), {root_pos.x - text_size.x/2, root_pos.y - text_size.y/2}, {0.f, 0.f}, 0, CODE_SIZE, 0, WHITE);
    drawNode(root->left, specialValue, frame, numFrame, font, isNotification);
    drawNode(root->right, specialValue, frame, numFrame, font, isNotification);
}

void AVLTreeVisualize::drawTree() {
    if (this->tree.getProcess().empty()) return;
    drawNode(this->step.root, this->step.highlight, this->frame, this->numFrameOfAnimation, this->font, this->step.type == -1);
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

void AVLTreeVisualize::drawButtons() {
    this->createButton.draw(50);
    this->deleteButton.draw(50);
    this->insertButton.draw(50);
    this->searchButton.draw(50);

    if(this->isCreateChosen) {
        this->randomButton.draw();
        this->loadFileButton.draw();
    }

    if(this->isDeleteChosen || this->isInsertChosen || this->isSearchChosen) {
        this->inputNumber.draw();
        this->inputNumber.update();
        this->playButton.draw();
    }
}

int AVLTreeVisualize::handle() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (this->createButton.getIsHovered()) {
            this->isCreateChosen = true;
            this->isDeleteChosen = false;
            this->isInsertChosen = false;
            this->isSearchChosen = false;
        }
        if (this->deleteButton.getIsHovered()) {
            this->inputNumber.resetText();
            this->isCreateChosen = false;
            this->isDeleteChosen = true;
            this->isInsertChosen = false;
            this->isSearchChosen = false;
        }
        if (this->insertButton.getIsHovered()) {
            this->inputNumber.resetText();
            this->isCreateChosen = false;
            this->isDeleteChosen = false;
            this->isInsertChosen = true;
            this->isSearchChosen = false;
        }
        if (this->searchButton.getIsHovered()) {
            this->inputNumber.resetText();
            this->isCreateChosen = false;
            this->isDeleteChosen = false;
            this->isInsertChosen = false;
            this->isSearchChosen = true;
        }
    }

    if (this->playButton.handle()) {
        if (this->isInsertChosen) {
            insert();
            printf("INSERTING...\n");
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
        createWithRandomizedData(20, 100);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    if (this->loadFileButton.handle()) {
        createFromFile();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }

    int flag = this->progressBar.handle();
    if ( flag != 10) {
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
            if (this->type == 0) {
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

void AVLTreeVisualize::draw() {
    
    //drawSideBar(type, this->step.code, this->step.line, this->step.infor, this->progressBar, this->font);
    drawButtons();
    drawTree();
}
