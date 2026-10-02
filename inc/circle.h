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
  double endtime;
  int normalset;
  int additionBank;
  int ApproachingTime;
  int Score;
  float xcord;
  float ycord;
  bool active;
  bool exiting;
  bool missed;
  bool ClockRunning;
  double CircleAP;
  Color ComboColor;
  float HitSoundVol;

} HitCircle;

void Init_HitCircle(void);
void Unload_HitCircle(void);

int Draw_HitCircle(HitCircle *circle);
int Hit_HitCircle(HitCircle *circle, HitCircle *prev, bool FirstCircle,
                  Vector2 cursor, bool HitInput);
int Draw_Guide(HitCircle *circle, HitCircle *next);
void RestWindow(HitCircle *circle);

extern int P300;
extern int P100;
extern int P50;
extern int miss;

#endif
