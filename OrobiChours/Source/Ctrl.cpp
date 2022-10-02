#include "Ctrl.h"

void Ctrl::prepareToPlay(double sr)
{
	parameter.reset(sr, PM_SMOOTHING_TIME);
	amount.reset(sr, PM_SMOOTHING_TIME);
}

void Ctrl::setParameter(float newValue)
{
	parameter.setTargetValue(newValue);
}

void Ctrl::setAmount(float newValue)
{
	amount.setTargetValue(newValue);
}

void Ctrl::processBlock(AudioBuffer<float>& buffer, const int numSamples)
{
	auto modArray = buffer.getArrayOfWritePointers();
	auto numCh = buffer.getNumChannels();

	// scalo modulante tra 0 e 1
	for (int ch = 0; ch < numCh; ++ch)
	{
		FloatVectorOperations::add(modArray[ch], 1.0, numSamples);
		FloatVectorOperations::multiply(modArray[ch], 0.5f, numSamples);
	}

	// Scalo la modulante tra 0 e la modulaziuone massima desiderata
	amount.applyGain(buffer, numSamples);

	// Sommo alla modulante il tempo di delay proveniente dal parameter
	if (parameter.isSmoothing())
		for (int smp = 0; smp < numSamples; ++smp)
		{
			const auto param = parameter.getNextValue();

			for (int ch = 0; ch < numCh; ++ch)
				modArray[ch][smp] += param;
		}
	else
		for (int ch = 0; ch < numCh; ++ch)
			FloatVectorOperations::add(modArray[ch], parameter.getCurrentValue(), numSamples);

	// controllo di essere entro il tempo di delay massimo
	for (int ch = 0; ch < numCh; ++ch)
		FloatVectorOperations::min(modArray[ch], modArray[ch], 2*MAX_DELAY_TIME, numSamples);


}
