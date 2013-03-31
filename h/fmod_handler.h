#ifndef FMOD_HANDLER_H
#define FMOD_HANDLER_H

//Exception handler for FMOD
void ERRCHECK(FMOD_RESULT result);

class FmodHandler
{
	FMOD::System	*visSystem;
	FMOD::Sound		*sound;
	FMOD::Channel	*channel;
	FMOD_RESULT		result;
	unsigned int	version;
	int 			outputfreq;
	float			*spectrum, *specL, *specR;
	void			(*updateCallback) (
						std::vector<float> , 
						std::vector<float>);
	int spectrumSlices;

public:
	FmodHandler(
		char *, 
		void (*) (std::vector<float> right, std::vector<float> left),
		int spectrumSlices
		);
	void release();
	void play();
	void update();
	
};

#endif //FMOD_HANDLER_H

