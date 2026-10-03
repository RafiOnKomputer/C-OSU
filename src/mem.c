/*
 *  mem.c
 *
 *  Gets Ram Usage.
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
 *    - Gets Ram Usage on windows and linux
 */

#include "mem.h"
#include <stdio.h>

#ifdef _WIN32
// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on
#endif

// this place is a fucking mess but works
// i will clean up later

float GetMemoryMB(void) {

  // for windows

#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
  return pmc.WorkingSetSize / 1024.0f / 1024.0f;
#else

  // for linux

  FILE *f = fopen("/proc/self/status", "r");
  char line[64];
  float Ram_Usage = 0;

  while (fgets(line, sizeof(line), f))
    if (sscanf(line, "VmRSS: %f", &Ram_Usage) == 1)
      break;

  fclose(f);
  return Ram_Usage / 1024.0f;
#endif
}
