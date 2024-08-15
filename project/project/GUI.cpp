#include "GUI.hpp"

Button::Button(Rectangle rect, const std::string text, float yText, Color textColor, float fontSize, Font font) {
    this->outerRect = rect;
    this->content = text;
    this->font = font;
    this->fontSize = fontSize;
    this->contentColor = textColor;
    Vector2 textSize = MeasureTextEx(this->font, text.c_str(), this->fontSize,2);
    this->contentPos = {this->outerRect.x + (this->outerRect.width-textSize.x)/2, (yText == -1) ? (rect.y + (rect.height - textSize.y)/2) : yText};
    this->isHovered = false;
}

void Button::draw(float radius) {
    this->isHovered = CheckCollisionPointRec(GetMousePosition(), this->outerRect);
    DrawRectangleRounded(this->outerRect, radius/100, 32, this->isHovered ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawTextEx(FONT, this->content.c_str(), this->contentPos, this->fontSize, 2, this->contentColor);
}

int Button::handle() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && this->isHovered) {
        return 1;
    }
    return 0;
}

ButtonImage::ButtonImage(Rectangle outerRect, Rectangle innerRect, const char* path, const std::string text, float yText, Color textColor, float fontSize, Font font) {
    this->outerRect = outerRect;
    this->innerRect = innerRect;
    this->texture = LoadTexture(path);
    this->content = text;
    this->contentPos = {this->outerRect.x + (this->outerRect.width - MeasureTextEx(font, text.c_str(), fontSize, 5).x)/2, yText};
    this->contentColor = textColor;
    this->fontSize = fontSize;
    this->font = font;
    
}

void ButtonImage::draw(float radius) {
    DrawRectangleRounded(outerRect, radius/100, 32,this->isHovered ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    drawPicture(this->texture, this->innerRect);
    DrawTextPro(this->font, this->content, this->contentPos, ORIGIN, 0, this->fontSize, 2, this->contentColor);
}

