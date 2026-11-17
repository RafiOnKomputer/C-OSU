.PHONY: build linux64 linux32 win64 win32 release run clean

CC  = gcc
CC_Win64 = x86_64-w64-mingw32-gcc
CC_Win32 = i686-w64-mingw32-gcc
WARN = -Wall -Wextra -Wpedantic
# WINDOWS XP AND DEBIAN 8 SUPPORT SOON !!!
WINDOWS_VER = -DWINVER=0x0601 -D_WIN32_WINNT=0x0601
CFLAGS = -O2 -flto=auto -ffunction-sections -fdata-sections -fno-math-errno -fno-trapping-math -Wl,--gc-sections -s
LIBS = -lSDL2 -lm
LIBS_WIN = -lm -lsetupapi -lole32 -loleaut32 -limm32 -lgdi32 -lversion -lwinmm
FILE = game.c $(wildcard src/*.c)
OUTPUT_PATH = ./C-OSU/

INC = -I./inc
SDL2_WIN_INC = -I./external/SDL2/x86_64-w64-mingw32/include
SDL2_WIN32_INC = -I./external/SDL2/i686-w64-mingw32/include

RAYLIB = -I./external/raylib/src/
RAYLIB_BIN = ./external/raylib/src/bins/liblinux64.a
RAYLIB_BIN32 = ./external/raylib/src/bins/liblinux32.a
RAYLIB_BIN_WIN = ./external/raylib/src/bins/libwin64.a
RAYLIB_BIN_WIN32 = ./external/raylib/src/bins/libwin32.a


SDL2_WIN = ./external/SDL2/x86_64-w64-mingw32/lib/libSDL2.a
SDL2_WIN32 = ./external/SDL2/i686-w64-mingw32/lib/libSDL2.a

OS   = $(shell uname -s)
ARCH = $(shell uname -m)

GREEN = $(shell printf '\033[38;2;0;255;0m')
AMBER = $(shell printf '\033[38;2;249;126;2m')
RED   = $(shell printf '\033[38;2;255;0;0m')
RESET = $(shell printf '\033[0m')

ifeq ($(OS),Linux)
  ifeq ($(ARCH),x86_64)
    AUTO = linux64
  else ifeq ($(ARCH),i686)
    AUTO = linux32
  else
    $(error $(RED)Unsupported CPU Architecture: $(ARCH)$(RESET))
  endif
else ifneq (,$(findstring _NT,$(OS)))
  ifeq ($(ARCH),x86_64)
    AUTO = win64
    CC_Win64 = gcc
  else ifeq ($(ARCH),i686)
    AUTO = win32
    CC_Win32 = gcc
  else
    $(error $(RED)Unsupported CPU Architecture: $(ARCH)$(RESET))
  endif
else
  $(error $(RED)Unsupported OS: $(OS)$(RESET))
endif

linux64:
	@echo  "$(GREEN)OS : GNU/Linux x86_64$(RESET)"
	@echo  "$(AMBER)Executable Binary : C-OSU.Linux64$(RESET)"
	$(CC) -m64 $(WARN) $(CFLAGS) -march=x86-64 -mtune=generic -fno-semantic-interposition -fno-plt $(INC) $(RAYLIB) $(FILE) $(RAYLIB_BIN) $(LIBS) -o $(OUTPUT_PATH)C-OSU.Linux64

linux32:
	@echo  "$(GREEN)OS : GNU/Linux x86$(RESET)"
	@echo  "$(AMBER)Executable Binary : C-OSU.Linux32$(RESET)"
	$(CC) -m32 $(WARN) $(CFLAGS) -march=i686 -mtune=i686 -fno-semantic-interposition -fno-plt $(INC) $(RAYLIB) $(FILE) $(RAYLIB_BIN32) $(LIBS) -o $(OUTPUT_PATH)C-OSU.Linux32

win64:
	@echo  "$(GREEN)OS : Microslop Windows x86_64$(RESET)"
	@echo  "$(AMBER)Executable Binary : C-OSU.Win64.exe$(RESET)"
	$(CC_Win64) -m64 -Wl,--allow-multiple-definition $(WARN) $(CFLAGS) -march=x86-64 -mtune=generic -static-libgcc  $(WINDOWS_VER) $(INC) $(SDL2_WIN_INC) $(RAYLIB) $(FILE) $(SDL2_WIN) $(RAYLIB_BIN_WIN)  $(LIBS_WIN) -o $(OUTPUT_PATH)C-OSU.Win64.exe

win32:
	@echo  "$(GREEN)OS : Microslop Windows x86$(RESET)"
	@echo  "$(AMBER)Executable Binary : C-OSU.Win32.exe$(RESET)"
	$(CC_Win32) -m32 -Wl,--allow-multiple-definition $(WARN) $(CFLAGS) -march=i686 -mtune=i686 -static-libgcc $(WINDOWS_VER) $(INC) $(SDL2_WIN32_INC) $(RAYLIB) $(FILE) $(SDL2_WIN32) $(RAYLIB_BIN_WIN32)  $(LIBS_WIN) -o $(OUTPUT_PATH)C-OSU.Win32.exe

run:
	cd $(OUTPUT_PATH) && gamemoderun ./C-OSU.Linux64

clean:
	rm -f $(OUTPUT_PATH)C-OSU.*

build: $(AUTO)


release: linux64 linux32 win64 win32

