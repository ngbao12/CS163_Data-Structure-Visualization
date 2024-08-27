#ifndef MaxHeap_hpp
#define MaxHeap_hpp

#include "GUI.hpp"


const std::string Push_code(
    "i = size, size++\n"
    "A[i] = value\n"
    "while (i > 0 && A[(i-1)/2] < A[i])\n"
    "    swap(A[(i-1)/2], A[i]), i = (i-1)/2\n");

const std::string Delete_code(
    "i = 0\n"
    "while (i < size && A[i] != value) i++\n"
    "if (i >= size) return\n"
    "A[i] = A[0]+1\n"
    "while (i > 0 && A[i/2] < A[i])\n"
    "    swap(A[(i-1)/2], A[i]), i = (i-1)/2\n"
    "A[0] = A[size-1]\n"
    "size--, i = 0\n"
    "while (A[i] < A[Largest])\n"
    "    swap(A[i], A[Largest]),    i = Largest\n");

const std::string Top_code("return A[0]\n");
const std::string Size_code("return sizeof(A)/sizeof(A[0]).\n");

namespace Max_Heap {
    struct Node {
        int value;
        Vector2 pos;
        float dx, fontSize;
        Color color;
    };

    struct Step {
        std::vector<Node> nodes;
        std::vector<int> lines;
        int highlight[2];
        std::string infor;
        std::string code;
        int type; //-1: none, 0: check, 1: swap, 2: change(num, text), 3: insert, 4: remove;
    };

    void drawEdge(Vector2 pos1, Vector2 pos2);
    void drawNode(std::vector<Node> scene, int index, Font font = FONT);
    void drawHeap(Step step, Font font = FONT);
}

class MaxHeap {
    private:
        std::vector<int> data;
        void heapifyDown(int index);
        void saveStep(int index1, int index2, std::vector<int> lines, const std::string infor, std::string code, int type);
        std::vector<Max_Heap::Step> steps;
    public:
        MaxHeap() {}
        int createFromFile(const char* filename);
        void createWithRandomizedData(int n, int range);
        void push(int value);
        int deleteElement(int value);
        void top();
        void size();
        std::vector<Max_Heap::Step> getSteps() {return this->steps;}
};

class MaxHeapVisualize {
    private:
        MaxHeap heap;
        Max_Heap::Step animation;
        bool isPause;
        ProgressBar progressBar;
        int stepIndex;
        int frame;
        int numFrameOfAnimation;
        
        Button  createButton;
        bool isCreateChosen;
        Button randomButton;
        Button loadFileButton;
        
        Button pushButton;
        bool isPushChosen;
        Button deleteButton;
        bool isDeleteChosen;
        InputStr inputNumber;
        Button playButton;
        
        Button topButton;
        Button sizeButton;
        
        Font font;
    public:
        MaxHeapVisualize(Font font);
        MaxHeapVisualize() : MaxHeapVisualize(FONT) {} ;
        Max_Heap::Step getAinimation() { return this->animation;}
        void updateStep(int index);
        int updateAnimation();
        void updateStep();
        void createWithRandomizedData(int n, int range);
        void createFromFile(const char* filename);
        void push();
        void deleteNode();
        void getTop();
        void getSize();
        
        void drawButtons();
        int handle();
        void draw();
        int buttonHandle();
};
#endif /* MaxHeap_hpp */
