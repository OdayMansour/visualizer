#ifndef CUBE_H
#define CUBE_H

#include "common.h"

class Cube: public SceneObject
{
public:
	int 		Index;

	Cube();
	void setState(State newState);
	State getState();
	void init();
	void render();
	glm::mat4 model();
	
	// quad generates two triangles for each face and assigns colors
	//    to the unit_vertices
	void quad( int a, int b, int c, int d );
};

#endif //CUBE_H