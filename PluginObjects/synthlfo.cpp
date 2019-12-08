#include "synthlfo.h"


bool SynthLFO::update(bool updateAllModRoutings)
{
	// --- Run priority modulators 
	double bipolarFMod = modulators->modulationInputs[kFrequencyMod];
	double range = (20.0 - 0.02) / 2.0;

	//double modVal = range * bipolarFMod;
	//boundValue(modVal, 0.02, 20);

	bipolarFMod *= range;
	bipolarFMod += parameters->frequency_Hz;
	boundValue(bipolarFMod, 0.02, 20);


	///this for some reason breaks the LFO. interesting.
	//modCounter = parameters->waveShapeX;


	// --- End Priority modulators
	if (!updateAllModRoutings)
		return true;


	//line for final LFO freq (GUI + modulated)

	double d = msecToSamples(sampleRate, parameters->ramp); //establishing the timer
	rampTime.setTargetValueInSamples(d);

		//implementing the lfo output ramp
	if (parameters->ramp > 0.0) {

		if (!(rampTime.timerExpired())) {
			//need to figure out how to make outputAmplitude start at 0
				parameters->outputAmplitude = parameters->outputAmplitude + (1 / d);
				rampTime.advanceTimer();

		}
	};

	double leftSlope = (parameters->waveShapeY) / (parameters->waveShapeX);
	double rightSlope = (1 - parameters->waveShapeY) / (1 - parameters->waveShapeX);

	if (modCounter < parameters->waveShapeY)
	{
		phaseInc = bipolarFMod / sampleRate * leftSlope;
	}
	else
	{
		phaseInc = bipolarFMod / sampleRate * rightSlope;

	}

	//phaseInc = parameters->frequency_Hz / sampleRate;

	//modify based on modulation frequency applied

	return true;
}

