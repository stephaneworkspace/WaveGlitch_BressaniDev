//
// Created by Stéphane on 14.08.23.
//

#include "WAVPlayer.h"

WAVPlayer::WAVPlayer(String inputFile, String folderConcat, float bpm) {
    bpmFile = bpm;
    folderComplete = folderConcat;

    // read info
    file = sf_open(inputFile.toStdString().c_str(), SFM_READ, &info);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + inputFile.toStdString());
    }

    // lecture
    formatManager.registerBasicFormats();

    auto file = juce::File(inputFile);
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader) {
        audioBuffer.setSize(reader->numChannels, (int) reader->lengthInSamples);
        reader->read(&audioBuffer, 0, (int)reader->lengthInSamples, 0, true, true);

        sampleRate = reader->sampleRate;

        setAudioChannels(0, reader->numChannels);
    }
}

WAVPlayer::~WAVPlayer() {
    sf_close(file);
    shutdownAudio();
}

void WAVPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    // ...
}

void WAVPlayer::releaseResources() {
    // ...
}

// Setter pour définir la fraction de mesure actuelle
void WAVPlayer::setBarFraction(BarFraction fraction) {
    currentBarFraction = fraction;
    // Calculer la taille en échantillons de la fraction de mesure
    double durationInSeconds = (60.0 / bpmFile) / static_cast<double>(fraction);
    barFractionSamples = (int) (sampleRate * durationInSeconds);
    barFractionPlayHead = 0;
}

void WAVPlayer::setPlaybackMode(PlaybackMode mode) {
    currentPlaybackMode = mode;
    if (mode == ADVANCED) {
        prepareAdvancedBuffer();
    }
}

void WAVPlayer::setWrite(bool sw) {
    sw_write = sw;
}

void WAVPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (!sw_write) {
        auto numInputChannels = audioBuffer.getNumChannels();
        auto numOutputChannels = bufferToFill.buffer->getNumChannels();

        int outputStartSample = bufferToFill.startSample;
        int outputNumSamples = bufferToFill.numSamples;

        bufferToFill.clearActiveBufferRegion();

        if (currentPlaybackMode == CLASSIC) {
            while (outputNumSamples > 0) {
                int samplesThisTime = jmin(outputNumSamples, barFractionSamples - barFractionPlayHead);
                for (auto channel = 0; channel < numOutputChannels; ++channel) {
                    bufferToFill.buffer->copyFrom(channel,
                                                  outputStartSample,
                                                  audioBuffer,
                                                  channel % numInputChannels,
                                                  barFractionPlayHead,
                                                  samplesThisTime);
                }
                outputStartSample += samplesThisTime;
                outputNumSamples -= samplesThisTime;
                barFractionPlayHead += samplesThisTime;

                if (barFractionPlayHead >= barFractionSamples) {
                    barFractionPlayHead = 0; // Retour au début de la fraction de mesure
                }
            }
        } else {
            // Copiez les données du preparedBuffer au lieu de audioBuffer
            while (outputNumSamples > 0) {
                int preparedBufferLength = preparedBuffer.getNumSamples();
                while (outputNumSamples > 0) {
                    int samplesAvailable = preparedBufferLength - barFractionPlayHead;
                    int samplesThisTime = jmin(outputNumSamples, samplesAvailable);
                    for (auto channel = 0; channel < numOutputChannels; ++channel) {
                        bufferToFill.buffer->copyFrom(channel,
                                                      outputStartSample,
                                                      preparedBuffer,
                                                      channel % numInputChannels,
                                                      barFractionPlayHead,
                                                      samplesThisTime);
                    }
                    outputStartSample += samplesThisTime;
                    outputNumSamples -= samplesThisTime;
                    barFractionPlayHead += samplesThisTime;

                    if (barFractionPlayHead >= preparedBufferLength) {
                        barFractionPlayHead = 0; // Bouclage du buffer
                    }
                }
            }
        }
    }
}

void WAVPlayer::prepareAdvancedBuffer() {
    // Cette méthode prépare le buffer pour le mode ADVANCED
    int totalSamples = audioBuffer.getNumSamples();
    int barLengthInSamples = barFractionSamples * currentBarFraction;
    int numBars = totalSamples / barLengthInSamples;

    preparedBuffer.setSize(audioBuffer.getNumChannels(), totalSamples + numBars * barFractionSamples);
    int writePosition = 0;

    for (int bar = 0; bar < numBars; bar++) {
        for (int fraction = 0; fraction <= currentBarFraction; fraction++) {
            int readPosition = bar * barLengthInSamples + barFractionSamples;
            if (readPosition + barFractionSamples <= audioBuffer.getNumSamples()) {
                //DBG("Lecture de position: " << readPosition << " écriture à la position: " << writePosition);
                for (auto channel = 0; channel < audioBuffer.getNumChannels(); channel++) {
                    preparedBuffer.copyFrom(channel, writePosition, audioBuffer, channel, readPosition, barFractionSamples);
                }
                writePosition += barFractionSamples;
            }
        }
    }

    if (sw_write) {
        // juce::Time now = juce::Time::getCurrentTime();
        // juce::String dateStr = now.formatted("%Y%m%d");
        String f = folderComplete; // + dateStr;
        File directory(f);
        if (!directory.exists()) {
            bool success = directory.createDirectory();
            if (!success) {
                DBG("Erreur de création du répertoire: " + f);
                // TODO popup
            }
        }
        f += "/";
        switch (currentBarFraction) {
            case Bar:
                f += "1-1.wav";
                break;
            case Half:
                f += "1-2.wav";
                break;
            case Quarter:
                f += "1-4.wav";
                break;
            case Eighth:
                f += "1-8.wav";
                break;
            case Sixteenth:
                f += "1-16.wav";
                break;
            case ThirtyTwoSecond:
                f += "1-32.wav";
                break;
            case SixtyFourth:
                f += "1-64.wav";
                break;
            case OneHundredTwentyEighth:
                f += "1-128.wav";
                break;
        }
        juce::File outputFile(f);

        if (outputFile.exists())
            outputFile.deleteFile();

        juce::WavAudioFormat wavFormat;
        std::unique_ptr<juce::AudioFormatWriter> writer;
        writer.reset(wavFormat.createWriterFor(new juce::FileOutputStream(outputFile),
                                               sampleRate,
                                               preparedBuffer.getNumChannels(),
                                               getPcmBitDepth(), // bits par échantillon
                                               {}, 0));
        if (writer != nullptr) {
            writer->writeFromAudioSampleBuffer(preparedBuffer, 0, preparedBuffer.getNumSamples());
        }
        writer.reset();
    }
}

int WAVPlayer::getPcmBitDepth() const {
    if ((info.format & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
        // Ce n'est pas un fichier WAV, donc on ne peut pas déterminer la profondeur de bit PCM
        return -1;
    }

    switch (info.format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_16:
            return 16;
        case SF_FORMAT_PCM_24:
            return 24;
        case SF_FORMAT_PCM_32:
            return 32;
        default:
            // Ce n'est pas un format PCM, donc on ne peut pas déterminer la profondeur de bit
            return -1;
    }
}