#include "Parameters.h"
#include "PluginEditor.h"

PluginEditor::PluginEditor (OrobiChorusAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    //Imposto gli slider dando loro un range, uno stile, un colore in aggiunta a posizione e dimensioni
    //Questo viene fatto per ogni slider in egual modo, a differenza del knob del Dry/Wet che 
    //è, per mia scelta, più grande degli altri

    timeSlider.reset (new juce::Slider ("Delay Time"));
    addAndMakeVisible (timeSlider.get());
    timeSlider->setRange (0, 10, 0);
    timeSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    timeSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    timeSlider->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x008e989b));
    timeSlider->setBounds (50, 48, 150, 120);

    amountSlider.reset (new juce::Slider ("Depth"));
    addAndMakeVisible (amountSlider.get());
    amountSlider->setRange (0, 10, 0);
    amountSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    amountSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    amountSlider->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x008e989b));
    amountSlider->setBounds(224, 48, 150, 120);

    rateSlider.reset (new juce::Slider ("Rate"));
    addAndMakeVisible (rateSlider.get());
    rateSlider->setRange (0, 10, 0);
    rateSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    rateSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    rateSlider->setColour (juce::Slider::textBoxOutlineColourId, juce::Colour (0x008e989b));
    rateSlider->setBounds (400, 48, 150, 120);

    phaseSlider.reset(new juce::Slider("Stereo Phase"));
    addAndMakeVisible(phaseSlider.get());
    phaseSlider->setRange(0, 10, 0);
    phaseSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    phaseSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    phaseSlider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x008e989b));
    phaseSlider->setBounds(575, 48, 150, 120);

    dryWetSlider.reset(new juce::Slider("Dry / Wet"));
    addAndMakeVisible(dryWetSlider.get());
    dryWetSlider->setRange(0, 10, 0);
    dryWetSlider->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryWetSlider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    dryWetSlider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x008e989b));
    dryWetSlider->setBounds(270, 240, 250, 180);

    timeAttachment.reset(new SliderAttachment(valueTreeState, NAME_DT, *timeSlider));
    dryWetAttachment.reset(new SliderAttachment(valueTreeState, NAME_DW, *dryWetSlider));
    rateAttachment.reset(new SliderAttachment(valueTreeState, NAME_FREQ, *rateSlider));
    amountAttachment.reset(new SliderAttachment(valueTreeState, NAME_AM, *amountSlider));
    phaseAttachment.reset(new SliderAttachment(valueTreeState, NAME_SP, *phaseSlider));

    this->setLookAndFeel(&theme);
    
    //Imposto grandezza della finestra del plugin
    setSize (775, 450);
}

PluginEditor::~PluginEditor()
{
    //Distruttore con tutti gli slider impostati uguali a null pointer

    timeAttachment.reset();
    dryWetAttachment.reset();
    rateAttachment.reset();
    amountAttachment.reset();
    phaseAttachment.reset();

    this->setLookAndFeel(nullptr);

    timeSlider = nullptr;
    dryWetSlider = nullptr;
    amountSlider = nullptr;
    rateSlider = nullptr;
    phaseSlider = nullptr;
}

void PluginEditor::paint (juce::Graphics& g)
{
    {
        //Imposto il colore di fondo del plugin con un gradiente che sfuma tra i due colori
        int x = 0, y = 0, width = 800, height = 500;
        juce::Colour fillColour1 = juce::Colour (16, 179, 179), fillColour2 = juce::Colour (27, 166, 166);
        g.setGradientFill (juce::ColourGradient (fillColour1,280.0f - 0.0f + x, 8.0f - 0.0f + y,fillColour2, 264.0f - 0.0f + x,368.0f - 0.0f + y,false));
        g.fillRect (x, y, width, height);
    }

    //Imposto le etichette posizionate sopra ogni knob
    {
        int x = 29, y = 21, width = 200, height = 30;
        juce::String text (TRANS("Delay Time"));
        juce::Colour fillColour = juce::Colours::white;
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 197, y = 21, width = 200, height = 30;
        juce::String text (TRANS("Depth"));
        juce::Colour fillColour = juce::Colours::white;
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 296, y = 200, width = 200, height = 30;
        juce::String text (TRANS("Dry / Wet"));
        juce::Colour fillColour = juce::Colours::white;

        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 373, y = 21, width = 200, height = 30;
        juce::String text (TRANS("Rate"));
        juce::Colour fillColour = juce::Colours::white;

        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 550, y = 21, width = 200, height = 30;
        juce::String text (TRANS("Stereo Phase"));
        juce::Colour fillColour = juce::Colours::white;

        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }
}

void PluginEditor::resized()
{
}