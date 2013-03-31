#include "../../h/effects.h"

void analyzeSpectrumVox(std::vector<float> specL, std::vector<float> specR)
{
	//Get the maximum frequency in order to normalize
	float apparentLoudness;
	float velocity;
	float lowLoudness, midLoudness, highLoudness;
	float displacement;
	float attenuation;
	float jumpRotation = 0;
	int marker = BAR1;
	float volumeRotation;
	
	// Separating bass from mids from trebles
	lowLoudness = 0;
	midLoudness = 0;
	highLoudness = 0;
	apparentLoudness = 0;

	for (int i=0; i<BAR1; i++) lowLoudness += specR[i] + specL[i];
	for (int i=BAR1; i<BAR2; i++) midLoudness += specR[i] + specL[i];
	for (int i=BAR2; i<SPECTRUMSIZE; i++) highLoudness += specR[i] + specL[i];	

	attenuation = 6.0*(SPECTRUMSIZE/1024);
	lowLoudness /= attenuation;
	midLoudness /= attenuation;
	highLoudness /= attenuation;
	apparentLoudness = (1.0/3.0)*(lowLoudness+midLoudness+highLoudness);

	avgLoudness = (49.0*avgLoudness + apparentLoudness)/50.0;

	fixColors();

	// if ( apparentLoudness > 1.7*avgLoudness ) { 
	// 	jumpRotation = 10*(2*(rand()%2)-1);
	// 	pulseColors();
	// } else {
	// 	jumpRotation = 0;
	// }

	volumeRotation = avgLoudness + jumpRotation;

	float oldDisplacement;
	float temp;
	
	for (int i=0; i<SPECTRUMSIZE; i++)
	{
		velocity = specR[i];

		oldDisplacement = sceneObjects[(int)SPECTRUMSIZE-i-1].Scale.y;
		temp = 300.0*velocity*((float)i/SPECTRUMSIZE) / ( 0.5 + (float)i/(float)marker);
		if ( temp < oldDisplacement )
			displacement = ( oldDisplacement*3 + temp ) / 4.0;
		else
			displacement = temp; 

		sceneObjects[(int)SPECTRUMSIZE-i-1].Scale.y = displacement;
		sceneObjects[(int)SPECTRUMSIZE-i-1].Color[0] = cTheme.r + (colorScaler * displacement);
		sceneObjects[(int)SPECTRUMSIZE-i-1].Color[1] = cTheme.g + (colorScaler * displacement);
		sceneObjects[(int)SPECTRUMSIZE-i-1].Color[2] = cTheme.b + (colorScaler * displacement);
		sceneObjects[(int)SPECTRUMSIZE-i-1].RotationPost.x += volumeRotation;

		velocity = specL[i];
		
		oldDisplacement = sceneObjects[i+(int)SPECTRUMSIZE].Scale.y;
		temp = 300.0*velocity*((float)i/SPECTRUMSIZE) / ( 0.5 + (float)i/(float)marker);
		if ( temp < oldDisplacement )
			displacement = ( oldDisplacement*3 + temp ) / 4.0;
		else
			displacement = temp;

		sceneObjects[i+(int)SPECTRUMSIZE].Scale.y = displacement;
		sceneObjects[i+(int)SPECTRUMSIZE].Color[0] = 1 - cTheme.r + (colorScaler * displacement);
		sceneObjects[i+(int)SPECTRUMSIZE].Color[1] = 1 - cTheme.g + (colorScaler * displacement);
		sceneObjects[i+(int)SPECTRUMSIZE].Color[2] = 1 - cTheme.b + (colorScaler * displacement);
		sceneObjects[i+(int)SPECTRUMSIZE].RotationPost.x += volumeRotation;

	}

	updateBackground();

	frameCount++;
	if ( time(0) > now ) {
		now = time(0);
		std::cout << frameCount << "            Fps       \r";
		std::cout.flush();
		frameCount = 0;
	}
}