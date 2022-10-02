#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MyTheme.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class PluginEditor  : public juce::AudioProcessorEditor
{
public:
    PluginEditor (OrobiChorusAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~PluginEditor() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    OrobiChorusAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;

    std::unique_ptr<SliderAttachment> timeAttachment;
    std::unique_ptr<SliderAttachment> dryWetAttachment;
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> amountAttachment;
    std::unique_ptr<SliderAttachment> phaseAttachment;

    MyLookAndFeel theme;

    std::unique_ptr<juce::Slider> timeSlider;
    std::unique_ptr<juce::Slider> dryWetSlider;
    std::unique_ptr<juce::Slider> amountSlider;
    std::unique_ptr<juce::Slider> rateSlider;
    std::unique_ptr<juce::Slider> phaseSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};