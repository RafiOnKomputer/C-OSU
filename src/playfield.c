#include "RaylibEx.h"
#include "beatmap.h"
#include "circle.h"
#include "config.h"
#include "cursor.h"
#include "mem.h"
#include "menu.h"
#include "raylib.h"
#include "skinmanager.h"
double accuracy = 100;
bool FirstCircle;
int ShowFPS;
float ShowFrametime;
double Clock = -1.0;
float Music_Clock = -1788.0;
double WaitTimer;
double CurrentTime;
static Color FpsColor;
static bool music_started = false;
static bool UI_Drawable = true;
int StartCircle;
int EndCircle;
int LastAnimate;
float UI_Fade;
float UI_Fade_Time;
bool DoneFadeIn;

float UI_Fade_BG;
float UI_Fade_Time_BG;

float UI_Fade_Result;
float UI_Fade_Time_Result;

bool StartGame;
bool CountFPS;

int FrameCount;
float TotalTime;
float AvgFPS;
float LocalClock;
void Draw_BG(void) {

  if (config.bg_image) {

    // BG image

    if (!(UI_Fade_BG >= 255.0f)) {
      UI_Fade_Time_BG += GetFrameTime() * 1000.0;
      UI_Fade_BG = 255.0 * (UI_Fade_Time_BG / 1000.0);

      ClearBackground(BLACK);

      if (UI_Fade_BG > 255.0f) {
        UI_Fade_BG = 255.0f;
      }
    }

    DrawTextureRec(
        BG_Image.texture,
        (Rectangle){0, 0, BG_Image.texture.width, -BG_Image.texture.height},
        (Vector2){0, 0}, (Color){255, 255, 255, UI_Fade_BG});
  }

  else {

    ClearBackground(BLACK); // remove this line for 10x fps
  }
}

void Draw_Playfield_Boarder(void) {

  // Playfield boarder (this scales up with CS size btw so not accurate circle
  // cords)

  Color HALF_WHITE = (Color){128, 128, 128, 255};

  DrawLine(BorderX, BorderY, BorderX + BorderWidth, BorderY, HALF_WHITE);

  DrawLine(BorderX, BorderY, BorderX, BorderY + BorderHeight, HALF_WHITE);

  DrawLine(BorderX + BorderWidth, BorderY, BorderX + BorderWidth,
           BorderY + BorderHeight, HALF_WHITE);

  DrawLine(BorderX, BorderY + BorderHeight, BorderX + BorderWidth,
           BorderY + BorderHeight, HALF_WHITE);
}

void Draw_Playfield(void) {

  Draw_BG();

  if (config.playfield) {

    Draw_Playfield_Boarder();
  }
}

void Draw_Circles(void) {

  // calls all func for drawing circles

  // Music Clock

  if (!music_started) {
    WaitTimer += LocalClock * 1000.0f;
    Music_Clock = WaitTimer - Approach_Time * 2.0f;

    if (WaitTimer >= Approach_Time * 2.0f) {
      PlayMusicStream(BG_Music);
      music_started = true;
    }
  }

  if (music_started) {
    UpdateMusicStream(BG_Music);
    Music_Clock = (GetMusicTimePlayed(BG_Music) * 1000.0f);
  }

  // and a clock

  CurrentTime = GetTime();

  // checks input every frame

  bool HitInput = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
                  IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) ||
                  IsKeyPressed(KEY_X) || IsKeyPressed(KEY_Z) ||
                  IsKeyPressed(KEY_SPACE);

  Vector2 Cursor = Get_Cursor_Position();

  // Circle Clicker

  for (int i = StartCircle; i < circle_count; i++) {

    bool FirstCircle = (i == 0);

    int ReturnValue;

    ReturnValue = Hit_HitCircle(&circles[i], &circles[i - 1], FirstCircle,
                                Cursor, HitInput);

    if (ReturnValue == 1) {
      break;
    } else if (ReturnValue == 100) {
      StartCircle = i;

    } else if (ReturnValue == 200) {
      EndCircle = i;
      break;
    }
  }

  // find which judge circles are still animating

  for (int i = LastAnimate; i < EndCircle; i++) {
    if (circles[i].exiting) {
      LastAnimate = i;
      break;
    }
  }

  // the guide thingy

  for (int i = LastAnimate; i < EndCircle; i++)
    Draw_Guide(&circles[i], &circles[i + 1]);

  // The acual circles

  for (int i = EndCircle; i >= LastAnimate; i--) {
    Draw_HitCircle(&circles[i]);
  }

  // Wait timer/Skiper i think it can be improved later

  for (int i = StartCircle; i < circle_count; i++) {
    if (circles[i].active) {
      RestWindow(&circles[i]);
      break;
    }
  }
}

