/*
 *  beatmap.c
 *
 *  OSU Map Loader.
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
 *    - Reads Map Data Such As : Music, Image, CS, OD, AR, Combo Colors and
 *       Beatmap Objects
 *    - Sets SoundBank for each circles
 *    - Calculates OSU Playfield to Screen Cords
 *    - Setup Bg Image
 *    - Calculates AR, OD,CS
 */

#include "beatmap.h"
#include "circle.h"
#include "config.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
Music BG_Music;
Texture2D BG_Image_RAW;
RenderTexture2D BG_Image;
int circle_count;
float AR;
float CS;
float Radius;
float Approach_Time;
float Diameter;
float ScreenRadius;
float ScreenDiameter;
float Window_300;
float Window_100;
float Window_50;
float OD;

float OR_AR;
float OR_CS;
float OR_OD;

float BorderX;
float BorderY;
float BorderWidth;
float BorderHeight;
float end_time;

float ScreenWidth;
float ScreenHeight;

float SkinScaleMode;
float AP_FadeIn;
#define MAX_CIRCLES 100000
#define MAX_COMBO_COLOURS 9

ComboColor combo_colours[MAX_COMBO_COLOURS];
HitCircle *circles;

int line_number = 0;
int end_line = 0;

int combo_count = 0;
TimingPoint timingpoints[MAX_TIMINGPOINTS];
int timingpoint_count = 0;
static const char *MapPath(const char *file) {
  return TextFormat("Songs/%s/%s", config.map, file);
}

