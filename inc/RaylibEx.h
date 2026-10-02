#ifndef RaylibEx
#define RaylibEx
#include "raylib.h"
void DrawTextCentered(const char *text, float X, float Y, int size,
                      Color color);

float GetCenterX(void);
float GetCenterY(void);
int Draw_UI_Button(float x, float y, float Width, float Height,
                   const char *Text);

extern bool StartButtonHovered;
#endif
