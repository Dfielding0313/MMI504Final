#pragma once

#include "synthdefs.h"
#include "guiconstants.h"


/**
\enum LFOWaveform

\ingroup SynthDefs

\brief LFO waveform type

 - kTriangle : triangle wave

 - kSin : sine wave

 - kSaw : saw tooth wave

 - kRSH : random sample and hold
 
 - kQRSH : quasi-random sample and hold

 - kNoise : random noise

 - kQRNoise : quasi-random noise

*/
// --- LFO may have very diff waveforms from pitched output
enum class LFOWaveform { kTriangle, kSin, kSaw, kRSH, kQRSH, kNoise, kQRNoise };

/**
\enum LFOMode

\ingroup SynthDefs

\brief LFO Mode of Operation

 - kSync : LFO restarts with each new note event

 - kOneShot : One cycle of LFO only

 - kFreeRun : Oscillator free runs after first note on event

*/

enum class LFOMode { kSync, kOneShot, kFreeRun };

// --- indexes in OscillatorOutputData: inputs array


enum {
	kLFONormalOutput,			// 0
	kLFONormalOutputInverted,	// 1 etc...
	kLFOQuadPhaseOutput,
	kLFOQuadPhaseOutputInverted,
	kUnipolarOutputFromMax,		///> this mimics an INVERTED EG going from MAX -> MAX */
	kUnipolarOutputFromMin		/* this mimics an EG going from 0.0 -> MAX */
};




// ---
/**
\struct SynthLFOParameters

\ingroup SynthDefs

\brief LFO parameters

 - waveform : the type of waveform

 - mode : the mode of the LFO

 - frequency_Hz : the frequency in Hz of the LFO

 - outputAmplitue : the amplitude of the output

*/

//inline double msecToSamples(double sampleRate, double timeMSec)
//
//{
//
//	return sampleRate * (timeMSec / 1000.0);;
//
//}

struct SynthLFOParameters
{
	SynthLFOParameters() {}
	SynthLFOParameters& operator=(const SynthLFOParameters& params)	// need this override for collections to work
	{
		if (this == &params)
			return *this;

		waveform = params.waveform;
		mode = params.mode;

		frequency_Hz = params.frequency_Hz;
		outputAmplitude = params.outputAmplitude;
		delay = params.delay;
		ramp = params.ramp;
		waveShapeX = params.waveShapeX;
		waveShapeY = params.waveShapeY;

		return *this;
	}

	// --- individual parameters
	LFOWaveform waveform = LFOWaveform::kTriangle;
	LFOMode mode = LFOMode::kSync;

	double frequency_Hz = 2.0;
	double outputAmplitude = 1.0;
	double delay = 0.0;
	double ramp = 0.0;
	double waveShapeX = 0.0;
	double waveShapeY = 0.0;

};


/**
\class SynthLFO
\ingroup SynthClasses
\brief Encapsulates a synth LFO

Outputs: contains 6 outputs

	- kLFONormalOutput			
	- kLFONormalOutputInverted	
	- kLFOQuadPhaseOutput
	- kLFOQuadPhaseOutputInverted
	- kUnipolarOutputFromMax	
	- kUnipolarOutputFromMin		

*/

// --- LFO object, note ISynthOscillator
class SynthLFO : public ISynthModulator//ISynthOscillator
{
public:

	// --- Recieving midi input data

	SynthLFO(const std::shared_ptr<MidiInputData> _midiInputData, std::shared_ptr<SynthLFOParameters> _parameters)
		: midiInputData(_midiInputData) 
	, parameters(_parameters){
		srand(time(NULL)); // --- seed random number generator

		// --- randomize the PN register
		pnRegister = rand();

	}	/* C-TOR */
	virtual ~SynthLFO() {}				/* D-TOR */

	// --- ISynthOscillator
	virtual bool reset(double _sampleRate)
	{
		sampleRate = _sampleRate;
		phaseInc = parameters->frequency_Hz / sampleRate;

		// --- timebase variables
		modCounter = 0.0;			///< modulo counter [0.0, +1.0]
		modCounterQP = 0.25;		///<Quad Phase modulo counter [0.0, +1.0]
		modT = 0.0;

		lfodelay.resetTimer();
		rampTime.resetTimer();

		return true;
	}

