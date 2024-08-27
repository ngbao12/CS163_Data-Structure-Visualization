#ifndef Trie_hpp
#define Trie_hpp

#include "GUI.hpp"

const std::string Trie_INSERT(
    "cur = root\n"
    "for (char c : str)\n"
    "   if cur.child[c] == null\n"
    "       cur.child[c] = new Node(c)\n"
    "       cur.numOfChild++\n"
    "   cur = cur.child[c]\n"
    "cur.isEndStr = true\n"
);

const std::string Trie_DELETE(
    "cur = search(str)\n"
    "if (cur = nullptr) return;3"
    "cur.isEndStr = false\n"
    "for (char c : rev(str))\n"
    "   cur.numOfChild--\n"
    "   cur = cur.parent\n"
    "   if cur.child[c].numOfChild == 0\n"
    "       delete cur.child[c]"
);

const std::string Trie_SEARCH(
    "cur = root\n"
    "for (char c : str)\n"
    "   if cur.child[c] == null\n"
    "       return NOT_FOUND\n"
    "   cur = cur.child[c]\n"
    "if cur.isEndStr return FOUND\n"
    "else return NOT_FOUND\n"
);

const int ALPHABET_SIZE = 26;

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    char character;
    bool isEndStr;
    int numOfChild;
    Vector2 start;
    Vector2 end;
    int width;
    
    TrieNode(char ch, Vector2 st = {779.f, 124.f}, Vector2 des = {779.f, 124.f}) {
        isEndStr = false;
        numOfChild = 0;
        width = 1;
        character = ch;
        start = st;
        end = des;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            children[i] = nullptr;
    }
    
    TrieNode(){
        isEndStr = false;
        numOfChild = 0;
        width = 1;
        character = '\0';
        start = end = {779.f, 124.f};
        for (int i = 0; i < ALPHABET_SIZE; i++)
            children[i] = nullptr;
    }
};

struct TrieStep {
    TrieNode* root;
    std::vector<int> line;
    TrieNode* highlight;
    std::string infor;
    std::string code;
    int type;   // -1: node, 0: check, 1: insert, 2:
};

class Trie {
private:
    TrieNode* root;
    void freeTrie(TrieNode* &node);
    void freeStep(TrieStep &step);
    int estimateWidth(TrieNode* root);
    std::pair<TrieNode*, TrieNode*> cloneTrie(TrieNode* root, TrieNode* highlight = nullptr); //first: root, second: highlight
    std::vector<TrieStep> process;
    void saveStep(TrieNode* highlight, int type, std::vector<int> lines, const std::string infor, const std::string code, bool forCreate = false);
    void updatePos(TrieNode* root, bool updateStart = true, bool forCreate = false, Vector2 parentPos = {779.f, 124.f}, Vector2 delta = {0.f, 0.f});
    // void update_target_pos(TrieNode* root, Vector2 parent_pos = {1013.f, 100.f}, Vector2 delta = {0.f, 0.f}, bool for_create = false);
    bool deleteWord(TrieNode* node, const std::string& key, int depth = 0);

public:
    Trie() { this->root = new TrieNode(); }
    bool empty() {return this->root->numOfChild == 0;}
    TrieNode* getRoot() {return this->root;}
    std::pair<TrieNode*, TrieNode*> cloneTrie(TrieNode* highlight = nullptr) {return cloneTrie(this->root, root->children[int('c' - 'a')]->children['a' - 'a']->children['d' - 'a']);}; //first: root, second: highlight
    int estimateWidth() {return estimateWidth(this->root);}
    void clearProcess();
    
    void insert(std::string key);
    void createFromFile(const char* filename);
    void createRandom(int numWords, int maxLength);
    void deleteStr(const std::string key) {clearProcess(); deleteWord(this->root, key, 0);}
    bool search(std::string key);

    std::vector<TrieStep> getProcess() {return this->process; }
};

class TrieVisualize {
private:
    Trie tree;
    TrieStep step;
    bool isPause;
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
    TrieVisualize(Font font);
    TrieVisualize() : TrieVisualize(FONT) {};
    void updateStep(int index);
    // int update_animation();
    void createWithRandomizedData(int n, int length);
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


#endif /* Trie_hpp */
