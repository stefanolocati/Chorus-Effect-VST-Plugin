#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Ctrl
{
public:
	Ctrl() {}
	~Ctrl() {}

	void prepareToPlay(double sr);

	void setParameter(float newValue);

	void setAmount(float newValue);

	void processBlock(AudioBuffer<float>& buffer, const int numSamples);

private:

	SmoothedValue<float, ValueSmoothingTypes::Linear> amount;
	SmoothedValue<float, ValueSmoothingTypes::Linear> parameter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Ctrl)
};