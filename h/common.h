#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include "fmod.hpp"
#include "fmod_errors.h"
#include "wincompat.h"
#include "fmod_handler.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glfw.h>

#include "shader.h"
#include "State.h"
#include "SceneObject.h"
#include "Cube.h"

#include "globals.h"
#include "generics.h"
#include "effects.h"

#define DEBUGMODE			false
#define SHOW(a) 			std::cout << #a << ": " << (a) << std::endl
#define ENTER(a)			if ( DEBUGMODE ) { std::cout << "<- "#a << std::endl; }
#define EXIT(a)				if ( DEBUGMODE ) { std::cout << "-> "#a << std::endl; }

#define OUTPUTRATE          44100
#define SPECTRUMRANGE       ((float) OUTPUTRATE / 2.0f)      /* 0 to nyquist */
#define BAR1				(float)(0.03*SPECTRUMSIZE)
#define BAR2				(float)(0.4*SPECTRUMSIZE)

typedef glm::vec4 color4;
typedef glm::vec4 point4;

bool whatever();

#endif //COMMON_H