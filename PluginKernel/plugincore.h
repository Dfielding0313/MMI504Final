// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
    \file   plugincore.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  base class interface file for ASPiK plugincore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"
#include "synthcore.h"

// **--0x7F1F--**

// --- Plugin Variables controlID Enumeration 

enum controlID {
	masterPitchBend = 9,
	masterTune = 19,
	masterVolume_dB = 29,
	lfo1Waveform = 50,
	lfo1Mode = 51,
	lfo1Frequency = 52,
	lfo1DelayTime_mSec = 53,
	lfo1RampTime_mSec = 54,
	lfo2Frequency = 30,
	waveShapeX = 32,
	waveShapeY = 33,
	osc1_waveForm = 55,
	osc1_BankIndex = 56,
	osc2_waveForm = 65,
	osc2_BankIndex = 66,
	osc3_waveForm = 57,
	osc3_BankIndex = 67,
	osc4_waveForm = 58,
	osc4_BankIndex = 68,
	EG1_attackTime_mSec = 72,
	EG1_decayTime_mSec = 74,
	EG1_sustainLevel = 75,
	EG1_releaseTime_mSec = 76,
	EG1_holdTime_mSec = 73,
	EG1_delayTime_mSec = 77,
	EG1_mode = 78,
	manualRT = 79,
	startVal = 71,
	endVal = 70,
	filterFc = 44,
	filterQ = 45,
	lfo1Amp = 90,
	eg1Amp = 100,
	osc1FoAmp = 81,
	osc2FoAmp = 82,
	lfo1toOsc1 = 91,
	lfo1toOsc2 = 92,
	eg1toOsc1 = 101,
	eg1toOsc2 = 102,
	keyTrackRatio = 47,
	enableKeyTrack = 48,
	engineMode = 20,
	unisonDetune = 21,
	arpOn = 0,
	arpScale = 1,
	arpMode = 2,
	arpTimeMsec = 3
};

	// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)


PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
    PluginCore();

	/** Destructor: empty in default version */
    virtual ~PluginCore(){}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);

	// --- preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	// --- uncomment and override this for buffer processing; see base class implementation for
	//     help on breaking up buffers and getting info from processBufferInfo
	//virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here

	SynthEngine synthEngine;
	void updateParameters();

	ICustomView* bankAndWaveGroup_0 = nullptr;

	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	double masterPitchBend = 0.0;
	double masterTune = 0.0;
	double masterVolume_dB = 0.0;
	double lfo1Frequency = 0.0;
	double lfo1DelayTime_mSec = 0.0;
	double lfo1RampTime_mSec = 0.0;
	double lfo2Frequency = 0.0;
	double waveShapeX = 0.0;
	double waveShapeY = 0.0;
	double EG1_attackTime_mSec = 0.0;
	double EG1_decayTime_mSec = 0.0;
	double EG1_sustainLevel = 0.0;
	double EG1_releaseTime_mSec = 0.0;
	double EG1_holdTime_mSec = 0.0;
	double EG1_delayTime_mSec = 0.0;
	double startVal = 0.0;
	double endVal = 0.0;
	double filterFc = 0.0;
	double filterQ = 0.0;
	double lfo1Amp = 0.0;
	double eg1Amp = 0.0;
	double osc1FoAmp = 0.0;
	double osc2FoAmp = 0.0;
	double keyTrackRatio = 0.0;
	int unisonDetune = 0;
	double arpTimeMsec = 0.0;

	// --- Discrete Plugin Variables 
	int lfo1Waveform = 0;
	enum class lfo1WaveformEnum { Triangle,Sin,Saw,RSH,QRSH,Noise,QRNoise };	// to compare: if(compareEnumToInt(lfo1WaveformEnum::Triangle, lfo1Waveform)) etc... 

	int lfo1Mode = 0;
	enum class lfo1ModeEnum { Sync,One_Shot,Free_Run };	// to compare: if(compareEnumToInt(lfo1ModeEnum::Sync, lfo1Mode)) etc... 

	int osc1_waveForm = 0;
	enum class osc1_waveFormEnum { wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31 };	// to compare: if(compareEnumToInt(osc1_waveFormEnum::wave0, osc1_waveForm)) etc... 

	int osc1_BankIndex = 0;
	enum class osc1_BankIndexEnum { Bank_0,Bank_1,Bank_2,Bank_3 };	// to compare: if(compareEnumToInt(osc1_BankIndexEnum::Bank_0, osc1_BankIndex)) etc... 

	int osc2_waveForm = 0;
	enum class osc2_waveFormEnum { wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31 };	// to compare: if(compareEnumToInt(osc2_waveFormEnum::wave0, osc2_waveForm)) etc... 

	int osc2_BankIndex = 0;
	enum class osc2_BankIndexEnum { bank0,bank1,bank2,bank3 };	// to compare: if(compareEnumToInt(osc2_BankIndexEnum::bank0, osc2_BankIndex)) etc... 

	int osc3_waveForm = 0;
	enum class osc3_waveFormEnum { wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31 };	// to compare: if(compareEnumToInt(osc3_waveFormEnum::wave0, osc3_waveForm)) etc... 

	int osc3_BankIndex = 0;
	enum class osc3_BankIndexEnum { bank0,bank1,bank2,bank3 };	// to compare: if(compareEnumToInt(osc3_BankIndexEnum::bank0, osc3_BankIndex)) etc... 

	int osc4_waveForm = 0;
	enum class osc4_waveFormEnum { wave0,wave1,wave2,wave3,wave4,wave5,wave6,wave7,wave8,wave9,wave10,wave11,wave12,wave13,wave14,wave15,wave16,wave17,wave18,wave19,wave20,wave21,wave22,wave23,wave24,wave25,wave26,wave27,wave28,wave29,wave30,wave31 };	// to compare: if(compareEnumToInt(osc4_waveFormEnum::wave0, osc4_waveForm)) etc... 

	int osc4_BankIndex = 0;
	enum class osc4_BankIndexEnum { bank0,bank1,bank2,bank3 };	// to compare: if(compareEnumToInt(osc4_BankIndexEnum::bank0, osc4_BankIndex)) etc... 

	int EG1_mode = 0;
	enum class EG1_modeEnum { ADSR,AHDSR,AHR,AHR_RT };	// to compare: if(compareEnumToInt(EG1_modeEnum::ADSR, EG1_mode)) etc... 

	int manualRT = 0;
	enum class manualRTEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(manualRTEnum::SWITCH_OFF, manualRT)) etc... 

	int lfo1toOsc1 = 0;
	enum class lfo1toOsc1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(lfo1toOsc1Enum::SWITCH_OFF, lfo1toOsc1)) etc... 

	int lfo1toOsc2 = 0;
	enum class lfo1toOsc2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(lfo1toOsc2Enum::SWITCH_OFF, lfo1toOsc2)) etc... 

	int eg1toOsc1 = 0;
	enum class eg1toOsc1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(eg1toOsc1Enum::SWITCH_OFF, eg1toOsc1)) etc... 

	int eg1toOsc2 = 0;
	enum class eg1toOsc2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(eg1toOsc2Enum::SWITCH_OFF, eg1toOsc2)) etc... 

	int enableKeyTrack = 0;
	enum class enableKeyTrackEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(enableKeyTrackEnum::SWITCH_OFF, enableKeyTrack)) etc... 

	int engineMode = 0;
	enum class engineModeEnum { MONO,POLY,UNISON };	// to compare: if(compareEnumToInt(engineModeEnum::MONO, engineMode)) etc... 

	int arpOn = 0;
	enum class arpOnEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(arpOnEnum::SWITCH_OFF, arpOn)) etc... 

	int arpScale = 0;
	enum class arpScaleEnum { MAJOR,MINOR,DIMINISHED,AUGMENTED };	// to compare: if(compareEnumToInt(arpScaleEnum::MAJOR, arpScale)) etc... 

	int arpMode = 0;
	enum class arpModeEnum { UP_CYCLICAL,UP_DOWN,DOWN_CYCLICAL,DOWN_UP };	// to compare: if(compareEnumToInt(arpModeEnum::UP_CYCLICAL, arpMode)) etc... 

	// **--0x1A7F--**
    // --- end member variables

public:
    /** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
    static const char* getPluginBundleName();

    /** static description: name

	\return name as a const char*
	*/
    static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();

};




#endif /* defined(__pluginCore_h__) */
