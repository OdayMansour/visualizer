#include "../h/common.h"

Cube::Cube(): SceneObject(36) 
{
	Index = 0; 
	init();
	state.TranslationPost = glm::vec3(0.0f);
	state.TranslationPre = glm::vec3(0.0f);
	state.RotationPost = glm::vec3(0.0f);
	state.RotationPre = glm::vec3(0.0f);
	state.Scale = glm::vec3(1.0f);
	state.Color[0] = 1.0;
	state.Color[1] = 1.0;
	state.Color[2] = 1.0;
	}

/* Cube initialization method for point, color, normal data */
void Cube::init()
{
	quad( 1, 0, 3, 2 );
	quad( 2, 3, 7, 6 );
	quad( 3, 0, 4, 7 );
	quad( 6, 5, 1, 2 );
	quad( 4, 5, 6, 7 );
	quad( 5, 4, 0, 1 );
	
}

/* Sets the state of the item (duh) */	
void Cube::setState( State newState ) {
	this->state.RotationPre = newState.RotationPre;
	this->state.RotationPost = newState.RotationPost;
	this->state.TranslationPre = newState.TranslationPre;
	this->state.TranslationPost = newState.TranslationPost;
	this->state.Scale = newState.Scale;
	this->state.Color[0] = newState.Color[0];
	this->state.Color[1] = newState.Color[1];
	this->state.Color[2] = newState.Color[2];

	// this->state = newState;
}

State Cube::getState() { return state; }

/* Render method for the instance */	
void Cube::render()
{
		glBindVertexArrayAPPLE(vao);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
		glBindVertexArrayAPPLE(0);
}

/* Method that generates two triangles for each face 
 * and assigns colors to the unit_vertices
 */
void Cube::quad( int a, int b, int c, int d )
{
	// Vertices of a unit cube centered at origin, sides aligned with axes
	point4 unit_vertices[8] = {
		point4( -0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5,  0.5,  0.5, 1.0 ),
		point4(  0.5,  0.5,  0.5, 1.0 ),
		point4(  0.5, -0.5,  0.5, 1.0 ),
		point4( -0.5, -0.5, -0.5, 1.0 ),
		point4( -0.5,  0.5, -0.5, 1.0 ),
		point4(  0.5,  0.5, -0.5, 1.0 ),
		point4(  0.5, -0.5, -0.5, 1.0 )
	};
	
	// RGBA colors
	color4 unit_colors[8] = {
		color4( 0.0, 0.0, 0.0, 1.0 ),  // black
		color4( 1.0, 0.0, 0.0, 1.0 ),  // red
		color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
		color4( 0.0, 1.0, 0.0, 1.0 ),  // green
		color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
		color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
		color4( 1.0, 1.0, 1.0, 1.0 ),  // white
		color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
	};
	
	/* Computation of the normal is false
	 * We need vec3 to be able to compute the cross
	 */	
	glm::vec4 u = unit_vertices[b] - unit_vertices[a];
	glm::vec4 v = unit_vertices[c] - unit_vertices[b];

	glm::vec4 normal = glm::vec4(1,2,3,4);;//glm::normalize(glm::cross(u,v));
	
	
	normals[Index] = normal; colors[Index] = unit_colors[a]; 
	points[Index] = unit_vertices[a]; Index++; 
	normals[Index] = normal; colors[Index] = unit_colors[b]; 
	points[Index] = unit_vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = unit_colors[c]; 
	points[Index] = unit_vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = unit_colors[a]; 
	points[Index] = unit_vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = unit_colors[c]; 
	points[Index] = unit_vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = unit_colors[d]; 
	points[Index] = unit_vertices[d]; Index++;
}

/* Method that computes and returns the Model matrix for the object */
glm::mat4 Cube::model()
{
	
	glm::mat4 model = glm::mat4(1.0f);


	model = glm::rotate(model, state.RotationPost.z, glm::vec3(0.0,0.0,1.0));
	model = glm::rotate(model, state.RotationPost.y, glm::vec3(0.0,1.0,0.0));
	model = glm::rotate(model, state.RotationPost.x, glm::vec3(1.0,0.0,0.0));

	model = glm::translate(model, state.TranslationPost);

	model = glm::rotate(model, state.RotationPre.z, glm::vec3(0.0,0.0,1.0));
	model = glm::rotate(model, state.RotationPre.y, glm::vec3(0.0,1.0,0.0));
	model = glm::rotate(model, state.RotationPre.x, glm::vec3(1.0,0.0,0.0));

	model = glm::scale(model, state.Scale);
	model = glm::translate(model, state.TranslationPre);

	return model;
}
