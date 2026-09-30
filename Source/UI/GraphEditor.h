#pragma once
#include <JuceHeader.h>
#include <array>
#include <cmath>
#include <vector>

struct GraphPoint
{
    juce::Point<float> pos;
    float tension = 0.0f;
};

class GraphEditor : public juce::Component
{
public:
    GraphEditor();
    ~GraphEditor() override;

    // Callback to push updated 512-point curve data directly to GraphMain
    std::function<void(const std::array<float, 512>&)> onTableUpdated;

    void notifyTableUpdated();

    // Synchronize playhead animation directly from GraphMain's audio phase
    void setAudioPhase(float phase, bool animationEnabled) noexcept;

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

    // Evaluates Bezier curve value for rendering and table generation
    float getSampleAtPhase(float phase) const;

private:

    juce::OpenGLContext openGLContext; //gpu acceleration

    juce::Point<float> pointToScreen(juce::Point<float> point) const;
    juce::Point<float> screenToPoint(juce::Point<float> position) const;
    int findPointAt(juce::Point<float> position) const;
    juce::Point<float> getTensionHandlePosition(int segmentIndex) const;
    int findTensionHandleAt(juce::Point<float> position) const;

    std::vector<GraphPoint> points{
        { { 0.0f, 0.0f }, 0.0f },
        { { 1.0f, 1.0f }, 0.0f }
    };

    int selectedPoint = -1;
    int selectedTension = -1;
    float dragStartMouseY = 0.0f;
    float dragStartTension = 0.0f;

    // Visual animation state driven solely by the audio thread
    float currentAudioPhase = 0.0f;
    bool active = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphEditor)
};