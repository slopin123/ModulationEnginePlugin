#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : globalControls(audioEngine),
      graphEditorUI(audioEngine.getModulationSource())
{

    addAndMakeVisible(globalControls);
    addAndMakeVisible(graphEditorUI);

    auto wavfile = juce::File::getCurrentWorkingDirectory()
        .getParentDirectory()
        .getParentDirectory()
        .getChildFile("Source")
        .getChildFile("chordSample.wav");

    audioEngine.load(wavfile);


    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioEngine.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    audioEngine.processBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audioEngine.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    auto globalControlsArea = area.removeFromTop(120);
    globalControls.setBounds(globalControlsArea);

    auto graphEditorArea = area.removeFromTop(220);
    graphEditorUI.setBounds(graphEditorArea);
}
