/*
 *  config.c
 *
 *  Loads config.ini.
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
 *    - reads config.ini
 */

#include "config.h"
#include <stdio.h>
Config config;

void LoadConfig(void) {
  FILE *f = fopen("config.ini", "r");
  if (!f)
    return;

  fscanf(f, "WIDTH=%d\n", &config.width);
  fscanf(f, "HEIGHT=%d\n", &config.height);
  fscanf(f, "FULLSCREEN=%d\n", &config.fullscreen);
  fscanf(f, "FPS_LIMIT=%d\n", &config.fpsLimit);
  fscanf(f, "VSYNC=%d\n", &config.vsync);
  fscanf(f, "MOUSE_SENS=%f\n", &config.MouseSens);

  fscanf(f, "BG_IMAGE=%d\n", &config.bg_image);
  fscanf(f, "PLAYFIELD=%d\n", &config.playfield);
  fscanf(f, "JUDGE_CIRCLE=%d\n", &config.judgecircle);

  fscanf(f, "PROGRESS=%d\n", &config.drawprogress);
  fscanf(f, "COUNTER=%d\n", &config.drawcounter);
  fscanf(f, "ACC=%d\n", &config.drawacc);
  fscanf(f, "GUIDER=%d\n", &config.drawguider);

  fscanf(f, "SHOW_FPS=%d\n", &config.drawfps);
  fscanf(f, "SHOW_FRAMETIME=%d\n", &config.drawframetime);
  fscanf(f, "SHOW_MEM=%d\n", &config.drawmem);

  fscanf(f, "CURSOR_SIZE=%f\n", &config.cursor_size);
  fscanf(f, "BILINEAR_FILTERING=%d\n", &config.bilinear);
  fscanf(f, "HI-RES_SKIN=%d\n", &config.hires);
  fscanf(f, "SKIN=%127s\n", config.skin);
  fscanf(f, "HitSound_Vol=%f\n", &config.EffectVol);
  fscanf(f, "MAP=%127s\n", config.map);

  fscanf(f, "OR_AR=%f\n", &config.OR_AR);
  fscanf(f, "OR_CS=%f\n", &config.OR_CS);
  fscanf(f, "OR_OD=%f\n", &config.OR_OD);

  fclose(f);
}
