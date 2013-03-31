#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "common.h"

typedef glm::vec4 color4;
typedef glm::vec4 point4;
typedef glm::vec4 normal4;
typedef GLfloat velocity;

/* Abstract base class SceneObject
 * The methods to be implemented are :
 * init: How to initialize point, color and normal data
 * render: How to render the data (GL_TRIANGLES, etc. )
 * model: Computation of the Model matrix
 */
class SceneObject
{
public:		
	GLuint		vao, vertexbuffer, colorbuffer, normalbuffer;
	point4		*points;
	color4		*colors;
	normal4		*normals;
	int			numVertices;

	glm::vec3	RotationPre, RotationPost, TranslationPre, TranslationPost, Scale;
	float		Color[3];
	State		state;
	
	SceneObject(int nv);
	virtual void init() = 0;
	virtual void render() = 0;
	virtual glm::mat4 model() = 0;



};

void initBuffers(SceneObject*, std::vector< std::pair<int, GLuint> >);

#endif //SCENEOBJECT_H