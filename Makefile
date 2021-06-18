NAME=memory
COMPILER=g++
MINGW=/usr/bin/i686-w64-mingw32-g++
CFLAGS=-std=c++17 -Wall -Werror -Wno-deprecated
OPTI=-g

LFLAGS=-lSDL2 -lSDL2_ttf
INCLUDE=-Iinclude
LINUX_INCLUDES=$(INCLUDE) -I/usr/include/SDL2 -I/usr/include/date

BUILD=build
DEBUGDIR=$(BUILD)/debug
RELEASEDIR=$(BUILD)/release

LIBDIR=$(BUILDDIR)/lib
OBJDIR=$(BUILDDIR)/obj
OUTPUT=$(BUILDDIR)/$(NAME)
BUILDDIR=$(DEBUGDIR)

ifeq ($(word 1, $(MAKECMDGOALS)), release)
BUILDDIR=$(RELEASEDIR)
OPTI=-O3 -s
else ifeq ($(word 1, $(MAKECMDGOALS)), win)
OPTI=-O3 -s
BUILDDIR=$(BUILD)/windows
OUTPUT=$(BUILDDIR)/$(NAME).exe
DIST=$(BUILDDIR)/$(NAME).zip
COMPILER=i686-w64-mingw32-g++
LFLAGS:=-Lextlib/lib -lmingw32 -lSDL2main $(LFLAGS) -mwindows
OTHER=-D WINDOWS
endif

SRCDIR=src
SRCEXT=.cpp
SRC=$(shell ls $(SRCDIR) | grep $(SRCEXT))

OBJ = $(patsubst %$(SRCEXT),$(OBJDIR)/%.o,$(SRC))

all: $(OUTPUT)
release: $(OUTPUT)
win: $(DIST)

$(DIST): DISTDIR=$(BUILDDIR)/$(NAME)
$(DIST): $(OUTPUT)
	@mkdir -p $(DISTDIR)
	@echo Preparing dist directory...
	@cp $(OUTPUT) $(DISTDIR)/
	@cp -r res $(DISTDIR)/
	@cp /usr/i686-w64-mingw32/bin/{libgcc_s_dw2-1.dll,libstdc++-6.dll,libwinpthread-1.dll} $(DISTDIR)/
	@cp extlib/bin/* $(DISTDIR)
	@echo Compressing dist directory...
	@cd $(BUILDDIR) && zip -r $(NAME).zip $(NAME) > /dev/null

$(OUTPUT): $(OBJ)
	@echo Linking $@
	@$(COMPILER) -o $(OUTPUT) $(OBJ) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo Compiling $<
	@mkdir -p $(OBJDIR)
	@$(COMPILER) $(CFLAGS) $(OPTI) $(LINUX_INCLUDES) -o $@ -c $< $(OTHER)

clean:
	@rm -r $(BUILD)

.PHONY: all release clean win