// --- Synth Core v1.0
//
#include "wavetableoscillator.h"
//


// --- oscillator
WaveTableOsc::WaveTableOsc(const std::shared_ptr<MidiInputData> _midiInputData, 
							std::shared_ptr<SynthOscParameters> _parameters, 
							std::shared_ptr<WaveTableData> _waveTableData)
	: midiInputData(_midiInputData)
	, parameters(_parameters)
	, waveTableData(_waveTableData)
{
	if (!parameters)
		parameters = std::make_shared<SynthOscParameters>();
	
	if (!waveTableData)
		waveTableData = std::make_shared<WaveTableData>();

	// --- SIK_TABLES are the default
	selectedWaveBank = waveTableData->getInterface(getBankIndex(bankSet, parameters->oscillatorBankIndex));


}

WaveTableOsc::~WaveTableOsc()
{
}

// --- ISynthOscillator
bool WaveTableOsc::reset(double _sampleRate)
{
	// --- save sample rate
	sampleRate = _sampleRate;

	// --- reset the VA counter; remove this for free-run operation
	modCounter = 0.0;
	phaseInc = 0.0;
	waveTableReadIndex = 0.0;
	arpTime.resetTimer();

	return true;
}

std::vector<std::string> WaveTableOsc::getWaveformNames(uint32_t bankIndex)

{

	std::vector<std::string> emptyVector;



	// --- decode bank index

	IWaveBank* bank = waveTableData->getInterface(getBankIndex(bankSet, bankIndex));

	if (bank)

		return bank->getWaveformNames();



	emptyVector.clear();

	return emptyVector;

}

bool WaveTableOsc::doNoteOn(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity)
{
	// --- save pitch
	midiNotePitch = midiPitch;
	midiNoteNumber = _midiNoteNumber;
	renderMidiNoteNumber = midiNoteNumber; // this will get changed later after all modulations
	
	if (!parameters->enableFreeRunMode)
	{
		modCounter = 0.0;
		waveTableReadIndex = 0.0;
	}
	phaseInc = 0.0;
	arpTime.resetTimer();
	mode = parameters->arpMode;
	return true;
}

bool WaveTableOsc::doNoteOff(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity)
{
	return true;
}

