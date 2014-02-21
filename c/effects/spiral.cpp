#include "../../h/effects.h"

void analyzeSpectrumSpiral(std::vector<float> specL, std::vector<float> specR)
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

	State newStateR,
				newStateL;
	
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
		avgLoudness = (199.0*avgLoudness + apparentLoudness)/200.0;

	fixColors();

	// timeTicker -= 0.04;

	int j;

	if ( apparentLoudness > 1.7*avgLoudness ) { 
		jumpRotation = 10*(2*(rand()%2)-1);
		//pulseColors();
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

	float mover;
	float radius;
	float bassMover;
	int decalage = SPECTRUMSIZE;

	for (int i=0; i<SPECTRUMSIZE; i++) {

		j = 5*i;
		mover = timeTicker + ( j /SPECTRUMSIZE ) *M_PI;
		radius = j/(3*SPECTRUMSIZE);
		bassMover = cappedBass*sin((1/cappedBass)*sineFreqMod*i/SPECTRUMSIZE);

		velocity = specR[i];
		newStateR.set(sceneObjects[i].getState());

		oldDisplacement = newStateR.Scale.y;
		temp = 0.02+200.0/avgLoudness*2.0*velocity*(i/SPECTRUMSIZE) / ( 1.0 + i/marker);
		if ( temp < oldDisplacement )
			displacement = ( oldDisplacement*3 + temp ) / 4.0;
		else
			displacement = temp;

		// newStateR.Scale.x = 0;
		newStateR.Scale.y = displacement + 0.5*bassMover;
		// newStateR.Scale.z = 0;

		// newStateR.TranslationPre.x = 0;
		// newStateR.TranslationPre.y = 0;
		// newStateR.TranslationPre.z = 0;

		newStateR.TranslationPost.x =  -cos( mover ) * radius;
		newStateR.TranslationPost.y =  -displacement - 1*bassMover;
		newStateR.TranslationPost.z =  -sin( mover ) * radius;

		// newStateR.RotationPre.x = 0;
		// newStateR.RotationPre.y = 0;
		// newStateR.RotationPre.z = 0;

		newStateR.RotationPost.x += 0;
		// newStateR.RotationPost.y = 0;
		// newStateR.RotationPost.z = 0;

		newStateR.Color[0] = cTheme.r - (colorScaler * displacement);
		newStateR.Color[1] = cTheme.g - (colorScaler * displacement);
		newStateR.Color[2] = cTheme.b - (colorScaler * displacement);

		sceneObjects[i].setState(newStateR);

		velocity = specL[i];
		newStateL.set(sceneObjects[i+(int)SPECTRUMSIZE].getState());

		oldDisplacement = newStateL.Scale.y;
		temp = 0.02+200.0/avgLoudness*2.0 *velocity*(i/SPECTRUMSIZE) / ( 1.0 + i/marker);
		if ( temp < oldDisplacement )
			displacement = ( oldDisplacement*3 + temp ) / 4.0;
		else
			displacement = temp;

		// newStateL.Scale.x = 0;
		newStateL.Scale.y = displacement + 0.5*bassMover;
		// newStateL.Scale.z = 0;

		// newStateL.TranslationPre.x = 0;
		// newStateL.TranslationPre.y = 0;
		// newStateL.TranslationPre.z = 0;

		newStateL.TranslationPost.x = -cos( mover ) * radius;
		newStateL.TranslationPost.y = displacement + 1*bassMover;
		newStateL.TranslationPost.z = -sin( mover ) * radius;

		// newStateL.RotationPre.x = 0;
		// newStateL.RotationPre.y = 0;
		// newStateL.RotationPre.z = 0;

		newStateL.RotationPost.x += 0;
		// newStateL.RotationPost.y = 0;
		// newStateL.RotationPost.z = 0;

		newStateL.Color[0] = cTheme.r + (colorScaler * displacement);
		newStateL.Color[1] = cTheme.g + (colorScaler * displacement);
		newStateL.Color[2] = cTheme.b + (colorScaler * displacement);

		sceneObjects[i+(int)SPECTRUMSIZE].setState(newStateL);

	}

	///////////////////////////// END SECOND EFFECT

	updateBackground();

	frameCount++;
	if ( time(0) > now ) {
		now = time(0);
		std::cout << frameCount << "            Fps       \r";
		std::cout.flush();
		frameCount = 0;
	}
}