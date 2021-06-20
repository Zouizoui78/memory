# Base variables
NAME=memory
COMPILER=g++
MINGW=/usr/bin/i686-w64-mingw32-g++

# -MMD -MP generates .d files that are Makefiles
# containing header dependencies for each object file.
CFLAGS=-std=c++17 -Wall -Wextra -Werror -Wno-deprecated -MMD -MP
OPTI=-g

# Linking flags
LFLAGS=-lSDL2 -lSDL2_ttf

# Include variables
INCLUDEEXT=.hpp
INCLUDEDIR=include
IFLAGS=-Iinclude -I/usr/include/SDL2 -I/usr/include/date

# SRCs variables
SRCDIR=src
SRCEXT=.cpp
SRC=$(shell ls $(SRCDIR) | grep $(SRCEXT))

# String substitution : *.cpp => $(OBJDIR)/*.o
OBJ=$(SRC:%$(SRCEXT)=$(OBJDIR)/%.o)

# String substitution : $(OBJDIR)/*.o => $(OBJDIR)/*.d
DEPS=$(OBJ:.o=.d)

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
	LFLAGS:=icon/icon.res -Lextlib/lib -lmingw32 -lSDL2main $(LFLAGS) -mwindows
	OTHER=-D WINDOWS
endif

all: $(OUTPUT)
release: $(OUTPUT)
win: $(DIST)

valgrind: $(OUTPUT)
	@valgrind --leak-check=full $(OUTPUT)

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
	@# $@ : Target name
	@echo Linking $@
	@$(COMPILER) -o $(OUTPUT) $(OBJ) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%$(SRCEXT)
	@# $< : First dependency
	@echo Compiling $<
	@mkdir -p $(OBJDIR)
	@$(COMPILER) $(CFLAGS) $(OPTI) $(IFLAGS) -o $@ -c $< $(OTHER)

# Include rules generated by -MMD -MP.
# Since this rules tell how to build object files,
# their dependencies are added to the previous rule.
-include $(DEPS)

clean:
	@rm -r $(BUILD)

.PHONY: all release clean win