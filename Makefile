CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wfatal-errors
INCLUDE_PATH =  -I"./libs/" \
				-I/opt/homebrew/Cellar/sdl2/2.30.7/include \
				-I/opt/homebrew/Cellar/sdl2_image/2.8.2_2/include \
				-I/opt/homebrew/Cellar/sdl2_mixer/2.8.0/include \
				-I/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include \
				-I"./src/Components/"
LIB_PATH = -L/opt/homebrew/Cellar/sdl2/2.30.7/lib \
		   -L/opt/homebrew/Cellar/sdl2_image/2.8.2_2/lib \
		   -L/opt/homebrew/Cellar/sdl2_mixer/2.8.0/lib \
		   -L/opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib \
		   -L/opt/homebrew/Cellar/lua/5.4.7/lib 
SRC_FILES = ./src/*cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetStore/*.cpp \
			./libs/imgui/*.cpp
LINKERS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua
OBJ_NAME = gameengine
# Build target
build:
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $(LINKERS) $(SRC_FILES) -o $(OBJ_NAME)
# Run target
run:
	./$(OBJ_NAME)	
# Clean target 
clean:
	rm -f $(OBJ_NAME)
