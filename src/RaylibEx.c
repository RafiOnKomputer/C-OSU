/*
 *  RaylibEx.c
 *
 *  just random shit.
 *
 *  Copyright (C) 2026 RafiOnKomputer
 *
 *  This file is part of C-OSU.
 *
 *  C-OSU is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  C-OSU is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along with
 *  C-OSU. If not, see <https://www.gnu.org/licenses/>.
 *
 *  DESCRIPTION :
 *
 *    - just random shit
 */

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
