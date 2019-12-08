// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"
#include "bankwaveviews.h"

/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**


	// --- Declaration of Plugin Parameter Objects 
	PluginParameter* piParam = nullptr;

	// --- continuous control: PB Range
	piParam = new PluginParameter(controlID::masterPitchBend, "PB Range", "semi", controlVariableType::kDouble, 1.000000, 24.000000, 7.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterPitchBend, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Master Tune
	piParam = new PluginParameter(controlID::masterTune, "Master Tune", "semi", controlVariableType::kDouble, -12.000000, 12.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterTune, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Master Volume
	piParam = new PluginParameter(controlID::masterVolume_dB, "Master Volume", "dB", controlVariableType::kDouble, -60.000000, 12.000000, -3.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterVolume_dB, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: LFO1 Wave
	piParam = new PluginParameter(controlID::lfo1Waveform, "LFO1 Wave", "Triangle,Sin,Saw,RSH,QRSH,Noise,QRNoise", "Triangle");
	piParam->setBoundVariable(&lfo1Waveform, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO1 Mode
	piParam = new PluginParameter(controlID::lfo1Mode, "LFO1 Mode", "Sync,One Shot,Free Run", "Sync");
	piParam->setBoundVariable(&lfo1Mode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 fo
	piParam = new PluginParameter(controlID::lfo1Frequency, "LFO1 fo", "Hz", controlVariableType::kDouble, 0.020000, 20.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1Frequency, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Dly
	piParam = new PluginParameter(controlID::lfo1DelayTime_mSec, "LFO1 Dly", "mSec", controlVariableType::kDouble, 0.000000, 2000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1DelayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Ramp
	piParam = new PluginParameter(controlID::lfo1RampTime_mSec, "LFO1 Ramp", "mSec", controlVariableType::kDouble, 0.000000, 2000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1RampTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO2 fo
	piParam = new PluginParameter(controlID::lfo2Frequency, "LFO2 fo", "Hz", controlVariableType::kDouble, 0.000000, 20.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo2Frequency, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Shape X
	piParam = new PluginParameter(controlID::waveShapeX, "Shape X", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&waveShapeX, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Shape Y
	piParam = new PluginParameter(controlID::waveShapeY, "Shape Y", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&waveShapeY, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Osc1 Wave
	piParam = new PluginParameter(controlID::osc1_waveForm, "Osc1 Wave", "wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31", "wave0");
	piParam->setBoundVariable(&osc1_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Banks
	piParam = new PluginParameter(controlID::osc1_BankIndex, "Banks", "Bank 0,Bank 1,Bank 2,Bank 3", "Bank 0");
	piParam->setBoundVariable(&osc1_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc2
	piParam = new PluginParameter(controlID::osc2_waveForm, "Osc2", "wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31", "wave0");
	piParam->setBoundVariable(&osc2_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc2 Bank
	piParam = new PluginParameter(controlID::osc2_BankIndex, "Osc2 Bank", "bank0,bank1,bank2,bank3", "bank0");
	piParam->setBoundVariable(&osc2_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc3 Wave
	piParam = new PluginParameter(controlID::osc3_waveForm, "Osc3 Wave", "wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31", "wave0");
	piParam->setBoundVariable(&osc3_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc3 Bank
	piParam = new PluginParameter(controlID::osc3_BankIndex, "Osc3 Bank", "bank0,bank1,bank2,bank3", "bank0");
	piParam->setBoundVariable(&osc3_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc4 Wave
	piParam = new PluginParameter(controlID::osc4_waveForm, "Osc4 Wave", "wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31", "wave0");
	piParam->setBoundVariable(&osc4_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc4 Bank
	piParam = new PluginParameter(controlID::osc4_BankIndex, "Osc4 Bank", "bank0,bank1,bank2,bank3", "bank0");
	piParam->setBoundVariable(&osc4_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Attack
	piParam = new PluginParameter(controlID::EG1_attackTime_mSec, "EG1 Attack", "mSec", controlVariableType::kDouble, 0.000000, 2000.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&EG1_attackTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Decay
	piParam = new PluginParameter(controlID::EG1_decayTime_mSec, "EG1 Decay", "mSec", controlVariableType::kDouble, 0.000000, 5000.000000, 100.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&EG1_decayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Sustain
	piParam = new PluginParameter(controlID::EG1_sustainLevel, "EG1 Sustain", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.707000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&EG1_sustainLevel, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Release
	piParam = new PluginParameter(controlID::EG1_releaseTime_mSec, "EG1 Release", "mSec", controlVariableType::kDouble, 0.000000, 10000.000000, 1000.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&EG1_releaseTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Hold
	piParam = new PluginParameter(controlID::EG1_holdTime_mSec, "EG1 Hold", "mSec", controlVariableType::kDouble, 0.000000, 40000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&EG1_holdTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Delay
	piParam = new PluginParameter(controlID::EG1_delayTime_mSec, "EG1 Delay", "mSec", controlVariableType::kDouble, 0.000000, 4000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&EG1_delayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: EG1 Mode
	piParam = new PluginParameter(controlID::EG1_mode, "EG1 Mode", "ADSR,AHDSR,AHR,AHR RT", "ADSR");
	piParam->setBoundVariable(&EG1_mode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Retrigger
	piParam = new PluginParameter(controlID::manualRT, "Retrigger", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&manualRT, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Start Value
	piParam = new PluginParameter(controlID::startVal, "EG1 Start Value", "Units", controlVariableType::kDouble, 0.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&startVal, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG 1 End Value
	piParam = new PluginParameter(controlID::endVal, "EG 1 End Value", "Units", controlVariableType::kDouble, 0.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&endVal, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Filter Fc
	piParam = new PluginParameter(controlID::filterFc, "Filter Fc", "Hz", controlVariableType::kDouble, 20.000000, 1800.000000, 880.000000, taper::kVoltOctaveTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&filterFc, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Q Control
	piParam = new PluginParameter(controlID::filterQ, "Q Control", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&filterQ, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Out
	piParam = new PluginParameter(controlID::lfo1Amp, "LFO1 Out", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1Amp, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Out
	piParam = new PluginParameter(controlID::eg1Amp, "EG1 Out", "", controlVariableType::kDouble, -1.000000, 1.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1Amp, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Osc1 fo In
	piParam = new PluginParameter(controlID::osc1FoAmp, "Osc1 fo In", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc1FoAmp, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Osc2 fo In
	piParam = new PluginParameter(controlID::osc2FoAmp, "Osc2 fo In", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc2FoAmp, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: 
	piParam = new PluginParameter(controlID::lfo1toOsc1, "", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&lfo1toOsc1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: 
	piParam = new PluginParameter(controlID::lfo1toOsc2, "", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&lfo1toOsc2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: 
	piParam = new PluginParameter(controlID::eg1toOsc1, "", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&eg1toOsc1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: 
	piParam = new PluginParameter(controlID::eg1toOsc2, "", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&eg1toOsc2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Ratio
	piParam = new PluginParameter(controlID::keyTrackRatio, "Ratio", "", controlVariableType::kDouble, 0.250000, 4.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&keyTrackRatio, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Key Track
	piParam = new PluginParameter(controlID::enableKeyTrack, "Key Track", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&enableKeyTrack, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Mode
	piParam = new PluginParameter(controlID::engineMode, "Mode", "MONO,POLY,UNISON", "MONO");
	piParam->setBoundVariable(&engineMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Unsion Detune
	piParam = new PluginParameter(controlID::unisonDetune, "Unsion Detune", "cents", controlVariableType::kInt, 0.000000, 99.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&unisonDetune, boundVariableType::kInt);
	addPluginParameter(piParam);

	// --- discrete control: Arpeggiator
	piParam = new PluginParameter(controlID::arpOn, "Arpeggiator", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&arpOn, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Arp Scale
	piParam = new PluginParameter(controlID::arpScale, "Arp Scale", "MAJOR,MINOR,DIMINISHED,AUGMENTED", "MAJOR");
	piParam->setBoundVariable(&arpScale, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Arp Mode
	piParam = new PluginParameter(controlID::arpMode, "Arp Mode", "UP CYCLICAL,UP DOWN,DOWN CYCLICAL,DOWN UP", "UP CYCLICAL");
	piParam->setBoundVariable(&arpMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Arp time
	piParam = new PluginParameter(controlID::arpTimeMsec, "Arp time", "msec", controlVariableType::kDouble, 50.000000, 1000.000000, 500.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&arpTimeMsec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- Aux Attributes
	AuxParameterAttribute auxAttribute;

	// --- RAFX GUI attributes
	// --- controlID::masterPitchBend
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterPitchBend, auxAttribute);

	// --- controlID::masterTune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterTune, auxAttribute);

	// --- controlID::masterVolume_dB
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterVolume_dB, auxAttribute);

	// --- controlID::lfo1Waveform
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo1Waveform, auxAttribute);

	// --- controlID::lfo1Mode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo1Mode, auxAttribute);

	// --- controlID::lfo1Frequency
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1Frequency, auxAttribute);

	// --- controlID::lfo1DelayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1DelayTime_mSec, auxAttribute);

	// --- controlID::lfo1RampTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1RampTime_mSec, auxAttribute);

	// --- controlID::lfo2Frequency
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo2Frequency, auxAttribute);

	// --- controlID::waveShapeX
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::waveShapeX, auxAttribute);

	// --- controlID::waveShapeY
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::waveShapeY, auxAttribute);

	// --- controlID::osc1_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc1_waveForm, auxAttribute);

	// --- controlID::osc1_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc1_BankIndex, auxAttribute);

	// --- controlID::osc2_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc2_waveForm, auxAttribute);

	// --- controlID::osc2_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc2_BankIndex, auxAttribute);

	// --- controlID::osc3_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc3_waveForm, auxAttribute);

	// --- controlID::osc3_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc3_BankIndex, auxAttribute);

	// --- controlID::osc4_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc4_waveForm, auxAttribute);

	// --- controlID::osc4_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc4_BankIndex, auxAttribute);

	// --- controlID::EG1_attackTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::EG1_attackTime_mSec, auxAttribute);

	// --- controlID::EG1_decayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::EG1_decayTime_mSec, auxAttribute);

	// --- controlID::EG1_sustainLevel
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::EG1_sustainLevel, auxAttribute);

	// --- controlID::EG1_releaseTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::EG1_releaseTime_mSec, auxAttribute);

	// --- controlID::EG1_holdTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::EG1_holdTime_mSec, auxAttribute);

	// --- controlID::EG1_delayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::EG1_delayTime_mSec, auxAttribute);

	// --- controlID::EG1_mode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::EG1_mode, auxAttribute);

	// --- controlID::manualRT
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1610612736);
	setParamAuxAttribute(controlID::manualRT, auxAttribute);

	// --- controlID::startVal
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::startVal, auxAttribute);

	// --- controlID::endVal
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::endVal, auxAttribute);

	// --- controlID::filterFc
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::filterFc, auxAttribute);

	// --- controlID::filterQ
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::filterQ, auxAttribute);

	// --- controlID::lfo1Amp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483652);
	setParamAuxAttribute(controlID::lfo1Amp, auxAttribute);

	// --- controlID::eg1Amp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483654);
	setParamAuxAttribute(controlID::eg1Amp, auxAttribute);

	// --- controlID::osc1FoAmp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483658);
	setParamAuxAttribute(controlID::osc1FoAmp, auxAttribute);

	// --- controlID::osc2FoAmp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483660);
	setParamAuxAttribute(controlID::osc2FoAmp, auxAttribute);

	// --- controlID::lfo1toOsc1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::lfo1toOsc1, auxAttribute);

	// --- controlID::lfo1toOsc2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::lfo1toOsc2, auxAttribute);

	// --- controlID::eg1toOsc1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::eg1toOsc1, auxAttribute);

	// --- controlID::eg1toOsc2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741826);
	setParamAuxAttribute(controlID::eg1toOsc2, auxAttribute);

	// --- controlID::keyTrackRatio
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::keyTrackRatio, auxAttribute);

	// --- controlID::enableKeyTrack
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::enableKeyTrack, auxAttribute);

	// --- controlID::engineMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::engineMode, auxAttribute);

	// --- controlID::unisonDetune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::unisonDetune, auxAttribute);

	// --- controlID::arpOn
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::arpOn, auxAttribute);

	// --- controlID::arpScale
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::arpScale, auxAttribute);

	// --- controlID::arpMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::arpMode, auxAttribute);

	// --- controlID::arpTimeMsec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::arpTimeMsec, auxAttribute);


	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;

	synthEngine.reset(resetInfo.sampleRate);
    // --- other reset inits
    return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

void PluginCore::updateParameters()
{
	SynthEngineParameters engineParams = synthEngine.getParameters();
	// --- collect GUI control update values

	engineParams.masterPitchBendSensCoarse = (unsigned int)masterPitchBend; // --- this is pitch bend max range in semitones

	engineParams.masterPitchBendSensFine = (unsigned int)(100.0*(masterPitchBend - engineParams.masterPitchBendSensCoarse)); // this is pitch bend max range in semitones

	// --- create two tuning offsets from one master tune value

	engineParams.masterTuningCoarse = (int)masterTune;

	engineParams.masterTuningFine = (int)(100.0*(masterTune - engineParams.masterTuningCoarse)); // --- get fraction and convert to cents (1/100th of a semitone)

	engineParams.masterVolume_dB = masterVolume_dB;

	//LFO1
	engineParams.voiceParameters->lfo1Parameters->frequency_Hz = lfo1Frequency;

	engineParams.voiceParameters->lfo1Parameters->waveform = convertIntToEnum(lfo1Waveform, LFOWaveform);

	engineParams.voiceParameters->lfo1Parameters->mode = convertIntToEnum(lfo1Mode, LFOMode);

	engineParams.voiceParameters->lfo1Parameters->delay = lfo1DelayTime_mSec;

	engineParams.voiceParameters->lfo1Parameters->ramp = lfo1RampTime_mSec;

	engineParams.voiceParameters->lfo1Parameters->waveShapeX = waveShapeX;

	engineParams.voiceParameters->lfo1Parameters->waveShapeX = waveShapeY;

	//osc1
	engineParams.voiceParameters->osc1Parameters->oscillatorWaveformIndex = osc1_waveForm;

	engineParams.voiceParameters->osc1Parameters->oscillatorBankIndex = osc1_BankIndex;


	//osc2
	engineParams.voiceParameters->osc2Parameters->oscillatorWaveformIndex = osc2_waveForm;

	engineParams.voiceParameters->osc2Parameters->oscillatorBankIndex = osc2_BankIndex;

	//osc3
	engineParams.voiceParameters->osc3Parameters->oscillatorWaveformIndex = osc3_waveForm;

	engineParams.voiceParameters->osc3Parameters->oscillatorBankIndex = osc3_BankIndex;

	//osc4
	engineParams.voiceParameters->osc4Parameters->oscillatorWaveformIndex = osc4_waveForm;

	engineParams.voiceParameters->osc4Parameters->oscillatorBankIndex = osc4_BankIndex;

	
	
	//LFO2
	engineParams.voiceParameters->lfo2Parameters->frequency_Hz = lfo2Frequency;

	//engineParams.masterUnisonDetune_Cents = detune;
	
	//arp
	engineParams.voiceParameters->osc1Parameters->arpOn = arpOn;
	engineParams.voiceParameters->osc1Parameters->arpScale = arpScale;
	engineParams.voiceParameters->osc1Parameters->arpMode = arpMode;
	engineParams.voiceParameters->osc1Parameters->arpTimeMsec = arpTimeMsec;
	engineParams.voiceParameters->osc2Parameters->arpOn = arpOn;
	engineParams.voiceParameters->osc2Parameters->arpScale = arpScale;
	engineParams.voiceParameters->osc2Parameters->arpMode = arpMode;
	engineParams.voiceParameters->osc2Parameters->arpTimeMsec = arpTimeMsec;
	engineParams.voiceParameters->osc3Parameters->arpOn = arpOn;
	engineParams.voiceParameters->osc3Parameters->arpScale = arpScale;
	engineParams.voiceParameters->osc3Parameters->arpMode = arpMode;
	engineParams.voiceParameters->osc3Parameters->arpTimeMsec = arpTimeMsec;
	engineParams.voiceParameters->osc4Parameters->arpOn = arpOn;
	engineParams.voiceParameters->osc4Parameters->arpScale = arpScale;
	engineParams.voiceParameters->osc4Parameters->arpMode = arpMode;
	engineParams.voiceParameters->osc4Parameters->arpTimeMsec = arpTimeMsec;

	//EG
	engineParams.voiceParameters->ampEGParameters->attackTime_mSec = EG1_attackTime_mSec;
	engineParams.voiceParameters->ampEGParameters->decayTime_mSec = EG1_decayTime_mSec;
	engineParams.voiceParameters->ampEGParameters->sustainLevel = EG1_sustainLevel;
	engineParams.voiceParameters->ampEGParameters->releaseTime_mSec = EG1_releaseTime_mSec;
	engineParams.voiceParameters->ampEGParameters->holdTime_mSec = EG1_holdTime_mSec;
	engineParams.voiceParameters->ampEGParameters->delayTime_mSec = EG1_delayTime_mSec;
	engineParams.voiceParameters->ampEGParameters->egContourType = convertIntToEnum(EG1_mode, egType);
	engineParams.voiceParameters->ampEGParameters->manualRT = manualRT;
	engineParams.voiceParameters->ampEGParameters->startVal = startVal;
	engineParams.voiceParameters->ampEGParameters->endVal = endVal;

	engineParams.voiceParameters->moogFilterParameters->fc = filterFc;
	engineParams.voiceParameters->moogFilterParameters->Q = filterQ;
	engineParams.voiceParameters->moogFilterParameters->enableKeyTrack = enableKeyTrack;
	engineParams.voiceParameters->moogFilterParameters->keyTrackRatio = keyTrackRatio;
	//LFO1 -> osc1fo
	engineParams.setMM_SourceMasterIntensity(kLFO1_Normal, lfo1Amp);
	engineParams.setMM_ChannelEnable(kLFO1_Normal, kOsc1_fo,lfo1toOsc1);
	engineParams.setMM_DestMasterIntensity(kOsc1_fo, osc1FoAmp);
	//LFO1 -> osc2fo
	engineParams.setMM_SourceMasterIntensity(kLFO1_Normal, lfo1Amp);
	engineParams.setMM_ChannelEnable(kLFO1_Normal, kOsc2_fo, lfo1toOsc2);
	engineParams.setMM_DestMasterIntensity(kOsc2_fo, osc2FoAmp);
	//EG1->osc1fo
	engineParams.setMM_SourceMasterIntensity(kEG1_Normal, eg1Amp);
	engineParams.setMM_ChannelEnable(kEG1_Normal, kOsc1_fo, eg1toOsc1);
	engineParams.setMM_DestMasterIntensity(kOsc1_fo, osc1FoAmp);
	if (engineMode == 0)
	{
		engineParams.mode = synthMode::kMono;
	}
	if (engineMode == 1)
	{
		engineParams.mode = synthMode::kPoly;
	}
	if (engineMode == 2)
	{
		engineParams.mode = synthMode::kUnison;
	}
	engineParams.masterUnisonDetune_Cents = unisonDetune;
	// --- THE update - this trickles all param updates
	// via the setParameters( ) of each
	synthEngine.setParameters(engineParams);
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

	// --- update all parameters ONCE per buffer
	updateParameters();

    return true;
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
    // --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();

    // --- decode the channelIOConfiguration and process accordingly
    //
	// --- Synth Plugin:
	// --- Synth Plugin --- remove for FX plugins// --- Synth Plugin 

	if (getPluginType() == kSynthPlugin)

	{

		// --- update GUI params: uncomment for processing on each sample interval

		//     otherwise, this is done PER-BUFFER in PluginCore::preProcessAudioBuffers( )

		// updateParameters();



		// --- do the synth render

		const SynthRenderData render = synthEngine.renderAudioOutput();



		// --- write left channel

		processFrameInfo.audioOutputFrame[0] = render.synthOutputs[0];



		// --- write right channel

		if (processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)

			processFrameInfo.audioOutputFrame[1] = render.synthOutputs[1];



		return true;	/// processed

	}






    // --- FX Plugin:
    if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Mono-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Stereo-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[1];

        return true; /// processed
    }

    return false; /// NOT processed
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		if (bankAndWaveGroup_0)
			bankAndWaveGroup_0->updateView();
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	{

		// --- decode name string

		if (messageInfo.inMessageString.compare("BankWaveController_0") == 0)

		{

			// --- (1) get the custom view interface via incoming message data*
			//         can use it for communication

			if (bankAndWaveGroup_0 != static_cast<ICustomView*>(messageInfo.inMessageData))

				bankAndWaveGroup_0 = static_cast<ICustomView*>(messageInfo.inMessageData);

			if (!bankAndWaveGroup_0) return false;

			// --- need to tell the subcontroller the bank names
			VSTGUI::BankWaveMessage subcontrollerMessage;

			subcontrollerMessage.message = VSTGUI::UPDATE_BANK_NAMES;

			subcontrollerMessage.bankNames = synthEngine.getBankNames(0, 0); // voice, voice-oscillator 

			bankAndWaveGroup_0->sendMessage(&subcontrollerMessage);


			// --- now add the wave names for each bank

			subcontrollerMessage.message = VSTGUI::ADD_BANK_WAVENAMES;


			// --- bank 0
			subcontrollerMessage.bankIndex = 0;

			subcontrollerMessage.waveformNames = synthEngine.getOscWaveformNames(0, 0, 0); // voice, voice-oscillator, bank 

			bankAndWaveGroup_0->sendMessage(&subcontrollerMessage);

			// --- bank 1
			subcontrollerMessage.bankIndex = 1;

			subcontrollerMessage.waveformNames = synthEngine.getOscWaveformNames(0, 0, 1); // voice,  voice-oscillator, bank 

			bankAndWaveGroup_0->sendMessage(&subcontrollerMessage);

			// --- registered!
			return true;
		}
	}
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}


/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	synthEngine.processMIDIEvent(event);
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	SynthEngineParameters engineParams = synthEngine.getParameters();
	engineParams.voiceParameters->vectorJSData = vectorJoysickData;
	synthEngine.setParameters(engineParams);

	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// --- Plugin Presets 
	int index = 0;
	PresetInfo* preset = nullptr;

	// --- Preset: Factory Preset
	preset = new PresetInfo(index++, "Factory Preset");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::masterPitchBend, 7.000000);
	setPresetParameter(preset->presetParameters, controlID::masterTune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::masterVolume_dB, -3.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Waveform, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Mode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Frequency, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::lfo1DelayTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1RampTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo2Frequency, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::waveShapeX, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::waveShapeY, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::osc1_waveForm, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1_BankIndex, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_waveForm, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_BankIndex, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc3_waveForm, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc3_BankIndex, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc4_waveForm, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc4_BankIndex, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::EG1_attackTime_mSec, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::EG1_decayTime_mSec, 100.000000);
	setPresetParameter(preset->presetParameters, controlID::EG1_sustainLevel, 0.707000);
	setPresetParameter(preset->presetParameters, controlID::EG1_releaseTime_mSec, 1000.000000);
	setPresetParameter(preset->presetParameters, controlID::EG1_holdTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::EG1_delayTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::EG1_mode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::manualRT, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::startVal, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::endVal, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::filterFc, 880.000000);
	setPresetParameter(preset->presetParameters, controlID::filterQ, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Amp, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1Amp, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1FoAmp, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2FoAmp, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1toOsc1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1toOsc2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1toOsc1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1toOsc2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::keyTrackRatio, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::enableKeyTrack, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::engineMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::unisonDetune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::arpOn, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::arpScale, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::arpMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::arpTimeMsec, 500.000000);
	addPreset(preset);


	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
