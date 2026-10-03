/*
 *  cursor.c
 *
 *  Renders Cursor.
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
 *    - Sets SDL2 Raw Mouse Input
 *    - Draws Cursor
 *    - Outputs Cursor Cord
 */

#define SDL_MAIN_HANDLED

#include "cursor.h"
#include "beatmap.h"
#include "config.h"
#include "raylib.h"
#include "skinmanager.h"

#include <SDL2/SDL.h>

static Vector2 CursorPos = {0};

void Init_Cursor(void) {

  HideCursor();

  // RAW INPUT!!

  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SYSTEM_SCALE, "0");
  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "0");
  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "0");

  SDL_SetRelativeMouseMode(SDL_TRUE);

  CursorPos.x = ScreenWidth / 2.0f;
  CursorPos.y = ScreenHeight / 2.0f;
}

void Draw_Cursor(void) {

  int dx;
  int dy;

  SDL_GetRelativeMouseState(&dx, &dy);

  // Sens

  CursorPos.x += (float)dx * config.MouseSens;
  CursorPos.y += (float)dy * config.MouseSens;

  // Prevents Mouse from Going off the screen

  if (CursorPos.x < 0.0f)
    CursorPos.x = 0.0f;

  if (CursorPos.y < 0.0f)
    CursorPos.y = 0.0f;

  if (CursorPos.x > (float)ScreenWidth)
    CursorPos.x = (float)ScreenWidth;

  if (CursorPos.y > (float)ScreenHeight)
    CursorPos.y = (float)ScreenHeight;

  float CusorSize = config.cursor_size * 8.0f;

  DrawTexturePro(CursorTexture,
                 (Rectangle){0, 0, CursorTexture.width, CursorTexture.height},
                 (Rectangle){CursorPos.x, CursorPos.y, CusorSize, CusorSize},
                 (Vector2){CusorSize / 2, CusorSize / 2}, 0.0f, WHITE);
}

Vector2 Get_Cursor_Position(void) { return CursorPos; }
