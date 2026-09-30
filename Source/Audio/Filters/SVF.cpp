#include "SVF.h"
#include <cmath>

// 2-pole State Variable Filter (SVF)

void SVF::prepareToPlay(double newSampleRate)
{
    sampleRate = static_cast<float>(newSampleRate);

    //20 ms smoothing
    smoothedCutoff.reset(sampleRate, 0.02);
    smoothedResonance.reset(sampleRate, 0.02);

    reset();
    updateCoefficient(smoothedCutoff.getTargetValue());
}

void SVF::setCutoff(float newCutoff)
{
    float clampedCutoff = juce::jlimit(20.0f, sampleRate * 0.45f, newCutoff);
    smoothedCutoff.setTargetValue(clampedCutoff);
}

void SVF::setResonance(float newResonance)
{
    float clampedResonance = juce::jlimit(0.0f, 1.0f, newResonance);
    smoothedResonance.setTargetValue(clampedResonance);
}

void SVF::updateCoefficient(float currentCutoff)
{
    coefficient = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * currentCutoff / sampleRate);
}

float SVF::processSample(float input, int channel)
{
    // Difference equation: y[n] = (2 - g*R - g^2) * y[n-1] - (1 - g*R) * y[n-2] + g^2 * x[n]
    channel = juce::jlimit(0, 1, channel);

    if (channel == 0)
    {
        if (smoothedCutoff.isSmoothing())
        {
            updateCoefficient(smoothedCutoff.getNextValue());
        }

        if (smoothedResonance.isSmoothing())
        {
            float currentRes = smoothedResonance.getNextValue();
            damping = 0.15f + 0.85f * (1.0f - currentRes);
        }
    }

    float high = input - low[channel] - damping * band[channel];   //  h[n] = x[n] - y[n-1] - R * b[n-1]
    float newBand = band[channel] + coefficient * high;            //  b[n] = b[n-1] + g * h[n]
    float newLow = low[channel] + coefficient * newBand;           //  y[n] = y[n-1] + g * b[n]

    band[channel] = newBand;
    low[channel] = newLow;

    return newLow;
}

void SVF::reset()
{
    low[0] = 0.0f;
    low[1] = 0.0f;

    band[0] = 0.0f;
    band[1] = 0.0f;

    smoothedCutoff.setCurrentAndTargetValue(smoothedCutoff.getTargetValue());
    smoothedResonance.setCurrentAndTargetValue(smoothedResonance.getTargetValue());
}