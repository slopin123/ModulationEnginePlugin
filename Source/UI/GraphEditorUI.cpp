#include "GraphEditorUI.h"

GraphEditorUI::GraphEditorUI(ModulationSource& graphProcessor)
    : modulationSource(graphProcessor)
{
    addAndMakeVisible(graphEditor);
    graphEditor.onTableUpdated = [this](const auto& table) {
        modulationSource.updateTable(table);
        };
    graphEditor.notifyTableUpdated();

    // Rate Slider
    rateSlider.setRange(0.1, 20.0, 0.1);
    rateSlider.setValue(1.0);
    rateSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    rateSlider.setTextValueSuffix(" Hz");
    rateSlider.onValueChange = [this] {
        modulationSource.setRateHz(static_cast<float>(rateSlider.getValue()));
        };
    addAndMakeVisible(rateSlider);

    // Loop Mode
    loopModeBox.addItem("Loop", 1);
    loopModeBox.addItem("One-Shot", 2);
    loopModeBox.addItem("Ping-Pong", 3);
    loopModeBox.setSelectedId(1);
    loopModeBox.onChange = [this] {
        int id = loopModeBox.getSelectedId();
        if (id == 1)      modulationSource.setLoopMode(ModulationSource::LoopMode::Loop);
        else if (id == 2) modulationSource.setLoopMode(ModulationSource::LoopMode::OneShot);
        else if (id == 3) modulationSource.setLoopMode(ModulationSource::LoopMode::PingPong);
        };
    addAndMakeVisible(loopModeBox);

    // Animation Button
    animationEnabledButton.setToggleState(false, juce::dontSendNotification);
    animationEnabledButton.onClick = [this] {
        animationEnabled = animationEnabledButton.getToggleState();
        graphEditor.setAudioPhase(modulationSource.getAudioPhase(), animationEnabled);
        };
    addAndMakeVisible(animationEnabledButton);

    // VBlank Animation Loop handled internally!
    vBlankAttachment = std::make_unique<juce::VBlankAttachment>(this, [this] {
        if (animationEnabled)
        {
            graphEditor.setAudioPhase(modulationSource.getAudioPhase(), animationEnabled);
        }
        });
}

void GraphEditorUI::resized()
{
    auto area = getLocalBounds().reduced(10);

    auto controlsArea = area.removeFromTop(80);

    auto rateArea = controlsArea.removeFromLeft(70);
    rateSlider.setBounds(rateArea.reduced(10));

    auto loopArea = controlsArea.removeFromLeft(100);
    loopModeBox.setBounds(loopArea.reduced(10));

    auto animationArea = controlsArea.removeFromLeft(100);
    animationEnabledButton.setBounds(animationArea.reduced(10));

    graphEditor.setBounds(area.reduced(10));
}