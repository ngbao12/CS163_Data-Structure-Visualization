#ifndef Application_hpp
#define Application_hpp

#include "raylib.h"

class Application {
private:
    int tab;
    Color theme;
public:
    Application() : tab(0), theme(WHITE) {}
    void run();
};

#endif /* Application_hpp */
