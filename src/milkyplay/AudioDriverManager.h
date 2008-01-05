/*
 *  AudioDriverManager.h
 *
 *  This class serves as provider and manager of different kind of
 *	AudioDriver instances. Different kinds of AudioDrivers can be
 *	requested on the same system. 
 *
 *  This code is copyright 2006 by Peter Barth (bape0016@hs-karlsruhe.de)
 *
 */
#ifndef __AUDIODRIVERMANAGER_H__
#define __AUDIODRIVERMANAGER_H__

#include "MilkyPlayTypes.h"

class AudioDriverBase;

class AudioDriverManager
{
private:
	AudioDriverBase**	driverList;
	mp_sint32			numDrivers;
	mp_sint32			defaultDriverIndex;
	mutable mp_sint32	enumerationIndex;
	
public:
	AudioDriverManager();
	~AudioDriverManager();

	/**
	 * Get the preferred audio driver for this system
	 * NULL is returned if either none is available or an alloc error occurred
	 * @return			audio driver instance
	 */
	AudioDriverBase* getPreferredAudioDriver();
	AudioDriverBase* getAudioDriverByName(const char* name);

	const char* getFirstDriverName() const;
	const char* getNextDriverName() const;
	
	mp_sint32 getPreferredAudioDriverBufferSize() const;
};

#endif