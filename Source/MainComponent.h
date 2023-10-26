//
// Created by Stéphane on 05.08.23.
//
#pragma once
#include <string>
#include <limits.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Misc.h"
#include "MyObjCWrapper.h"
#include "CustomLookAndFeel.h"
#include "AudioFileProperties.h"
#include "WAVPlayer.h"

using namespace std;
using namespace juce;
namespace fsys = std::filesystem;

class MainComponent: public Component,
                     public KeyListener,
                     public TextEditor::Listener,
                     public DragAndDropContainer,
                     public FileDragAndDropTarget,
                     private Timer {
public:
    MainComponent();
    ~MainComponent() override;
    void paint(Graphics &g) override;
    void update();
    void resized() override;
    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped(const StringArray &files, int x, int y) override;
    bool keyPressed(const KeyPress &key, Component *originatingComponent) override;
    void textEditorTextChanged(TextEditor &editor) override;
    void rootFolderSelectButtonClicked();
    void fileSelectButtonClicked();
    void processingButtonClicked();
    void aboutButtonClicked();
private:
    void timerCallback() override;
    void loadPNG(const String& path);
    unique_ptr<Drawable> loadSVG(const String &path);
    bool createDirectories();
    void stop();

    CustomLookAndFeel customLookAndFeel;

    string appName;
    string appVersion;
    Label fileLabel;
    Label channelsLabel;
    Label sampleRateLabel;
    Label durationLabel;
    Label rootLabel;
    Label bpmLabel;
    TextEditor bpmEditor;
    Label toneLabel;
    ComboBox toneSelect;
    Label yearLabel;
    TextEditor yearEditor;
    Label songLabel;
    TextEditor songEditor;
    Label soundLabel;
    TextEditor soundEditor;
    TextButton closeButton;
    TextButton chooseButton;
    TextButton processingButton;
    TextButton fileSelectButton;
    TextButton aboutButton;
    juce::Image pngImage;
    unique_ptr<Drawable> svgDrawable1;
    unique_ptr<Drawable> svgDrawable2;
    unique_ptr<Drawable> svgDrawable3;
    float rotationAngle = 0.0f;

    PropertiesFile::Options options;
    std::unique_ptr<PropertiesFile> props;
    string fileWav;
    string rootFolder;
    String folderComplete;

    std::unique_ptr<WAVPlayer> player;

    bool swPlay;
    Label soundPreviewLabel;
    TextButton oneButton;
    TextButton twoButton;
    TextButton fourButton;
    TextButton eighthButton;
    TextButton sixteenthButton;
    TextButton thirtytwoButton;
    TextButton sixtyfourthButton;
    TextButton onehundredtwentyeigthButton;
    TextButton stoplisteningButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};