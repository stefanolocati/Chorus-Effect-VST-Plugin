#pragma once
#include <JuceHeader.h>
#include "Parameters.h"

class Delay
{
public:
	Delay() {}
	~Delay() {}

	void prepareToPlay(double sampleRate, int maxNumSamples);

	void releaseResources();

	void processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& delayTimeBuffer);

private:

	void store(const AudioBuffer<float>& buffer);

	virtual void moveDelayedTo(AudioBuffer<float>& buffer, AudioBuffer<float>& modulationSignal);

	void updateWriteHead(int leap);

	AudioBuffer<float> delayMemory;
	
	int memorySize = 0;
	int writeIndex= 0;
	double sr = 1;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Delay)
};