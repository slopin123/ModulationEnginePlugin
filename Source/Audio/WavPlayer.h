#pragma once
#include <JuceHeader.h>

class WavPlayer
{
public:
    WavPlayer();

    void load(const juce::File& file);

    void play();
    void stop();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void processBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavPlayer)
};