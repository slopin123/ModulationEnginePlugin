#include "WavPlayer.h"

WavPlayer::WavPlayer()
{
    formatManager.registerBasicFormats();
}

void WavPlayer::load(const juce::File& file)
{
    auto* reader = formatManager.createReaderFor(file);

    if (reader != nullptr)
    {
        auto newSource =
            std::make_unique<juce::AudioFormatReaderSource>(reader, true);

        newSource->setLooping(true);

        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);

        readerSource = std::move(newSource);
    }
}

void WavPlayer::play()
{
    transportSource.start();
}

void WavPlayer::stop()
{
    transportSource.stop();
}

void WavPlayer::prepareToPlay(
    int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void WavPlayer::processBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
}

void WavPlayer::releaseResources()
{
    transportSource.releaseResources();
}