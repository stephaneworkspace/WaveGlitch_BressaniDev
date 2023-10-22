//
// Created by Stéphane on 14.08.23.
//

#ifndef WAVEGLITCHBRESSANIDEV_WAVPLAYER_H
#define WAVEGLITCHBRESSANIDEV_WAVPLAYER_H
#include <juce_core/juce_core.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h> // Incluez juce_gui_basics pour JUCEApplication

using namespace juce;

class WAVPlayer : public juce::AudioAppComponent {
public:
    enum BarFraction {
        Bar = 1,
        Half = 2,
        Quarter = 4,
        Eighth = 8,
        Sixteenth = 16,
        ThirtyTwoSecond = 32,
        SixtyFourth = 64,
        OneHundredTwentyEighth = 128
    };
    enum PlaybackMode {
        CLASSIC,
        ADVANCED
    };
    WAVPlayer();
    ~WAVPlayer() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void setBarFraction(BarFraction fraction);
    void setPlaybackMode(PlaybackMode mode);
    void setWrite(bool sw);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
private:
    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer audioBuffer;
    double sampleRate = 0.0;
    int barFractionSamples = 0;
    int barFractionPlayHead = 0;
    BarFraction currentBarFraction = Bar;

    PlaybackMode currentPlaybackMode = CLASSIC;
    juce::AudioSampleBuffer preparedBuffer;
    bool sw_write = false;

    void prepareAdvancedBuffer();
};



#endif //WAVEGLITCHBRESSANIDEV_WAVPLAYER_H
