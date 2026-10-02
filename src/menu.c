#include "menu.h"
#include "RaylibEx.h"
#include "beatmap.h"
#include "config.h"
#include "cursor.h"
#include "playfield.h"
#include "raylib.h"
#include "stdio.h"
#include <string.h>

bool MapsIndexed;
float Scroll = 0.6f;
unsigned int TotalMaps;
Maps MapIndex[1000];
Texture2D BG_Image_Map;
Music BG_Music_Map;
float gap;

void StartButton(void) {
  ClearBackground(BLACK);

  if (Draw_UI_Button((ScreenWidth - 210), (ScreenHeight - 210), 200, 200,
                     "Start")) {

    UnloadMusicStream(BG_Music_Map);
    UnloadTexture(BG_Image_Map);
    Init_Beatmap();
    StartGame = true;
  }
}

void IndexMaps(void) {

  FilePathList Maps = LoadDirectoryFilesEx("./Songs/", "DIRS*", false);

  for (unsigned int i = 0; i < Maps.count; i++) {
    MapIndex[i].name = GetFileName(Maps.paths[i]);

    printf("%s\n", MapIndex[i].name);
  }

  // UnloadDirectoryFiles(Maps);

  printf("Total Maps = %d\n", Maps.count);
  TotalMaps = Maps.count;
  BG_Image_Map = LoadTexture(TextFormat("Songs/%s/bg.jpg", config.map));
  BG_Music_Map = LoadMusicStream(TextFormat("Songs/%s/bg.mp3", config.map));
  PlayMusicStream(BG_Music_Map);
  MapsIndexed = true;
}

void DrawMaps(Maps *IndexMaps, float gap) {

  float RecW = ScreenWidth - (ScreenWidth * 0.7f);
  float RecX = ScreenWidth * 0.7f;
  float RecY = (ScreenHeight / 2.0f) + gap;

  Rectangle MapButton = {RecX, RecY, RecW, 80};

  bool Hovered = CheckCollisionPointRec(Get_Cursor_Position(), MapButton);

  Color MapButtonColor = Fade(DARKGREEN, 0.7f);

  if (!StartButtonHovered && Hovered) {

    MapButtonColor = Fade(GREEN, 0.7f);
    RecX = ScreenWidth * 0.6f;

  } else if (strcmp(config.map, IndexMaps->name) == 0) {

    MapButtonColor = Fade(DARKBLUE, 0.7f);
    RecX = ScreenWidth * 0.65f;
  }

  MapButton.x = RecX;

  DrawRectangleRec(MapButton, MapButtonColor);

  DrawText(IndexMaps->name, RecX, RecY, 20, WHITE);
  if (!StartButtonHovered &&
      (Hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {

    strcpy(config.map, IndexMaps->name);
    BG_Image_Map = LoadTexture(TextFormat("Songs/%s/bg.jpg", config.map));
    BG_Music_Map = LoadMusicStream(TextFormat("Songs/%s/bg.mp3", config.map));
    PlayMusicStream(BG_Music_Map);
  }
}

void Draw_Map_BG(void) {
  UpdateMusicStream(BG_Music_Map);
  if (config.bg_image) {

    Color DARKNESS = (Color){100, 100, 100, 255};

    SetTextureFilter(BG_Image_Map, TEXTURE_FILTER_BILINEAR);

    float To_Scale = (float)ScreenHeight / BG_Image_Map.height;
    float BG_Width = BG_Image_Map.width * To_Scale;

    float BG_X = (ScreenWidth - BG_Width) / 2.0f;

    DrawTexturePro(BG_Image_Map,
                   (Rectangle){0, 0, BG_Image_Map.width, BG_Image_Map.height},
                   (Rectangle){BG_X, 0, BG_Width, ScreenHeight},
                   (Vector2){0, 0}, 0.0f, DARKNESS);
  }
}

void Draw_Menu(void) {

  if (!MapsIndexed) {
    IndexMaps();
  }

  Draw_Map_BG();
  float ScrollRaw = GetMouseWheelMove();

  if (ScrollRaw > 0) {

    Scroll += 80.0f;
  } else if (ScrollRaw < 0) {

    Scroll -= 80.0f;
  }

  float MapGap = 100.0f;

  for (unsigned i = 0; i < TotalMaps; i++) {
    DrawMaps(&MapIndex[i], i * MapGap + Scroll);
  }

  StartButton();
}