void Init_Beatmap(void) {
  circles = calloc(MAX_CIRCLES, sizeof(HitCircle));
  // circles = malloc(MAX_CIRCLES * sizeof(HitCircle));

  if (circles == NULL) {
    printf("Failed to allocate circles\n");
    return;
  }

  BG_Music = LoadMusicStream(MapPath("bg.mp3"));
  BG_Image_RAW = LoadTexture(MapPath("bg.jpg"));
  FILE *f = fopen(MapPath("map.osu"), "r");

  char line[1024];
  char SampleSet[16] = {0};

  // SampleSet
  while (fgets(line, sizeof(line), f)) {
    line_number++;

    if (sscanf(line, "SampleSet: %15s", SampleSet) == 1) {
      break;
    }
  }

  // CS
  while (fgets(line, sizeof(line), f)) {
    line_number++;

    if (sscanf(line, "CircleSize: %f", &CS) == 1) {

      break;
    }
  }

  // OD
  while (fgets(line, sizeof(line), f)) {
    line_number++;

    if (sscanf(line, "OverallDifficulty: %f", &OD) == 1) {

      break;
    }
  }

  // AR
  while (fgets(line, sizeof(line), f)) {
    line_number++;

    if (sscanf(line, "ApproachRate: %f", &AR) == 1) {

      break;
    }
  }

  // Timing Points

  while (fgets(line, sizeof(line), f)) {
    line_number++;
    if (strstr(line, "[TimingPoints]")) {
      break;
    }
  }
  while (fgets(line, sizeof(line), f)) {
    line_number++;
    if (strstr(line, "[Colours]")) {
      break;
    }
    if (sscanf(line, "%d,%*f,%*d,%d,%*d,%d,%*d,%*d",
               &timingpoints[timingpoint_count].time,
               &timingpoints[timingpoint_count].bank,
               &timingpoints[timingpoint_count].hitvol) == 3) {
      timingpoint_count++;
    }
  }

  // Combo Colors

  while (fgets(line, sizeof(line), f)) {
    line_number++;

    if (strstr(line, "[HitObjects]")) {
      break;
    }

    if (sscanf(line, " %*[^:]: %d,%d,%d", &combo_colours[combo_count].r,
               &combo_colours[combo_count].g,
               &combo_colours[combo_count].b) == 3) {

      combo_count++;
    }
  }

  // Beatmap Objects

  while (fgets(line, sizeof(line), f)) {
    int matched =
        sscanf(line, "%d,%d,%d,%d,%d", &circles[circle_count].x,
               &circles[circle_count].y, &circles[circle_count].time,
               &circles[circle_count].type, &circles[circle_count].soundtype);

    if (matched != 5) {
      continue;
    }

    circles[circle_count].normalset = 0;
    circles[circle_count].additionBank = 0;

    char *last_comma = strrchr(line, ',');

    if (last_comma && strchr(last_comma, ':') && !strchr(last_comma, '|')) {
      sscanf(last_comma + 1, "%d:%d", &circles[circle_count].normalset,
             &circles[circle_count].additionBank);
    }

    circles[circle_count].active = true;
    circle_count++;
  }

  // Set Combo Colors

  int current_color = 0;
  int combo_number = 1;

  for (int i = 0; i < circle_count; i++) {

    if (circles[i].type & 4) {
      current_color += ((circles[i].type >> 4) & 7) + 1;

      if (current_color >= combo_count)
        current_color %= combo_count;

      combo_number = 1;
    }

    circles[i].color = current_color;
    circles[i].ComboColor =
        (Color){combo_colours[current_color].r, combo_colours[current_color].g,
                combo_colours[current_color].b, 255};

    circles[i].combo_number = combo_number;

    combo_number++;
  }

  // Set SoundBanks

  int SoundBank = 1;

  if (strcmp(SampleSet, "Drum") == 0) {
    SoundBank = 3;
  } else if (strcmp(SampleSet, "Soft") == 0) {
    SoundBank = 2;
  } else {
    SoundBank = 1;
  }

  for (int i = 0; i < circle_count; i++) {
    circles[i].soundbank = SoundBank;
  }

  for (int a = 0; a < timingpoint_count; a++) {

    for (int i = 0; i < circle_count; i++) {

      if (circles[i].time >= timingpoints[a].time) {

        if (timingpoints[a].bank == 0)
          circles[i].soundbank = SoundBank;
        else
          circles[i].soundbank = timingpoints[a].bank;
      }
    }
  }

  // Set HitSound Vol

  for (int i = 0; i < circle_count; i++) {

    circles[i].HitSoundVol = 1.0;
  }

  for (int a = 0; a < timingpoint_count; a++) {

    for (int i = 0; i < circle_count; i++) {

      if (circles[i].time >= timingpoints[a].time) {

        circles[i].HitSoundVol = timingpoints[a].hitvol / 100.0f;
      }
    }
  }

  ScreenWidth = (float)GetScreenWidth();
  ScreenHeight = (float)GetScreenHeight();

  if (config.bg_image) {

    float ScreenAspect = (float)ScreenWidth / ScreenHeight;
    float ImageAspect = (float)BG_Image_RAW.width / BG_Image_RAW.height;

    if (ImageAspect < ScreenAspect) {

      // Used only if image is not wide

      Color DARKNESS = (Color){100, 100, 100, 255};

      BG_Image = LoadRenderTexture(ScreenWidth, ScreenHeight);

      BeginTextureMode(BG_Image);

      ClearBackground(BLACK);
      SetTextureFilter(BG_Image_RAW, TEXTURE_FILTER_BILINEAR);

      float BG_Scale = (float)ScreenWidth / BG_Image_RAW.width;
      float BG_Width = ScreenWidth;
      float BG_Height = BG_Image_RAW.height * BG_Scale;

      float BG_Y = (ScreenHeight - BG_Height) / 2.0f;

      Image BG_Blurred = LoadImageFromTexture(BG_Image_RAW);
      ImageBlurGaussian(&BG_Blurred, 3);
      Texture2D BG_Blurred_Texture = LoadTextureFromImage(BG_Blurred);
      DrawTexturePro(BG_Blurred_Texture,
                     (Rectangle){0, 0, BG_Image_RAW.width, BG_Image_RAW.height},
                     (Rectangle){0, BG_Y, BG_Width, BG_Height}, (Vector2){0, 0},
                     0.0f, DARKNESS);
      float To_Scale = (float)ScreenHeight / BG_Image_RAW.height;
      float width = BG_Image_RAW.width * To_Scale;

      int CenterX = (ScreenWidth - width) / 2;

      DrawTexturePro(BG_Image_RAW,
                     (Rectangle){0, 0, BG_Image_RAW.width, BG_Image_RAW.height},
                     (Rectangle){CenterX, 0, width, ScreenHeight},
                     (Vector2){0, 0}, 0.0f, DARKNESS);

      EndTextureMode();
      UnloadTexture(BG_Blurred_Texture);
      UnloadTexture(BG_Image_RAW);
      UnloadImage(BG_Blurred);
      // test
    } else {

      // used for normal bg

      Color DARKNESS = (Color){100, 100, 100, 255};
      float To_Scale = (float)ScreenHeight / BG_Image_RAW.height;
      float width = BG_Image_RAW.width * To_Scale;

      int CenterX = (ScreenWidth - width) / 2;

      BG_Image = LoadRenderTexture(ScreenWidth, ScreenHeight);

      BeginTextureMode(BG_Image);

      ClearBackground(BLACK);
      SetTextureFilter(BG_Image_RAW, TEXTURE_FILTER_BILINEAR);

      DrawTexturePro(BG_Image_RAW,
                     (Rectangle){0, 0, BG_Image_RAW.width, BG_Image_RAW.height},
                     (Rectangle){CenterX, 0, width, ScreenHeight},
                     (Vector2){0, 0}, 0.0f, DARKNESS);

      EndTextureMode();
    }
  };

  // Display res to OSU playfiled convertion

  const float Playfield_Width = 512.0f;
  const float Playfield_Height = 384.0f;
  const float BorderTop = 0.1166667f;
  const float BorderBottom = 0.0833333f;

  float TopBorder = BorderTop * ScreenHeight;
  float BottomBorder = BorderBottom * ScreenHeight;
  float PlayfieldMaxHeight = ScreenHeight - TopBorder - BottomBorder;

  float Scale;
  if ((ScreenWidth / Playfield_Width) > (PlayfieldMaxHeight / Playfield_Height))
    Scale = PlayfieldMaxHeight / Playfield_Height;
  else
    Scale = ScreenWidth / Playfield_Width;

  float PlayFieldWidth = Playfield_Width * Scale;
  float PlayFieldHeight = Playfield_Height * Scale;

  float PlayFieldx = (ScreenWidth - PlayFieldWidth) / 2.0f;
  float PlayFieldy = ScreenHeight - PlayFieldHeight - BottomBorder;

  float CircleRadius = (54.4f - 4.48f * CS) * 1.00041f;
  float BorderOffset = CircleRadius * Scale;

  BorderX = PlayFieldx - BorderOffset;
  BorderY = PlayFieldy - BorderOffset;

  BorderWidth = PlayFieldWidth + BorderOffset * 2.0f;
  BorderHeight = PlayFieldHeight + BorderOffset * 2.0f;

  if (BorderY < 0.0f) {
    BorderY = 0.0f;
  }

  if (BorderY + BorderHeight > ScreenHeight) {
    BorderHeight = ScreenHeight - BorderY;
  }

  for (int i = 0; i < circle_count; i++) {

    circles[i].xcord = PlayFieldx + (circles[i].x / 512.0f) * PlayFieldWidth;
    circles[i].ycord = PlayFieldy + (circles[i].y / 384.0f) * PlayFieldHeight;
  }

  if (config.OR_AR != 0.0) {
    AR = config.OR_AR;
  }

  if (config.OR_CS != 0.0) {
    CS = config.OR_CS;
  }

  if (config.OR_OD != 0.0) {
    OD = config.OR_OD;
  }

  // AR

  if (AR <= 5.0f) {
    Approach_Time = 1800.0f - 120.0f * AR;
  } else {
    Approach_Time = 1200.0f - 150.0f * (AR - 5.0f);
  }
  for (int i = 0; i < circle_count; i++) {

    circles[i].ApproachingTime = circles[i].time - Approach_Time;
  }
  AP_FadeIn = Approach_Time * (2.0f / 3.0f);

  // CS

  Radius = (54.4f - 4.48f * CS) * 1.00041f;
  Diameter = Radius * 2.0f;

  ScreenRadius = Radius * Scale;
  ScreenDiameter = Diameter * Scale;

  // OD

  Window_300 = floor(80.0 - 6.0 * OD) - 0.5;
  Window_100 = floor(140.0 - 8.0 * OD) - 0.5;
  Window_50 = floor(200.0 - 10.0 * OD) - 0.5;

  // making the array smaller

  HitCircle *tmp = realloc(circles, circle_count * sizeof(*circles));

  if (tmp != NULL)
    circles = tmp;

  fclose(f);

  end_time = circles[circle_count - 1].time;
  SkinScaleMode = config.hires ? 256.0f : 128.0f;
}

void Unload_Beatmap(void) {
  UnloadMusicStream(BG_Music);
  UnloadTexture(BG_Image_RAW);
  UnloadRenderTexture(BG_Image);
  free(circles);
  circles = NULL;
}
