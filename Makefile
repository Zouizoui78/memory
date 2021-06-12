GCC=g++
MINGW=i686-w64-mingw32-g++
FLAGS=-O3 -std=c++17 -Wall -Werror -Wno-deprecated
SRC=src/*.cpp
LINK=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude/

all: linux win

linux: BUILD=build/linux
linux:
	@mkdir -p $(BUILD)
	@$(GCC) $(FLAGS) -o $(BUILD)/memory $(SRC) $(INCLUDE) -I/usr/include/SDL2/ -I/usr/include/date $(LINK)

win: BUILD=build/windows/memory
win:

	@mkdir -p $(BUILD)
	@$(MINGW) $(FLAGS) -s -o $(BUILD)/memory.exe $(SRC) icon/icon.res $(INCLUDE) -Iextlib/include -Lextlib/lib -lmingw32 -lSDL2main $(LINK) -lstdc++fs -mwindows -static-libgcc -static-libstdc++ -D WINDOWS
	@cp extlib/bin/* $(BUILD)
	@cp -r res $(BUILD)
	@cd $(BUILD)/.. && zip -r memory.zip memory/

clean:
	@rm -r build