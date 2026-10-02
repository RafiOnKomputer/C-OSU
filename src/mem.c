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
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
  return pmc.WorkingSetSize / 1024.0f / 1024.0f;
#else
  FILE *f = fopen("/proc/self/status", "r");
  char line[64];
  float mb = 0;

  while (fgets(line, sizeof(line), f))
    if (sscanf(line, "VmRSS: %f", &mb) == 1)
      break;

  fclose(f);
  return mb / 1024.0f;
#endif
}
