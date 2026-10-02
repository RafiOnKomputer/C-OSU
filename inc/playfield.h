#ifndef PLAYFIELD_H
#define PLAYFIELD_H

void Draw_Playfield(void);
void Draw_Circles(void);
void Draw_UI(void);
void Draw_COSU(void);
extern float Music_Clock;
extern double accuracy;
extern double CurrentTime;
extern float AvgFPS;
extern float UI_Fade;
extern float LocalClock;
extern bool StartGame;

#endif
