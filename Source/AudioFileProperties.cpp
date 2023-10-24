//
// Created by Stéphane on 04.08.23.
//

#include "AudioFileProperties.h"

AudioFileProperties::AudioFileProperties(const std::string& filePath) : filePath(filePath) {
    file = sf_open(filePath.c_str(), SFM_READ, &info);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
}

AudioFileProperties::~AudioFileProperties() {
    sf_close(file);
}

int AudioFileProperties::getChannels() const {
    return info.channels;
}

int AudioFileProperties::getSampleRate() const {
    return info.samplerate;
}

double AudioFileProperties::getDuration() const {
    return static_cast<double>(getFrames()) / getSampleRate();
}

std::string AudioFileProperties::getFilePath() const {
    return filePath;
}

std::string AudioFileProperties::getFilePathWithoutExt() const {
    std::regex regex("\\.wav$", std::regex_constants::icase);
    return std::regex_replace(filePath, regex, "");
}

int64_t AudioFileProperties::getFrames() const {
    return info.frames;
}

int AudioFileProperties::getPcmBitDepth() const {
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