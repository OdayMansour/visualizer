#include "../h/common.h"

/* Constructor
 * Allocate memory for Object
 */
SceneObject::SceneObject(int nv)
{
	numVertices = nv;
	points = new point4[nv];
	colors = new color4[nv];
	normals = new normal4[nv];
	glGenVertexArraysAPPLE(1,&vao);
}


/* Initialize buffers. 
 * Takes a SceneObject and a vector of pairs representing the 
 * attributes in the shader.
 * pair<0, GLuint> - point data
 * pair<1, GLuint> - color data
 * pair<2, GLuint> - normals data
 */
void initBuffers(SceneObject *so, std::vector< std::pair<int,GLuint> > addresses)
{
	glBindVertexArrayAPPLE(so->vao);

	for (int i=0; i<addresses.size(); i++)
	{
		if (addresses[i].first == 0)
		{
			glGenBuffers(1, &so->vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, so->vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*so->numVertices, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4)*so->numVertices, so->points);
			
			glEnableVertexAttribArray(addresses[i].second);
			glVertexAttribPointer(addresses[i].second, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
		}
		
		if (addresses[i].first == 1)
		{
			glGenBuffers(1, &so->colorbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, so->colorbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*so->numVertices, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4)*so->numVertices, so->colors);
			
			glEnableVertexAttribArray(addresses[i].second);
			glVertexAttribPointer(addresses[i].second, 4,GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
		}
		
		if (addresses[i].first == 2)
		{
			glGenBuffers(1, &so->normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, so->normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*so->numVertices, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4)*so->numVertices, so->normals);
			
			glEnableVertexAttribArray(addresses[i].second);
			glVertexAttribPointer(addresses[i].second, 4,GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
		}

	}

	glBindVertexArrayAPPLE(0);

}