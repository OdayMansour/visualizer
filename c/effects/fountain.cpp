#include "../../h/effects.h"

void analyzeSpectrumFountain(std::vector<float> specL, std::vector<float> specR)
{
	// General parameters
	float marker = 0.5 * (BAR1+BAR2);
	float sineFreqMod = 20.0;

	//// Overall audio
	float lowLoudness = 0, 
		  midLoudness = 0, 
		  highLoudness = 0,
		  apparentLoudness = 0,
		  smoothedBass = 0,
		  avgBass = 0,
		  cappedBass = 0;


	//// Specific audio
	float velocity = 0,
		  bassMover = 0;
	State newStateR,
		  newStateL;

	//// Overall video
	float displacement = 0,
		  oldDisplacement = 0,
		  newDisplacement = 0,
		  attenuation = 0,
		  jumpRotation = 0;

	bool pulse = false;

	//// Collect overall audio¡
	for (int i=0; i<BAR1; i++) { lowLoudness += specR[i] + specL[i]; }
	for (int i=BAR1; i<BAR2; i++) { midLoudness += specR[i] + specL[i]; }
	for (int i=BAR2; i<SPECTRUMSIZE; i++) { highLoudness += specR[i] + specL[i]; }
	
	apparentLoudness = lowLoudness + midLoudness + highLoudness;
	attenuation = 6.0*(SPECTRUMSIZE/1024.0);
	apparentLoudness = apparentLoudness/attenuation;

	//// Slowly decaying average
	if ( apparentLoudness > avgLoudness ) { avgLoudness = (19.0*avgLoudness + apparentLoudness)/20.0; }
	else { avgLoudness = (199.0*avgLoudness + apparentLoudness)/200.0; }

	//// Bass component
	if ( ( ((specR[0] + specL[0])/2.0) > (avgBass*1.5)) ) { smoothedBass = ((specR[0] + specL[0])/2.0); }
	else { smoothedBass = smoothedBass * 4.0 / 5.0; }
	cappedBass = min(smoothedBass, 0.2);
	avgBass = (avgBass*6.0 + ((specR[0]+ specL[0])/2.0))/7.0;

	fixColors();

	timeTicker -= 0.01*apparentLoudness;

	Projection = glm::perspective((float)(sqrt(avgLoudness+0.7)/1.7)*45.0f, (float)w_width/(float)w_height, 0.1f, 100.0f);

	//// Actual Magic happens here
	for (int i=0; i<SPECTRUMSIZE; i++) {

		//// Bass additive
		bassMover = cappedBass*sin((1/cappedBass)*sineFreqMod*i/SPECTRUMSIZE);

		//// Start Right Channel
		velocity = specR[i];
		newStateR.set(sceneObjects[i].getState());

		oldDisplacement = newStateR.Scale.y;

		newDisplacement = 0.02+200.0/avgLoudness*2.0*velocity*(i/SPECTRUMSIZE) / ( 1.0 + i/marker);
		if ( newDisplacement < oldDisplacement )
			displacement = ( oldDisplacement*3 + newDisplacement ) / 4.0;
		else
			displacement = newDisplacement;

		// newStateR.Scale.x = 0;
		newStateR.Scale.y = displacement + 0.5*bassMover;;
		// newStateR.Scale.z = 0;

		// newStateR.TranslationPre.x = 0;
		// newStateR.TranslationPre.y = 0;
		// newStateR.TranslationPre.z = 0;

		// newStateR.TranslationPost.x = 0;
		newStateR.TranslationPost.y = - newStateR.Scale.y*0.5;
		// newStateR.TranslationPost.z = 0;

		// newStateR.RotationPre.x = 0;
		// newStateR.RotationPre.y = 0;
		// newStateR.RotationPre.z = 0;

		// newStateR.RotationPost.x = 0;
		newStateR.RotationPost.y = 50*timeTicker;
		// newStateR.RotationPost.z = 0;

		newStateR.Color[0] = cTheme.r - (colorScaler * displacement);
		newStateR.Color[1] = cTheme.g - (colorScaler * displacement);
		newStateR.Color[2] = cTheme.b - (colorScaler * displacement);

		sceneObjects[i].setState(newStateR);

		//// Start Left Channel
		velocity = specL[i];
		newStateL.set(sceneObjects[i+(int)SPECTRUMSIZE].getState());

		oldDisplacement = newStateL.Scale.y;
		newDisplacement = 0.02+200.0/avgLoudness*2.0*velocity*(i/SPECTRUMSIZE) / ( 1.0 + i/marker);
		if ( newDisplacement < oldDisplacement )
			displacement = ( oldDisplacement*3 + newDisplacement ) / 4.0;
		else
			displacement = newDisplacement;

		// newStateL.Scale.x = 0;
		newStateL.Scale.y = displacement + 0.5*bassMover;
		// newStateL.Scale.z = 0;

		// newStateL.TranslationPre.x = 0;
		// newStateL.TranslationPre.y = 0;
		// newStateL.TranslationPre.z = 0;

		// newStateL.TranslationPost.x = 0;
		newStateL.TranslationPost.y = newStateL.Scale.y*0.5;
		// newStateL.TranslationPost.z = 0;

		// newStateL.RotationPre.x = 0;
		// newStateL.RotationPre.y = 0;
		// newStateL.RotationPre.z = 0;

		// newStateL.RotationPost.x = 0;
		newStateL.RotationPost.y = 50*timeTicker;
		// newStateL.RotationPost.z = 0;

		newStateL.Color[0] = cTheme.r + (colorScaler * displacement);
		newStateL.Color[1] = cTheme.g + (colorScaler * displacement);
		newStateL.Color[2] = cTheme.b + (colorScaler * displacement);

		sceneObjects[i+(int)SPECTRUMSIZE].setState(newStateL);

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