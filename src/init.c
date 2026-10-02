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

  LoadConfig();

  unsigned int flags = 0;

  if (config.vsync)
    flags |= FLAG_VSYNC_HINT;

  if (config.fullscreen)
    flags |= FLAG_FULLSCREEN_MODE;

  SetConfigFlags(flags);

  InitWindow(config.width, config.height, "C-OSU");

  if (!IsWindowReady())
    return;

  InitAudioDevice();

  if (!IsAudioDeviceReady()) {
    CloseWindow();
    return;
  }

  SetTargetFPS(config.fpsLimit);

  ScreenWidth = (float)GetScreenWidth();
  ScreenHeight = (float)GetScreenHeight();

  init_skin();
  Init_Cursor();

  printf("Raylib OpenGL Backend: %d\n", rlGetVersion());

  if (rlGetVersion() == RL_OPENGL_21)
    printf("Raylib backend: OpenGL 2.1\n");
  else if (rlGetVersion() == RL_OPENGL_33)
    printf("Raylib backend: OpenGL 3.3\n");
}
