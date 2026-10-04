

echo "Removing Older Ver..."

find . -mindepth 1  ! -name 'config.h' ! -name 'Makefile' ! -name 'update.sh' -exec rm -rf -- {} +


printf "Download SDL2 for Windows? [y/n] :"

read input

if [[ "$input" == "Y" || "$input" == "y" ]]; then
    printf "DOWNLOADING SDL2 FOR WINDOWS \n \n"
    wget "$(curl -s https://www.libsdl.org/release/ | grep -oE 'SDL2-devel-[0-9.]+-mingw\.zip' | sort -V | tail -1 | sed 's|^|https://www.libsdl.org/release/|')" && unzip -o "$(ls -t SDL2-devel-*-mingw.zip | head -1)" && mv "$(find . -maxdepth 1 -type d -name 'SDL2-*' | head -1)" SDL2 && rm  "$(ls -t SDL2-devel-*-mingw.zip | head -1)"
fi

printf "DOWNLOADING RAYLIB\n\n"

git clone https://github.com/raysan5/raylib /tmp/raylib && rm /tmp/raylib/src/Makefile && rm /tmp/raylib/src/config.h && cp -r  /tmp/raylib/src/*  ./ && rm -rf /tmp/raylib



