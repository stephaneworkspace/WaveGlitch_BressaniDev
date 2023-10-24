//
// Created by Stéphane on 04.08.23.
//

#pragma once
#include <sndfile.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <regex>
#include <sstream>
#include <iomanip>

class AudioFileProperties
{
public:
    AudioFileProperties(const std::string& filePath);
    ~AudioFileProperties();

    int getChannels() const;
    int getSampleRate() const;
    double getDuration() const;
    std::string getFilePath() const;
    std::string getFilePathWithoutExt() const;
    int64_t getFrames() const;
    // Retourne la profondeur de bit du fichier PCM ou -1 si le fichier n'est pas PCM
    int getPcmBitDepth() const;

private:
    SNDFILE* file;
    SF_INFO info;
    std::string filePath;
};
