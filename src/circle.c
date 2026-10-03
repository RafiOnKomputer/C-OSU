#include "circle.h"
#include "beatmap.h"
#include "config.h"
#include "playfield.h"
#include "raylib.h"
#include "skinmanager.h"
#include <math.h>
#include <stdbool.h>
int P300;
int P100;
int P50;
int miss;

static void ReleaseClock(HitCircle *circle) { circle->ClockRunning = false; }

static double FadeOutClock(HitCircle *circle) {
  return (CurrentTime - circle->endtime) * 1000.0f;
}

static double FadeInClock(HitCircle *circle) {

  // used for scaling, opacity and approach circle when circle fades in

  if (!circle->ClockRunning) {
    circle->CircleAP =
        CurrentTime - (Music_Clock - circle->ApproachingTime) / 1000.0f;
    circle->ClockRunning = true;
  }

  return (CurrentTime - circle->CircleAP) * 1000.0f;
}

int Draw_HitCircle(HitCircle *circle) {

  // main func that draws all circles

  if (!circle->active && !circle->exiting)
    return 0;

  if (circle->exiting) {

    // this animation is only for missed/clicked circles
    // its fade out

    double elapsed = FadeOutClock(circle); // Clock Starts here (for fade out)

    float CanvasMapX = circle->xcord;
    float CanvasMapY = circle->ycord;

    if (elapsed < 200.0f) { // animation is 200ms only for now

      float Time = elapsed / 200.0f;
      if (Time > 1.0f)
        Time = 1.0f;

      float Fade_Amount = 0.9f - Time; // i made it fade out 10% faster

      float Scale = 1.0f + (0.38f * Time); // it will max scale .38
      float CS_R = ScreenDiameter * Scale;

      Color ComboColor = Fade(circle->ComboColor, Fade_Amount);
      Color OverlayColor = Fade(WHITE, Fade_Amount);

      DrawTexturePro(
          HitCircleTexture,
          (Rectangle){0, 0, HitCircleTexture.width, HitCircleTexture.height},
          (Rectangle){CanvasMapX, CanvasMapY, CS_R, CS_R},
          (Vector2){CS_R / 2.0f, CS_R / 2.0f}, 0.0f, ComboColor);

      float OverlayScale = (ScreenDiameter / SkinScaleMode) * Scale;
      float OverlayWidth = OverlayCircle.width * OverlayScale;
      float OverlayHeight = OverlayCircle.height * OverlayScale;
      // test
      DrawTexturePro(
          OverlayCircle,
          (Rectangle){0, 0, OverlayCircle.width, OverlayCircle.height},
          (Rectangle){CanvasMapX, CanvasMapY, OverlayWidth, OverlayHeight},
          (Vector2){OverlayWidth / 2.0f, OverlayHeight / 2.0f}, 0.0f,
          OverlayColor);

      if (circle->combo_number >= 1 && circle->combo_number <= 9) {

        // only combo number 1-9 is supported for now..

        Texture2D texture = NumTextures[circle->combo_number];

        float NumScale = (ScreenDiameter / SkinScaleMode) * 0.8f * Scale;

        DrawTexturePro(
            texture, (Rectangle){0, 0, texture.width, texture.height},
            (Rectangle){CanvasMapX, CanvasMapY, texture.width * NumScale,
                        texture.height * NumScale},
            (Vector2){texture.width * NumScale / 2.0f,
                      texture.height * NumScale / 2.0f},
            0.0f, OverlayColor);
      }
    }

    if (config.judgecircle && elapsed < 800.0f) {

      // this is the mean judge circles they are not programmed here as osu way
      // but it i will do for now i will fix it before v 1.0.0 release...i hope

      float radius = 75.0f; // fixed random num for now
      float diameter = 150.0f;
      float fade;

      if (elapsed < 120.0f) {

        // fade in

        fade = elapsed / 120.0f;

      } else if (elapsed < 500.0f) {

        // stays opaq

        fade = 1.0f;

      } else {

        // fade out

        fade = 1.0f - (elapsed - 500.0f) / 300.0f;

        if (fade < 0.0f)
          fade = 0.0f;
      }

      Color color = (Color){255, 255, 255, (unsigned char)(255 * fade)};

      if (circle->missed) {

        DrawTexturePro(Miss, (Rectangle){0, 0, Miss.width, Miss.height},
                       (Rectangle){CanvasMapX, CanvasMapY, diameter, diameter},
                       (Vector2){radius, radius}, 0.0f, color);

      } else if (circle->Score == 50) {

        DrawTexturePro(judge50,
                       (Rectangle){0, 0, judge50.width, judge50.height},
                       (Rectangle){CanvasMapX, CanvasMapY, diameter, diameter},
                       (Vector2){radius, radius}, 0.0f, color);

      } else if (circle->Score == 100) {

        DrawTexturePro(judge100,
                       (Rectangle){0, 0, judge100.width, judge100.height},
                       (Rectangle){CanvasMapX, CanvasMapY, diameter, diameter},
                       (Vector2){radius, radius}, 0.0f, color);
      }
    }

    if (elapsed >= (config.judgecircle ? 800.0 : 200.0)) {
      circle->exiting = false;
    }

    return 0;
    // end of fade out animations
  }

  if (!circle->active)
    return 0;

  if (Music_Clock < circle->ApproachingTime)
    return 100;

  // now starts fade in circle animations

  double Elapsed = FadeInClock(circle); // clock starts here for fade in circles

  float Fade_Amount = Elapsed / AP_FadeIn;

  if (Fade_Amount > 1.0f)
    Fade_Amount = 1.0f;

  Color ComboColor = Fade(circle->ComboColor, Fade_Amount);
  Color OverlayColor = Fade(WHITE, Fade_Amount);

  float CanvasMapX = circle->xcord;
  float CanvasMapY = circle->ycord;

  if (Elapsed >= Approach_Time + Window_50) {

    // count as miss if too late

    circle->missed = true;
    circle->exiting = true;
    circle->endtime = CurrentTime;
    circle->active = false;

    miss = miss + 1;
    ReleaseClock(circle); // release the clock and exit on miss
    return 0;
  }

  if (Elapsed < Approach_Time) {

    // Approach Circle

    float Scale = 4.0f + (1.0f - 4.0f) * (Elapsed / Approach_Time);

    float ApproachRadius = (ScreenDiameter * Scale) / 2.0f;
    float Thickness = 8.0f;

    // this will be replaced with texture later

    DrawRing((Vector2){CanvasMapX, CanvasMapY}, ApproachRadius - Thickness,
             ApproachRadius, 0.0f, 360.0f, 64, ComboColor);
  }

  // Hit circle

  DrawTexturePro(
      HitCircleTexture,
      (Rectangle){0, 0, HitCircleTexture.width, HitCircleTexture.height},
      (Rectangle){CanvasMapX, CanvasMapY, ScreenDiameter, ScreenDiameter},
      (Vector2){ScreenDiameter / 2, ScreenDiameter / 2}, 0, ComboColor);

  float OverlayScale = ScreenDiameter / SkinScaleMode;

  float OverlayWidth = OverlayCircle.width * OverlayScale;
  float OverlayHeight = OverlayCircle.height * OverlayScale;

  // Hit Circle Overlay

  DrawTexturePro(
      OverlayCircle,
      (Rectangle){0, 0, OverlayCircle.width, OverlayCircle.height},
      (Rectangle){CanvasMapX, CanvasMapY, OverlayWidth, OverlayHeight},
      (Vector2){OverlayWidth / 2.0f, OverlayHeight / 2.0f}, 0.0f, OverlayColor);

  if (circle->combo_number >= 1 && circle->combo_number <= 9) {
    Texture2D texture = NumTextures[circle->combo_number];

    float NumScale = (ScreenDiameter / SkinScaleMode) * 0.8f;

    // Combo Numbers

    DrawTexturePro(texture, (Rectangle){0, 0, texture.width, texture.height},
                   (Rectangle){CanvasMapX, CanvasMapY, texture.width * NumScale,
                               texture.height * NumScale},
                   (Vector2){texture.width * NumScale / 2.0f,
                             texture.height * NumScale / 2.0f},
                   0, OverlayColor);
  } else if (circle->combo_number >= 10 && circle->combo_number <= 99) {
    int tens = circle->combo_number / 10;
    int ones = circle->combo_number % 10;

    Texture2D texture1 = NumTextures[tens];
    Texture2D texture2 = NumTextures[ones];

    float NumScale = (ScreenDiameter / SkinScaleMode) * 0.8f;

    float NumWidth1 = texture1.width * NumScale;
    float NumWidth2 = texture2.width * NumScale;
    float NumStartX = CanvasMapX - (NumWidth1 + NumWidth2) / 2.0f;

    // Combo Numbers

    DrawTexturePro(
        texture1, (Rectangle){0, 0, texture1.width, texture1.height},
        (Rectangle){NumStartX + NumWidth1 / 2.0f, CanvasMapY, NumWidth1,
                    texture1.height * NumScale},
        (Vector2){NumWidth1 / 2.0f, texture1.height * NumScale / 2.0f}, 0,
        OverlayColor);

    DrawTexturePro(
        texture2, (Rectangle){0, 0, texture2.width, texture2.height},
        (Rectangle){NumStartX + NumWidth1 + NumWidth2 / 2.0f, CanvasMapY,
                    NumWidth2, texture2.height * NumScale},
        (Vector2){NumWidth2 / 2.0f, texture2.height * NumScale / 2.0f}, 0,
        OverlayColor);
  }
  return 0;
}

