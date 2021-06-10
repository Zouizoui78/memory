GCC=g++
MINGW=i686-w64-mingw32-g++
FLAGS=-Wall -Werror -std=c++11
SRC=src/*.cpp
LINK=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude/

# g++ -Wall -Werror -o memory  src/*.cpp -I include/ -I /usr/include/SDL2/ -I/usr/include/date -lSDL2 -lSDL2_ttf -std=c++11

# i686-w64-mingw32-g++ -o memory.exe -I include/ -I extlib_win/include src/*.cpp -std=c++11 -L extlib_win/lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows -static-libgcc -static-libstdc++

all: BUILD=build/linux
all:
	@mkdir -p $(BUILD)
	$(GCC) $(FLAGS) -o $(BUILD)/memory $(SRC) $(INCLUDE) -I/usr/include/SDL2/ -I/usr/include/date $(LINK)

win: BUILD=build/windows/memory
win:

	@mkdir -p $(BUILD)
	@cp extlib_win/bin/* $(BUILD)
	@cp -r res $(BUILD)
	$(MINGW) $(FLAGS) -o $(BUILD)/memory.exe $(SRC) $(INCLUDE) -Iextlib_win/include -Lextlib_win/lib -lmingw32 -lSDL2main $(LINK) -mwindows -static-libgcc -static-libstdc++
	cd $(BUILD)/.. && zip -r memory.zip memory/

clean:
	@rm -r build