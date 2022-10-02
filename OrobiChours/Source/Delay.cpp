#include "Delay.h"

void Delay::prepareToPlay(double sampleRate, int maxNumSamples)
{
	sr = sampleRate;
	memorySize = roundToInt((2*MAX_DELAY_TIME) * sr) + maxNumSamples;
	delayMemory.setSize(2, memorySize);
}

void Delay::releaseResources()
{
	delayMemory.setSize(0, 0);
	memorySize = 0;
}

void Delay::processBlock(AudioBuffer<float>& buffer, AudioBuffer<float>& modulationSignal)
{
	store(buffer);
	moveDelayedTo(buffer, modulationSignal);
	updateWriteHead(buffer.getNumSamples());
}

void Delay::store(const AudioBuffer<float>& buffer)
{
	const auto numInputSamples = buffer.getNumSamples();
	const auto numChannels = buffer.getNumChannels();

	for (int ch = 0; ch < numChannels; ++ch)
	{
		if (writeIndex + numInputSamples <= memorySize)
			delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, numInputSamples);
		else
		{
			const int fittingSamples = memorySize - writeIndex;
			const int remainingSamples = numInputSamples - fittingSamples;

			delayMemory.copyFrom(ch, writeIndex, buffer, ch, 0, fittingSamples);
			delayMemory.copyFrom(ch, 0, buffer, ch, fittingSamples, remainingSamples);
		}
	}
}

//moveDelayedTo presa da modDelay
void Delay::moveDelayedTo(AudioBuffer<float>& buffer, AudioBuffer<float>& modulationSignal)
{
	const auto numOutputSamples = buffer.getNumSamples();
	const auto numChannels = buffer.getNumChannels();
	const auto numModChannels = modulationSignal.getNumChannels();

	auto** outputData = buffer.getArrayOfWritePointers();
	auto** delayArray = delayMemory.getArrayOfWritePointers();
	auto** modArray = modulationSignal.getArrayOfReadPointers();

	for (int smp = 0; smp < numOutputSamples; ++smp)
	{
		for (int ch = 0; ch < numChannels; ++ch)
		{
			auto dt = modArray[jmin(ch, numModChannels - 1)][smp];

			auto actualWriteIndex = (writeIndex + smp) % memorySize;
			auto actualReadIndex = memorySize + actualWriteIndex - (dt * sr);

			auto integerPart = (int)actualReadIndex;
			auto fractionalPart = actualReadIndex - integerPart;

			auto A = (integerPart + memorySize) % memorySize;
			auto B = (A + 1) % memorySize;

			//uso l'interpolazione lineare
			auto sampleValue = delayArray[ch][A] * (1.0 - fractionalPart) + delayArray[ch][B] * fractionalPart;

			outputData[ch][smp] = sampleValue;
		}
	}
}

void Delay::updateWriteHead(int leap)
{
	writeIndex += leap;
	writeIndex %= memorySize;
}
