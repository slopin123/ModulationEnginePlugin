#pragma once

#include <JuceHeader.h>
#include "../Audio/AudioEngine.h"

class GlobalControls : public juce::Component
{
public:
    GlobalControls(AudioEngine& engine);

    void resized() override;

private:
    AudioEngine& audioEngine;


    juce::ToggleButton audioToggle{ "Play Audio" };

    juce::Slider gainSlider;
    juce::Label  gainLabel;

    juce::Slider cutoffSlider;

  //  std::atomic<float> gain{ 0.8f };
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlobalControls)
};