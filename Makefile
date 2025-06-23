CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wfatal-errors
INCLUDE_PATH =  -I"./libs/" \
				-I/opt/homebrew/Cellar/sdl2/2.32.4/include \
				-I/opt/homebrew/Cellar/sdl2_image/2.8.8/include \
				-I/opt/homebrew/Cellar/sdl2_mixer/2.8.1_1/include \
				-I/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include \
				-I/opt/homebrew/Cellar/glew/2.2.0_1/include \
				-I"./src/Components/" \
				-I"/opt/homebrew/Cellar/lua/5.4.7/include"
LIB_PATH = -L/opt/homebrew/Cellar/sdl2/2.32.4/lib \
		   -L/opt/homebrew/Cellar/sdl2_image/2.8.8/lib \
		   -L/opt/homebrew/Cellar/sdl2_mixer/2.8.1_1/lib \
		   -L/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib \
		   -L/opt/homebrew/Cellar/lua/5.4.7/lib \
			 -L/opt/homebrew/Cellar/glew/2.2.0_1/lib \
			 -framework OpenGL
SRC_FILES = ./src/*cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetStore/*.cpp \
			./libs/imgui/*.cpp
LINKERS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua -lGLEW
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
