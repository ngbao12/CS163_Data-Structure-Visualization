#include "Trie.hpp"

void Trie::freeTrie(TrieNode* &node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            freeTrie(node->children[i]);
        }
    }
    if (node->character) {
        delete node;
        node = nullptr;
    }
}

void Trie::freeStep(TrieStep &st) {
    freeTrie(st.root);
    st.line.clear();
    st.highlight = nullptr;
}

void Trie::clearProcess() {
    for (auto it = this->process.begin(); it != this->process.end(); ++it) {
        freeStep(*it);
    }
    this->process.clear();
}

int Trie::estimateWidth(TrieNode* root) {
    if (!root) return 0;
    if (root->numOfChild == 0) {
        root->width = 1;
        return 1;
    }
    int width = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            width += estimateWidth(root->children[i]);
        }
    }
    root->width = width;
    return width;
}

void Trie::updatePos(TrieNode* root, bool updateStart, bool forCreate, Vector2 parentPos, Vector2 delta) {
    if (!root) return;
    if (forCreate) root->start = {779.f, 124.f};
    if (updateStart && !forCreate) {
        root->start = Vector2Add(parentPos, delta);
    }
    root->end = Vector2Add(parentPos, delta);
    
    float pos = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(!root->children[i]) continue;
        int childWidth = root->children[i]->width;
        updatePos(root->children[i], updateStart, forCreate, root->end, {50.f * (pos + float(childWidth)/2 - float(root->width)/2), 60.f});
        pos += childWidth;
    }
}

void Trie::saveStep(TrieNode* highlight, int type, std::vector<int> lines, const std::string infor, const std::string code, bool forCreate) {
    std::pair<TrieNode*, TrieNode*> newTrie = cloneTrie(this->root, highlight);
    estimateWidth(newTrie.first);
    updatePos(newTrie.first, false, forCreate, {779.f, 124.f}, {0.f, 0.f});
    this->process.push_back({newTrie.first, lines, newTrie.second, infor, code, type});
    estimateWidth(this->root);
    updatePos(this->root, true, false);
}

std::pair<TrieNode*, TrieNode*> Trie::cloneTrie(TrieNode* root, TrieNode* highlight) {
    if (!root) return {nullptr, nullptr};

    TrieNode* newNode = new TrieNode(root->character, root->start, root->end);
    newNode->isEndStr = root->isEndStr;
    newNode->numOfChild = root->numOfChild;

    std::pair<TrieNode*, TrieNode*> newElement = {newNode, nullptr};

    for(int i = 0; i < ALPHABET_SIZE; i++) {
        std::pair<TrieNode*, TrieNode*> tmp = cloneTrie(root->children[i], highlight);
        newElement.first->children[i] = tmp.first;
        if (tmp.second) {
            newElement.second = tmp.second;
        }
    }

    if (root == highlight) {
        newElement.second = newNode;
    }

    return newElement;
}

bool Trie::deleteWord(TrieNode* node, const std::string& key, int depth) {
    if (!node) {
        saveStep(nullptr, -1, {1}, "Not Found Str", Trie_DELETE);
        return false;
    }

    if (depth == key.size()) {
        if (node->isEndStr){
            node->isEndStr = false;
            saveStep(node, 0, {2}, "remove str...", Trie_DELETE);
        }

        return node->isEndStr == false && !node->numOfChild;
    }

    int index = key[depth] - 'a';
    saveStep(node, 0, {0}, "Searching Str...", Trie_DELETE);
    if (deleteWord(node->children[index], key, depth + 1)) {
        delete node->children[index];
        node->numOfChild--;
        node->children[index] = nullptr;
        saveStep(node->children[index], 0, {3,4,5,6,7}, "remove non_exist node...", Trie_DELETE);
        return !node->isEndStr && !node->numOfChild;
    }

    return false;
}

void Trie::insert(std::string key) {
    clearProcess();
    TrieNode* node = this->root;
    saveStep(node, 0, {0}, "set node <- root", Trie_INSERT);
    for (int i = 0; i < key.length(); i++) {
        saveStep(nullptr, 0, {1}, "for ch in str", Trie_INSERT);
        int index = key[i] - 'a';
        if (!node->children[index]){
            node->children[index] = new TrieNode(key[i], node->end);
            saveStep(node->children[index], 1, {2,3}, TextFormat("insert node with char %c", key[i]), Trie_INSERT);
        }
        node->numOfChild++;
        node = node->children[index];
        saveStep(node, 0, {4,5}, TextFormat("update node"), Trie_INSERT);
    }
    node->isEndStr = true;
    saveStep(node, 0, {6}, TextFormat("node->isEnd = true"), Trie_INSERT);
}

bool Trie::search(std::string key) {
    clearProcess();
    TrieNode* node = this->root;
    saveStep(node, 0, {0}, "Searching...", Trie_SEARCH);
    for (int i = 0; i < key.length(); i++) {
        saveStep(node, 0, {1}, TextFormat("Checking %c", key[i]), Trie_SEARCH);
        int index = key[i] - 'a';
        if (!node->children[index]){
            saveStep(nullptr, -1, {2,3}, "NOT_FOUND", Trie_SEARCH);
            return false;
        }
        node = node->children[index];
        saveStep(node, 0, {4}, "next char", Trie_SEARCH);
    }
    if (node->isEndStr) {
        saveStep(node, 0, {5}, "FOUND", Trie_SEARCH);
    } else {
        saveStep(nullptr, -1, {6}, "NOT_FOUND", Trie_SEARCH);
    }
    return (node != nullptr && node->isEndStr);
}

