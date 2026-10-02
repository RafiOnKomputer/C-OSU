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

      return 0;
    }

    Draw_Cursor();

    EndDrawing();
  }

  Unload_COSU();
  return 0;
}
