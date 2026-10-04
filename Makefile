.DEFAULT_GOAL := linux64

CC      = gcc
WIN64CC = x86_64-w64-mingw32-gcc
WIN32CC = i686-w64-mingw32-gcc

OUT  = ./C-OSU
SRC  = game.c $(wildcard src/*.c)
INC  = -I./inc -I./Raylib/
WARN = -Wall -Wextra -Wpedantic


NPROC := $(shell nproc 2>/dev/null || echo 4)
ifeq ($(MAKELEVEL),0)
MAKEFLAGS += -j$(NPROC)
endif

GAMEMODE := $(shell command -v gamemoderun 2>/dev/null)

CFLAGS = -std=c17 -O3 -flto=$(NPROC) \
         -ffunction-sections -fdata-sections \
         -fno-semantic-interposition -fno-plt \
         -fno-math-errno -fno-trapping-math \
         $(WARN) $(INC)

LDFLAGS = -flto=$(NPROC) -Wl,--gc-sections -s

X86_32_FLAGS = -m32 -fno-pie
X86_32_LDFLAGS = -no-pie

DEV_OPT    ?= -O0
DEV_CFLAGS  = -std=c17 $(DEV_OPT) -g0 -pipe $(WARN) $(INC) -m64 -MMD -MP
DEV_OBJ     = $(patsubst %.c,build/dev/%.o,$(SRC))
DEV_BIN     = $(OUT)/C-OSU.dev.linux64

DEBUG_CFLAGS = -std=c17 -O1 -g3 \
               -fsanitize=address,undefined \
               -fno-omit-frame-pointer \
               $(WARN) $(INC) -m64 -MMD -MP

DEBUG_LDFLAGS = -fsanitize=address,undefined
DEBUG_OBJ     = $(patsubst %.c,build/debug/%.o,$(SRC))
DEBUG_BIN     = $(OUT)/C-OSU.debug.linux64

REL_DIR    = build/rel
REL64_OBJ  = $(patsubst %.c,$(REL_DIR)/linux64/%.o,$(SRC))
REL32_OBJ  = $(patsubst %.c,$(REL_DIR)/linux32/%.o,$(SRC))
RELW64_OBJ = $(patsubst %.c,$(REL_DIR)/win64/%.o,$(SRC))
RELW32_OBJ = $(patsubst %.c,$(REL_DIR)/win32/%.o,$(SRC))


# Linux 64
LINUX64_LIBS = ./Raylib/bins/liblinux64.a \
               -L/usr/lib/x86_64-linux-gnu \
               -lm -ldl -lpthread \
               -lX11 -lxcb -lGL -lGLX -lXext \
               -lGLdispatch -lXau -lXdmcp \
               -lSDL2

LINUX64_STATIC_LIBS = ./Raylib/bins/liblinux64.a \
               /usr/lib/x86_64-linux-gnu/libSDL2.a \
               -lm -ldl -lpthread \
               -lX11 -lxcb -lGL -lGLX -lXext \
               -lGLdispatch -lXau -lXdmcp \
               -lasound -lpulse -lpulse-simple -lsamplerate \
               -lXcursor -lXi -lXfixes -lXrandr -lXss \
               -ldrm -lgbm -lwayland-egl -lwayland-client -lwayland-cursor \
               -lxkbcommon -ldecor-0

# Linux 32
LINUX32_LIBS = ./Raylib/bins/liblinux32.a \
               -L/usr/lib/i386-linux-gnu \
               -lm -ldl -lpthread \
               -lX11 -lxcb -lGL -lGLX -lXext \
               -lGLdispatch -lXau -lXdmcp \
               -lSDL2

LINUX32_STATIC_LIBS = ./Raylib/bins/liblinux32.a \
               /usr/lib/i386-linux-gnu/libSDL2.a \
               -lm -ldl -lpthread \
               -lX11 -lxcb -lGL -lGLX -lXext \
               -lGLdispatch -lXau -lXdmcp \
               -lasound -lpulse -lpulse-simple -lsamplerate \
               -lXcursor -lXi -lXfixes -lXrandr -lXss \
               -ldrm -lgbm -lwayland-egl -lwayland-client -lwayland-cursor \
               -lxkbcommon -ldecor-0

# Windows 64
WIN64_FLAGS = $(CFLAGS) -m64 \
              -I./Raylib/SDL2/x86_64-w64-mingw32/include

WIN64_LIBS = ./Raylib/bins/libwin64.a \
             ./Raylib/SDL2/x86_64-w64-mingw32/lib/libSDL2main.a \
             ./Raylib/SDL2/x86_64-w64-mingw32/lib/libSDL2.a \
             -lopengl32 -lgdi32 -lwinmm -lpsapi \
             -luser32 -lshell32 -ladvapi32 \
             -lole32 -loleaut32 -limm32 \
             -lversion -lsetupapi

# Windows 32
WIN32_FLAGS = $(CFLAGS) -m32 \
              -I./Raylib/SDL2/i686-w64-mingw32/include

WIN32_LIBS = ./Raylib/bins/libwin32.a \
             ./Raylib/SDL2/i686-w64-mingw32/lib/libSDL2main.a \
             ./Raylib/SDL2/i686-w64-mingw32/lib/libSDL2.a \
             -lopengl32 -lgdi32 -lwinmm -lpsapi \
             -luser32 -lshell32 -ladvapi32 \
             -lole32 -loleaut32 -limm32 \
             -lversion -lsetupapi


build/dev/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(DEV_CFLAGS) -c $< -o $@

$(DEV_BIN): $(DEV_OBJ) ./Raylib/bins/liblinux64.a
	$(CC) -m64 -o $@ $(DEV_OBJ) $(LINUX64_LIBS)

build/debug/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(DEBUG_BIN): $(DEBUG_OBJ) ./Raylib/bins/liblinux64.a
	$(CC) -m64 $(DEBUG_LDFLAGS) -o $@ $(DEBUG_OBJ) $(LINUX64_LIBS)

-include $(DEV_OBJ:.o=.d) $(DEBUG_OBJ:.o=.d)

build-dev-linux64: $(DEV_BIN)
build-debug-linux64: $(DEBUG_BIN)


FORCE:

$(REL_DIR)/linux64/%.o: %.c FORCE
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -m64 -c $< -o $@

$(REL_DIR)/linux32/%.o: %.c FORCE
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(X86_32_FLAGS) -c $< -o $@

$(REL_DIR)/win64/%.o: %.c FORCE
	@mkdir -p $(@D)
	$(WIN64CC) $(WIN64_FLAGS) -c $< -o $@

$(REL_DIR)/win32/%.o: %.c FORCE
	@mkdir -p $(@D)
	$(WIN32CC) $(WIN32_FLAGS) -c $< -o $@

build-linux64: $(REL64_OBJ)
	@rm -f $(OUT)/C-OSU.linux64
	$(CC) $(CFLAGS) -m64 $(LDFLAGS) \
	-o $(OUT)/C-OSU.linux64 $(REL64_OBJ) $(LINUX64_LIBS)

build-linux64-static: $(REL64_OBJ)
	@rm -f $(OUT)/C-OSU.linux64
	$(CC) $(CFLAGS) -m64 $(LDFLAGS) \
	-o $(OUT)/C-OSU.linux64 $(REL64_OBJ) $(LINUX64_STATIC_LIBS)

build-linux32: $(REL32_OBJ)
	@rm -f $(OUT)/C-OSU.linux32
	$(CC) $(CFLAGS) $(X86_32_FLAGS) $(LDFLAGS) $(X86_32_LDFLAGS) \
	-o $(OUT)/C-OSU.linux32 $(REL32_OBJ) $(LINUX32_LIBS)

build-linux32-static: $(REL32_OBJ)
	@rm -f $(OUT)/C-OSU.linux32
	$(CC) $(CFLAGS) $(X86_32_FLAGS) $(LDFLAGS) $(X86_32_LDFLAGS) \
	-o $(OUT)/C-OSU.linux32 $(REL32_OBJ) $(LINUX32_STATIC_LIBS)

build-win64: $(RELW64_OBJ)
	@rm -f $(OUT)/C-OSU.win64.exe
	$(WIN64CC) $(WIN64_FLAGS) \
	$(LDFLAGS) -Wl,--allow-multiple-definition \
	-o $(OUT)/C-OSU.win64.exe $(RELW64_OBJ) $(WIN64_LIBS)

build-win32: $(RELW32_OBJ)
	@rm -f $(OUT)/C-OSU.win32.exe
	$(WIN32CC) $(WIN32_FLAGS) \
	$(LDFLAGS) -Wl,--allow-multiple-definition \
	-o $(OUT)/C-OSU.win32.exe $(RELW32_OBJ) $(WIN32_LIBS)


clean:
	rm -f $(OUT)/C-OSU.dev.linux64
	rm -f $(OUT)/C-OSU.linux64
	rm -f $(OUT)/C-OSU.linux32
	rm -f $(OUT)/C-OSU.debug.linux64
	rm -f $(OUT)/C-OSU.win64.exe
	rm -f $(OUT)/C-OSU.win32.exe
	rm -rf build


# make
linux64: $(DEV_BIN)

	cd $(OUT) && $(GAMEMODE) ./C-OSU.dev.linux64

# make all
all:
	@$(MAKE) --no-print-directory build-linux64-static
	@$(MAKE) --no-print-directory build-linux32-static
	@$(MAKE) --no-print-directory build-win64
	@$(MAKE) --no-print-directory build-win32


# make debug
debug: $(DEBUG_BIN)

	cd $(OUT) && \
	ASAN_OPTIONS=abort_on_error=1:detect_leaks=1 \
	UBSAN_OPTIONS=print_stacktrace=1 \
	./C-OSU.debug.linux64


.PHONY: all linux64 debug clean FORCE \
        build-linux64 build-dev-linux64 build-debug-linux64 build-linux32 \
        build-linux64-static build-linux32-static \
        build-win64 build-win32
