#include "../h/common.h"

bool initializeGlobals() {
	effect = 0;
	SPECTRUMSIZE = 512.0;
	TAG = ">\t";
	w_height = 900;
	w_width = 1440;
	cTheme = glm::vec4(0.0,0.0,0.0,0.0);
	destTheme = glm::vec4(0.0,0.0,0.0,0.0);
	colorIncrement = 0.001;
	avgLoudness = 0.3;
	pulse = false;
	pulseMag = 0.03;
	colorScaler = 1.5;
	cubeScale = 0.0045f;
	timeTicker = 0;
	intensityRotation = 0;
	frameCount = 0;
	now = 0;
	smoothedBass = 0;
	cappedBass = 0;
	avgBass = 0;
	colorSaturation = 1;
}

bool initScene( EFFECT effect )
{
	if (!glfwInit()) {return false;}
	
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 16);

	if (!glfwOpenWindow(
		w_width,
		w_height,
		8, //red
		8, //green
		8, //blue
		8, //alpha
		24, //depth
		0, //stencil
		GLFW_FULLSCREEN // Windowed or Fullscreen
	))
	{
		glfwTerminate();
		return false;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {return false;}

	program = InitShader("glsl/vshader.glsl", "glsl/fshader.glsl");
	glUseProgram(program);
	vPosition = glGetAttribLocation(program, "vPosition");
	vColor = glGetAttribLocation(program, "vColor");
	vNormal = glGetAttribLocation(program, "vNormal");
	mvpID = glGetUniformLocation(program, "MVP");
	heatID = glGetUniformLocation(program, "heat");
	
	attributes.push_back(std::make_pair(0,vPosition));
	attributes.push_back(std::make_pair(1,vColor));
	attributes.push_back(std::make_pair(2,vNormal));

	if ( effect == SPIRAL || effect == FOUNTAIN || effect == SPHERE ) {
		Projection = glm::perspective(45.0f, (float)w_width/(float)w_height, 0.1f, 100.0f);
		View = glm::lookAt(	glm::vec3(2,3,0),	glm::vec3(0,0,0),	glm::vec3(0,1,0));
	} else if ( effect == VOX ) {
		Projection = glm::perspective(45.0f, (float)w_width/(float)w_height, 0.1f, 100.0f);
		View = glm::lookAt(	glm::vec3(2,0,0),	glm::vec3(0,0,0),	glm::vec3(0,1,0));
	} else if ( effect == LOOP ) {
		Projection = glm::perspective(45.0f, (float)w_width/(float)w_height, 0.1f, 100.0f);
		View = glm::lookAt(	glm::vec3(3,2,0),	glm::vec3(0,0,0),	glm::vec3(0,1,0));
	}

	Model = glm::mat4(1.0f);
	glClearColor(1,1,1,1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return true;
}

void initCubes( EFFECT effect )
{ ENTER(initCubes)
	sceneObjects = new Cube[(int)SPECTRUMSIZE*2];

	if ( effect == SPIRAL || effect == LOOP ) {
		for (int i=0; i<SPECTRUMSIZE*2; i++) {
			sceneObjects[i].state.TranslationPost = glm::vec3(0.0,0.0,0.0);
			sceneObjects[i].state.Scale = glm::vec3(cubeScale, cubeScale, cubeScale);
			initBuffers(&sceneObjects[i], attributes);
		}
	} else if ( effect == VOX ) {
		for (int i=0; i<SPECTRUMSIZE*2; i++) {
			sceneObjects[i].state.TranslationPost = glm::vec3(0.0,0.0,((float)i/(SPECTRUMSIZE)-1));
			sceneObjects[i].state.Scale = glm::vec3(cubeScale, cubeScale, cubeScale);
			initBuffers(&sceneObjects[i], attributes);
		}
	} else if ( effect == FOUNTAIN ) {
		float angle, distance;
		for (int i=0; i<SPECTRUMSIZE; i++) {
			angle = rand()%360;
			distance = (int)(i/SPECTRUMSIZE*2*5)/5.0 + 0.1;

			State newStateR;
			newStateR.TranslationPost = glm::vec3(distance*sin(angle),0.0,distance*cos(angle));
			newStateR.Scale = glm::vec3(cubeScale, cubeScale, cubeScale);
			sceneObjects[i].setState(newStateR);
			
			// sceneObjects[i].state.TranslationPost = glm::vec3(distance*sin(angle),0.0,distance*cos(angle));
			// sceneObjects[i].state.Scale = glm::vec3(cubeScale, cubeScale, cubeScale);
			initBuffers(&sceneObjects[i], attributes);
			
			State newStateL;
			newStateL.TranslationPost = sceneObjects[i].state.TranslationPost;
			newStateL.Scale = sceneObjects[i].state.Scale;
			sceneObjects[i+(int)SPECTRUMSIZE].setState(newStateL);

			// sceneObjects[i+(int)SPECTRUMSIZE].state.TranslationPost = sceneObjects[i].state.TranslationPost;
			// sceneObjects[i+(int)SPECTRUMSIZE].state.Scale = sceneObjects[i].state.Scale;
			initBuffers(&sceneObjects[i+(int)SPECTRUMSIZE], attributes);
		}
	} else if ( effect == SPHERE ) {

		float r;
		float y;
		float grandStep = 15;
		int i = 1;
		for ( float heightAlpha = 90-grandStep/2.0; heightAlpha >= -90+grandStep/2.0; heightAlpha -= 8 ) {
			r = cos(heightAlpha/180.0*M_PI);
			y = sin(heightAlpha/180.0*M_PI);
			for ( float angle = 0; angle < 360; angle += grandStep ) {
				sceneObjects[i].state.TranslationPost = glm::vec3(r*cos(angle/180.0*M_PI),y,r*sin(angle/180.0*M_PI));
				sceneObjects[i].state.Scale = glm::vec3(cubeScale, cubeScale, cubeScale);
				sceneObjects[i].state.TranslationPre = glm::vec3(0.0,0.5,0.0);
				sceneObjects[i].state.RotationPre.x = - heightAlpha - 90.0;		
				sceneObjects[i].state.RotationPre.y = 90.0 - angle;
				initBuffers(&sceneObjects[i], attributes);

				sceneObjects[i+(int)SPECTRUMSIZE].state.TranslationPost = glm::vec3(r*cos(angle/180.0*M_PI),y,r*sin(angle/180.0*M_PI));
				sceneObjects[i+(int)SPECTRUMSIZE].state.Scale = glm::vec3(cubeScale, cubeScale, cubeScale);
				sceneObjects[i+(int)SPECTRUMSIZE].state.TranslationPre = glm::vec3(0.0,0.5,0.0);
				sceneObjects[i+(int)SPECTRUMSIZE].state.RotationPre.x = - heightAlpha - 90.0;		
				sceneObjects[i+(int)SPECTRUMSIZE].state.RotationPre.y = 90.0 - angle;

				initBuffers(&sceneObjects[i+(int)SPECTRUMSIZE], attributes);
				i++;
			}
		}
		std::cout << i << std::endl;
	}

EXIT(initCubes) }

float min( float x, float y ) { if ( x < y ) return x; return y; }
float max( float x, float y) { if ( x < y ) return y; return x; }

float abs( float x ) { if ( x >= 0 ) return x; return -x; }

bool migrateColor( glm::vec4 &source, glm::vec4 &dest, float increment ) {
	bool red, green, blue;
	red = false;
	green = false;
	blue = false;

	if ( abs(source.r - dest.r) < increment ) red = true;
	else ( source.r > dest.r ) ? source.r -= increment : source.r += increment;
	if ( abs(source.g - dest.g) < increment ) green = true;
	else ( source.g > dest.g ) ? source.g -= increment : source.g += increment;
	if ( abs(source.b - dest.b) < increment ) blue = true;
	else ( source.b > dest.b ) ? source.b -= increment : source.b += increment;

	return (red && green && blue);
}

float hue2rgb(float p, float q, float t) {
        if ( t < 0 ) t += 1;
        if ( t > 1 ) t -= 1;
        if ( t < 1/6.0 ) return p + (q - p) * 6 * t;
        if ( t < 1/2.0 ) return q;
        if ( t < 2/3.0 ) return p + (q - p) * (2/3.0 - t) * 6;
        return p;
}

void updateBackground() {
	glClearColor(cTheme.r, cTheme.g, cTheme.b, cTheme.a);
	if ( migrateColor(cTheme,destTheme,colorIncrement) ) {
		float H = (rand()%360)/360.0;
		float S = 0.75;
		float L = 0.30;
		float q = L < 0.5 ? L * (1 + S) : L + S - L * S;
        float p = 2.0 * L - q;
        float R = hue2rgb(p, q, H + 1/3.0);
        float G = hue2rgb(p, q, H);
        float B = hue2rgb(p, q, H - 1/3.0);

		destTheme = glm::vec4(R,G,B,1);
	}
	// glClearColor(0,0,0,1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pulseColors() {
	cTheme = cTheme + glm::vec4(pulseMag,pulseMag,pulseMag,0);
	pulse = true;
}

void fixColors() {
	if ( pulse ) { 
		cTheme = cTheme - glm::vec4(pulseMag,pulseMag,pulseMag,0);
		pulse = false;
	}
}

void draw()
{
	for (int i=0; i<SPECTRUMSIZE*2; i++)
	{
		MVP = Projection * View * sceneObjects[i].model();
		glUniform3fv(heatID, 1, &sceneObjects[i].state.Color[0]);
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
		sceneObjects[i].render();
	}
	glfwSwapBuffers();
}