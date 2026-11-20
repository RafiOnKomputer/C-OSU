#ifndef CIRCLE_H
#define CIRCLE_H
#include "raylib.h"
#include <stdbool.h>

typedef struct {
  int x;
  int y;
  int time;
  int type;
  int soundtype;
  int soundbank;
  int color;
  int combo_number;
  int normalset;
  int additionBank;
  double ApproachingTime;
  int Score;
  float xcord;
  float ycord;
  float HitSoundVol;
  double CircleAP;
  double endtime;
  bool active;
  bool exiting;
  bool missed;
  bool ClockRunning;
  Color ComboColor;

} HitCircle;

int Render_HitObjects(HitCircle *circle);
int Hit_Checker(HitCircle *circle, HitCircle *prev, bool FirstCircle,
                Vector2 cursor, bool HitInput);
int Draw_Guide(HitCircle *circle, HitCircle *next);
void RestWindow(HitCircle *circle);

extern int P300;
extern int P100;
extern int P50;
extern int miss;

#endif
