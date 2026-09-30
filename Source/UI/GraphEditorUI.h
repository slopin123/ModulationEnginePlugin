#pragma once
#include <JuceHeader.h>
#include "GraphEditor.h"
#include "../Audio/ModulationSource.h"

class GraphEditorUI : public juce::Component
{
public:
    GraphEditorUI(ModulationSource& graphProcessor);

    void resized() override;

private:
    ModulationSource& modulationSource;
    GraphEditor graphEditor;

    juce::Slider rateSlider;
    juce::ComboBox loopModeBox;
    juce::ToggleButton animationEnabledButton;

    std::unique_ptr<juce::VBlankAttachment> vBlankAttachment;
    bool animationEnabled = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphEditorUI)
};