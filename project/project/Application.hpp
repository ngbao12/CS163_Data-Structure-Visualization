#ifndef Application_hpp
#define Application_hpp

#include "GUI.hpp"
#include "HashTable.hpp"
#include "MaxHeap.hpp"
#include "Graph.hpp"
#include "AVL.hpp"
#include "Tree234.hpp"
#include "Trie.hpp"

class Menu {
    private:
        ButtonImage avlTree;
        ButtonImage hashTable;
        ButtonImage Tree234;
        ButtonImage trie;
        ButtonImage maxHeap;
        ButtonImage graph;
        Texture2D mode;
        bool isDarkMode = false;
    public:
        Menu();
        void draw();
        void hanlde();
        int8_t clicked();
        bool getDarkMode() { return this->isDarkMode;}
};

class Application {
    private:
        int tab;
        Color theme;
    public:
        Application(): tab(0), theme(THEME.BACKGROUND) {}
        void run();
        int menu();
        int hashTable();
        int maxHeap();
        int avlTree();
        int Tree234();
        int trie();
        int graph();
};


#endif /* Application_hpp */
