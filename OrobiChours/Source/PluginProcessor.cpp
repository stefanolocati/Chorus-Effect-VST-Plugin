#include "PluginProcessor.h"
#include "Parameters.h"
#include "PluginEditor.h"

OrobiChorusAudioProcessor::OrobiChorusAudioProcessor()
      //si inizializza parameters come un array di parametri
    : parameters(*this, nullptr, "OrobiChorusParameters", {
            std::make_unique<AudioParameterFloat>(NAME_DW, "Dry/Wet", 0.0f, 1.0f, DEFAULT_DW),
            std::make_unique<AudioParameterFloat>(NAME_DT, "Delay time (s)", NormalisableRange<float>(MIN_DELAY_TIME, MAX_DELAY_TIME, 0.001f, 0.7f), DEFAULT_DT),
            std::make_unique<AudioParameterFloat>(NAME_FREQ, "Rate (Hz)", NormalisableRange<float>(0.1f, 5.0f, 0.01f, 0.3f), DEFAULT_FREQ),
            std::make_unique<AudioParameterChoice>(NAME_WF, "Waveform", StringArray{"Sin" , "altre waveforms"}, DEFAULT_WF),
            std::make_unique<AudioParameterFloat>(NAME_AM, "Mod amount (s)", NormalisableRange<float>(0.01f, MAX_DELAY_TIME / 2.0f, 0.001f), DEFAULT_AM),
            std::make_unique<AudioParameterFloat>(NAME_SP, "Stereo Phase", 0.0f, 0.99f, DEFAULT_SP)
        })
{
    parameters.addParameterListener(NAME_DW, this);
    drywetter.setMix(DEFAULT_DW);
    parameters.addParameterListener(NAME_DT, this);
    parameters.addParameterListener(NAME_FREQ, this);
    parameters.addParameterListener(NAME_WF, this);
    parameters.addParameterListener(NAME_SP, this);

    LFO.setFrequency(DEFAULT_FREQ);
    LFO.setWaveform(DEFAULT_WF);
    LFO.setStereoPhase(DEFAULT_SP);

    parameters.addParameterListener(NAME_AM, this);
    delayTime.setAmount(DEFAULT_AM);
    delayTime.setParameter(DEFAULT_DT);
}

OrobiChorusAudioProcessor::~OrobiChorusAudioProcessor()
{
}

void OrobiChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    delay.prepareToPlay(sampleRate, samplesPerBlock);
    LFO.prepareToPlay(sampleRate);
    delayTime.prepareToPlay(sampleRate);
    modulationSignal.setSize(2, samplesPerBlock);
}

void OrobiChorusAudioProcessor::releaseResources()
{
    drywetter.releaseResources();
    delay.releaseResources();
    modulationSignal.setSize(0, 0);
}

void OrobiChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto numSamples = buffer.getNumSamples();
    auto totalNumInputChannels  = getTotalNumInputChannels();

    //generazione di una modulante
    LFO.getNextAudioBlock(modulationSignal, numSamples);

    //scalo la modulante
    delayTime.processBlock(modulationSignal, numSamples);

    //salvo il segnale in input pulito
    drywetter.setDry(buffer);

    //applico delay
    delay.processBlock(buffer, modulationSignal);

    // Miscelo il segnale pulito salvato in drywetter con quello processato da delay
    drywetter.merge(buffer);
}

void OrobiChorusAudioProcessor::parameterChanged(const String& paramID, float newvalue)
{
    if (paramID == NAME_DW)
    {
        drywetter.setMix(newvalue);
    }

    if (paramID == NAME_FREQ)
    {
        LFO.setFrequency(newvalue);
    }

    if (paramID == NAME_SP)
    {
        LFO.setStereoPhase(newvalue);
    }

    if (paramID == NAME_DT)
    {
        delayTime.setParameter(newvalue);
    }

    if (paramID == NAME_AM)
    {
        delayTime.setAmount(newvalue);
    }
}

juce::AudioProcessorEditor* OrobiChorusAudioProcessor::createEditor()
{
    return new PluginEditor(*this, parameters);
}

void OrobiChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void OrobiChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrobiChorusAudioProcessor();
}