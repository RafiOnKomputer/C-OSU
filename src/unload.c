/*
 *  unload.c
 *
 *  unloads game.
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
 *    - unloads game
 */

#include "beatmap.h"
#include "raylib.h"
#include "skinmanager.h"

void Unload_COSU(void) {

  Unload_Skin();
  Unload_Beatmap();
  CloseAudioDevice();
  CloseWindow();
}
