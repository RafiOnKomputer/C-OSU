#ifndef SKINMANAGER_H
#define SKINMANAGER_H
#include "raylib.h"
void init_skin(void);
void Unload_Skin(void);

extern Texture2D CursorTexture;
extern Texture2D HitCircleTexture;
extern Texture2D OverlayCircle;
extern Texture2D ApproachCircle;

extern Texture2D NumTextures[10];
extern Texture2D Miss;
extern Texture2D judge50;
extern Texture2D judge100;

extern Texture2D accSS;
extern Texture2D accS;
extern Texture2D accA;
extern Texture2D accB;
extern Texture2D accC;
extern Texture2D accD;

extern Sound MissSound;

extern Sound normal_hitclap;
extern Sound normal_hitfinish;
extern Sound normal_hitnormal;
extern Sound normal_hitwhistle;

extern Sound soft_hitclap;
extern Sound soft_hitfinish;
extern Sound soft_hitnormal;
extern Sound soft_hitwhistle;
extern Sound soft_hitsoft;

extern Sound drum_hitclap;
extern Sound drum_hitfinish;
extern Sound drum_hitnormal;
extern Sound drum_hitwhistle;

extern bool normal_hitnormal_valid;
extern bool normal_hitwhistle_valid;
extern bool normal_hitfinish_valid;
extern bool normal_hitclap_valid;

extern bool soft_hitnormal_valid;
extern bool soft_hitwhistle_valid;
extern bool soft_hitfinish_valid;
extern bool soft_hitclap_valid;

extern bool drum_hitnormal_valid;
extern bool drum_hitwhistle_valid;
extern bool drum_hitfinish_valid;
extern bool drum_hitclap_valid;

#endif
