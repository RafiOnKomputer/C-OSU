#ifndef CONFIG_H
#define CONFIG_H

typedef struct {

  int nativeres;
  int width;
  int height;
  int fullscreen;
  int fpsLimit;
  int vsync;
  int bg_image;
  int playfield;
  int drawprogress;
  int drawcounter;
  int drawacc;
  int drawguider;
  int drawfps;
  int drawframetime;
  int drawmem;
  int bilinear;
  int hires;
  int judgecircle;
  char skin[128];
  char map[128];
  float MouseSens;
  float cursor_size;
  float EffectVol;
  float OR_AR;
  float OR_CS;
  float OR_OD;

} Config;

extern Config config;

void LoadConfig(void);

#endif