	// --- ISynthModulator cont'd
	virtual bool update(bool updateAllModRoutings = true);
	virtual bool doNoteOn(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity) 
	{ 
		renderComplete = false;
		if (parameters->mode == LFOMode::kSync || parameters->mode == LFOMode::kOneShot)
		{
			//modCounter = 0.0;			///< modulo counter [0.0, +1.0]
			modCounterQP = 0.25;		///< Quad Phase modulo counter [0.0, +1.0]
			modCounter = parameters->waveShapeX;
			phaseInc = parameters->waveShapeY;
			lfodelay.resetTimer();
			rampTime.resetTimer();

		}
	

		randomSHCounter = -1; // -1 = reset
		return true; 
	}

	virtual bool doNoteOff(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity)
	{
		if (parameters->ramp > 0.0) parameters->outputAmplitude = 0.0;
		return true; 
	}

	// --- the oscillator function
	const ModOutputData renderModulatorOutput();

	// --- get our modulators
	virtual std::shared_ptr<ModInputData> getModulators() {
		return modulators;
	}

	// --- not used here;
	virtual void setModulators(std::shared_ptr<ModInputData> _modulators) {
		modulators = _modulators;
	}

protected:
	// --- MIDI Data Interface
	const std::shared_ptr<MidiInputData> midiInputData = nullptr;
	
	// --- modulators
	std::shared_ptr<ModInputData> modulators = std::make_shared<ModInputData>();

	// --- parameters
	std::shared_ptr<SynthLFOParameters> parameters = nullptr;

	// --- sample rate
	double sampleRate = 0.0;			///< sample rate


	Timer lfodelay;
	Timer rampTime;


	// --- timebase variables
	double modCounter = 0.0;			///< modulo counter [0.0, +1.0]
	double phaseInc = 0.0;				///< phase inc = fo/fs
	double modCounterQP = 0.25;			///< Quad Phase modulo counter [0.0, +1.0]
	bool renderComplete = false;		///< flag for one-shot

	double modT = 0.0;

	// --- 32-bit register for RS&H
	uint32_t pnRegister = 0;			///< 32 bit register for PN oscillator
	int randomSHCounter = -1;			///< random sample/hold counter;  -1 is reset condition
	double randomSHValue = 0.0;			///< current output, needed because we hold this output for some number of samples = (sampleRate / oscFrequency)

	/**
	\struct checkAndWrapModulo
	\brief Check a modulo counter and wrap it if necessary
	*/
	inline bool checkAndWrapModulo(double& moduloCounter, double phaseInc)
	{
		// --- for positive frequencies
		if (phaseInc > 0 && moduloCounter >= 1.0)
		{
			moduloCounter -= 1.0;
			return true;
		}

		// --- for negative frequencies
		if (phaseInc < 0 && moduloCounter <= 0.0)
		{
			moduloCounter += 1.0;
			return true;
		}

		return false;
	}

	/**
	\struct advanceAndCheckWrapModulo
	\brief Advance, and then check a modulo counter and wrap it if necessary
	*/
	inline bool advanceAndCheckWrapModulo(double& moduloCounter, double phaseInc)
	{
		// --- advance counter
		moduloCounter += phaseInc;

		// --- for positive frequencies
		if (phaseInc > 0 && moduloCounter >= 1.0)
		{
			moduloCounter -= 1.0;
			return true;
		}

		// --- for negative frequencies
		if (phaseInc < 0 && moduloCounter <= 0.0)
		{
			moduloCounter += 1.0;
			return true;
		}

		return false;
	}

	// --- increment the modulo counter
	inline void advanceModulo(double& moduloCounter, double phaseInc) { moduloCounter += phaseInc; }

	// --- sine approximation with parabolas
	const double B = 4.0 / kPi;
	const double C = -4.0 / (kPi* kPi);
	const double P = 0.225;
	// http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
	//
	// --- angle is -pi to +pi
	inline double parabolicSine(double angle)
	{
		double y = B * angle + C * angle * fabs(angle);
		y = P * (y * fabs(y) - y) + y;
		return y;
	}
};

