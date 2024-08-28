#include "HashTable.hpp"

void HashTable::saveStep(int hightlightindex, int type, std::vector<int> line, const std::string infor, const std::string code, bool forCreate) {
    this->process.push_back({table, line, hightlightindex, infor, code, type});
}

int HashTable::hash(int key)  {
    return key % capacity;
}

int HashTable::linearProbe(int key) {
    int index = hash(key);
    int originalIndex = index;

    while (table[index] != -1 && table[index] != -2 && table[index] != key) {
        saveStep(index, 0, {}, "Finding suitable position!", "", 1);
        index = (index + 1) % capacity;
        if (index == originalIndex) {
            return -1;
        }
    }
    return index;
}

void HashTable::resize(int k) {
    this->process.clear();
    this->capacity = k;
    this->size = 0;
    this->table.clear();
    this->table.resize(k, -1);
    saveStep(-1, 0, {}, "Reset table and resize!", "", 1);
}

void HashTable::insert(int key) {
    this->process.clear();
    int exist = search(key);
    if (exist != -1) {
        saveStep(exist, 0, {}, "Key already exist!", "", 1);
        std::cout << "Key already exists: " << key << std::endl;
        return;
    }
    this->process.clear();

    int index = linearProbe(key);
    if (index == -1) {
        saveStep(-1, 0, {}, "Memory Fault!", "", 1);
        std::cout << "Hash table is full. Cannot insert key: " << key << std::endl;
        return;
    }

    if (table[index] == -1 || table[index] == -2) {
        table[index] = key;
        saveStep(index, 0, {}, "Insert Key!", "", 1);
        size++;
    }
}

void HashTable::deleteKey(int key) {
    this->process.clear();
    int index = hash(key);
    int originalIndex = index;

    while (table[index] != -1) {
        saveStep(index, 0, {}, "Finding key!", "", 1);
        if (table[index] == key) {
            table[index] = -2;  // Mark as deleted
            saveStep(index, 0, {}, "Delete key!", "", 1);
            size--;
            saveStep(-1, -1, {}, "Finish!", "", 1);
            return;
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) {
            break;
        }
    }
    saveStep(-1, -1, {}, "NOT FOUND!", "", 1);
    std::cout << "Key not found: " << key << std::endl;
}

int HashTable::search(int key) {
    this->process.clear();
    int index = hash(key);
    int originalIndex = index;

    while (table[index] != -1) {
        saveStep(index, 0, {}, "Finding key!", "", 1);
        if (table[index] == key) {
            saveStep(index, 0, {}, "FOUND!", "", 1);
            return index;
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) {
            break;
        }
    }
    saveStep(-1, 0, {}, "NOT FOUND!", "", 1);

    return -1;
}

void HashTable::createFromFile(const std::string& filename) {
    clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return;
    }

    int key;
    while (file >> key) {
        insert(key);
    }
    file.close();
    this->process.clear();
    saveStep(-1, 0, {}, "Creating...", "", true);
}

void HashTable::createRandom(int numKeys, int maxValue) {
    clear();
    srand((int)time(0));
    for (int i = 0; i < numKeys; ++i) {
        int key = rand() % maxValue;
        insert(key);
    }
    this->process.clear();
    saveStep(-1, 0, {}, "Creating...", "", true);
}

HashTableVisualize::HashTableVisualize(Font font) {
    this->table = HashTable(40);
    this->step = this->table.getProcess().front();
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    this->isInsertChosen = false;
    this->isDeleteChosen = false;
    this->isSearchChosen = false;
    this->stepIndex = 0;
    this->frame = 0;
    this->numFrameOfAnimation = FPS;

    this->inputSize = InputStr(156.5, 422, 110, 30, "", 20, this->font);
    this->resizeButton = Button({156.5, 466, 110, 30}, "Resize", -1, BLACK, 20, font);
    this->createButton = Button({8, 415, 110, 30}, "Create", -1, BLACK, 20, font);
    this->randomButton = Button({156.5, 511, 110, 30}, "Random", -1, BLACK, 20, font);
    this->loadFileButton = Button({156.5, 552, 110, 30}, "Load File", -1, BLACK, 20, font);
    this->insertButton = Button({8, 458, 110, 30}, "Insert", -1, BLACK, 20, font);
    this->deleteButton = Button({8, 504, 110, 30}, "Delete", -1, BLACK, 20, font);
    this->inputNumber = InputStr(151.5, 422, 120, 25, "", 20, this->font);
    this->playButton = Button({173, 492, 70, 30}, "Play", -1, BLACK, 20, font);
    this->searchButton = Button({8, 545, 110, 30}, "Search", -1, BLACK, 20, font);
}

void HashTableVisualize::updateStep(int index) {
    this->step = this->table.getProcess()[index];
    this->frame = 0;
    this->stepIndex = index;
}

