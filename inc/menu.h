#ifndef MENU_H
#define MENU_H

void Draw_Menu(void);

typedef struct {
  const char *name;
} Maps;

extern Maps MapIndex[1000];

#endif
