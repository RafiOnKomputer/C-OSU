.DEFAULT_GOAL := linux64

CC      = gcc
WIN64CC = x86_64-w64-mingw32-gcc
WIN32CC = i686-w64-mingw32-gcc

SRC = game.c $(wildcard src/*.c)

# Common flags
CFLAGS = -std=c17 -O3 -flto \
         -ffunction-sections -fdata-sections \
         -fno-semantic-interposition -fno-plt \
         -Wall -Wextra -Wpedantic \
         -I./inc -I./Raylib/

LDFLAGS = -flto -Wl,--gc-sections -s


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

 # Linux 64 Debug
DEBUG_CFLAGS = -std=c17 -O1 -g3 \
               -fsanitize=address,undefined \
               -fno-omit-frame-pointer \
               -Wall -Wextra -Wpedantic \
               -I./inc -I./Raylib/

DEBUG_LDFLAGS = -fsanitize=address,undefined

build-debug-linux64:
	$(CC) $(DEBUG_CFLAGS) -m64 $(DEBUG_LDFLAGS) \
	-o ./C-OSU/C-OSU.debug.linux64 $(SRC) $(LINUX64_LIBS)

build-linux64:
	$(CC) $(CFLAGS) -m64 $(LDFLAGS) \
	-o ./C-OSU/C-OSU.linux64 $(SRC) $(LINUX64_LIBS)

build-linux64-static:
	$(CC) $(CFLAGS) -m64 $(LDFLAGS) \
	-o ./C-OSU/C-OSU.linux64 $(SRC) $(LINUX64_STATIC_LIBS)


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

build-linux32:
	$(CC) $(CFLAGS) -m32 $(LDFLAGS) \
	-o ./C-OSU/C-OSU.linux32 $(SRC) $(LINUX32_LIBS)

build-linux32-static:
	$(CC) $(CFLAGS) -m32 $(LDFLAGS) \
	-o ./C-OSU/C-OSU.linux32 $(SRC) $(LINUX32_STATIC_LIBS)


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

build-win64:
	$(WIN64CC) $(WIN64_FLAGS) \
	$(LDFLAGS) -Wl,--allow-multiple-definition \
	-o ./C-OSU/C-OSU.win64.exe $(SRC) $(WIN64_LIBS)


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

build-win32:
	$(WIN32CC) $(WIN32_FLAGS) \
	$(LDFLAGS) -Wl,--allow-multiple-definition \
	-o ./C-OSU/C-OSU.win32.exe $(SRC) $(WIN32_LIBS)




# Delete binaries
clean:
	rm -f ./C-OSU/C-OSU.linux64
	rm -f ./C-OSU/C-OSU.linux32
	rm -f ./C-OSU/C-OSU.debug.linux64
	rm -f ./C-OSU/C-OSU.win64.exe
	rm -f ./C-OSU/C-OSU.win32.exe


# make
linux64: clean build-linux64

	cd ./C-OSU && gamemoderun ./C-OSU.linux64

# make all
all: clean build-linux64-static build-linux32-static build-win64 build-win32


# make debug
debug: clean build-debug-linux64

	cd ./C-OSU && \
	ASAN_OPTIONS=abort_on_error=1:detect_leaks=1 \
	UBSAN_OPTIONS=print_stacktrace=1 \
	./C-OSU.debug.linux64


.PHONY: all linux64 debug clean \
        build-linux64 build-debug-linux64 build-linux32 \
        build-linux64-static build-linux32-static \
        build-win64 build-win32
