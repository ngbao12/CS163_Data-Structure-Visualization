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
#endif /* GUI_hpp */
