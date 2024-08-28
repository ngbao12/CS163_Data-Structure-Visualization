
#ifndef General_hpp
#define General_hpp

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>
#include <functional>
#include <stdint.h>
#include "portable-file-dialogs.h"

extern Texture2D PLAY;
extern Texture2D PAUSE;
extern Texture2D REPLAY;
extern Texture2D BACK;
extern Texture2D DOUBLE_BACK;
extern Texture2D NEXT;
extern Texture2D DOUBLE_NEXT;
extern Texture2D SPEED_UP;
extern Texture2D SPEED_DOWN;
extern Texture2D BACK_PAGE;

void initResource();
Vector2 Vector2Lerp(Vector2 start, Vector2 end, float amount);
float FloatLerp(float start, float end, float amount);

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 750;
const int FPS = 60;

// Color
struct theme {
    Color BACKGROUND;
    
    Color MENU_BUTTON;
    Color HOVER_MENU;
    
    Color BUTTON;
    Color HOVER_BUTTON;
    Color SEPERATOR;
    
    Color INFOR;
    Color HIGHLIGHT_TEXT;
    
    Color HIGHLIGHT_NODE_1;
    Color HIGHLIGHT_NODE_2;
    Color NODE;
    Color LINE;
    
    Color SIDEBAR;
    Color WEIGHT;
    bool isDarkMode = true;
    void change();
};


/*const Color LIGHT_THEME = {229,247,255,255};
const Color DARK_THEME = {45,69,81,255};
const Color NONHOVERED_BUTTON_LIGHT_THEME = {255,255,255,255};
const Color HOVERED_BUTTON_LIGHT_THEME = {190,190,190,255};

const Color NODE_COLOR = {200,200,200,255};
const Color HIGHLIGHT_TEXT_COLOR = {200,0,0,255};
const Color INFOR_COLOR = SKYBLUE;
const Color HIGHLIGHT_NODE_COLOR_1 = RED;
const Color HIGHLIGHT_NODE_COLOR_2 = BLUE;

const Color SIDEBAR_COLOR = {150,150,150,200};
const Color SEPERATOR_COLOR = SKYBLUE;
*/
const float CODE_SIZE = 15;
const float NODE_RADIUS = 15;

extern theme THEME;
extern Font FONT;

//GUI

const Vector2 ORIGIN ={0.0f, 0.0f};
void drawPicture(const char* path, Rectangle desRec, float rotation=0.0f, Vector2 origin=ORIGIN, Color color=WHITE);
void drawPicture(Texture2D texture, Rectangle desRec, float rotation=0.0f, Vector2 origin=ORIGIN, Color color=WHITE);
void drawRectangleWithBorder(Rectangle rec, Color colorRec, float widthBorder, Color colorBorder, int radiusBorder=0);
Texture2D textureFromImageWithRoundedCorner(const char* path, int radius = 0);
void drawShrinkingText(const char* text, Vector2 position, int fontSize, float* scale, float* alpha, float shrinkRate, float fadeRate);

void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color);
#endif /* General_hpp */
