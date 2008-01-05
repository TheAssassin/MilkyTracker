/*
 *  SectionHDRecorder.h
 *  MilkyTracker
 *
 *  Created by Peter Barth on 26.10.05.
 *  Copyright 2005 milkytracker.net, All rights reserved.
 *
 */

#ifndef SECTIONHDRECORDER__H
#define SECTIONHDRECORDER__H

#include "BasicTypes.h"
#include "Event.h"
#include "SectionUpperLeft.h"

class PPControl;
class Tracker;
class PPCheckBox;

class SectionHDRecorder : public SectionUpperLeft
{
private:
	enum RecorderModes
	{
		RecorderModeToFile,
		RecorderModeToSample
	};

	RecorderModes recorderMode;
	pp_int32 fromOrder;
	pp_int32 toOrder;
	pp_int32 mixerVolume;
	pp_uint32 resampler;

	pp_int32 insIndex;
	pp_int32 smpIndex;
	
	PPSystemString currentFileName;

	class RespondMessageBox* respondMessageBox;

	bool getSettingsRamping();
	void setSettingsRamping(bool b);
	
	pp_uint32 getSettingsResampler();
	void setSettingsResampler(pp_uint32 resampler);
	
	bool getSettingsAllowMuting();
	void setSettingsAllowMuting(bool b);
	
	pp_int32 getSettingsFrequency();
	void setSettingsFrequency(pp_int32 freq);
	
	pp_int32 getSettingsMixerVolume() { return mixerVolume; }
	void setSettingsMixerVolume(pp_int32 vol) { mixerVolume = vol; }
	
	pp_int32 getSettingsMixerShift();
	void setSettingsMixerShift(pp_int32 shift);

	void validate();

public:
	SectionHDRecorder(Tracker& tracker);
	virtual ~SectionHDRecorder();

	// Derived from SectionAbstract
	virtual pp_int32 handleEvent(PPObject* sender, PPEvent* event);
	
	virtual void init() { SectionUpperLeft::init(); }
	virtual void init(pp_int32 x, pp_int32 y);
	virtual void show(bool bShow); 
	virtual void update(bool repaint = true);

	virtual void notifyInstrumentSelect(pp_int32 index);
	virtual void notifySampleSelect(pp_int32 index);
	
	void exportWAVAs(const PPSystemString& fileName);
	void exportWAVWithPanel(const PPSystemString& defaultFileName);
	void exportWAVAsFileName(const PPSystemString& fileName);
	
	void exportWAVAsSample();
	
	void getPeakLevel();
	
	void resetCurrentFileName();
	void setCurrentFileName(const PPSystemString& fileName);
	
	void adjustOrders();

private:
	// Message box with list of resampler
	void showResamplerMessageBox();

	void storeResampler(pp_uint32 resampler);

	class MessageBoxResponderHDRec* messageBoxResponder;
	// Responder should be friend
	friend class MessageBoxResponderHDRec;	
	
	friend class Tracker;
};

#endif