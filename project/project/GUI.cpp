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
    DrawTextPro(this->font, this->content.c_str(), this->contentPos, ORIGIN, 0, this->fontSize, 2, this->contentColor);
}

void InputStr::draw() {
    DrawRectangleRounded(this->textBox, 0.2f, 64, {229,247,255,255});
    DrawRectangleRoundedLinesEx(this->textBox, 0.2f, 64, 3, mouseOnText ? BLACK : GRAY);
    Vector2 size = MeasureTextEx(this->font, this->text.c_str(), this->fontSize, 2);
    if (this->text.empty() && !this->mouseOnText) {
        DrawTextEx(this->font, this->placeHolder.c_str(), {this->textBox.x + 16 + size.x, this->textBox.y + float(this->textBox.height - size.y)/2}, this->fontSize, 2, BLACK);
    }
    else {
        DrawTextEx(this->font, this->text.c_str(), {this->textBox.x + 16 + size.x, this->textBox.y + float(this->textBox.height - size.y)/2}, this->fontSize, 2, BLACK);
    }
    
    if (this->mouseOnText) {
        if (this->text.length() < 30) {
            if (((this->framesCounter/20) % 2) == 0) {
                DrawTextEx(this->font, "|", {this->textBox.x + 16 + size.x, this->textBox.y + float(this->textBox.height - size.y)/2}, this->fontSize, 2, BLACK);
            }
        }
    }
}

void InputStr::resetText() {
    srand((int)time(0));
    this->changePlaceHolder(TextFormat("%d", rand() % 100));
}

void InputStr::update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), this->textBox)) {
            this->mouseOnText = true;
        } else {
            this->mouseOnText = false;
        }
    }

    if (this->mouseOnText) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && this->text.length() < 30) {
                this->text += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!this->text.empty()) {
                this->text.pop_back();
            }
        }
    }

    this->framesCounter++;
}

void InputStr::changePlaceHolder(const std::string newPlaceHolder) {
    this->placeHolder = newPlaceHolder;
}

std::string InputStr::getText() {
    std::string tmp = this->text;
    this->text.clear();
    return tmp.empty() ? this->placeHolder : tmp;
}

void CircleButton::draw() {
    DrawCircleV(this->center, this->radius, this->isHovered ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    drawPicture(this->texture, {center.x - radius + 2, center.y - radius +2, 2*radius -4, 2*radius -4});
}

void CircleButton::handle() {
    this->isHovered = CheckCollisionPointCircle(GetMousePosition(), this->center, this->radius);
}

void CircleButton::changeTexture(const char* file) {
    UnloadTexture(this->texture);
    this->texture = LoadTexture(file);
}
