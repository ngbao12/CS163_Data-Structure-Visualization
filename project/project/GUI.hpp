#ifndef GUI_hpp
#define GUI_hpp

#include "General.hpp"

class Button {
protected:
    Rectangle outerRect;
    std::string content;
    Vector2 contentPos;
    Color contentColor;
    Font font;
    float fontSize;
    bool isHovered;
public:
    Button(Rectangle rect, const std::string text, float yText, Color textColor, float fontSize, Font font = FONT);
    Button() : Button({0,0,0,0}, "", 0.0f, HOVERED_BUTTON_LIGHT_THEME, 0) {}
    void draw(float radius = 15);
    int handle();
    bool getIsHovered() {return this->isHovered;}
    
    virtual ~Button() = default;
};

class ButtonImage : public Button {
protected:
    Rectangle innerRect;
    Texture2D texture;
public:
    ButtonImage(Rectangle outerRect, Rectangle innerRect, const char* path, const std::string text, float yText, Color textColor, float fontSize, Font font=FONT);
    ButtonImage() : Button() {}
    void draw(float radius = 15);
};

class InputStr {
private:
    Rectangle textBox;
    std::string text;
    std::string placeHolder;
    bool mouseOnText;
    int framesCounter;
    float fontSize;
    Font font;
public:
    InputStr() {} ;
    InputStr(float x, float y, float width, float height, std::string placeHolder, float fontSize, Font font) : textBox{x,y,width,height}, text(""), placeHolder(placeHolder), mouseOnText(false), framesCounter(0), fontSize(fontSize), font(font) {}
    void update();
    void draw();
    void resetText();
    std::string getText();
    void changePlaceHolder(const std::string newPlaceHolder);
};

class CircleButton {
private:
    Vector2 center;
    float radius;
    Texture2D texture;
    bool isHovered;
public:
    CircleButton() {}
    CircleButton(Vector2 center, float radius, const char* file) : center(center), radius(radius), texture(LoadTexture(file)), isHovered(false) {};
    void draw();
    void handle();
    void changeTexture(const char* file);
    bool getIsHovered() {return this->isHovered;}
};

class ProgressBar {
private:
    bool doubleBack;
    bool back;
    bool doubleNext;
    bool next;
    bool up;
    bool down;
    bool play;
    
    int curStep;
    int maxStep;
    float speed;
    
    int funcID;
    Font font;
public:
    ProgressBar(Font font) : doubleBack(false), back(false), doubleNext(false), next(false), up(false), down(false), play(false), curStep(0), maxStep(0), speed(1), funcID(0), font(font) {}
    ProgressBar() : ProgressBar(FONT) {}
    void draw();
    void updateMaxStep(int max);
    void updateStep(int step);
    void updateSpeed(float speed);
    float getSpeed() {return this->speed;}
    int getStep() {return this->curStep;}
    int getMaxStep() {return this->maxStep;}
    
    int handle();
};

void drawCode(std::string code, size_t n ,std::vector<int> highlight, Font font);
void drawInfor(std::string infor, Font font = FONT);
void drawSideBar(std::string code, std::vector<int> lines, std::string infor, ProgressBar bar, Font font = FONT);
#endif /* GUI_hpp */