bool WaveTableOsc::update(bool updateAllModRoutings)
{
	// --- Run priority modulators 
	// --- note that this is a dummy call, just to keep the glide modulator synced
	glideModulator.getNextGlideModSemitones();
	if (parameters->arpOn)
	{
		arpTime.advanceTimer();
	}
	// --- End Priority modulators
	if (!updateAllModRoutings)
		return true;

	// --- calculate MIDI pitch bend range
	double midiPitchBendRange = midiInputData->globalMIDIData[kMIDIMasterPBSensCoarse] +
		(midiInputData->globalMIDIData[kMIDIMasterPBSensFine] / 100.0);

	// --- calculate MIDI pitch bend (USER)
	double midiPitchBend = midiPitchBendRange * midiPitchBendToBipolar(midiInputData->globalMIDIData[kMIDIPitchBendDataLSB],
		midiInputData->globalMIDIData[kMIDIPitchBendDataMSB]);

	// --- coarse (semitones): -64 to +63 maps-> 0, 127 (7-bit)
	int mtCoarse = midiInputData->globalMIDIData[kMIDIMasterTuneCoarseMSB];
	mapIntValue(mtCoarse, 0, 127, -64, +63, false); // false = no rounding

	// --- fine (cents): -100 to +100 as MIDI 14-bit
	double mtFine = midi14_bitToDouble(midiInputData->globalMIDIData[kMIDIMasterTuneFineLSB], midiInputData->globalMIDIData[kMIDIMasterTuneFineMSB], -100.0, 100.0);

	// --- this gives proper int.fraction value
	double masterTuning = (double)mtCoarse + ((double)mtFine / 100.0);
	
	// --- calculate combined tuning offsets by simply adding values in semitones
	double fmodInput = modulators->modulationInputs[kBipolarMod] * kOscBipolarModRangeSemitones;

	// --- do the portamento
	double glideMod = glideModulator.getNextGlideModSemitones();

	//arpeggiator
	int scalesUp[4][4] = { { 0, 4, 7, 12 } ,{ 0, 3, 7, 12 } ,{ 0, 3, 6, 9 },{ 0, 4, 8, 12 } };
	int scalesUpDown[4][6] = { { 0, 4, 7, 12, 7, 4 } ,{ 0, 3, 7, 12 , 7, 3} ,{ 0, 3, 6, 9, 6, 3 },{ 0, 4, 8, 12, 8, 4 } };
	int scalesDown[4][4] = { { 0, -5, -8, -12 } ,{ 0, -5, -9, -12 } ,{ 0, -3, -6, -9 },{ 0, -4, -8, -12 } };
	int scalesDownUp[4][6] = { { 0, -5, -8, -12, -8, -5 } ,{ 0, -5, -9, -12 , -9, -5} ,{ 0, -3, -6, -9, -6, -3 },{ 0, -4, -8, -12, -8, -4 } };
	double arp = 0;
	double x = parameters->arpTimeMsec * sampleRate / 1000;
	arpTime.setTargetValueInSamples(x);
	if (parameters->arpOn)
	{
		if (arpTime.timerExpired())
		{
			index++;
			arpTime.resetTimer();
			if ((mode == 0 || mode == 2) && index > 3)
			{
				index = 0;
			}
			if ((mode == 1 || mode == 3) && index > 5)
			{
				index = 0;
			}
		}
		if(mode == 0)
			arp = scalesUp[parameters->arpScale][index];
		if (mode == 1)
			arp = scalesUpDown[parameters->arpScale][index];
		if (mode == 2)
			arp = scalesDown[parameters->arpScale][index];
		if (mode == 3)
			arp = scalesDownUp[parameters->arpScale][index];
	}
	// --- calculate combined tuning offsets by simply adding values in semitones
	double currentPitchModSemitones = glideMod + arp +
		fmodInput +
		midiPitchBend +
		masterTuning +
		(parameters->detuneOctaves * 12) +						/* octave*12 = semitones */
		(parameters->detuneSemitones) +							/* semitones */
		(parameters->detuneCents / 100.0) +						/* cents/100 = semitones */
		(parameters->unisonDetuneCents / 100.0);				/* cents/100 = semitones */

	// --- lookup the pitch shift modifier (fraction)
	//double pitchShift = pitchShiftTableLookup(currentPitchModSemitones);

	// --- direct calculation version 2^(n/12) - note that this is equal temperatment
	double pitchShift = pow(2.0, currentPitchModSemitones / 12.0);

	// --- calculate the moduated pitch value
	oscillatorFrequency = midiNotePitch*pitchShift*parameters->fmRatio;
	oscillatorFrequencySlaveOsc = oscillatorFrequency*parameters->hardSyncRatio;

	// --- BOUND the value to our range - in theory, we would bound this to any NYQUIST
	boundValue(oscillatorFrequency, 0.0, sampleRate / 2.0);
	boundValue(oscillatorFrequencySlaveOsc, 0.0, sampleRate / 2.0);
	
	// --- find the midi note closest to the pitch to select the wavetable
	renderMidiNoteNumber = midiNoteNumberFromOscFrequency(oscillatorFrequency);

	// --- BANK is set here; can have any number of banks
	selectedWaveBank = waveTableData->getInterface(getBankIndex(bankSet, parameters->oscillatorBankIndex));

	uint32_t tableLen = kDefaultWaveTableLength;
	// --- calculate phase inc; this uses FINAL oscFrequency variable above
	//
	//     NOTE: uses selected bank from line of code above; these must be in pairs.
	selectedWaveTable = selectedWaveBank->selectTable(parameters->oscillatorWaveformIndex, renderMidiNoteNumber, tableLen);
	
	// --- if table size changed, need to reset the current read location
	//     to be in the same relative location as before
	if (tableLen != currentTableLength)
	{
		// --- need to reset the read location to reflect the new table
		double position = waveTableReadIndex / (double)currentTableLength;
		waveTableReadIndex = position*tableLen;
		currentTableLength = tableLen;
	}

	// --- note that we need the current table length for this calculation, and we save it
	phaseInc = calculateWaveTablePhaseInc(oscillatorFrequency, sampleRate, currentTableLength);

	return true;
}

const OscillatorOutputData WaveTableOsc::renderAudioOutput()
{
	// --- prep output buffer
	oscillatorAudioData.outputs[0] = 0.0;
	oscillatorAudioData.outputs[1] = 0.0;

	// --- render into left channel
	oscillatorAudioData.outputs[0] = readWaveTable(waveTableReadIndex, phaseInc);

	// --- scale by output amplitude
	oscillatorAudioData.outputs[0] *= (parameters->outputAmplitude * modulators->modulationInputs[kAmpMod]);

	// --- copy to right channel
	oscillatorAudioData.outputs[1] = oscillatorAudioData.outputs[0];

	return oscillatorAudioData;
}

// --- read a table and do linear interpolation
double WaveTableOsc::readWaveTable(double& readIndex, double _phaseInc)
{
	// --- read wave table
	double output = 0.0;
	double phaseModulator = modulators->modulationInputs[kPhaseMod];

	// --- NOTE: when using hard-sync, phase distortion (oscillator shape) is turned off
	if (parameters->enableHardSync)
	{
		// --- add your hard sync code here!

	}
	else
	{
		// --- apply phase modulation, if any -- HINT: you can sum phase modulations
		double phaseModReadIndex = readIndex + phaseModulator * currentTableLength;

		// --- check for multi-wrapping on new read index
		checkAndWrapWaveTableIndex(phaseModReadIndex, currentTableLength);

		// --- do the table read operation
		output = selectedWaveBank->readWaveTable(selectedWaveTable,phaseModReadIndex);
	}

	// --- increment index
	readIndex += _phaseInc;
	checkAndWrapWaveTableIndex(readIndex, currentTableLength);

	// --- adjust with final volume (amplitude)
	return output;
}