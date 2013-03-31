#ifndef GENERICS_H
#define GENERICS_H

#include "common.h"

bool initializeGlobals();
bool initScene( EFFECT effect );
void initCubes( EFFECT effect );
float min( float x, float y );
float max( float x, float y);
float abs( float x );
bool migrateColor( glm::vec4 &source, glm::vec4 &dest, float increment );
float hue2rgb(float p, float q, float t);
void updateBackground();
void pulseColors();
void fixColors();
void draw();
void usage();

#endif //GENERICS_H