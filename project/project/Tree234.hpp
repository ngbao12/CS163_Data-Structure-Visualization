#ifndef Tree234_hpp
#define Tree234_hpp

#include "GUI.hpp"

struct Key234 {
    int value;
    Vector2 start, end;
    bool highlight;
    Key234() {}
    Key234(int v, Vector2 st = {779.f-15, 124.f}, Vector2 des = {779.f-15, 124.f}) : value(v), start(st), end(des), highlight(0) {}
};

struct Node234 {
    std::vector<Key234> keys;
    std::vector<Node234*> children;
    Node234* parent;
    int width;
    Vector2 start;
    Vector2 end;
    Node234() : parent(nullptr), keys({}), width(1), start({779.f, 124.f}), end(start) {}
    Node234(int key, Vector2 start = {779.f, 124.f}, Vector2 end={779.f, 124.f}, Node234* parent = nullptr);

    bool isLeaf() const;
    bool isFull() const;
    int insert(int k);
    void remove(int key);
};

struct Step234 {
    Node234* root;
    std::vector<int> line;
    std::string infor;
    std::string code;
    int type;   // -1: node, 0: check, 1: insert, 2:
};

class Tree234 {
    private:
        Node234* root;
        
        Node234* split(Node234* &node);
        Node234* merge(Node234* &predecessor);
        
        void freeTree(Node234* &node);
        void freeStep(Step234 &st);
        void clearProcess();
        int estimateWidth(Node234* root);
        Node234* cloneTree(Node234* root, Node234* highlight=nullptr, int index=-1);
        std::vector<Step234> process;
        void saveStep(Node234* highlight, int indexHighlight, int type, std::vector<int> lines, const std::string infor, const std::string code, bool forCreate = false);
        void updatePos(Node234* root, bool updateStart = false, bool forCreate = false, Vector2 parentPos = {779.f, 124.f}, Vector2 delta = {0.f, 0.f});
    public:
        Tree234() : root(nullptr) {}
        void insert(int key);
        bool search(int key);
        bool remove(int key);
        void createFromFile(const char* filename);
        void createRandom(int n, int range);
        
        
        std::vector<Step234> getProcess() {return this->process;}
        void inorderTraversal(Node234* node) {
            if (node == nullptr) return;
            if (!node->children.empty()) inorderTraversal(node->children[0]);
            for (int i = 0; i < node->keys.size(); ++i) {
                std::cout << node->keys[i].value << " ";
                if (!node->children.empty() && i + 1 < node->children.size()) {
                    inorderTraversal(node->children[i + 1]);
                }
            }
        }

        void inorder() {
            inorderTraversal(root);
            std::cout << std::endl;
        }

        void BFT() {
            if (!root) return;
            estimateWidth(root);
            updatePos(this->root, true, false);
            Node234* newRoot = cloneTree(this->root);
            std::queue<Node234*> q;
            q.push(newRoot);

            while (!q.empty()) {
                Node234* current = q.front();
                q.pop();
                for (Node234* child : current->children) {
                    if (child != nullptr) {
                        q.push(child);
                    }
                }
            }
        }
};


class Tree234Visualize {
    private:
        Tree234 tree;
        Step234 step;
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
        Tree234Visualize(Font font);
        Tree234Visualize() : Tree234Visualize(FONT) {};
        void updateStep(int index);
        void createWithRandomizedData(int n = 15, int range = 100);
        void createFromFile();
        void insert();
        void deleteNode();
        void search();
        
        void drawNode(Node234 *root, int frame, int num_frame, Font font, bool is_notification = false);
        void drawTree();
        void drawButtons();
        int handle();
        void draw();
        int buttonHandle();
};

#endif /* Tree234_hpp */
