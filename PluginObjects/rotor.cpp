#include "synthlfo.h"
#include "rotor.h"


bool Rotor::update(bool updateAllModRoutings)
{
	lfo1.update();
	lfo2.update();

	SynthLFOParameters param;
	param.frequency_Hz = parameters->rate1;
	param.outputAmplitude = parameters->depth1;
	
	return true;
}







