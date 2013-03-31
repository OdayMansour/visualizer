#include "../h/common.h"

int 									effect;
float 									SPECTRUMSIZE;
int										key;
std::string								TAG;
GLuint 									program, vPosition, vColor, vNormal, mvpID, heatID;
glm::mat4 								Projection, View, Model, MVP;
Cube*									sceneObjects;
std::vector<std::pair<int, GLuint> > 	attributes;
int 									w_height;
int 									w_width;
glm::vec4 								cTheme;
glm::vec4 								destTheme;
float 									colorIncrement;
float 									avgLoudness;
bool									pulse;
float									pulseMag;
float									colorScaler;
float									cubeScale;
float									timeTicker;
float 									intensityRotation;
int 									frameCount;
int										now;
float									smoothedBass;
float 									cappedBass;
float 									avgBass;
float									colorSaturation;

/* Function that displays the usage of command line tool */
void usage() { 
	std::cout << "Usage: ./visualizer mediafilename [effect]" << std::endl; 
	std::cout << "" << std::endl;
	std::cout << "Effects:" << std::endl;
	std::cout << "0 - Vox" << std::endl;
	std::cout << "1 - Crown" << std::endl;
	std::cout << "2 - Spiral" << std::endl;
	std::cout << "3 - Fountain (reflection mode)" << std::endl;
	std::cout << "4 - Sphere" << std::endl;
	std::cout << "" << std::endl;
}

int main(int argc, char *argv[])
{

	initializeGlobals();

	EFFECT effect = NONE;

	if ( argc < 2 )
	{
		std::cout << "Insufficient arguments.\n"; 
		usage();
		exit(EXIT_FAILURE);
	}
	
	if ( argc == 3 ) {
		switch ( (int)*argv[2] ) {
			case (int)'0': effect = VOX; break;
			case (int)'1': effect = LOOP; break;
			case (int)'2': effect = SPIRAL; break;
			case (int)'3': effect = FOUNTAIN; break;
			case (int)'4': effect = SPHERE; break;
		}
	} else
		effect = SPHERE;

	/* Initialize */

	void (*renderer)(std::vector<float> specL, std::vector<float> specR);

	if ( effect == SPIRAL ) {
		renderer = analyzeSpectrumSpiral;
		SPECTRUMSIZE = 512;
	} else if ( effect == VOX ) {
		renderer = analyzeSpectrumVox;
		SPECTRUMSIZE = 128;
	} else if ( effect == LOOP ) {
		renderer = analyzeSpectrumLoop;
		SPECTRUMSIZE = 512;
	} else if ( effect == FOUNTAIN ) {
		renderer = analyzeSpectrumFountain;
		SPECTRUMSIZE = 512;
	} else if ( effect == SPHERE ) {
		renderer = analyzeSpectrumSphere;
		SPECTRUMSIZE = 512;
	} else {
		usage();
		exit(EXIT_FAILURE);
	}
	
	FmodHandler fmodHandler(argv[1],renderer,(int)SPECTRUMSIZE);

	fmodHandler.play();
	if (!initScene(effect)) {exit(EXIT_FAILURE);}
	srand ( time(NULL) );
	
	//Initialize cubes
	initCubes(effect);

	/* Main Event Loop. */
	int running = GL_TRUE;
	while (running)
	{
		draw(); 
		fmodHandler.update();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
	
	/* Shut down */
	glfwTerminate();
	fmodHandler.release();
	
	return 0;
}