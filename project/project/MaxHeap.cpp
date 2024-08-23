#include "MaxHeap.hpp"

void Max_Heap::drawEdge(Vector2 pos1, Vector2 pos2) {
    DrawLineEx(pos1, pos2, 2, NODE_COLOR);
}

void Max_Heap::drawNode(std::vector<Node> scene, int index, Font font) {
    int size =(int) scene.size();
    int left = 2*index + 1, right = 2*index + 2;
    if (left < size) drawEdge(scene[index].pos, scene[left].pos);
    if (right < size) drawEdge(scene[index].pos, scene[right].pos);
    DrawCircle(scene[index].pos.x, scene[index].pos.y, NODE_RADIUS, scene[index].color);
    if (scene[index].fontSize == 0) return;
    Vector2 textSize = MeasureTextEx(font, TextFormat("%d", scene[index].value), scene[index].fontSize, 0);
    DrawTextPro(font, TextFormat("%d", scene[index].value), {scene[index].pos.x - textSize.x/2, scene[index].pos.y - textSize.y/2}, ORIGIN, 0, scene[index].fontSize, 0, WHITE);
}

void Max_Heap::drawHeap(Step step, Font font) {
    int size =(int) step.nodes.size();
    for (int i=0; i<size; i++) {
        drawNode(step.nodes, i);
    }
}

void MaxHeap::saveStep(int index1, int index2, std::vector<int> lines, const std::string infor, std::string code, int type) {
    if (this->data.empty()) return;
    
    int size =(int) this->data.size();
    Vector2 pos = {1013,100};
    float dx = 50*log2(size), dy = 100;
    std::vector<Max_Heap::Node> nodes;
    nodes.push_back({this->data[0], pos, dx, CODE_SIZE, NODE_COLOR});
    
    for (int i=1; i<size; i++) {
        dx = nodes[(i-1)/2].dx;
        pos = nodes[(i-1)/2].pos;
        nodes.push_back({this->data[i], {pos.x + ((i & 1) ? (-dx) : dx), pos.y + dy}, dx/2, CODE_SIZE, NODE_COLOR});
    }
    
    if (type == 0 || type == 1 || type == 2) {
        nodes[index2].color = HIGHTLIGHT_NODE_COLOR_2;
        nodes[index1].color = HIGHTLIGHT_NODE_COLOR_1;
    }
    
    this->steps.push_back({nodes, lines, {index1, index2}, infor, code, type});
}

void MaxHeap::heapifyDown(int index) {
    int size =(int) data.size();
    int largest = index;
    int left = 2*index + 1;
    int right = 2*index +2;
    
    if (left < size && this->data[left] > this->data[largest]) {
        largest = left;
    }
    if (right < size && this->data[right] > this->data[largest]) {
        largest = right;
    }
    
    if (largest == index) return;
    
    if (largest != index) {
        std::swap(this->data[index], this->data[largest]);
        heapifyDown(largest);
    }
}

int MaxHeap::createFromFile(const char* filename) {
    this->steps.clear();
    
    std::ifstream file(filename);
    if (!file) {
        std::cerr <<"File could not be opened!" << std::endl;
        return 0;
    }
    this->data.clear();
    int number;
    while (file >> number) {
        this->data.push_back(number);
        int i =(int) this->data.size() - 1;
        saveStep(i, -1, {}, "Creating...", "", 3);
        
    }
    int size =(int) this->data.size() -1;
    for (int i= size/2 -1; i>=0; --i) {
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
    for (int i=0; i < n; i++) {
        this->data.push_back(rand() % range);
        saveStep(i, -1, {}, "Creating...", "", 3);
    }
    
    for (int i=(int)this->data.size()/2 -1; i >=0; i--) {
        heapifyDown(i);
    }
    saveStep(-1, -1, {}, "Create with randomized succesfully!", "", -1);
}

void MaxHeap::push(int value) {
    this->steps.clear();
    
    this->data.push_back(value);
    int index =(int) this->data.size() -1 ;
    saveStep(index, -1, {0,1}, TextFormat("%d is inserted at the back of compact array A", value), Push_code, 3);
    while (index >0 && this->data[index] > this->data[(index-1)/2]) {
        saveStep(index, (index-1)/2, {2}, "Comparing between curent node and its parent", Push_code, 0);
        saveStep(index, (index-1)/2, {3}, TextFormat("%d > %d, so swap them.\n\n Update index.", this->data[index], this->data[(index-1)/2]), Push_code, 1);
        index = (index-1)/2;
    }
    saveStep(-1, -1, {}, TextFormat("The processing is completed. \n%d is push into the Heap", value), Push_code, -1);
}

int MaxHeap::deleteElement(int value) {
    this->steps.clear();
    
    int size =(int) this->data.size() -1, i=0;
    saveStep(-1, -1, {}, TextFormat("Starting delete processing...", value), Delete_code, -1);
    for ( i=0; i<size && this->data[i] != value; i++) {
        saveStep(-1, -1, {0}, TextFormat("Finding %d in Heap...", value), Delete_code, 0);
        if (this->data[i] == value) break;
    }
    if ( i == size) {
        saveStep(-1, -1, {2}, TextFormat("%d is not in Heap", value), Delete_code, -1);
        return 0;
    }
    
    saveStep(i,i, {1}, TextFormat("%d is at index %d", value, i), Delete_code, 0);
    this->data[i] = this->data[0] + 1;
    while (i>0 && this->data[(i-1)/2] < this->data[i]) {
        saveStep(i, (i-1)/2, {4}, "Comparing between current node and its parent.", Delete_code, 0);
        saveStep(i, (i-2)/2, {5}, TextFormat("%d > %d, so swap them.\n Update index i", this->data[i], this->data[(i-1)/2]), Delete_code, 1);
        std::swap(this->data[i], this->data[(i-1)/2]);
        i = (i-1)/2;
    }
    this->data[0] = this->data[size - 1];
    saveStep(0,0,{6}, "Change root value to value of last element.", Delete_code,2);
    saveStep(size-1, -1, {7}, "Remove last element.", Delete_code, 4);
    this->data.pop_back();
    size =(int) this->data.size();

    i=0;
    while(true) {
        int largest = i;
        if (2*i + 1 < size && this->data[largest] < this->data[2*i+1]) largest = 2*i + 1;
        saveStep(i, 2*i + 1, {8}, TextFormat("Find the largest node of current node and it's child node..."), Delete_code, 0);
        if (2*i + 2 < size && this->data[largest] < this->data[2*i+2]) largest = 2*i + 2;
        saveStep(i, largest, {9}, TextFormat("Find the largest node of current node and it's child node..."), Delete_code, 0);
        saveStep(i, largest, {8}, TextFormat("The largest node is index %d, with valude %d.", largest, this->data[largest]), Delete_code, 0);
        if (i== largest) break;
        saveStep(i, largest, {9}, TextFormat("Swap two values at indexs %d and %d.\n. Change current index i to largest.", i, largest), Delete_code, 1);
        std::swap(this->data[i], this->data[largest]);
        i = largest;
    }
    saveStep(-1, -1, {}, TextFormat("The processing is completed. \n%d has removed from the Heap", value), Delete_code, -1);
    return 1;
}

void MaxHeap::top() {
    this->steps.clear();
    saveStep(0, 0, {0}, TextFormat("Top of the heap is : %d", this->data[0]), Top_code, 0);
}

void MaxHeap::size() {
    this->steps.clear();
    saveStep(-1, -1, {0}, TextFormat("Size of the Heap is: %d.", this->data.size()), Size_code, -1);
}
