NAME=mastermind
BUILD_DIR=bin

INCLUDE_DIRS = -Iinclude/SDL2 -Iinclude/imgui
LIB_DIR = -Llib/SDL2
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

SRC=$(wildcard src/*.cpp) $(wildcard imgui/*.cpp)

build:
	g++ $(SRC) -o $(BUILD_DIR)/$(NAME) $(INCLUDE_DIRS) $(LIB_DIR) $(LIBS)
