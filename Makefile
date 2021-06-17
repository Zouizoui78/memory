GCC=g++
CFLAGS=-std=c++17 -Wall -Werror -Wno-deprecated
OPTIFLAGS=-O3 -s
SRC=src/*.cpp
LINK=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude/
BUILD=build/memory

.PHONY: all dist debug win build clean

all: win

dist: win
	@cd $(BUILD)/.. && zip -r memory.zip memory/

debug:
	$(MAKE) build FLAGS="$(CFLAGS) -g"

win:
	$(MAKE) build FLAGS="$(CFLAGS) $(OPTIFLAGS)"

build:
	@mkdir.exe -p $(BUILD)
	@$(GCC) $(FLAGS) -o $(BUILD)/memory.exe $(SRC) icon/icon64.res $(INCLUDE) -Iextlib/include -Lextlib/lib -lmingw32 -lSDL2main $(LINK) -mwindows -static-libgcc -static-libstdc++ -D WINDOWS
	@cp -r res $(BUILD)
	@cp extlib/bin/* $(BUILD)

clean:
	@rm -r build