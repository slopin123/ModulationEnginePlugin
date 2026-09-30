#pragma once
#include <JuceHeader.h>
#include "../Audio/WavPlayer.h"
#include "../UI/GlobalControls.h"
#include "../Audio/AudioEngine.h"
#include "../UI/GraphEditorUI.h"

class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    
    AudioEngine audioEngine;
    GlobalControls globalControls;
    GraphEditorUI graphEditorUI;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