const ModOutputData SynthLFO::renderModulatorOutput()
{
	// --- setup for output
	ModOutputData lfoOutputData; // should auto-zero on instantiation
	lfoOutputData.clear();

	if (renderComplete)
	{
		return lfoOutputData;
	}
	

	//This is the implementation of the lfo delay
	double dd = msecToSamples(sampleRate, parameters->delay);
	lfodelay.setTargetValueInSamples(dd);
	if(!(lfodelay.timerExpired()))
	{
		lfodelay.advanceTimer();
		if (parameters->mode == LFOMode::kOneShot || parameters->mode == LFOMode::kSync)
		{
			return lfoOutputData;
		}

		if (parameters->mode == LFOMode::kFreeRun)
		{
			return lfoOutputData;
		}
	}

	////implementing the lfo output ramp
	//else if (parameters->ramp > 0.0) {
	//	double d = msecToSamples(sampleRate, parameters->ramp); //establishing the timer
	//	rampTime.setTargetValueInSamples(d);
	//	if (!(rampTime.timerExpired())) {
	//		//need to figure out how to make outputAmplitude start at 0
	//		parameters->outputAmplitude = parameters->outputAmplitude + (1 / d);
	//		rampTime.advanceTimer();

	//		return lfoOutputData;
	//	}
	//};


	// --- always first!
	bool bWrapped = checkAndWrapModulo(modCounter, phaseInc);
	if (bWrapped && parameters->mode == LFOMode::kOneShot)
	{
		renderComplete = true;
		return lfoOutputData;
	}

	// --- QP output always follows location of current modulo; first set equal

	modCounterQP = modCounter;  ///this is the old code

	// --- then, advance modulo by quadPhaseInc = 0.25 = 90 degrees, AND wrap if needed
	advanceAndCheckWrapModulo(modCounterQP, 0.25);

	// --- calculate the oscillator value
	if (parameters->waveform == LFOWaveform::kSin)
	{
		// --- calculate normal angle
		double angle = modCounter*2.0*kPi - kPi;

		// --- norm output with parabolicSine approximation
		lfoOutputData.modulationOutputs[kLFONormalOutput] = parabolicSine(-angle);

		// --- calculate QP angle
		angle = modCounterQP*2.0*kPi - kPi;

		// --- calc QP output
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = parabolicSine(-angle);
	}
	else if (parameters->waveform == LFOWaveform::kTriangle)
	{
		// --- triv saw
		lfoOutputData.modulationOutputs[kLFONormalOutput] = unipolarToBipolar(modCounter);

		// --- bipolar triagle
		lfoOutputData.modulationOutputs[kLFONormalOutput] = 2.0*fabs(lfoOutputData.modulationOutputs[kLFONormalOutput]) - 1.0;

		// -- quad phase
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = unipolarToBipolar(modCounterQP);

		// bipolar triagle
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = 2.0*fabs(lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput]) - 1.0;
	}
	else if (parameters->waveform == LFOWaveform::kSaw)
	{
		lfoOutputData.modulationOutputs[kLFONormalOutput] = unipolarToBipolar(modCounter);
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = unipolarToBipolar(modCounterQP);
	}
	else if (parameters->waveform == LFOWaveform::kNoise)
	{
		lfoOutputData.modulationOutputs[kLFONormalOutput] = doWhiteNoise();
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = doWhiteNoise();
	}
	else if (parameters->waveform == LFOWaveform::kQRNoise)
	{
		lfoOutputData.modulationOutputs[kLFONormalOutput] = doPNSequence(pnRegister);
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = doPNSequence(pnRegister);
	}
	else if (parameters->waveform == LFOWaveform::kRSH || parameters->waveform == LFOWaveform::kQRSH)
	{
		// --- is this is the very first run? if so, form first output sample
		if (randomSHCounter < 0)
		{
			if (parameters->waveform == LFOWaveform::kRSH)
				randomSHValue = doWhiteNoise();
			else
				randomSHValue = doPNSequence(pnRegister);

			// --- init the sample counter, will be advanced below
			randomSHCounter = 1.0;
		}
		// --- has hold time been exceeded? if so, generate next output sample
		else if (randomSHCounter > (sampleRate / parameters->frequency_Hz))
		{
			// --- wrap counter
			randomSHCounter -= sampleRate / parameters->frequency_Hz;

			if (parameters->waveform == LFOWaveform::kRSH)
				randomSHValue = doWhiteNoise();
			else
				randomSHValue = doPNSequence(pnRegister);
		}

		// --- advance the sample counter
		randomSHCounter += 1.0;

		lfoOutputData.modulationOutputs[kLFONormalOutput] = randomSHValue;
		lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] = randomSHValue;
	}



	// --- scale by amplitude
	lfoOutputData.modulationOutputs[kLFONormalOutput] *= parameters->outputAmplitude;
	lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput] *= parameters->outputAmplitude;

	// --- invert two main outputs to make the opposite versions, scaling carries over
	lfoOutputData.modulationOutputs[kLFONormalOutputInverted] = -lfoOutputData.modulationOutputs[kLFONormalOutput];
	lfoOutputData.modulationOutputs[kLFOQuadPhaseOutputInverted] = -lfoOutputData.modulationOutputs[kLFOQuadPhaseOutput];
	
	// --- special unipolar from max output for tremolo
	//
	// --- first, convert to unipolar
	lfoOutputData.modulationOutputs[kUnipolarOutputFromMax] = bipolarToUnipolar(lfoOutputData.modulationOutputs[kLFONormalOutput]);
	lfoOutputData.modulationOutputs[kUnipolarOutputFromMin] = bipolarToUnipolar(lfoOutputData.modulationOutputs[kLFONormalOutput]);

	// --- then shift upwards by enough to put peaks right at 1.0
	//     NOTE: leaving the 0.5 in the equation - it is the unipolar offset when convering bipolar; but it could be changed...
	lfoOutputData.modulationOutputs[kUnipolarOutputFromMax] = lfoOutputData.modulationOutputs[kUnipolarOutputFromMax] + (1.0 - 0.5 - (parameters->outputAmplitude / 2.0));
	
	// --- then shift down enough to put troughs at 0.0
	lfoOutputData.modulationOutputs[kUnipolarOutputFromMin] = lfoOutputData.modulationOutputs[kUnipolarOutputFromMin] - (1.0 - 0.5 - (parameters->outputAmplitude / 2.0));

	// --- setup for next sample period
	advanceModulo(modCounter, phaseInc);

	// --- scale by amplituded
	return lfoOutputData;
}



