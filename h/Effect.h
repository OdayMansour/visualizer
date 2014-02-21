#ifndef EFFECT_H
#define EFFECT_H

#include "common.h"

class Effect
{
public:
	String		name;
	gml::mat4	projection;
	gml::mat4	view;
	int			spectrumSize;

	Effect();
	void initializeSceneObjects(State newState);
	void initializeProjectionView();
	void updateSceneObjects();
};

#endif //EFFECT_H