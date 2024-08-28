#include "GUI.hpp"

struct HashStep {
    std::vector<int> table;
    std::vector<int> line;
    int highlightIndex;
    std::string infor;
    std::string code;
    int type;
};

class HashTable {
private:
    std::vector<int> table;
    int capacity;
    int size;

    std::vector<HashStep> process;
    void saveStep(int hightlightindex, int type, std::vector<int> line, const std::string infor, const std::string code, bool forCreate = false);
    int hash(int key);

    int linearProbe(int key);
    void clear() {
        std::fill(table.begin(), table.end(), -1);
        size = 0;
    }
    
public:
    HashTable(int capacity) : capacity(capacity), size(0) {
        table.resize(capacity, -1);  // -1 indicates empty slot, -2 indicates deleted slot
        saveStep(-1, 0, {}, "", "", 1);
    }
    HashTable() {};
    void resize(int k);

    void insert(int key);

    void deleteKey(int key);

    int search(int key);

    void createFromFile(const std::string& filename);

    void createRandom(int numKeys, int maxValue);
    std::vector<HashStep> getProcess() {return this->process;}
    void display();
};

class HashTableVisualize {
private:
    HashTable table;
    HashStep step;
    int type = 0;
    ProgressBar progressBar;
    int stepIndex;
    int frame;
    int numFrameOfAnimation;
    
    Button createButton;
    bool isCreateChosen;
    InputStr inputSize;
    Button resizeButton;
    Button randomButton;
    Button loadFileButton;
    
    Button insertButton;
    bool isInsertChosen;
    Button deleteButton;
    bool isDeleteChosen;
    Button searchButton;
    bool isSearchChosen;
    InputStr inputNumber;
    Button playButton;
    
    Font font;
public:
    HashTableVisualize(Font font);
    HashTableVisualize() : HashTableVisualize(GetFontDefault()) {}
    void updateStep(int step);
    void resize();
    void createWithRandomizedData(int n = 15, int range = 100);
    void createFromFile();
    void insert();
    void deleteNode();
    void search();
    
    void drawTable();
    void drawButtons();
    int handle();
    
    void draw();
    int buttonHandle();
};