#include "../h/common.h"

/* Exception handler for FMOD */
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout<<"FMOD error! ("<<result<<") "<<FMOD_ErrorString(result)<<std::endl;
		exit(EXIT_FAILURE);
	}
}

/* Constructor for an FmodHandler, 
 * takes the name of a file to play,
 * and a callback function that will hold the spectrum data
 */
FmodHandler::FmodHandler(
				char *fname, 
				void (*uC) (std::vector<float> right, std::vector<float> left),
				int spectrumSlices
				)
{
	//Initialize variables
	this->spectrumSlices = spectrumSlices;
	channel = 0;
	specL = new float[spectrumSlices];
	specR = new float[spectrumSlices];
	updateCallback = uC;
	
	//Initialize FMOD
	result = FMOD::System_Create(&visSystem); ERRCHECK(result);
	result = visSystem->getVersion(&version); ERRCHECK(result);
	if (version < FMOD_VERSION)
	{
		std::cout<<"Error! Incompatible version of FMOD, you require "
			<<FMOD_VERSION<<"."<<std::endl;
			exit(EXIT_FAILURE);
	}
	result = visSystem->setSoftwareFormat(
				OUTPUTRATE, 
				FMOD_SOUND_FORMAT_PCM16, 
				2, 
				2, 
				FMOD_DSP_RESAMPLER_LINEAR
				);
	ERRCHECK(result);
	
	result = visSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);

	result = visSystem->init(32, FMOD_INIT_NORMAL, 0); ERRCHECK(result);
	
	result = visSystem->createSound(
			fname, 
			(FMOD_MODE)(FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL), 
			0, 
			&sound);
	ERRCHECK(result);
	
	visSystem->getSoftwareFormat(&outputfreq, 0, 0, 0, 0, 0);
	ERRCHECK(result);
}

/* Method that plays the sound given to Fmod */
void FmodHandler::play()
{
	result = visSystem->playSound(FMOD_CHANNEL_FREE, sound, 0, &channel);
	ERRCHECK(result);
}

/* Update function that must run in the game loop
 * The callback will act on the left and right data that 
 * is acquired during update.
 */
void FmodHandler::update()
{
	//Retrieve the spectrum
	result = channel->getSpectrum(specL, this->spectrumSlices, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
	ERRCHECK(result);
	result = channel->getSpectrum(specR, this->spectrumSlices, 1, FMOD_DSP_FFT_WINDOW_TRIANGLE);
	ERRCHECK(result);
	
	//Fill vectors for the callback 
	std::vector<float> left, right;
	for (int i=0; i<spectrumSlices; i++)
	{
		left.push_back(specL[i]); right.push_back(specR[i]);	
	}
	updateCallback(left, right);
	
	visSystem->update();

	Sleep(20);
}

/* Destructor for media objects */
void FmodHandler::release()
{
	result = sound->release(); ERRCHECK(result);
	result = visSystem->close(); ERRCHECK(result);
	result = visSystem->release(); ERRCHECK(result);	
};

