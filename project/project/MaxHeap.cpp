#include "MaxHeap.hpp"

void Max_Heap::drawEdge(Vector2 pos1, Vector2 pos2) {
    DrawLineEx(pos1, pos2, 2, THEME.LINE);
}

void Max_Heap::drawNode(std::vector<Node> scene, int index, Font font) {
    int size =(int) scene.size();
    int left = 2*index + 1, right = 2*index + 2;
    if (left < size) drawEdge(scene[index].pos, scene[left].pos);
    if (right < size) drawEdge(scene[index].pos, scene[right].pos);
    DrawCircle(scene[index].pos.x, scene[index].pos.y, NODE_RADIUS, scene[index].color);
    if(scene[index].fontSize == 0) return;
    Vector2 text_size = MeasureTextEx(font, TextFormat("%d", scene[index].value), scene[index].fontSize, 0);
    DrawTextPro(font, TextFormat("%d", scene[index].value), {scene[index].pos.x - text_size.x/2, scene[index].pos.y - text_size.y/2}, {0.f, 0.f}, 0, scene[index].fontSize, 0, BLACK);
}

void Max_Heap::drawHeap(Step step, Font font) {
    int size =(int) step.nodes.size();
    for (int i = 0; i < size; i++) {
        drawNode(step.nodes, i, font);
    }
}

void MaxHeap::saveStep(int index1, int index2, std::vector<int> lines, const std::string infor, std::string code, int type) {
    
    if (this->data.empty()) {
            this->steps.push_back({{}, {}, {-1, -1}, "Heap is empty!", "", -1});
            return;
    }
    int size =(int) this->data.size();
    Vector2 pos = {779, 124};
    float dx = 50*log2(this->data.size()), dy = 100;
    std::vector<Max_Heap::Node> nodes;
    nodes.push_back({this->data[0], pos, dx, CODE_SIZE, THEME.NODE});

    for (int i = 1; i < size; i++) {
        dx = nodes[(i - 1) / 2].dx;
        pos = nodes[(i - 1) / 2].pos;
        nodes.push_back({this->data[i], pos.x + ((i & 1) ? (-dx) : dx), pos.y + dy, dx / 2, CODE_SIZE, THEME.NODE});
    }

    if (type == 0 || type == 1 || type == 2) {
        nodes[index2].color = THEME.HIGHLIGHT_NODE_2;
        nodes[index1].color = THEME.HIGHLIGHT_NODE_1;
    }
    if (type == 3 || type == 4) {
        index2 = index1 ? (index1-1)/2 : 0;
    }
    this->steps.push_back({nodes, lines, {index1, index2}, infor, code, type});
}

void MaxHeap::heapifyDown(int index) {
    int size =(int) data.size();
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && this->data[left] > this->data[largest]) {
        largest = left;
    }

    if (right < size && this->data[right] > this->data[largest]) {
        largest = right;
    }

    if (largest == index) {
        return;
    }

    if (largest != index) {
        std::swap(this->data[index], this->data[largest]);
        heapifyDown(largest);
    }
}

int MaxHeap::createFromFile(const char* filename) {
    this->steps.clear();

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "File could not be opened!" << std::endl;
        return 0;
    }
    this->data.clear();
    int number;
    while (infile >> number) {
        this->data.push_back(number);
        int i = (int)this->data.size() - 1;
        saveStep(i, -1, {}, "Creating...", "", 3);
    }
    int size = (int)this->data.size();
    for (int i = size / 2 - 1; i >= 0; --i) {
        heapifyDown(i);
    }
    saveStep(-1, -1, {}, "Create from file succesfully!", "", -1);
    return 1;
}

void MaxHeap::createWithRandomizedData(int n, int range) {
    this->steps.clear();
    srand((int)time(0));
    n = rand() % n;
    this->data.clear();
    for (int i = 0; i < n; i++) {
        this->data.push_back(rand() % range);
        saveStep(i, -1, {}, "Creating...", "", 3);
    }
    for (int i = (int)this->data.size() / 2 - 1; i >= 0; --i) {
        heapifyDown(i);
    }
    saveStep(-1, -1, {}, "Create with randomized data succesfully!", "", -1);
}

