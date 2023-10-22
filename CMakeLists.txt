cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(WaveGlitch_BressaniDev)

include(FetchContent)

FetchContent_Declare(
    libsndfile
    URL https://github.com/libsndfile/libsndfile/archive/refs/tags/1.2.2.tar.gz
)

FetchContent_Declare(
    rtmidi
    URL https://github.com/thestk/rtmidi/archive/refs/tags/6.0.0.tar.gz
)

FetchContent_Declare(
    juce
    URL https://github.com/juce-framework/JUCE/archive/refs/tags/7.0.6.tar.gz
)

FetchContent_MakeAvailable(libsndfile rtmidi juce)

# Si les bibliothèques déclarent des cibles, vous pouvez les utiliser dans target_link_libraries
add_executable(WaveGlitch_BressaniDev main.cpp WAVPlayer.cpp WAVPlayer.h)

if (APPLE)
target_link_libraries(WaveGlitch_BressaniDev PRIVATE
    sndfile
    rtmidi
    juce::juce_core
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_basics
    juce::juce_gui_extra
)
endif ()