void HashTableVisualize::resize() {
    this->table.resize(std::stoi(this->inputSize.getText()));

    this->numFrameOfAnimation = 5/this->progressBar.getSpeed();
    this->step = this->table.getProcess().front();
    this->progressBar.updateMaxStep((int)this->table.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void HashTableVisualize::createFromFile() {
    auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                           { "Text Files (.txt .text)", "*.txt *.text",
                               "All Files", "*" },
                           pfd::opt::force_path);
    if (f.result().empty()) {
        return;
    }
       
    auto path = f.result().back();
    
    this->table.createFromFile(path.c_str());
    this->numFrameOfAnimation = 5/this->progressBar.getSpeed();

    this->step = this->table.getProcess().front();
    this->progressBar.updateMaxStep((int)this->table.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void HashTableVisualize::createWithRandomizedData(int n, int range) {
    this->table.createRandom(n, range);

    this->numFrameOfAnimation = 5/this->progressBar.getSpeed();

    this->step = this->table.getProcess().front();
    this->progressBar.updateMaxStep((int)this->table.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void HashTableVisualize::insert() {
    this->table.insert(std::stoi(this->inputNumber.getText()));
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->table.getProcess().front();
    this->progressBar.updateMaxStep((int)this->table.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void HashTableVisualize::deleteNode() {
    this->table.deleteKey(std::stoi(this->inputNumber.getText()));
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->table.getProcess().front();
    this->progressBar.updateMaxStep((int)this->table.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void HashTableVisualize::search() {
    this->table.search(std::stoi(this->inputNumber.getText()));
    this->numFrameOfAnimation = 60/this->progressBar.getSpeed();

    this->step = this->table.getProcess().front();
    this->progressBar.updateMaxStep((int)this->table.getProcess().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void HashTableVisualize::drawTable() {
    if (this->table.getProcess().empty()) return;

    for (int i = 0; i < this->step.table.size(); i++) {
        int x = 350 + i % 10 * 80;
        int y = 150 + i / 10 * 80;
        DrawCircle(x, y, 15, this->step.highlightIndex == i ? THEME.HIGHLIGHT_NODE_1 : THEME.NODE);

        Vector2 size = MeasureTextEx(this->font, TextFormat("%d", i), CODE_SIZE - 2, 2);
        
        DrawTextEx(this->font, TextFormat("%d", i), {x - size.x/2, y + 25 - size.y/2}, CODE_SIZE - 2, 2, THEME.WEIGHT);

        if (this->step.table[i] >= 0) {
            size = MeasureTextEx(this->font, TextFormat("%d", this->step.table[i]), CODE_SIZE, 2);
            DrawTextEx(this->font, TextFormat("%d", this->step.table[i]), {x - size.x/2, y - size.y/2}, CODE_SIZE, 2, BLACK);
        }
    }

    if (this->type == 2 || this->type == 0) return;
    this->frame++;
    if(this->frame >= this->numFrameOfAnimation && !this->table.getProcess().empty()) {
        if (stepIndex == this->table.getProcess().size() - 1) {
            this->type = 2;
            return;
        }
        updateStep(this->stepIndex + 1);
        this->progressBar.updateStep(1);
    }
}

void HashTableVisualize::drawButtons() {
    this->createButton.draw(50);
    this->deleteButton.draw(50);
    this->insertButton.draw(50);
    this->searchButton.draw(50);

    if(this->isCreateChosen) {
        this->randomButton.draw(50);
        this->loadFileButton.draw(50);
        DrawLineEx({126, 501}, {297, 501}, 1.3, THEME.SEPERATOR);
        this->inputSize.draw();
        this->inputSize.update();
        this->resizeButton.draw(50);
    }

    if(this->isDeleteChosen || this->isInsertChosen || this->isSearchChosen) {
        this->inputNumber.draw();
        this->inputNumber.update();
        this->playButton.draw();
    }
}

int HashTableVisualize::handle() {
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
        createWithRandomizedData(20, 100);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    if (this->loadFileButton.handle()) {
        createFromFile();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 5;
    }
    if(this->resizeButton.handle()) {
        resize();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return -10;
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
            if(stepIndex == this->table.getProcess().size() - 1) break;
            this->progressBar.updateStep(1);
            updateStep(this->stepIndex + 1);
            break;

        case 2:
            if (this->progressBar.getMaxStep() == 0) break;
            updateStep((int)this->table.getProcess().size() - 1);
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
            if (this->type == 0 && this->table.getProcess().size()) {
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

void HashTableVisualize::draw() {
    drawSideBar(this->type, this->step.code, this->step.line, this->step.infor, this->progressBar, this->font);
    drawButtons();
    drawTable();
}
