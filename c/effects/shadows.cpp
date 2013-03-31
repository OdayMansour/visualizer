#include "../../h/effects.h"

void analyzeSpectrumShadows(std::vector<float> specL, std::vector<float> specR)
{
	//Get the maximum frequency in order to normalize
	float apparentLoudness;
	float velocity;
	float lowLoudness, midLoudness, highLoudness;
	float displacement;
	float attenuation;
	float jumpRotation;
	float marker = 0.5 * (BAR1+BAR2);
	float volumeRotation;
	float sineFreqMod = 20.0;
	
	// Separating bass from mids from trebles
	lowLoudness = 0;
	midLoudness = 0;
	highLoudness = 0;
	apparentLoudness = 0;

	for (int i=0; i<BAR1; i++) lowLoudness += specR[i] + specL[i];
	for (int i=BAR1; i<BAR2; i++) midLoudness += specR[i] + specL[i];
	for (int i=BAR2; i<SPECTRUMSIZE; i++) highLoudness += specR[i] + specL[i];	

	for (int i=0; i<SPECTRUMSIZE; i++)
		apparentLoudness += specR[i] + specL[i];

	attenuation = 6.0*(SPECTRUMSIZE/1024.0);
	apparentLoudness = apparentLoudness/attenuation;

	if ( apparentLoudness > avgLoudness )
		avgLoudness = (19.0*avgLoudness + apparentLoudness)/20.0;
	else
		avgLoudness = (139.0*avgLoudness + apparentLoudness)/140.0;

	fixColors();

	// timeTicker -= 0.04;

	int j;

	if ( apparentLoudness > 1.7*avgLoudness ) { 
		jumpRotation = 10*(2*(rand()%2)-1);
	} else {
		jumpRotation = 0;
	}

	volumeRotation = avgLoudness + jumpRotation;
	float oldDisplacement;
	float temp;
	intensityRotation += 4*avgLoudness;

	timeTicker -= 0.01*apparentLoudness;

	if ( ( ((specR[0] + specL[0])/2.0) > (avgBass*1.5)) )
		smoothedBass = ((specR[0] + specL[0])/2.0);
	else
		smoothedBass = smoothedBass * 4.0 / 5.0;

	cappedBass = min(smoothedBass, 0.2);

	avgBass = (avgBass*6.0 + ((specR[0] + specL[0])/2.0))/7.0;

	sceneObjects[0].Scale = glm::vec3(0,0,0);
	sceneObjects[((int)SPECTRUMSIZE)].Scale = glm::vec3(0,0,0);

	float bassMover;

	Projection = glm::perspective((float)(sqrt(avgLoudness+0.7)/1.7)*45.0f, (float)w_width/(float)w_height, 0.1f, 100.0f);

	for (int i=1; i<SPECTRUMSIZE; i++) {

		velocity = (specR[i] + specL[i])/2.0;

		bassMover = cappedBass*sin((1/cappedBass)*sineFreqMod*i/SPECTRUMSIZE);

		oldDisplacement = sceneObjects[(int)SPECTRUMSIZE+i].Scale.y;
		temp = 0.02+200.0/avgLoudness*2.0 *velocity*(i/SPECTRUMSIZE) / ( 1.0 + i/marker);
		if ( temp < oldDisplacement )
			displacement = ( oldDisplacement*3 + temp ) / 4.0;
		else
			displacement = temp;

		sceneObjects[i+(int)SPECTRUMSIZE].Scale.y = displacement + 0.5*bassMover;

		// sceneObjects[i+(int)SPECTRUMSIZE].Translation.y = sceneObjects[i+(int)SPECTRUMSIZE].Scale.y*0.5;

		sceneObjects[i+(int)SPECTRUMSIZE].Color[0] = cTheme.r + (colorScaler * displacement);
		sceneObjects[i+(int)SPECTRUMSIZE].Color[1] = cTheme.g + (colorScaler * displacement);
		sceneObjects[i+(int)SPECTRUMSIZE].Color[2] = cTheme.b + (colorScaler * displacement);

		sceneObjects[i+(int)SPECTRUMSIZE].RotationPost.y = 50*timeTicker;

		sceneObjects[i].Scale.y = sceneObjects[i+(int)SPECTRUMSIZE].Scale.y/5.0;

		sceneObjects[i].Color[0] = cTheme.r - (colorScaler * displacement);
		sceneObjects[i].Color[1] = cTheme.g - (colorScaler * displacement);
		sceneObjects[i].Color[2] = cTheme.b - (colorScaler * displacement);

		sceneObjects[i].RotationPost.y = 50*timeTicker;

		
	}

	///////////////////////////// END SECOND EFFECT

	updateBackground();

	// std::cout << avgLoudness << "                   \r";
	// std::cout.flush();

	frameCount++;
	if ( time(0) > now ) {
		now = time(0);
		std::cout << frameCount << "            Fps       \r";
		std::cout.flush();
		frameCount = 0;
	}
}