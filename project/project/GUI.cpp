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
    DrawTextPro(this->font, this->content.c_str(), this->contentPos, ORIGIN, 0, this->fontSize, 2, this->contentColor);
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
    this->isHovered = CheckCollisionPointRec(GetMousePosition(), this->outerRect);
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

void ProgressBar::draw() {
    DrawCircle(33.5, 643.5, 17.5, doubleBack ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawCircle(72.5, 643.5, 17.5 , back ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawCircle(122, 643.5, 25.5 , play ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawCircle(169.5, 643.5, 17.5 , next ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawCircle(208.5, 643.5, 17.5 , doubleNext ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawCircle(263.5, 610.5, 12.5 , up ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    DrawCircle(263.5, 670.5, 12.5 , down ? HOVERED_BUTTON_LIGHT_THEME : NONHOVERED_BUTTON_LIGHT_THEME);
    
    DrawTextPro(font, TextFormat("%.1fx", this->speed), {368,775}, ORIGIN, 0.f, 15, 2, BLACK);
    
    drawPicture(DOUBLE_BACK, {19.5, 627.75, 28, 30.63});
    drawPicture(BACK, {56.75, 630.38, 28, 28});
    drawPicture(PLAY, {145, 650.5, 25, 25}, 0, {35.f, 35.f});
    drawPicture(NEXT, {194, 626, 28, 30.63});
    drawPicture(DOUBLE_NEXT, {254, 601, 20, 20});
    drawPicture(SPEED_UP, {251, 740, 30, 30});
    drawPicture(SPEED_DOWN, {370, 795, 30, 30});

    DrawRectangleRec({25, 728, 300, 10}, NONHOVERED_BUTTON_LIGHT_THEME);
    DrawRectangleRec({25, 728, this->maxStep != 0 ? this->curStep*300.f/this->maxStep : 0, 10}, SEPERATOR_COLOR);
}

void ProgressBar::updateMaxStep(int max) {
    this->maxStep = max;
    this->curStep = 0;
}

void ProgressBar::updateSpeed(float speed) {
    this->speed = speed;
}

//default -2 for skip back, 2 for skip next, else: back, next(-1,1)
void ProgressBar::updateStep(int step) {
    if (step == -2) this->curStep = 0;
    else if (step == 2) this->curStep = maxStep;
    else this->curStep += step;
}

int ProgressBar::handle() {
    if (CheckCollisionPointCircle(GetMousePosition(), {40, 790}, 25)) {
        this->doubleBack = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return -2;
        }
    } else this->doubleBack = false;
    
    if (CheckCollisionPointCircle(GetMousePosition(), {100, 790}, 25)) {
        this->back = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return -1;
        }
    } else this->back = false;
    
    if (CheckCollisionPointCircle(GetMousePosition(), {172, 790}, 37.5)) {
        this->play = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return 0;
        }
    } else this->play = false;

    if (CheckCollisionPointCircle(GetMousePosition(), {245, 790}, 25)) {
        this->next = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return 1;
        }
    } else this->next = false;

    if (CheckCollisionPointCircle(GetMousePosition(), {305, 790}, 25)) {
        this->doubleNext = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return 2;
        }
    } else this->doubleNext = false;

    if (CheckCollisionPointCircle(GetMousePosition(), {385, 755}, 15)) {
        this->up = true;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            // std::this_thread::sleep_for(std::chrono::milliseconds(200));
            this->speed += (this->speed < 2) ? 0.1 : 0;
            return 3;
        }
    } else this->up = false;

    if (CheckCollisionPointCircle(GetMousePosition(), {385, 810}, 15)) {
        this->down = true;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            // std::this_thread::sleep_for(std::chrono::milliseconds(200));
            this->speed -= (this->speed > 0.5) ? 0.1 : 0;
            return -3;
        }
    } else this->down = false;

    return 10;
}

void drawInfor(const std::string infor, Font font) {
    DrawTextEx(font, infor.c_str(), {50,371}, CODE_SIZE, 2, INFOR_COLOR);
}

void drawCode(std::string code, size_t n ,std::vector<int> highlight, Font font) {
    int lineHeight = 17;
    Vector2 CodeOffset = {25, 100};
    
    for (size_t i = 0; i<n; i++) {
        bool flag = false;
        for (auto line : highlight) {
            if (i == line) {
                flag = true;
                break;
            }
        }
        Color color = flag ? HIGHLIGHT_TEXT_COLOR : Color {0,0,0,0};
        DrawRectangle(CodeOffset.x, CodeOffset.y + i * lineHeight, 375, lineHeight, color);
    }
    DrawTextEx(font, code.c_str(), {CodeOffset.x, CodeOffset.y + 1}, CODE_SIZE, 2, WHITE);
}

void drawSideBar(std::string code, std::vector<int> lines, std::string infor, ProgressBar bar, Font font) {
    DrawRectangleRounded({0, 70, 425, 765}, 0.10f, 32, SIDEBAR_COLOR);
    DrawLineEx({0, 350}, {425, 350}, 2, SEPERATOR_COLOR);
    DrawLineEx({0, 470}, {425, 470}, 2, SEPERATOR_COLOR);
    DrawLineEx({170, 470}, {170, 715}, 2, SEPERATOR_COLOR);
    DrawLineEx({0, 715}, {425, 715}, 2, SEPERATOR_COLOR);
    drawCode(code, 8, lines, font);
    drawInfor(infor, font);
    bar.draw();
}
