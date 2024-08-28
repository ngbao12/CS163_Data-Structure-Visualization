#include "Application.hpp"


Menu::Menu() {
    this->avlTree = ButtonImage({180, 260, 240, 160}, {190, 270, 220, 120}, "./asset/HashTable.png", "Hash Table", 390, {145, 95, 35, 255}, 20);
    this->hashTable = ButtonImage({520, 260, 240, 160}, {530, 270, 220, 120}, "./asset/AVL.png", "AVL Tree", 390, {145, 95, 35, 255}, 20);
    this->Tree234 = ButtonImage({860, 260, 240, 160}, {870, 270, 220, 120}, "./asset/Tree234.png", "2-3-4 Tree", 390, {145, 95, 35, 255}, 20);
    this->maxHeap = ButtonImage({180, 500, 240, 160}, {190, 510, 220, 120}, "./asset/MaxHeap.png", "Max Heap", 635, {145, 95, 35, 255}, 20);
    this->trie = ButtonImage({520, 500, 240, 160}, {530, 510, 220, 120}, "./asset/Trie.png", "Trie", 635, {145, 95, 35, 255}, 20);
    this->graph = ButtonImage({860, 500, 240, 160}, {870, 510, 220, 120}, "./asset/Graph.png", "Graph", 635, {145, 95, 35, 255}, 20);
    this->mode = isDarkMode ? LoadTexture("./asset/Dark.png") : LoadTexture("./asset/Light.png");
}


void Menu::draw() {
    DrawTextPro(FONT, "DATA STRUCTURE VISUALIZATION", {(SCREEN_WIDTH - MeasureTextEx(FONT, "DATA STRUCTURE VISUALIZATION", 45, 5).x)/2, 75.0f}, {0.0f, 0.0f}, 0, 45, 5, {255, 160, 0, 255});
    avlTree.draw();
    hashTable.draw();
    Tree234.draw();
    maxHeap.draw();
    trie.draw();
    graph.draw();
    DrawCircle(1220, 60, 27.5, CheckCollisionPointCircle(GetMousePosition(), {1220,60}, 27.5) ? THEME.HOVER_MENU : THEME.MENU_BUTTON);
    drawPicture(mode, {1200,40,40,40});
}

void Menu::hanlde() {
    avlTree.handle();
    hashTable.handle();
    Tree234.handle();
    maxHeap.handle();
    trie.handle();
    graph.handle();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(), {1220,60}, 27.5)) {
        this->isDarkMode = !this->isDarkMode;
        THEME.change();
        printf("123");
    }
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
    SetTextureFilter(FONT.texture, TEXTURE_FILTER_POINT);
    SetTargetFPS(FPS);
    while (!WindowShouldClose() && this->tab != -1) {
        switch (this->tab) {
            case 0:
                this->tab = menu();
                break;
            case 1:
                break;
            case 2:
                this->tab = avlTree();
                break;
            case 3:
                this->tab = Tree234();
                break;
            case 4:
                this->tab = maxHeap();
                break;
            case 5:
                this->tab = trie();
                break;
            case 6:
                this->tab = graph();
                break;
        
            default:
                this->tab = -1;
                break;
        }
        
    }
    CloseWindow();
}

int Application::menu() {
    Menu menu;
    int8_t flag = -1;
    do {
        menu.hanlde();
        BeginDrawing();
            ClearBackground(THEME.BACKGROUND);
            menu.draw();
            flag = menu.clicked();
        EndDrawing();
    } while (!WindowShouldClose() && !flag);
    return flag;
}

int Application::maxHeap() {
    // MaxHeapVisualize heap;
    MaxHeapVisualize maxHeap = MaxHeapVisualize(FONT);
    TittleButton tittle = TittleButton({465, 34, 350, 40}, "Max Heap", -1, BLACK, 20);
    while(!WindowShouldClose()) {
        if (tittle.handle() == 1) {
            return 0;
        }
        maxHeap.handle();
        BeginDrawing();
        ClearBackground(THEME.BACKGROUND);
        maxHeap.draw();
        tittle.draw();
        EndDrawing();
    }
    return -1;
}

int Application::avlTree() {
    
    AVLTreeVisualize avlTree = AVLTreeVisualize(FONT);
    TittleButton tittle = TittleButton({465, 34, 350, 40}, "AVL Tree", -1, BLACK, 20);
    while(!WindowShouldClose()) {
        if (tittle.handle() == 1) return 0;
        avlTree.handle();
        BeginDrawing();
        ClearBackground(THEME.BACKGROUND);
        avlTree.draw();
        tittle.draw();
        EndDrawing();
    }
    return -1;
}

int Application::Tree234() {
    
    Tree234Visualize Tree234 = Tree234Visualize(FONT);
    TittleButton tittle = TittleButton({465, 34, 350, 40}, "2-3-4 Tree", -1, BLACK, 20);
    while(!WindowShouldClose()) {
        if (tittle.handle() == 1) return 0;
        Tree234.handle();
        BeginDrawing();
        ClearBackground(THEME.BACKGROUND);
        Tree234.draw();
        tittle.draw();
        EndDrawing();
    }
    return -1;
}

int Application::trie() {
    
    TrieVisualize trie = TrieVisualize(FONT);
    TittleButton tittle = TittleButton({465, 34, 350, 40}, "Trie", -1, BLACK, 20);
    while(!WindowShouldClose()) {
        if (tittle.handle() == 1) return 0;
        trie.handle();
        BeginDrawing();
        ClearBackground(THEME.BACKGROUND);
        trie.draw();
        tittle.draw();
        EndDrawing();
    }
    return -1;
}

int Application::graph() {
    
    GraphVisualize graph = GraphVisualize(FONT);
    TittleButton tittle = TittleButton({465, 34, 350, 40}, "Graph", -1, BLACK, 20);
    while(!WindowShouldClose()) {
        if (tittle.handle() == 1) return 0;
        graph.handle();
        BeginDrawing();
        ClearBackground(THEME.BACKGROUND);
        graph.draw();
        tittle.draw();
        EndDrawing(); 
    }
    return -1;
}
