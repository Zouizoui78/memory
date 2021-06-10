GCC=g++
MINGW=i686-w64-mingw32-g++
FLAGS=-O3 -Wall -Werror -std=c++11
SRC=src/*.cpp
LINK=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude/

all: linux win

linux: BUILD=build/linux
linux:
	@mkdir -p $(BUILD)
	$(GCC) $(FLAGS) -o $(BUILD)/memory $(SRC) $(INCLUDE) -I/usr/include/SDL2/ -I/usr/include/date $(LINK)

win: BUILD=build/windows/memory
win:

	@mkdir -p $(BUILD)
	@cp extlib/bin/* $(BUILD)
	@cp -r res $(BUILD)
	@cp /usr/i686-w64-mingw32/bin/libwinpthread-1.dll $(BUILD)
	$(MINGW) $(FLAGS) -o $(BUILD)/memory.exe $(SRC) $(INCLUDE) -Iextlib/include -Lextlib/lib -lmingw32 -lSDL2main $(LINK) -mwindows -static-libgcc -static-libstdc++
	cd $(BUILD)/.. && zip -r memory.zip memory/

clean:
	@rm -r build