void MaxHeap::push(int value) {
    this->steps.clear();

    this->data.push_back(value);
    int index = (int)this->data.size() - 1;
    saveStep(index, -1, {0, 1}, TextFormat("%d is inserted at the back of compact array A.", value), Push_code, 3);
    while (index > 0 && this->data[index] > this->data[(index - 1)/2]) {
        saveStep(index, (index - 1)/2, {2}, "Comparing between current node and its parent.", Push_code, 0);
        saveStep(index, (index - 1)/2, {3}, TextFormat("%d > %d, so swap them.\n Update index.", this->data[index], this->data[(index-1)/2]), Push_code, 1);
        std::swap(this->data[index], this->data[(index-1)/2]);
        index = (index - 1)/2;
    }
    saveStep(-1, -1, {}, TextFormat("The processing is completed.\n%d is push in to the Heap.\n", value), Push_code, -1);
}

int MaxHeap::deleteElement(int value) {
    this->steps.clear();

    int size = (int)this->data.size(), i = 0;
    saveStep(-1, -1, {0}, TextFormat("Starting delete processing...", value), Delete_code, -1);
    for(i = 0; i < size && this->data[i] != value; i++) {
        saveStep(i, i, {1}, TextFormat("Finding %d in Heap...", value), Delete_code, 0);
        if (this->data[i] == value) break;
    }
    if (i == size) {
        saveStep(-1, -1, {2}, TextFormat("%d is not in Heap!", value), Delete_code, -1);
        return 0;
    }
    saveStep(i, i, {1}, TextFormat("%d is at index %d", value, i), Delete_code, 0);
    this->data[i] = this->data[0] + 1;
    saveStep(i, i, {3}, "Value at index i is update to new max.\n(this value is A[0] + 1)\nStarting heapify...", Delete_code, 0);
    while (i > 0 && this->data[(i-1)/2] < this->data[i]) {
        saveStep(i, (i-1)/2, {4}, "Comparing between current node and its parrent.", Delete_code, 0);
        saveStep(i, (i-1)/2, {5}, TextFormat("%d > %d, so swap them.\n Update index i.", this->data[i], this->data[(i-1)/2]), Delete_code, 1);
        std::swap(this->data[i], this->data[(i-1)/2]);
        i = (i - 1)/2;
    }

    this->data[0] = this->data[size - 1];
    saveStep(0, 0, {6}, "Change root value to value of last element.", Delete_code, 2);
    saveStep(size - 1, -1, {7}, "Remove last element.", Delete_code, 4);
    this->data.pop_back();
    i = 0;
    while (true) {
        int largest = i;
        if (2*i+1 < size && this->data[largest] < this->data[2*i+1]) largest = 2*i+1;
        saveStep(i, 2*i+1, {8}, TextFormat("Find the largest node of current node\n and it's child node..."), Delete_code, 0);

        if (2*i+2 < size && this->data[largest] < this->data[2*i+2]) largest = 2*i+2;
        saveStep(i, 2*i+2, {8}, TextFormat("Find the largest node of current node\n and it's child node..."), Delete_code, 0);
        saveStep(i, largest, {8}, TextFormat("The largest node is index %d, with value %d.", largest, this->data[largest]), Delete_code, 0);
        if (i == largest) break;
        saveStep(i, largest, {9}, TextFormat("Swap two values at indexs %d and %d.\nChange current index i to largest.", i, largest), Delete_code, 1);
        std::swap(this->data[i], this->data[largest]);
        i = largest;
    }
    saveStep(-1, -1, {}, TextFormat("The processing is completed.\n%d has removed from the Heap.", value), Delete_code, -1);
    return 1;
}

void MaxHeap::top() {
    this->steps.clear();
    if(this->data.empty()) {
        saveStep(-1, -1, {0}, TextFormat("Top of the Heap is: NULL."), Top_code, -1);
        return;
    }
    saveStep(0, -1, {0}, TextFormat("Top of the Heap is: %d.", this->data[0]), Top_code, 0);
}

void MaxHeap::size() {
    this->steps.clear();

    saveStep(-1, -1, {0}, TextFormat("Size of the Heap is: %d.", this->data.size()), Size_code, -1);
}

