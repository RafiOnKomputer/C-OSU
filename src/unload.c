#include "beatmap.h"
#include "raylib.h"
#include "skinmanager.h"

void Unload_COSU(void) {

  Unload_Skin();
  Unload_Beatmap();
  CloseAudioDevice();
  CloseWindow();
}
