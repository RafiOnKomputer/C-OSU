#include "RaylibEx.h"
#include "beatmap.h"
#include "cursor.h"
#include "raylib.h"
// just random shit

bool StartButtonHovered;

float GetCenterX() { return ScreenWidth / 2.0f; }

float GetCenterY(void) { return ScreenHeight / 2.0f; }

void DrawTextCentered(const char *text, float X, float Y, int size,
                      Color color) {

  DrawText(text, (GetCenterX() - MeasureText(text, size) / 2.0f) * X,
           ScreenHeight * Y, size, color);
}
int Draw_UI_Button(float x, float y, float Width, float Height,
                   const char *Text) {

  Rectangle Button = {x, y, Width, Height};

  bool Hovered = CheckCollisionPointRec(Get_Cursor_Position(), Button);

  static Color ButtonColor;

  if (Hovered) {

    ButtonColor = BLUE;
    StartButtonHovered = true;
  } else {
    ButtonColor = DARKBLUE;
    StartButtonHovered = false;
  }

  DrawRectangleRec(Button, ButtonColor);

  DrawText(Text, x + Width / 2.0 - (MeasureText(Text, 24) / 2.0),
           y + Height / 2.0 - (24 / 2.0), 24, WHITE);

  if (Hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    return 1;
  }
  return 0;
}