MaxHeapVisualize::MaxHeapVisualize(Font font) {
    this->font = font;
    this->progressBar = ProgressBar(font);
    this->isCreateChosen = false;
    this->isPushChosen = false;
    this->isDeleteChosen = false;
    this->stepIndex = 0;
    this->frame = 0;
    this->type = 0;
    this->numFrameOfAnimation = FPS;

    this->createButton = Button({8, 415, 110, 30}, "Create", -1, BLACK, 20, font);
    this->randomButton = Button({156.5, 449.3, 110, 30}, "Random", -1, BLACK, 20, font);                        /////////////////
    this->loadFileButton = Button({156.5, 520.6, 110, 30}, "Load File", -1, BLACK, 20, font);                       ///////////
    this->pushButton = Button({8, 450, 110, 30}, "Push", -1, BLACK, 20, font);
    this->deleteButton = Button({8, 485, 110, 30}, "Delete", -1, BLACK, 20, font);
    this->inputNumber = InputStr(156.5, 449.3, 110, 30, "", 20, this->font);        /////////////////////////////////
    this->playButton = Button({173, 492, 70, 30}, "Play", -1, BLACK, 20, font);                             /////////////////////////////////
    this->topButton = Button({8, 555, 110, 30}, "Top", -1, BLACK, 20, font);
    this->sizeButton = Button({8, 520, 110, 30}, "Size", -1, BLACK, 20, font);
}

void MaxHeapVisualize::updateStep(int index) {
    this->animation = this->heap.getSteps()[index];
    this->frame = 0;
    this->stepIndex = index;
    updateAnimation();
}

int MaxHeapVisualize::updateAnimation() {
    if (this->type == 2 || this->type == 0) return 0;
    this->frame++;
    int flag = 0;
    if(this->frame >= this->numFrameOfAnimation && !this->heap.getSteps().empty()) {
        if (stepIndex == this->heap.getSteps().size() - 1) {
            this->type = 2;
            return 0;
        }
        updateStep(this->stepIndex + 1);
        this->progressBar.updateStep(1);
        flag = 1;
    }

    switch (this->animation.type)
    {
        case 1:{
            Vector2 pos1 = this->heap.getSteps()[this->stepIndex].nodes[this->animation.highlight[0]].pos;
            Vector2 pos2 = this->heap.getSteps()[this->stepIndex].nodes[this->animation.highlight[1]].pos;
            this->animation.nodes[this->animation.highlight[0]].pos = Vector2Lerp(pos1, pos2, float(this->frame)/this->numFrameOfAnimation);
            this->animation.nodes[this->animation.highlight[1]].pos = Vector2Lerp(pos2, pos1, float(this->frame)/this->numFrameOfAnimation);
            break;
        }

        case 2:{
            if(this->frame < this->numFrameOfAnimation/2){
                this->animation.nodes[this->animation.highlight[0]].fontSize = FloatLerp(CODE_SIZE, 0, 2*float(this->frame)/this->numFrameOfAnimation);
            } else {
                this->animation.nodes[this->animation.highlight[0]].fontSize = FloatLerp(0, CODE_SIZE, 1.f - 2*float(this->frame)/this->numFrameOfAnimation);
            }
            break;
        }

        case 3:{
            Vector2 pos1 = this->heap.getSteps()[this->stepIndex].nodes[this->animation.highlight[1]].pos;
            Vector2 pos2 = this->heap.getSteps()[this->stepIndex].nodes[this->animation.highlight[0]].pos;
            this->animation.nodes[this->animation.highlight[0]].pos = Vector2Lerp(pos1, pos2, float(this->frame)/this->numFrameOfAnimation);
            this->animation.nodes[this->animation.highlight[0]].fontSize = FloatLerp(0, CODE_SIZE, float(this->frame)/this->numFrameOfAnimation);
            break;
        }

        case 4:{
            Vector2 pos1 = this->heap.getSteps()[this->stepIndex].nodes[this->animation.highlight[1]].pos;
            Vector2 pos2 = this->heap.getSteps()[this->stepIndex].nodes[this->animation.highlight[0]].pos;
            this->animation.nodes[this->animation.highlight[0]].pos = Vector2Lerp(pos2, pos1, float(this->frame)/this->numFrameOfAnimation);
            this->animation.nodes[this->animation.highlight[0]].fontSize = FloatLerp(CODE_SIZE, 0, float(this->frame)/this->numFrameOfAnimation);
            break;
        }

        default:
            break;
    }
    return flag;
}