void Draw_UI(void) {

  // All Ui elements

  // 300ms fade in

  if (Music_Clock > 0.1 && !(UI_Fade == 255.0f) && UI_Drawable && !DoneFadeIn) {

    UI_Fade_Time += LocalClock * 1000.0;
    UI_Fade = 255.0 * (UI_Fade_Time / 300.0f);

    if (UI_Fade > 255.0f) {
      UI_Fade = 255.0f;
      DoneFadeIn = true;
      CountFPS = true;
    }
  }

  float time_left = (Music_Clock / end_time) * 100.0f;

  if (P300 + P100 + P50 + miss == 0) {
    accuracy = 100.0;
  } else {
    accuracy = (300.0 * P300 + 100.0 * P100 + 50.0 * P50) /
               (300.0 * (P300 + P100 + P50 + miss)) * 100.0;
  }

  // The Music Progress in bottom

  if (config.drawprogress && UI_Drawable) {
    const char *text = TextFormat("Progress %.2f%%", time_left);
    DrawTextCentered(text, 1.0f, 0.95f, 32, (Color){253, 249, 0, UI_Fade});
  }

  // the jude numbers in top left

  if (config.drawcounter && UI_Drawable) {
    DrawText(
        TextFormat(
            " 300 = %d\n 100 = %d\n 50 = %d\n Miss = %d\n Accuracy = %.2f%\n",
            P300, P100, P50, miss, accuracy),
        8, 8, 28, (Color){255, 255, 255, UI_Fade});
  }

  // the acc icon in bottom

  if (config.drawacc && UI_Drawable) {
    if (accuracy == 100.0) {

      DrawTexture(accSS, (ScreenWidth / 2.0f - (accSS.width / 2.0f)),
                  ScreenHeight * 0.85f, (Color){255, 255, 255, UI_Fade});
    } else if (accuracy >= 95 && miss == 0) {

      DrawTexture(accS, (ScreenWidth / 2.0f - (accS.width / 2.0f)),
                  ScreenHeight * 0.85f, (Color){255, 255, 255, UI_Fade});
    } else if (accuracy >= 90) {

      DrawTexture(accA, (ScreenWidth / 2.0f - (accA.width / 2.0f)),
                  ScreenHeight * 0.85f, (Color){255, 255, 255, UI_Fade});
    } else if (accuracy >= 80) {

      DrawTexture(accB, (ScreenWidth / 2.0f - (accB.width / 2.0f)),
                  ScreenHeight * 0.85f, (Color){255, 255, 255, UI_Fade});
    } else if (accuracy >= 70) {

      DrawTexture(accC, (ScreenWidth / 2.0f - (accC.width / 2.0f)),
                  ScreenHeight * 0.85f, (Color){255, 255, 255, UI_Fade});
    } else {

      DrawTexture(accD, (ScreenWidth / 2.0f - (accD.width / 2.0f)),
                  ScreenHeight * 0.85f, (Color){255, 255, 255, UI_Fade});
    }
  }

  // The Fps and Frametime in bottom right

  if ((config.drawfps || config.drawframetime) && UI_Drawable) {
    int CurrentFps = GetFPS();
    float CurrentFrametime = LocalClock * 1000.0f;
    Clock += LocalClock;

    if (Clock >= 0.1) {

      // updates every 100ms  or this shit is unreadable

      ShowFPS = CurrentFps;
      ShowFrametime = CurrentFrametime;
      Clock = 0.0;
    }

    // Draws FPS

    if (config.drawfps && UI_Drawable) {
      if (ShowFPS >= 1000) {
        FpsColor = (Color){0, 228, 48, UI_Fade};
      } else if (ShowFPS <= 100) {
        FpsColor = (Color){230, 41, 55, UI_Fade};
      } else {
        FpsColor = (Color){255, 161, 0, UI_Fade};
      }
      const char *fps = TextFormat("%d FPS", ShowFPS);
      float fpsx = ScreenWidth - MeasureText(fps, 24) - 15;
      float fpsy = ScreenHeight - 24 - 15;

      DrawText(fps, fpsx, fpsy, 24, FpsColor);
    }

    // Draws Frametime

    if (config.drawframetime && UI_Drawable) {
      const char *frametime = TextFormat("%.2f ms", ShowFrametime);

      float frametimex = ScreenWidth - MeasureText(frametime, 20) - 15;
      float frametimey = ScreenHeight - 20 - 40;

      DrawText(frametime, frametimex, frametimey, 20,
               (Color){253, 249, 0, UI_Fade});
    }
  }

  // draws ram usage in botom right

  if (config.drawmem && UI_Drawable) {

    static float MemoryMB;
    static float Clock_mem;

    Clock_mem += LocalClock;

    if (Clock_mem >= 3.0 || (MemoryMB == 0.0)) {

      // updates every 3000ms (3s)

      MemoryMB = GetMemoryMB();
      Clock_mem = 0.0;
    }

    const char *mem = TextFormat("%.2f MB", MemoryMB);
    float memx = ScreenWidth - MeasureText(mem, 20) - 15;
    float memy = ScreenHeight - 20 - 65;

    DrawText(mem, memx, memy, 20, (Color){0, 121, 241, UI_Fade});
  }

  // Counts AVG FPS whole time

  if (CountFPS && Music_Clock < end_time) {

    FrameCount++;

    TotalTime += LocalClock;

    AvgFPS = FrameCount / TotalTime;
  }

  // result screen

  if ((Music_Clock - 1000.0f >= end_time) &&
      circles[circle_count - 1].active == false) {

    // fades in the result

    if (!(UI_Fade_Result == 255.0f)) {
      UI_Fade_Time_Result += LocalClock * 1000.0;
      UI_Fade_Result = 255.0 * (UI_Fade_Time_Result / 500.0f);

      if (UI_Fade_Result > 255.0f) {
        UI_Fade_Result = 255.0f;
      }
    }

    // fades out all the ui elements

    if (Music_Clock > 0.1 && !(UI_Fade == 0.0f) && UI_Drawable) {
      UI_Fade_Time += LocalClock * 1000.0;
      UI_Fade = 255.0 - (255.0 * (UI_Fade_Time / 250.0f));

      if (UI_Fade < 0.0f) {
        UI_Fade = 0.0f;
        UI_Drawable = false;
      }
    }

    // draws the result

    float textY = ScreenHeight * 0.25f;

    float CloseIn = GetMusicTimeLength(BG_Music) - GetMusicTimePlayed(BG_Music);
    DrawText(TextFormat("300s = %d ", P300), 10, textY, 56,
             (Color){0, 228, 48, UI_Fade_Result});

    DrawText(TextFormat("100s = %d ", P100), 10, textY * 1.3f, 56,
             (Color){253, 249, 0, UI_Fade_Result});

    DrawText(TextFormat("50s = %d ", P50), 10, textY * 1.6f, 56,
             (Color){255, 161, 0, UI_Fade_Result});

    DrawText(TextFormat("Missed = %d", miss), 10, textY * 1.9f, 56,
             (Color){230, 41, 55, UI_Fade_Result});

    DrawText(TextFormat("Accuracy = %.2f%%", accuracy), 10, textY * 2.2f, 56,
             (Color){255, 255, 255, UI_Fade_Result});

    DrawText(TextFormat("Closing Window In %.2fs", CloseIn), 10, textY * 2.5f,
             56, (Color){255, 255, 255, UI_Fade_Result});

    DrawText(TextFormat("AVG FPS = %.2f", AvgFPS), 10, textY * 2.8f, 56,
             (Color){255, 255, 255, UI_Fade_Result});
  }
}

// gets called from game.c to draw all this

void Draw_COSU(void) {

  if (!StartGame) {
    Draw_Menu();
    return;
  }

  LocalClock = GetFrameTime();
  Draw_Playfield();
  Draw_Circles();
  Draw_UI();
}
