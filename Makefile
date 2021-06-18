GCC=g++
MINGW=/usr/bin/i686-w64-mingw32-g++
CFLAGS=-std=c++17 -Wall -Werror -Wno-deprecated
OPTIFLAGS=-O3 -s
SRC=src/*.cpp
LINK=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude/

.PHONY: all dist debug win build clean

all:
	$(MAKE) linux FLAGS="$(CFLAGS) $(OPTIFLAGS)"

valgrind: debug
	valgrind --leak-check=full ./build/linux/memory

debug:
	$(MAKE) linux FLAGS="$(CFLAGS) -g"

linux: BUILD=build/linux
linux:
	@mkdir -p $(BUILD)
	$(GCC) $(FLAGS) -o $(BUILD)/memory $(SRC) $(INCLUDE) -I/usr/include/SDL2 -I/usr/include/date $(LINK)

win: BUILD=build/windows/memory
win:
	@mkdir -p $(BUILD)
	$(MINGW) $(CFLAGS) $(OPTIFLAGS) -o $(BUILD)/memory.exe $(SRC) icon/icon.res $(INCLUDE) -Iextlib/include -Lextlib/lib -lmingw32 -lSDL2main $(LINK) -mwindows -static-libgcc -static-libstdc++ -D WINDOWS
	@cp -r res $(BUILD)
	@cp extlib/bin/* $(BUILD)
	@cd $(BUILD)/.. && zip -r memory.zip memory/

clean:
	@rm -r build