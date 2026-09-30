#include "GlobalControls.h"

GlobalControls::GlobalControls(AudioEngine& engine)
    : audioEngine(engine)
{

    //----------
    addAndMakeVisible(audioToggle);

    audioToggle.setToggleState(false, juce::dontSendNotification);

    audioToggle.onClick = [this]
        {
            if (audioToggle.getToggleState())
                audioEngine.play();
            else
                audioEngine.stop();
        };

    //----------
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
    gainSlider.setRange(-60.0, 0.0, 0.1);
    gainSlider.setValue(-12.0);
    gainSlider.setTextValueSuffix(" dB");
    addAndMakeVisible(gainSlider);

   // gain.store(juce::Decibels::decibelsToGain((float)gainSlider.getValue()));

    gainSlider.onValueChange = [this]
        {
            const float linearGain = juce::Decibels::decibelsToGain(static_cast<float>(gainSlider.getValue()));

            audioEngine.setGain(linearGain);
        };

    //----------
    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
    cutoffSlider.setRange(20.0, 20000.0, 1.0);
    cutoffSlider.setValue(10000.0);
    cutoffSlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(cutoffSlider);

    cutoffSlider.onValueChange = [this]
        {
            audioEngine.setCutoff(static_cast<float>(cutoffSlider.getValue()));
        };

    //----------
}

//void GlobalControls::resized()
//{
//    audioToggle.setBounds(400, 5, 60, 30);
//    gainSlider.setBounds(20, 30, 60, 80);
//}

void GlobalControls::resized()
{
    auto area = getLocalBounds();

    auto gainArea = area.removeFromLeft(70);
    gainSlider.setBounds(gainArea.reduced(10));

    auto cutoffArea = area.removeFromLeft(70);
    cutoffSlider.setBounds(cutoffArea.reduced(10));

    auto buttonArea = area.removeFromLeft(100);
    audioToggle.setBounds(buttonArea.reduced(10));
}