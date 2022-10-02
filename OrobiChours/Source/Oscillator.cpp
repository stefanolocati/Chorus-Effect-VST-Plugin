#include "Oscillator.h"

void Oscillator::prepareToPlay(double sr)
{
	samplePeriod = 1.0 / sr;
	frequency.reset(sr, GLIDE_TIME);
}

void Oscillator::setFrequency(float newValue)
{
	frequency.setTargetValue(newValue);
}

void Oscillator::setWaveform(float newValue)
{
	waveform = roundToInt(newValue);
}

void Oscillator::setStereoPhase(float newValue)
{
	stereoPhase = newValue;
}

void Oscillator::getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples)
{
	const int numCh = buffer.getNumChannels();
	auto bufferData = buffer.getArrayOfWritePointers();

	for (int smp = 0; smp < numSamples; ++smp)
	{
		bufferData[0][smp] = getNextAudioSample(0);

		bufferData[1][smp] = getNextAudioSample(stereoPhase);

		phaseIncrement = frequency.getNextValue() * samplePeriod;
		currentPhase += phaseIncrement;
	}
}

float Oscillator::getNextAudioSample(const float delta)
{
	auto sampleValue = 0.0f;
	float phase = fmod(currentPhase + delta, 1);

	sampleValue = sin(MathConstants<float>::twoPi * phase);
	
	return sampleValue;
}
