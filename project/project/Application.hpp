#ifndef Application_hpp
#define Application_hpp

#include "GUI.hpp
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
    
    public:
        Menu();
        void draw();
        void hanlde();
        int8_t clicked();
};

class Application {
    private:
        int tab;
        Color theme;
    public:
        Application(): tab(0), theme(DARK_THEME) {}
        void run();
        int menu();
        int maxHeap();
};


#endif /* Application_hpp */
