#pragma once
#include <JuceHeader.h>
#include "DryWet.h"
#include "Delay.h"
#include "Oscillator.h"
#include "Ctrl.h"

class OrobiChorusAudioProcessor  : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    OrobiChorusAudioProcessor();
    ~OrobiChorusAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    void parameterChanged(const String& paramID, float newvalue) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override   { return false; }
    bool producesMidi() const override  { return false; }
    bool isMidiEffect() const override  { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override    { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //Passaggio per gestire i parametri
    AudioProcessorValueTreeState parameters;

    DryWet drywetter;
    Delay delay;
    Oscillator LFO;
    Ctrl delayTime;
    AudioBuffer<float> modulationSignal;

    //passaggio per rilasciare memoria dopo che il programma viene chiuso
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrobiChorusAudioProcessor)
};