int Hit_HitCircle(HitCircle *circle, HitCircle *prev, bool FirstCircle,
                  Vector2 cursor, bool HitInput) {

  // this func Handles Input

  if (!circle->active)
    return 100;

  if (Music_Clock < circle->ApproachingTime)
    return 200;

  if (!FirstCircle && prev->active)
    return 0;

  double Elapsed = FadeInClock(circle); // clock starts here

  float CanvasMapX = circle->xcord;
  float CanvasMapY = circle->ycord;

  bool MouseOnCircle = CheckCollisionPointCircle(
      cursor, (Vector2){CanvasMapX, CanvasMapY}, ScreenRadius);

  if (!MouseOnCircle || !HitInput)

    return 0;

  // the click sound system is mess
  // i will fix later

  int normalset = circle->normalset;

  if (normalset == 0)
    normalset = circle->soundbank;

  int additionset = circle->additionBank;

  if (additionset == 0)
    additionset = normalset;

  float EffectVol = config.EffectVol;

  // NORMAL
  if (normalset == 1) {
    if (IsSoundValid(normal_hitnormal)) {
      SetSoundVolume(normal_hitnormal, circle->HitSoundVol * EffectVol);
      PlaySound(normal_hitnormal);
    }
  } else if (normalset == 2) {
    if (IsSoundValid(soft_hitnormal)) {
      SetSoundVolume(soft_hitnormal, circle->HitSoundVol * EffectVol);
      PlaySound(soft_hitnormal);
    }
  } else if (normalset == 3) {
    if (IsSoundValid(drum_hitnormal)) {
      SetSoundVolume(drum_hitnormal, circle->HitSoundVol * EffectVol);
      PlaySound(drum_hitnormal);
    }
  }

  // WHISTLE
  if (circle->soundtype & 2) {

    if (additionset == 1) {
      if (IsSoundValid(normal_hitwhistle)) {
        SetSoundVolume(normal_hitwhistle, circle->HitSoundVol * EffectVol);
        PlaySound(normal_hitwhistle);
      }
    } else if (additionset == 2) {
      if (IsSoundValid(soft_hitwhistle)) {
        SetSoundVolume(soft_hitwhistle, circle->HitSoundVol * EffectVol);
        PlaySound(soft_hitwhistle);
      }
    } else if (additionset == 3) {
      if (IsSoundValid(drum_hitwhistle)) {
        SetSoundVolume(drum_hitwhistle, circle->HitSoundVol * EffectVol);
        PlaySound(drum_hitwhistle);
      }
    }
  }

  // FINISH
  if (circle->soundtype & 4) {

    if (additionset == 1) {
      if (IsSoundValid(normal_hitfinish)) {
        SetSoundVolume(normal_hitfinish, circle->HitSoundVol * EffectVol);
        PlaySound(normal_hitfinish);
      }
    } else if (additionset == 2) {
      if (IsSoundValid(soft_hitfinish)) {
        SetSoundVolume(soft_hitfinish, circle->HitSoundVol * EffectVol);
        PlaySound(soft_hitfinish);
      }
    } else if (additionset == 3) {
      if (IsSoundValid(drum_hitfinish)) {
        SetSoundVolume(drum_hitfinish, circle->HitSoundVol * EffectVol);
        PlaySound(drum_hitfinish);
      }
    }
  }

  // CLAP
  if (circle->soundtype & 8) {

    if (additionset == 1) {
      if (IsSoundValid(normal_hitclap)) {
        SetSoundVolume(normal_hitclap, circle->HitSoundVol * EffectVol);
        PlaySound(normal_hitclap);
      }
    } else if (additionset == 2) {
      if (IsSoundValid(soft_hitclap)) {
        SetSoundVolume(soft_hitclap, circle->HitSoundVol * EffectVol);
        PlaySound(soft_hitclap);
      }
    } else if (additionset == 3) {
      if (IsSoundValid(drum_hitclap)) {
        SetSoundVolume(drum_hitclap, circle->HitSoundVol * EffectVol);
        PlaySound(drum_hitclap);
      }
    }
  }

  // This checks how late/early u clicked and judges the point

  double offset = Elapsed - (double)Approach_Time;

  double diff = fabs(offset);
  if (circle->type & 2) {

    // sliders are not added yet so its auto 300 for now
    // sliders are planed for v 0.2.0

    P300++;
    circle->Score = 300;
  } else if (diff <= Window_300) {
    P300++;
    circle->Score = 300;
  } else if (diff <= Window_100) {
    P100++;
    circle->Score = 100;
  } else if (diff <= Window_50) {
    P50++;
    circle->Score = 50;
  } else {

    miss++;
    circle->missed = true;
    circle->Score = 0;
  }
  circle->active = false;
  circle->exiting = true;
  circle->endtime = CurrentTime;

  ReleaseClock(circle); // release the clock and exit
  return 1;
}

