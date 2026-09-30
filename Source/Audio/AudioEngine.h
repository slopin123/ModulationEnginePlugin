#pragma once
#include <JuceHeader.h>
#include "WavPlayer.h"
#include "Filters/SVF.h"
#include "ModulationSource.h"

class AudioEngine
{
public:
    AudioEngine();

    void load(const juce::File& file);

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void processBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    void play();
    void stop();

    void setGain(float newGain);
    void setCutoff(float newCutoff);

    ModulationSource& getModulationSource();
private:
    WavPlayer wavPlayer;
    SVF svf;
    ModulationSource modulationSource;
    

    std::atomic<float> gain{ 0.8f };
    std::atomic<float> cutoff{ 1000.0f };

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};