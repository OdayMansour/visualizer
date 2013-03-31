#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

extern int 										effect;
extern float 									SPECTRUMSIZE;

extern int										key;
extern std::string								TAG;
extern GLuint 									program, vPosition, vColor, vNormal, mvpID, heatID;
extern glm::mat4 								Projection, View, Model, MVP;
extern Cube*									sceneObjects;
extern std::vector<std::pair<int, GLuint> > 	attributes;
extern int 										w_height;
extern int 										w_width;
extern glm::vec4 								cTheme;
extern glm::vec4 								destTheme;
extern float 									colorIncrement;
extern float 									avgLoudness;
extern bool										pulse;
extern float									pulseMag;
extern float									colorScaler;
extern float									cubeScale;
extern float									timeTicker;
extern float 									intensityRotation;
extern int 										frameCount;
extern int										now;
extern float									smoothedBass;
extern float 									cappedBass;
extern float 									avgBass;
extern float									colorSaturation;

enum EFFECT {
	NONE,
	SPIRAL,
	VOX,
	LOOP,
	FOUNTAIN,
	SHADOW,
	SPHERE
};

#endif //GLOBALS_H