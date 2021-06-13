GCC=g++
FLAGS=-O3 -std=c++17 -Wall -Werror -Wno-deprecated
SRC=src/*.cpp
LINK=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude/

all: win

win: BUILD=build
win:
	@$(GCC) $(FLAGS) -s -o $(BUILD)/memory.exe $(SRC) $(INCLUDE) -Iextlib/include -Lextlib/lib -lmingw32 -lSDL2main $(LINK) -lstdc++fs -mwindows -static-libgcc -static-libstdc++ -D WINDOWS
	@cp extlib/bin/* $(BUILD)
	@cp -r res $(BUILD)

clean:
	@rm -r build/*