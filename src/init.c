/*
 *  init.c
 *
 *  code that runs at init.
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
 *  You should have received a copy of the GNU General Public License along
 * with C-OSU. If not, see <https://www.gnu.org/licenses/>.
 *
 *  DESCRIPTION :
 *
 *    - Sets Vsync and fullscreen
 *    - Sets FPS limit
 *    - Gets Screen Res
 */
#include "beatmap.h"
#include "config.h"
#include "cursor.h"
#include "raylib.h"
#include "rlgl.h"
#include "skinmanager.h"
#include <stdio.h>

#ifdef _WIN32
extern __declspec(dllimport) int __stdcall SetProcessDPIAware(void);
#endif

// i got all init stuff here so game.c will look clean

void Init_COSU(void) {

#ifdef _WIN32
  SetProcessDPIAware();
#endif

  printf("\n\n=== C-OSU Alpha ===\n\n");

  LoadConfig();

  unsigned int flags = 0;

  if (config.vsync)
    flags |= FLAG_VSYNC_HINT;

  if (config.fullscreen)
    flags |= FLAG_FULLSCREEN_MODE;

  if (config.native_res) {

    InitWindow(0, 0, "C-OSU");
    int CurrentMonitor = GetCurrentMonitor();
    config.width = GetMonitorWidth(CurrentMonitor);
    config.height = GetMonitorHeight(CurrentMonitor);
    printf("\nAUTO RES %dx%d\n", config.width, config.height);
    CloseWindow();
  }

  SetConfigFlags(flags);

  InitWindow(config.width, config.height, "C-OSU");
  InitAudioDevice();

  SetTargetFPS(config.fpsLimit);

  ScreenWidth = GetScreenWidth();
  ScreenHeight = GetScreenHeight();

  init_skin();
  Init_Cursor();

  if (rlGetVersion() == RL_OPENGL_21) {
    printf("\nRaylib Backend: OpenGL 2.1\n");
  }

  else if (rlGetVersion() == RL_OPENGL_33) {
    printf("\nRaylib Backend: OpenGL 3.3\n");
  }
}
