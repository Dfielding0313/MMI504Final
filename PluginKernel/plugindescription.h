// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugindescription.h
//
/**
    \file   plugindescription.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  plugin description strings, codes, unique identifiers and
    		unique cocoa view names
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- STEP 1 for all plugins: change these variables accordingly

// --- FOR AU PLUGINS ONLY ------------------------------------------------- //
//     try to make this as unique as possible (Cocoa has a flat namespace)
//     here I'm appending the VST3 FUID string (unique but must be generated for each plugin)
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_E78C2E1E446A4EEEB0C7647C76139479
#define AU_COCOA_VIEW_NAME AUCocoaView_E78C2E1E446A4EEEB0C7647C76139479

// --- MacOS Bundle Identifiers
//     NOTE: these ID strings must EXACTLY MATCH the PRODUCT_BUNDLE_IDENTIFIER setting in Build Settings for the Plugin Target
const char* kAAXBundleID = "rackafxuser.aax.Synthtest.bundleID";
const char* kVST3BundleID = "rackafxuser.vst3.Synthtest.bundleID";

// --- For AU: make sure BOTH the Plugin Target and the CocoaUI Target PRODUCT_BUNDLE_IDENTIFIER settings match these strings
const char* kAUBundleID = "rackafxuser.au.Synthtest.bundleID";

// --- plugin NAME
/* AU only: this MUST EXACLTY match the Product Name in Build Settings for BOTH the CocoaUI and your Plugin Targets! */
const char* kPluginName = "Synth test";     // 31 chars max for AAX
const char* kShortPluginName = "Synth test";    // 15 chars max for AAX
const char* kAUBundleName = "Synth test";

// --- type of plugin
const pluginType kPluginType = pluginType::kSynthPlugin;

// --- vendor info
const char* kVendorName = "RackAFX User";
const char* kVendorURL = "www.yourcompany.com";
const char* kVendorEmail = "help@yourcompany.com";

// --- VST3 & AAX only
const int32_t kFourCharCode = 'lklk'; /// must be unique for each plugin in your company

// --- VST3 specific, see www.willpirkle.com/forum/ for information on generating FUIDs
//     use GUIDGEN.exe from compiler/tools (Win) or UUID Generator (free Mac App)
const char* kVSTFUID = "{E78C2E1E-446A-4EEE-B0C7-647C76139479}"; /// NOTE: you need the enclosing { }

// --- AAX specific
const int32_t kManufacturerID = 'VNDR';
const int32_t kAAXProductID = 'lklj';

// --- Plugin Options
const bool kWantSidechain = false;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = 0;

#endif