int Draw_Guide(HitCircle *circle, HitCircle *next) {

  // this is the line that guides to next circle
  // need improvement and might replace with texture later
  // but the combo colors looks cool as f on this ngl
  // but i cant put anti aliasin so i need to replace it with texture later

  if (!circle->active)
    return 0;
  if (!next->active)
    return 0;
  if (Music_Clock < circle->ApproachingTime)
    return 0;
  if (next->time - circle->ApproachingTime > 1000)
    return 0;

  float CircleX = circle->xcord;
  float CircleY = circle->ycord;
  float NextX = next->xcord;
  float NextY = next->ycord;
  double Elapsed = FadeInClock(circle);

  float gap = next->time - circle->time;
  if (gap <= 0.0)
    return 0;

  float Fade_Amount = Elapsed / gap;

  if (Fade_Amount > 1.0f)
    Fade_Amount = 1.0f;

  float alpha;

  if (Fade_Amount < 0.5f) {
    alpha = 0.3f + (Fade_Amount / 0.5f) * 0.7f;
  } else {
    alpha = 0.3f + ((1.0f - Fade_Amount) / 0.5f) * 0.7f;
  }
  Color comboColor = Fade(circle->ComboColor, alpha);
  DrawLineEx((Vector2){CircleX, CircleY}, (Vector2){NextX, NextY}, 3.0f,
             comboColor);
  return 1;
}

void RestWindow(HitCircle *circle) {

  // its the intro wait counter that lets u skip too
  // this can be used to skip in gaps in middle of map too
  // i will make 2 saparate skiper later one for intro and one for middle of map

  float gap = circle->time - Music_Clock;

  if (gap <= 0.0)
    return;

  if (gap < 5000.0f) {
    return;
  }
  if (Music_Clock < 1.0f) {
    return;
  }

  float wait = gap - 5000.0f;

  float skip_time = (circle->time - Approach_Time * 2.0f) / 1000.0f;

  const char *textWait = TextFormat("Wait %.0f sec", wait / 1000);
  const char *textskip = TextFormat("Press Space To Skip");

  DrawText(textWait, ScreenWidth / 2.0f - MeasureText(textWait, 50) / 2.0f,
           GetScreenHeight() / 2, 50, (Color){255, 161, 0, UI_Fade});

  DrawText(textskip, ScreenWidth / 2.0f - MeasureText(textskip, 20) / 2.0f,
           (GetScreenHeight() / 2.0f) * 1.1f, 20,
           (Color){255, 255, 255, UI_Fade});

  if (IsKeyPressed(KEY_SPACE)) {

    SeekMusicStream(BG_Music, skip_time);
  }
}