void MaxHeapVisualize::createWithRandomizedData(int n, int range) {
    this->heap.createWithRandomizedData(n, range);
    this->numFrameOfAnimation = 3;
    this->animation = this->heap.getSteps().front();
    this->progressBar.updateMaxStep((int)this->heap.getSteps().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void MaxHeapVisualize::createFromFile() {
    auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                           { "Text Files (.txt .text)", "*.txt *.text",
                               "All Files", "*" },
                           pfd::opt::force_path);
    if (f.result().empty()) {
        return;
    }
    auto path = f.result().back();
    this->heap.createFromFile(path.c_str());
    this->numFrameOfAnimation = 3;
    this->animation = this->heap.getSteps().front();
    this->progressBar.updateMaxStep((int)this->heap.getSteps().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void MaxHeapVisualize::push() {
    std::string text = this->inputNumber.getText();
    if (text.empty()) return;
    this->heap.push(std::stoi(text));
    this->numFrameOfAnimation = 60;

    this->animation = this->heap.getSteps().front();
    this->progressBar.updateMaxStep((int)this->heap.getSteps().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void MaxHeapVisualize::deleteNode() {
    std::string text = this->inputNumber.getText();
    if (text.empty()) return;
    this->heap.deleteElement(std::stoi(text));
    this->numFrameOfAnimation = 60;

    this->animation = this->heap.getSteps().front();
    this->progressBar.updateMaxStep((int)this->heap.getSteps().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void MaxHeapVisualize::getTop() {
    this->heap.top();
    this->numFrameOfAnimation = 60;
    this->animation = this->heap.getSteps().front();
    this->progressBar.updateMaxStep((int)this->heap.getSteps().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
    this->type = 1;
}

void MaxHeapVisualize::getSize() {
    this->heap.size();
    this->numFrameOfAnimation = 60;

    this->animation = this->heap.getSteps().front();
    this->progressBar.updateMaxStep((int)this->heap.getSteps().size() - 1);
    this->stepIndex = 0;
    this->frame = 0;
    this->progressBar.updateStep(0);
}

void MaxHeapVisualize::drawButtons() {
    this->createButton.draw(50);
    this->deleteButton.draw(50);
    this->pushButton.draw(50);
    this->topButton.draw(50);
    this->sizeButton.draw(50);
    if(this->isCreateChosen) {
        this->randomButton.draw();
        this->loadFileButton.draw();
    }
    if(this->isDeleteChosen || this->isPushChosen) {
        this->inputNumber.draw();
        this->inputNumber.update();
        this->playButton.draw();
    }
}

void MaxHeapVisualize::draw() {
    drawSideBar(this->type, this->animation.code, this->animation.lines, this->animation.infor, this->progressBar, this->font);
    drawButtons();
    Max_Heap::drawHeap(this->animation, this->font);
}

int MaxHeapVisualize::handle() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (this->createButton.getIsHovered()) {
            this->isCreateChosen = true;
            this->isDeleteChosen = false;
            this->isPushChosen = false;
        }
        if (this->deleteButton.getIsHovered()) {
            this->isCreateChosen = false;
            //this->inputNumber.resetText();
            this->isDeleteChosen = true;
            this->isPushChosen = false;
        }
        if (this->pushButton.getIsHovered()) {
            this->isCreateChosen = false;
            this->isDeleteChosen = false;
            //this->inputNumber.resetText();
            this->isPushChosen = true;
        }
    }

    if (this->playButton.handle()) {
        if (this->isPushChosen) {
            push();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 2;
        }
        if (this->isDeleteChosen) {
            deleteNode();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 3;
        }
    }

    if (this->topButton.handle()) {
        getTop();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 4;
    }
    if (this->sizeButton.handle()) {
        getSize();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 5;
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
            if(stepIndex == (int)this->heap.getSteps().size() - 1) break;
            this->progressBar.updateStep(1);
            updateStep(this->stepIndex + 1);
            break;

        case 2:
            if (this->progressBar.getMaxStep() == 0) break;
            updateStep((int)this->heap.getSteps().size() - 1);
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
            if (this->type == 0 && !this->animation.nodes.empty()) {
                this->type = 1;
                break;
            }
            break;

        case 3: case -3:
            this->numFrameOfAnimation = FPS/this->progressBar.getSpeed();

            break;

        default:
            break;
    }

    updateAnimation();

    return 0;
}
