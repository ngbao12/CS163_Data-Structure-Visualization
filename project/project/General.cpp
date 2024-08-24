#include "General.hpp"

Texture2D PLAY;
Texture2D PAUSE;
Texture2D REPLAY;
Texture2D NEXT;
Texture2D DOUBLE_NEXT;
Texture2D BACK;
Texture2D DOUBLE_BACK;
Texture2D SPEED_UP;
Texture2D SPEED_DOWN;
Texture2D BACK_PAGE;
Font FONT;
void initResource() {
    PLAY = LoadTexture("asset/Play.png");
    PAUSE = LoadTexture("asset/Pause.png");
    REPLAY = LoadTexture("asset/Replay.png");
    BACK = LoadTexture("asset/Chevron left.png");
    DOUBLE_BACK = LoadTexture("asset/Chevrons left.png");
    NEXT = LoadTexture("asset/Chevron right.png");
    DOUBLE_NEXT = LoadTexture("asset/Chevrons right.png");
    SPEED_UP = LoadTexture("asset/Chevron up.png");
    SPEED_DOWN = LoadTexture("asset/Chevron down.png");
    BACK_PAGE = LoadTexture("asset/BackPage.png");
    //FONT = LoadFont("./Font/Roboto-Regular.ttf");
    FONT = LoadFontEx("./Font/Roboto-Regular.ttf", 50, 0, 255);
}

Vector2 Vector2Lerp(Vector2 start, Vector2 end, float amount) {
    return (Vector2) {
        start.x + amount * (end.x - start.x),
        start.y + amount * (end.y - start.y)
    };
}

float FloatLerp(float start, float end, float amount) {
    return start + amount * (end - start);
}

void drawPicture(const char* path, Rectangle desRec, float rotation, Vector2 origin, Color color) {
    
    Texture2D texture = LoadTexture(path);
    if (texture.id == 0) return;
    Rectangle srcRect = {0.0f, 0.0f, (float) texture.width, (float) texture.height};
    DrawTexturePro(texture, srcRect, desRec, origin, rotation, color);
    UnloadTexture(texture);
}

void drawPicture(Texture2D texture, Rectangle desRec, float rotation, Vector2 origin, Color color) {
    if (texture.id == 0) return;
    Rectangle srcRect = {0.0f, 0.0f, (float) texture.width, (float) texture.height};
    DrawTexturePro(texture, srcRect, desRec, origin, rotation, color);
}

void drawRectangleWithBorder(Rectangle rec, Color colorRec, float widthBorder, Color colorBorder, int radiusBorder) {
    DrawRectangleRounded(rec, 1.0*radiusBorder/100, 64, colorRec);
    //DrawRectangleRoundedLines(rec, 1.0*radiusBorder/100, 64, widthBorder, colorBorder);
    DrawRectangleRoundedLinesEx(rec, 1.0*radiusBorder/100, 64, widthBorder, colorBorder);
}

Texture2D textureFromImageWithRoundedCorner(const char* path, int radius) {
    Image src = LoadImage(path);
    Color *pixels = LoadImageColors(src);
    
    for (int y = 0; y < src.height; y++) {
        for (int x = 0; x < src.width; x++) {
            bool drawPixel = true;
            if ((x < radius && y < radius && sqrt((x - radius)*(x - radius) + (y - radius)*(y - radius)) > radius) || // Top-left corner
                (x > src.width - radius && y < radius && sqrt((x - (src.width - radius))*(x - (src.width - radius)) + (y - radius)*(y - radius)) > radius) || // Top-right corner
                (x < radius && y > src.height - radius && sqrt((x - radius)*(x - radius) + (y - (src.height - radius))*(y - (src.height - radius))) > radius) || // Bottom-left corner
                (x > src.width - radius && y > src.height - radius && sqrt((x - (src.width - radius))*(x - (src.width - radius)) + (y - (src.height - radius))*(y - (src.height - radius))) > radius)) // Bottom-right corner
            {
                drawPixel = false;
            }

            if (drawPixel) {
                pixels[y * src.width + x] = pixels[y * src.width + x];
            }
            else {
                pixels[y * src.width + x] = BLANK;
            }
        }
    }
    
    free(src.data);
    src.data = pixels;
    Texture2D texture = LoadTextureFromImage(src);
    
    UnloadImage(src);
    
    return texture;
}

void drawShrinkingText(const char* text, Vector2 position, int fontSize, float* scale, float* alpha, float shrinkRate, float fadeRate) {
    *scale -= shrinkRate;
    *alpha -= fadeRate;
    
    if (*scale < 0) *scale = 0;
    if (*alpha < 0) *alpha = 0;
    
    Vector2 textSize = MeasureTextEx(FONT, text, fontSize*(*scale), 1);
    Vector2 textPosition = {position.x - textSize.x / 2, position.y - textSize.y / 2};
    
    DrawTextEx(FONT, text, textPosition, fontSize* (*scale), 1, Fade(BLACK, *alpha));
}

void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color) {
    DrawRectangleRoundedLines(rec, roundness, segments, color);
}
