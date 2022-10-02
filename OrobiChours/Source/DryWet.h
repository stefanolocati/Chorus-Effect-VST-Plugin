#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class DryWet
{
public:
	DryWet() {}
	~DryWet() {}

	void prepareToPlay(double sr, int maxNumSamples);

	//releaseResources serve per rilasciare memoria
	void releaseResources();

	//il buffer va solo letto quindi va messo come const
	void setDry(const AudioBuffer<float>& buffer);

	//su questo buffer invece bisognerà scrivere, quindi niente const
	void merge(AudioBuffer<float>& buffer);

	void setMix(const float newValue);

private:

	float drywetter = 0.5;
	AudioBuffer<float> dry;

	SmoothedValue<float, ValueSmoothingTypes::Linear> dryLevel;
	SmoothedValue<float, ValueSmoothingTypes::Linear> wetLevel;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWet)
};

