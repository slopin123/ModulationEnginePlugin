#pragma once
#include <JuceHeader.h>

class SVF
{
public:
    void prepareToPlay(double newSampleRate);

    void setCutoff(float newCutoff);
    void setResonance(float newResonance);

    float processSample(float input, int channel);

    void reset();

private:
    void updateCoefficient(float currentCutoff);

    float sampleRate{ 44100.0f };

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedCutoff{ 1000.0f };
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedResonance{ 0.0f };

    float damping = 1.0f;
    float coefficient = 0.0f;

    float low[2] = {};
    float band[2] = {};


};