void Trie::createFromFile(const char* filename) {
    freeTrie(this->root);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string word;
    while (std::getline(file, word)) {
        if (!word.empty()) {
            insert(word);
        }
    }
    
    file.close();
    printf("DONE");
    clearProcess();
    saveStep(nullptr, -1, {}, "Creating...", "", true);
}

void Trie::createRandom(int numWords, int maxLength) {
    freeTrie(this->root);
    srand(static_cast<unsigned int>(time(nullptr)));
    
    for (int i=0; i<numWords; i++) {
        int length = rand() % maxLength + 1;
        std::string word;
        for (int i=0; i<length; i++) {
            word += 'a' + rand() % 26;
        }
        insert(word);
    }
    clearProcess();
    saveStep(nullptr, -1, {}, "Creating...", "", true);
}

TrieVisualize::TrieVisualize(Font font) {
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    this->isInsertChosen = false;
    this->isDeleteChosen = false;
    this->isSearchChosen = false;
    this->stepIndex = 0;
    this->frame = 0;
    this->isPause = false;
    this->numFrameOfAnimation = FPS;

    this->createButton = Button({8, 415, 110, 30}, "Create", -1, BLACK, 20, font);
    this->randomButton = Button({156.5, 449.3, 110, 30}, "Random", -1, BLACK, 20, font);
    this->loadFileButton = Button({156.5, 520.6, 110, 30}, "Load File", -1, BLACK, 20, font);
    this->insertButton = Button({8, 458, 110, 30}, "Insert", -1, BLACK, 20, font);
    this->deleteButton = Button({8, 504, 110, 30}, "Delete", -1, BLACK, 20, font);
    srand((int)time(0));
    this->inputNumber = InputStr(156.5, 449.3, 110, 30, TextFormat("%d", rand() % 100), 20, this->font);
    this->playButton = Button({173, 492, 70, 30}, "Play", -1, BLACK, 20, font);
    this->searchButton = Button({8, 545, 110, 30}, "Search", -1, BLACK, 20, font);

}

void TrieVisualize::updateStep(int index) {
    this->step = this->tree.getProcess()[index];
    this->frame = 0;
    this->stepIndex = index;
}

void TrieVisualize::createFromFile() {
    this->tree.createFromFile("Data/input.txt");
    this->numFrameOfAnimation = 10/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
}

void TrieVisualize::createWithRandomizedData(int n, int length) {
    this->tree.createRandom(n, length);
    this->numFrameOfAnimation = 10/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
}

void TrieVisualize::insert() {
    this->tree.insert(this->inputNumber.getText());
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
}

void TrieVisualize::deleteNode() {
    this->tree.deleteStr(this->inputNumber.getText());
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
}

void TrieVisualize::search() {
    this->tree.search(this->inputNumber.getText());
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->tree.getProcess().front();
    this->progressBar.updateMaxStep((int)this->tree.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
}

void drawNode(TrieNode *root, TrieNode* highlight, int frame, int numFrame, Font font, bool isNotification = false) {
    if (!root) return;
    Vector2 rootPos = Vector2Lerp(root->start, root->end, float(frame)/numFrame);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(root->children[i]) {
            DrawLineEx(rootPos, Vector2Lerp(root->children[i]->start, root->children[i]->end, float(frame)/numFrame), 2, NODE_COLOR);
        }
    }
    Color color = NODE_COLOR;
    if(root->isEndStr) color = HIGHLIGHT_NODE_COLOR_2;
    if(root == highlight && !isNotification) color = HIGHLIGHT_NODE_COLOR_1;
    DrawCircle(rootPos.x, rootPos.y, NODE_RADIUS, color);
    Vector2 textSize = MeasureTextEx(font, TextFormat("%c", root->character), CODE_SIZE, 0);
    DrawTextPro(font, TextFormat("%c", root->character), {rootPos.x - textSize.x/2, rootPos.y - textSize.y/2}, {0.f, 0.f}, 0, CODE_SIZE, 0, WHITE);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if(root->children[i]) {
            drawNode(root->children[i], highlight, frame, numFrame, font, isNotification);
        }
    }
}

void TrieVisualize::drawTree() {
    if (this->tree.empty()) {
        DrawCircle(779, 124, NODE_RADIUS, NODE_COLOR);
        return;
    }
    if (this->tree.getProcess().empty()) return;
    drawNode(this->step.root, this->step.highlight, this->frame, this->numFrameOfAnimation, this->font, this->step.type == -1);
    
    if (this->isPause) {
        return;
    }
    this->frame++;
    if(this->frame >= this->numFrameOfAnimation && !this->tree.getProcess().empty()) {
        if (stepIndex == this->tree.getProcess().size() - 1) {
            this->frame--;
            return;
        }
        updateStep(this->stepIndex + 1);
        this->progressBar.updateStep(1);
    }

}

void TrieVisualize::drawButtons() {
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

int TrieVisualize::handle() {
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

        case 0:
            this->isPause = !this->isPause;
            break;

        case 3: case -3:
            this->numFrameOfAnimation = FPS/this->progressBar.getSpeed();

            break;

        default:
            break;
    }

    return 0;
}

void TrieVisualize::draw() {
    drawSideBar(this->step.code, this->step.line, this->step.infor, this->progressBar, this->font);
    drawButtons();
    drawTree();
}

