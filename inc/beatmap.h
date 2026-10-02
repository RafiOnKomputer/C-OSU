#ifndef BEATMAP_H
#define BEATMAP_H

#include "circle.h"
#include "raylib.h"

#define MAX_COMBO_COLOURS 9
#define MAX_TIMINGPOINTS 10000

void Init_Beatmap(void);
void Unload_Beatmap(void);
extern float Music_Clock;
extern HitCircle *circles;
extern int circle_count;
extern Music BG_Music;
extern Texture2D BG_Image_RAW;
extern RenderTexture2D BG_Image;
extern float AR;
extern float CS;
extern float OD;
extern float Radius;
extern float Diameter;
extern float ScreenRadius;
extern float ScreenDiameter;
extern float Approach_Time;
extern float Window_300;
extern float Window_100;
extern float Window_50;
extern float BorderX;
extern float BorderY;
extern float BorderWidth;
extern float BorderHeight;
extern float end_time;
extern float SkinScaleMode;
extern float ScreenWidth;
extern float ScreenHeight;
extern float AP_FadeIn;

typedef struct {
  int r;
  int g;
  int b;
} ComboColor;

typedef struct {
  int time;
  int bank;
  int hitvol;
} TimingPoint;

extern TimingPoint timingpoints[MAX_TIMINGPOINTS];
extern int timingpoint_count;

extern ComboColor combo_colours[MAX_COMBO_COLOURS];
extern int combo_count;

#endif
