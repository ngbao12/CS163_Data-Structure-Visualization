#ifndef AVL_hpp
#define AVL_hpp

#include "GUI.hpp"

const std::string AVL_INSERT(
    "Insert v\n"
    "Check balance factor (bf):\n"
    "   LL: rt_Right\n"
    "   LR: left.rt_Left, rt_Right\n"
    "   RR: rt_Lef\n"
    "   RL: right.rt_Right, rt_Left\n"
);

const std::string AVL_DELETE(
    "Delete v\n"
    "Check balance factor (bf):\n"
    "   LL: rt_Right\n"
    "   LR: left.rt_Left, rt_Right\n"
    "   RR: rt_Lef\n"
    "   RL: right.rt_Right, rt_Left\n"
);

const std::string AVL_SEARCH(
    "if this == null\n"
    "   return NOT_FOUND\n"
    "if this.data == value\n"
    "   return FOUND\n"
    "if value < this.data\n"
    "   searchLeft\n"
    "if value > this.data\n"
    "   sreachRight\n"
);

struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
    Vector2 start;
    Vector2 end;
    
    AVLNode(int key, Vector2 begin, Vector2 dest, int height=1) : key(key), left(nullptr), right(nullptr), height(height), start(begin), end(dest) {}
};

struct AVLStep {
    AVLNode* root;
    std::vector<int> line;
    int highlight;
    std::string infor;
    std::string code;
    int type;
};

class AVLTree {
    private:
        AVLNode* root;
        
        void freeTree(AVLNode* &root);
        void freeStep(AVLStep &step);
        void clearProcess();
        
        std::vector<AVLStep> process;
        AVLNode* cloneTree(AVLNode* node);
        
        void saveStep(int speacialValue, int type, std::vector<int> line, const std::string infor, const std::string code, bool forCreate = false);
        void updateStandardPos(AVLNode* root, Vector2 parentPos = {1013, 100}, Vector2 delta = {0.0f, 0.0f});
        void updateTargetPos(AVLNode* root, Vector2 parentPos = {1013,100}, Vector2 delta = {0.0f, 0.0f}, bool forCreate = false);
        
        int height (AVLNode* node) {
            return node ? node->height : 0;
        }
        
        int getBalance(AVLNode* node) {
            return node ? height(node->left) - height(node->right) : 0;
        }
        
        AVLNode* rightRotate(AVLNode* y);
        AVLNode* leftRotate(AVLNode* x);
        
        void updateHeight(AVLNode* node) {
            node->height = 1 + std::max(height(node->left), height(node->right));
        }
        
        int updateAllHeight(AVLNode* node);
        std::pair<AVLNode*, int> balance(AVLNode* node, const std::string code);
    public:
        AVLTree() : root(nullptr) {}
        std::vector<AVLStep> getProcess() {return this->process;}
        void createFromFile (const char* filename);
        void createWithRandomizedData(int n, int range);
        void insert(int key, Vector2 start = {1013,100}, Vector2 end = {1013,100});
        void deleteNode(int key);
        bool search(int key);
        void printInOrder();
};

class AVLTreeVisualize {
private:
    AVLTree tree;
    AVLStep step;
    int type = 0;
    ProgressBar progressBar;
    int stepIndex;
    int frame;
    int numFrameOfAnimation;
    
    Button createButton;
    bool isCreateChosen;
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
    AVLTreeVisualize(Font font);
    AVLTreeVisualize() : AVLTreeVisualize(FONT){}
    void updateStep(int step);
    void createWithRandomizedData(int n, int range);
    void createFromFile();
    void insert();
    void deleteNode();
    void search();
    
    void drawTree();
    void drawButtons();
    int handle();
    
    void draw();
    int buttonHandle();
};

#endif /* AVL_hpp */
