
LIB_PATH = ./lib
LIB_NAME = libfmodex.dylib
ARCH = -arch x86_64
INC = -Iinc
OBJECTS =  ./c/Cube.o ./c/effects/fountain.o ./c/effects/loop.o ./c/effects/sphere.o ./c/effects/spiral.o ./c/effects/vox.o ./c/fmod_handler.o ./c/generics.o ./c/InitShader.o ./c/SceneObject.o ./c/State.o ./c/templateEffect.o ./c/visualizer.o
TARGET = visualizer
OPENGL = -lglfw -framework Cocoa -framework OpenGL -lGLEW

.PHONY: all clean

all: $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) $(LIB_PATH)/$(LIB_NAME) $(OPENGL)
	install_name_tool -change ./$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME) visualizer
	rm -rf c/*.o fmod.log

%.o: %.cpp
	g++ $(INC) -c -g $< -o $@

clean:
	rm -rf $(TARGET) $(OBJECTS) fmod.log
