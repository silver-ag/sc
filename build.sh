# linux 64
gcc sc.c -o bin/linux64/sc

# linux 32
gcc sc.c -o bin/linux32/sc -m32

# windows 64
x86_64-w64-mingw32-gcc -o bin/windows64/sc.exe sc.c -lws2_32

# windows 32
i686-w64-mingw32-gcc -o bin/windows32/sc.exe sc.c -lws2_32
