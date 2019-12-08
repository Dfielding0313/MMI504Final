#pragma once

#include "synthdefs.h"
#include "guiconstants.h"
#include "synthLFO.h"


struct RotorParameters
{
	RotorParameters() {}
	RotorParameters& operator=(const RotorParameters& params)	// need this override for collections to work
	{
		if (this == &params)
			return *this;

		rate1 = params.rate1;
		rate2 = params.rate2;
		depth1 = params.depth1;
		depth2 = params.depth2;


		return *this;
	}

	// --- individual parameters

	double rate1 = 0.0;
	double rate2 = 0.0;
	double depth1 = 0.0;
	double depth2 = 0.0;

	//std::shared_ptr<SynthLFOParameters> lfo1parameters = std::shared_ptr<SynthLFOParameters> parameters = nullptr;

};


/**
\class Rotor
\ingroup SynthClasses

*/

// --- LFO object, note ISynthOscillator
class Rotor : public ISynthModulator//ISynthOscillator
{
public:

	/* C-TOR */
	virtual ~Rotor() {}				/* D-TOR */

	// --- ISynthOscillator
	virtual bool reset(double _sampleRate)
	{
		sampleRate = _sampleRate;
		lfo1.reset(_sampleRate);
		lfo2.reset(_sampleRate);

		return true;
	}

	// --- ISynthModulator cont'd
	virtual bool update(bool updateAllModRoutings = true);

	//lfo1.reset(new SynthLFO(midiInputData, parameters->lfo1Parameters));
	//lfo2.reset(new SynthLFO(midiInputData, parameters->lfo2Parameters));

	//const RenderOutput();

protected:

	std::shared_ptr<RotorParameters> parameters = nullptr;

	// --- sample rate
	double sampleRate = 0.0;			///< sample rate

	SynthLFO lfo1;
	SynthLFO lfo2;
};

