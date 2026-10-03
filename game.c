/*
 *  game.c
 *
 *  Renders the game.
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
 *    - Just main file that loads the game and runs draw loop
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#endif

#include "beatmap.h"
#include "cursor.h"
#include "init.h"
#include "playfield.h"
#include "raylib.h"
#include "stdio.h"
#include "unload.h"

int main() {

  Init_COSU();

  while (!WindowShouldClose()) {

    BeginDrawing();

    Draw_COSU();

    DrawText("C-OSU  v0.1.3-pre-alpha", 10, ScreenHeight * 0.97f, 20,
             (Color){255, 255, 255, 128});

    if (GetMusicTimePlayed(BG_Music) >= GetMusicTimeLength(BG_Music) - 0.1f) {
      printf(" 300 = %d\n 100 = %d\n 50 = %d\n Miss = %d\n Accuracy = %.2f%% "
             "\n AVG FPS = %.2f\n",
             P300, P100, P50, miss, accuracy, AvgFPS);
      Unload_COSU();
      return 0;
    }

    Draw_Cursor();

    EndDrawing();
  }

  Unload_COSU();
  return 0;
}
