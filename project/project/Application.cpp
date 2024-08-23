#include "Application.hpp"


Menu::Menu() {
    this->avlTree = ButtonImage({180, 260, 240, 160}, {190, 270, 220, 120}, "./asset/HashTable.png", "Hash Table", 390, {145, 95, 35, 255}, 20);
    this->hashTable = ButtonImage({520, 260, 240, 160}, {530, 270, 220, 120}, "./asset/AVL.png", "AVL Tree", 390, {145, 95, 35, 255}, 20);
    this->Tree234 = ButtonImage({860, 260, 240, 160}, {870, 270, 220, 120}, "asset/Tree234.png", "2-3-4 Tree", 390, {145, 95, 35, 255}, 20);
    this->maxHeap = ButtonImage({180, 500, 240, 160}, {190, 510, 220, 120}, "asset/MaxHeap.png", "Max Heap", 635, {145, 95, 35, 255}, 20);
    this->trie = ButtonImage({520, 500, 240, 160}, {530, 510, 220, 120}, "asset/Trie.png", "Trie", 635, {145, 95, 35, 255}, 20);
    this->graph = ButtonImage({860, 500, 240, 160}, {870, 510, 220, 120}, "asset/Graph.png", "Graph", 635, {145, 95, 35, 255}, 20);
}


void Menu::draw() {
    DrawTextPro(FONT, "DATA STRUCTURE VISUALIZATION", {(SCREEN_WIDTH - MeasureTextEx(FONT, "DATA STRUCTURE VISUALIZATION", 45, 5).x)/2, 75.0f}, {0.0f, 0.0f}, 0, 45, 5, {255, 160, 0, 255});

    avlTree.draw();
    hashTable.draw();
    Tree234.draw();
    maxHeap.draw();
    trie.draw();
    graph.draw();
}

void Menu::hanlde() {
    avlTree.handle();
    hashTable.handle();
    Tree234.handle();
    maxHeap.handle();
    trie.handle();
    graph.handle();
}

int8_t Menu::clicked() {
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if(avlTree.getIsHovered()) return 1;
        if(hashTable.getIsHovered()) return 2;
        if(Tree234.getIsHovered()) return 3;
        if(maxHeap.getIsHovered()) return 4;
        if(trie.getIsHovered()) return 5;
        if(graph.getIsHovered()) return 6;
    }
    return 0;
}

void Application::run() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Data Visualized");
    initResource();
    SetTargetFPS(FPS);
    while (!WindowShouldClose() && this->tab != -1) {
        switch (this->tab) {
            case 0:
                this->tab = menu();
                break;

            case 4:
                this->tab = maxHeap();
                break;
        
            default:
                this->tab = -1;
                printf("HEHE\n");
                break;
        }
        
    }
    CloseWindow();
}

int Application::menu() {
    Menu menu;
    int8_t flag = -1;
    do {
        BeginDrawing();
            ClearBackground(this->theme);
            menu.hanlde();
            menu.draw();
            flag = menu.clicked();
        EndDrawing();
    } while (!WindowShouldClose() && !flag);
    return flag;
}

int Application::maxHeap() {
    // MaxHeapVisualize heap;
    MaxHeapVisualize maxHeap = MaxHeapVisualize(FONT);
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_THEME);
        maxHeap.draw();
         
        EndDrawing();
    }
    return -1;
}
