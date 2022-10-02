#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Oscillator
{
public:
	Oscillator() {}
	~Oscillator() {}

	void prepareToPlay(double sr);

	void setFrequency(float newValue);

	void setWaveform(float newValue);

	void setStereoPhase(float newValue);

	void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples);

	float getNextAudioSample(const float delta);

private:

	int waveform = 0;
	float currentPhase = 0.0f;
	double samplePeriod = 1.0;
	float phaseIncrement = 0.0f;
	float stereoPhase = 0.0f;
	
	SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Oscillator)
};