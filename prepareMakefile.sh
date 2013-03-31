#!/bin/bash

rm Makefile
touch Makefile

echo '' >> Makefile
echo 'LIB_PATH = ./lib' >> Makefile
echo 'LIB_NAME = libfmodex.dylib' >> Makefile
echo 'ARCH = -arch x86_64' >> Makefile
echo 'INC = -Iinc' >> Makefile
echo 'OBJECTS = ' `find . | grep cpp | sed 's/cpp/o/g' | tr "\\n" " "` >> Makefile
echo 'TARGET = visualizer' >> Makefile
echo 'OPENGL = -lglfw -framework Cocoa -framework OpenGL -lGLEW' >> Makefile

echo '' >> Makefile
echo '.PHONY: all clean' >> Makefile
echo '' >> Makefile
echo 'all: $(OBJECTS)' >> Makefile
echo '	g++ -o $(TARGET) $(OBJECTS) $(LIB_PATH)/$(LIB_NAME) $(OPENGL)' >> Makefile
echo '	install_name_tool -change ./$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME) visualizer' >> Makefile
echo '	rm -rf c/*.o fmod.log' >> Makefile
echo '' >> Makefile
echo '%.o: %.cpp' >> Makefile
echo '	g++ $(INC) -c -g $< -o $@' >> Makefile
echo '' >> Makefile
echo 'clean:' >> Makefile
echo '	rm -rf $(TARGET) $(OBJECTS) fmod.log' >> Makefile
