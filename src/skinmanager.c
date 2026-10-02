#include "config.h"
#include "raylib.h"

Texture2D CursorTexture;
Sound MissSound;
Texture2D HitCircleTexture;
Texture2D OverlayCircle;
Texture2D Miss;
Texture2D judge100;
Texture2D judge50;
Texture2D NumTextures[10];
Texture2D accSS;
Texture2D accS;
Texture2D accA;
Texture2D accB;
Texture2D accC;
Texture2D accD;

Sound normal_hitnormal;
Sound normal_hitwhistle;
Sound normal_hitfinish;
Sound normal_hitclap;

Sound soft_hitnormal;
Sound soft_hitwhistle;
Sound soft_hitfinish;
Sound soft_hitclap;

Sound drum_hitnormal;
Sound drum_hitwhistle;
Sound drum_hitfinish;
Sound drum_hitclap;

static const char *SkinPath(const char *file) {
  return TextFormat("Skins/%s/%s", config.skin, file);
}

static void FilterTextureBilinear(Texture2D texture) {
  if (IsTextureValid(texture)) {
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
  }
}

void init_skin(void) {

  MissSound = LoadSound("Skins/shared/miss.mp3");

  normal_hitnormal = LoadSound(SkinPath("normal-hitnormal.wav"));
  normal_hitwhistle = LoadSound(SkinPath("normal-hitwhistle.wav"));
  normal_hitfinish = LoadSound(SkinPath("normal-hitfinish.wav"));
  normal_hitclap = LoadSound(SkinPath("normal-hitclap.wav"));

  soft_hitnormal = LoadSound(SkinPath("soft-hitnormal.wav"));
  soft_hitwhistle = LoadSound(SkinPath("soft-hitwhistle.wav"));
  soft_hitfinish = LoadSound(SkinPath("soft-hitfinish.wav"));
  soft_hitclap = LoadSound(SkinPath("soft-hitclap.wav"));

  drum_hitnormal = LoadSound(SkinPath("drum-hitnormal.wav"));
  drum_hitwhistle = LoadSound(SkinPath("drum-hitwhistle.wav"));
  drum_hitfinish = LoadSound(SkinPath("drum-hitfinish.wav"));
  drum_hitclap = LoadSound(SkinPath("drum-hitclap.wav"));

  if (!IsSoundValid(normal_hitnormal))
    UnloadSound(normal_hitnormal);

  if (!IsSoundValid(normal_hitwhistle))
    UnloadSound(normal_hitwhistle);

  if (!IsSoundValid(normal_hitfinish))
    UnloadSound(normal_hitfinish);

  if (!IsSoundValid(normal_hitclap))
    UnloadSound(normal_hitclap);

  if (!IsSoundValid(soft_hitnormal))
    UnloadSound(soft_hitnormal);

  if (!IsSoundValid(soft_hitwhistle))
    UnloadSound(soft_hitwhistle);

  if (!IsSoundValid(soft_hitfinish))
    UnloadSound(soft_hitfinish);

  if (!IsSoundValid(soft_hitclap))
    UnloadSound(soft_hitclap);

  if (!IsSoundValid(drum_hitnormal))
    UnloadSound(drum_hitnormal);

  if (!IsSoundValid(drum_hitwhistle))
    UnloadSound(drum_hitwhistle);

  if (!IsSoundValid(drum_hitfinish))
    UnloadSound(drum_hitfinish);

  if (!IsSoundValid(drum_hitclap))
    UnloadSound(drum_hitclap);
  // TEXTURES
  if (config.hires) {
    accSS = LoadTexture(SkinPath("ranking-X-small@2x.PNG"));
    accS = LoadTexture(SkinPath("ranking-S-small@2x.PNG"));
    accA = LoadTexture(SkinPath("ranking-A-small@2x.png"));
    accB = LoadTexture(SkinPath("ranking-B-small@2x.png"));
    accC = LoadTexture(SkinPath("ranking-C-small@2x.png"));
    accD = LoadTexture(SkinPath("ranking-D-small@2x.png"));

    CursorTexture = LoadTexture(SkinPath("cursor@2x.png"));
    HitCircleTexture = LoadTexture(SkinPath("hitcircle@2x.png"));
    OverlayCircle = LoadTexture(SkinPath("hitcircleoverlay@2x.png"));

    Miss = LoadTexture(SkinPath("hit0-0@2x.png"));
    judge50 = LoadTexture(SkinPath("hit50-0@2x.png"));
    judge100 = LoadTexture(SkinPath("hit100-0@2x.png"));

    for (int i = 0; i < 10; i++) {
      NumTextures[i] =
          LoadTexture(TextFormat("Skins/%s/default-%d@2x.png", config.skin, i));
    }

  } else {
    accSS = LoadTexture(SkinPath("ranking-X-small.PNG"));
    accS = LoadTexture(SkinPath("ranking-S-small.PNG"));
    accA = LoadTexture(SkinPath("ranking-A-small.png"));
    accB = LoadTexture(SkinPath("ranking-B-small.png"));
    accC = LoadTexture(SkinPath("ranking-C-small.png"));
    accD = LoadTexture(SkinPath("ranking-D-small.png"));

    CursorTexture = LoadTexture(SkinPath("cursor.png"));
    HitCircleTexture = LoadTexture(SkinPath("hitcircle.png"));
    OverlayCircle = LoadTexture(SkinPath("hitcircleoverlay.png"));

    Miss = LoadTexture(SkinPath("hit0-0.png"));
    judge50 = LoadTexture(SkinPath("hit50-0.png"));
    judge100 = LoadTexture(SkinPath("hit100-0.png"));

    for (int i = 0; i < 10; i++) {
      NumTextures[i] =
          LoadTexture(TextFormat("Skins/%s/default-%d.png", config.skin, i));
    }
  }

  // BILINEAR FILTER
  if (config.bilinear) {
    FilterTextureBilinear(CursorTexture);
    FilterTextureBilinear(HitCircleTexture);
    FilterTextureBilinear(OverlayCircle);
    FilterTextureBilinear(Miss);
    FilterTextureBilinear(judge100);
    FilterTextureBilinear(judge50);

    FilterTextureBilinear(accSS);
    FilterTextureBilinear(accS);
    FilterTextureBilinear(accA);
    FilterTextureBilinear(accB);
    FilterTextureBilinear(accC);
    FilterTextureBilinear(accD);

    for (int i = 0; i < 10; i++) {
      FilterTextureBilinear(NumTextures[i]);
    }
  }
}
void Unload_Skin(void) {

  // TEXTURES
  UnloadTexture(CursorTexture);

  UnloadTexture(accSS);
  UnloadTexture(accS);
  UnloadTexture(accA);
  UnloadTexture(accB);
  UnloadTexture(accC);
  UnloadTexture(accD);

  UnloadTexture(HitCircleTexture);
  UnloadTexture(OverlayCircle);
  UnloadTexture(Miss);

  UnloadTexture(judge100);
  UnloadTexture(judge50);

  for (int i = 0; i < 10; i++) {
    UnloadTexture(NumTextures[i]);
  }

  // SOUNDS

  if (IsSoundValid(MissSound))
    UnloadSound(MissSound);

  if (IsSoundValid(normal_hitnormal))
    UnloadSound(normal_hitnormal);
  if (IsSoundValid(normal_hitwhistle))
    UnloadSound(normal_hitwhistle);
  if (IsSoundValid(normal_hitfinish))
    UnloadSound(normal_hitfinish);
  if (IsSoundValid(normal_hitclap))
    UnloadSound(normal_hitclap);

  if (IsSoundValid(soft_hitnormal))
    UnloadSound(soft_hitnormal);
  if (IsSoundValid(soft_hitwhistle))
    UnloadSound(soft_hitwhistle);
  if (IsSoundValid(soft_hitfinish))
    UnloadSound(soft_hitfinish);
  if (IsSoundValid(soft_hitclap))
    UnloadSound(soft_hitclap);

  if (IsSoundValid(drum_hitnormal))
    UnloadSound(drum_hitnormal);
  if (IsSoundValid(drum_hitwhistle))
    UnloadSound(drum_hitwhistle);
  if (IsSoundValid(drum_hitfinish))
    UnloadSound(drum_hitfinish);
  if (IsSoundValid(drum_hitclap))
    UnloadSound(drum_hitclap);
}
