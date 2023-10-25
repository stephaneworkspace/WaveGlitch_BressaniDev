//
// Created by Stéphane on 05.08.23.
//
#include "MainComponent.h"

MainComponent::MainComponent() : fileLabel("", "No file loaded..."),
                                 rootLabel("", "No root folder defined..."),
                                 closeButton("Close"),
                                 chooseButton("Choose root export folder"),
                                 processingButton("Processing"),
                                 fileSelectButton("Select .wav file"),
                                 aboutButton("About"),
                                 bpmEditor(),
                                 toneSelect(),
                                 yearEditor(),
                                 songEditor(),
                                 soundEditor()
                                 {
    // Open .env
    Misc misc;
    appName = misc.getAppName();
    appVersion = misc.getAppVersion();

    // JUCE settings
    setSize(1024, 768);
    startTimerHz(60); // Change this value to control the frame rate
    setWantsKeyboardFocus(true); // Ceci permet à MainComponent d'obtenir le focus clavier

    loadPNG("./Assets/bg.png");
    // Loading SVGs
    svgDrawable1 = loadSVG("./Assets/logobg.svg");
    if (svgDrawable1 != nullptr) {
        addAndMakeVisible(svgDrawable1.get());
        svgDrawable1->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(600, 600), juce::RectanglePlacement::centred);
    }
    svgDrawable2 = loadSVG("./Assets/logo.svg");
    if (svgDrawable2 != nullptr) {
        svgDrawable2->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(300, 300), juce::RectanglePlacement::centred);
    }
    svgDrawable3 = loadSVG("./Assets/bressani.dev.svg");
    if (svgDrawable3 != nullptr) {
        svgDrawable3->replaceColour(juce::Colours::black, juce::Colours::white);
        addAndMakeVisible(svgDrawable3.get());
        svgDrawable3->setTransformToFit(getLocalBounds().toFloat().withSizeKeepingCentre(600, 600), juce::RectanglePlacement::centred);
    }

    bpmEditor.addListener(this);
    yearEditor.addListener(this);
    songEditor.addListener(this);
    soundEditor.addListener(this);
    Component::getTopLevelComponent()->addKeyListener(this);

    addAndMakeVisible(rootLabel);
    addAndMakeVisible(fileLabel);
    addAndMakeVisible(channelsLabel);
    addAndMakeVisible(sampleRateLabel);
    addAndMakeVisible(durationLabel);
    addAndMakeVisible(bpmLabel);
    addAndMakeVisible(bpmEditor);
    addAndMakeVisible(toneLabel);
    addAndMakeVisible(toneSelect);
    addAndMakeVisible(yearLabel);
    addAndMakeVisible(yearEditor);
    addAndMakeVisible(songLabel);
    addAndMakeVisible(songEditor);
    addAndMakeVisible(soundLabel);
    addAndMakeVisible(soundEditor);
    addAndMakeVisible(closeButton);
    addAndMakeVisible(chooseButton);
    addAndMakeVisible(processingButton);
    addAndMakeVisible(fileSelectButton);
    addAndMakeVisible(aboutButton);

    rootLabel.setBounds(10, getHeight() - 30, getWidth() - 20, 20);
    fileLabel.setBounds(10, getHeight() - 60, getWidth() - 20, 20);
    durationLabel.setText("", juce::dontSendNotification);
    durationLabel.setBounds(10, getHeight() - 90, getWidth() - 20, 20);
    sampleRateLabel.setText("", juce::dontSendNotification);
    sampleRateLabel.setBounds(10, getHeight() - 120, getWidth() - 20, 20);
    channelsLabel.setText("", juce::dontSendNotification);
    channelsLabel.setBounds(10, getHeight() - 150, getWidth() - 20, 20);

    bpmLabel.setFont (juce::Font (30.0f));
    bpmLabel.setText("Bpm: ", juce::dontSendNotification);
    bpmLabel.setJustificationType (juce::Justification::left);
    bpmEditor.setFont (juce::Font (30.0f));
    bpmEditor.setInputRestrictions(0, "0123456789."); // Autoriser uniquement les chiffres et le point.
    bpmEditor.setJustification(juce::Justification::centred);
    bpmEditor.setText("120.0");

    toneLabel.setFont (juce::Font (30.0f));
    toneLabel.setText("Tone: ", juce::dontSendNotification);
    toneLabel.setJustificationType (juce::Justification::left);
    toneSelect.setLookAndFeel(&customLookAndFeel);
    toneSelect.addItem("C", 1);
    toneSelect.addItem("C#/Db", 2);
    toneSelect.addItem("D", 3);
    toneSelect.addItem("D#/Eb", 4);
    toneSelect.addItem("E", 5);
    toneSelect.addItem("F", 6);
    toneSelect.addItem("F#/Gb", 7);
    toneSelect.addItem("G", 8);
    toneSelect.addItem("G#/Ab", 9);
    toneSelect.addItem("A", 10);
    toneSelect.addItem("A#/Bb", 11);
    toneSelect.addItem("B", 12);
    toneSelect.setSelectedId(1); // TODO .env

    yearLabel.setFont (juce::Font (30.0f));
    yearLabel.setText("Year: ", juce::dontSendNotification);
    yearLabel.setJustificationType (juce::Justification::left);
    yearEditor.setFont (juce::Font (30.0f));
    yearEditor.setInputRestrictions(4, "0123456789"); // Autoriser uniquement les chiffres et le point.
    yearEditor.setJustification(juce::Justification::centred);

    auto today = std::chrono::system_clock::now();
    auto days_since_epoch = std::chrono::duration_cast<std::chrono::days>(today.time_since_epoch());
    std::chrono::sys_days sd{days_since_epoch};
    auto ymd = std::chrono::year_month_day{sd};
    auto year = ymd.year();
    int yearInt = static_cast<int>(year);
    std::string yearString = std::to_string(yearInt);
    yearEditor.setText(yearString);

    songLabel.setFont (juce::Font (30.0f));
    songLabel.setText("Song: ", juce::dontSendNotification);
    songLabel.setJustificationType (juce::Justification::left);
    songEditor.setFont (juce::Font (30.0f));
    songEditor.setJustification(juce::Justification::left);
    songEditor.setText("");

    soundLabel.setFont (juce::Font (30.0f));
    soundLabel.setText("Sound: ", juce::dontSendNotification);
    soundLabel.setJustificationType (juce::Justification::left);
    soundEditor.setFont (juce::Font (30.0f));
    soundEditor.setJustification(juce::Justification::left);
    soundEditor.setText("");

    closeButton.setBounds(getWidth() - 100, 10, 80, 30);
    closeButton.onClick = [this] { juce::JUCEApplication::getInstance()->systemRequestedQuit(); };
    chooseButton.setBounds(getWidth() - 100, 50, 80, 30);
    chooseButton.onClick = [this] { rootFolderSelectButtonClicked(); };
    fileSelectButton.setButtonText("Select .wav file");
    fileSelectButton.setBounds(getWidth() - 100, 90, 80, 30);
    fileSelectButton.onClick = [this] { fileSelectButtonClicked(); };
    processingButton.setBounds(getWidth() - 100, 130, 80, 30);
    processingButton.onClick = [this] { processingButtonClicked(); };
    aboutButton.setButtonText("About");
    aboutButton.setBounds(getWidth() - 100, 170, 80, 30);
    aboutButton.onClick = [this] { aboutButtonClicked(); };

    oneButton.setButtonText("1/1");
    oneButton.setBounds(getWidth() - 100, 210, 80, 30);
    oneButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::Bar);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    twoButton.setButtonText("1/2");
    twoButton.setBounds(getWidth() - 100, 250, 80, 30);
    twoButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::Half);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    fourButton.setButtonText("1/4");
    fourButton.setBounds(getWidth() - 100, 290, 80, 30);
    fourButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::Quarter);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    eighthButton.setButtonText("1/8");
    eighthButton.setBounds(getWidth() - 100, 340, 80, 30);
    eighthButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::Eighth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    sixteenthButton.setButtonText("1/16");
    sixteenthButton.setBounds(getWidth() - 100, 390, 80, 30);
    sixteenthButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::Sixteenth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    thirtytwoButton.setButtonText("1/32");
    thirtytwoButton.setBounds(getWidth() - 100, 440, 80, 30);
    thirtytwoButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::ThirtyTwoSecond);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    sixtyfourthButton.setButtonText("1/64");
    sixtyfourthButton.setBounds(getWidth() - 100, 490, 80, 30);
    sixtyfourthButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::SixtyFourth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    onehundredtwentyeigthButton.setButtonText("1/128");
    onehundredtwentyeigthButton.setBounds(getWidth() - 100, 540, 80, 30);
    onehundredtwentyeigthButton.onClick = [this] {
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::OneHundredTwentyEighth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
    };
    stoplisteningButton.setButtonText("Stop listening");
    stoplisteningButton.setBounds(getWidth() - 100, 590, 80, 30);
    stoplisteningButton.onClick = [this] {
        player->shutdownAudio();
        oneButton.setVisible(false);
        twoButton.setVisible(false);
        fourButton.setVisible(false);
        eighthButton.setVisible(false);
        sixteenthButton.setVisible(false);
        thirtytwoButton.setVisible(false);
        sixtyfourthButton.setVisible(false);
        onehundredtwentyeigthButton.setVisible(false);
        stoplisteningButton.setVisible(false);

    };

    // Load datas
    options.applicationName = "WaveGlitch";
    options.folderName = "WaveGlitch";
    options.filenameSuffix = ".settings";
    options.osxLibrarySubFolder = "Preferences";
    props = std::make_unique<PropertiesFile>(options);
    // get values
    String getRoot = props->getValue("root");
    if (!getRoot.isEmpty()) {
        rootFolder = getRoot.toStdString();
        String folderPath = "Root folder: " + rootFolder;
        rootLabel.setText(folderPath, juce::dontSendNotification);
    }
    String getBpm = props->getValue("bpm");
    if (!getBpm.isEmpty()) {
        bpmEditor.setText(getBpm);
    }
    String getTone = props->getValue("tone");
    if (!getTone.isEmpty()) {
        toneSelect.setText(getTone);
    }
    String getYear = props->getValue("year");
    if (!getYear.isEmpty()) {
        yearEditor.setText(getYear);
    }
    String getSong = props->getValue("song");
    if (!getSong.isEmpty()) {
        songEditor.setText(getSong);
    }
}

MainComponent::~MainComponent() {
    Component::getTopLevelComponent()->removeKeyListener(this);
    toneSelect.setLookAndFeel(nullptr);
    player = nullptr;  // Détruit le WAVPlayer
    props.reset();
}

void MainComponent::paint(Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(
            juce::ResizableWindow::backgroundColourId));
    g.drawImageAt(pngImage, 0, 0); // Dessine l'image à la position souhaitée
    if (svgDrawable2)
    {
        svgDrawable2->draw(g, 1.0f,
                           juce::AffineTransform::rotation(rotationAngle,
                                                           getLocalBounds().getCentreX(),
                                                           getLocalBounds().getCentreY()));
    }
}

void MainComponent::update()
{
    rotationAngle += 0.01f; // Change this value to control the speed of rotation
    if (rotationAngle > juce::MathConstants<float>::twoPi)
        rotationAngle -= juce::MathConstants<float>::twoPi;
    repaint();
}

void MainComponent::resized()
{
    if (svgDrawable1 != nullptr) {
        svgDrawable1->setBounds(getLocalBounds().reduced(10));
    }
    //if (svgDrawable2 != nullptr) {
    //    svgDrawable2->setBounds(getLocalBounds().reduced(10));
    //}
    if (svgDrawable3 != nullptr) {
        svgDrawable3->setBounds(getLocalBounds().reduced(10));
    }
    rootLabel.setBounds(10, getHeight() - 30, getWidth() - 10, 20);
    fileLabel.setBounds(10, getHeight() - 60, getWidth() - 10, 20);
    channelsLabel.setBounds(10, getHeight() -90, getWidth() - 10, 20);
    sampleRateLabel.setBounds(10, getHeight() -120, getWidth() - 10, 20);
    durationLabel.setBounds(10, getHeight() -150, getWidth() - 10, 20);
    bpmLabel.setBounds(500, getHeight() - 240, 100, 40);
    bpmEditor.setBounds(600, getHeight() - 240, 150, 40);
    toneLabel.setBounds(760, getHeight() - 240, 100, 40);
    toneSelect.setBounds(850, getHeight() - 240, 150, 40);
    yearLabel.setBounds(500, getHeight() - 180, 100, 40);
    yearEditor.setBounds(600, getHeight() - 180, 150, 40);
    songLabel.setBounds(500, getHeight() - 120, 100, 40);
    songEditor.setBounds(600, getHeight() - 120, 400, 40);
    soundLabel.setBounds(500, getHeight() - 60, 100, 40);
    soundEditor.setBounds(600, getHeight() - 60, 400, 40);
    closeButton.setBounds(getWidth() - 100, 10, 80, 30);
    fileSelectButton.setBounds(getWidth() - 100, 50, 80, 30);
    processingButton.setBounds(getWidth() - 100, 90, 80, 30);
    aboutButton.setBounds(getWidth() - 100, 140, 80, 30);
}

bool MainComponent::isInterestedInFileDrag(const StringArray &files) {
    for(auto &file : files) {
        if (file.endsWith(".wav")) {
            return true;
        }
    }
    return false;
}

void MainComponent::filesDropped(const StringArray &files, int x, int y) {
    for(auto &file : files) {
        if (file.endsWith(".wav")) {
            // Vérification si le fichier est valide
            if (files.size() == 1)
            {
                File audioFile(files[0]);

                SF_INFO sfInfo;
                sfInfo.format = 0; // Laissez la libsndfile déterminer le format

                SNDFILE* f = sf_open(audioFile.getFullPathName().toRawUTF8(), SFM_READ, &sfInfo);

                if (f == nullptr)
                {
                    // Si le fichier n'est pas un fichier WAV valide, affichez un message d'erreur et réinitialisez le champ fileLabel.
                    AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                      "Invalid file",
                                                      "The selected file is not a valid WAV file.");
                    fileLabel.setText ("No file loaded...", dontSendNotification);
                    fileWav = "";
                    durationLabel.setText ("", dontSendNotification);
                    sampleRateLabel.setText ("", dontSendNotification);
                    channelsLabel.setText ("", dontSendNotification);
                }
                else
                {
                    if ((sfInfo.format & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV)
                    {
                        // Le fichier n'est pas au format WAV
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          "Invalid file",
                                                          "The selected file is not a valid WAV file.");
                        fileLabel.setText ("No file loaded...", dontSendNotification);
                        fileWav = "";
                        durationLabel.setText ("", dontSendNotification);
                        sampleRateLabel.setText ("", dontSendNotification);
                        channelsLabel.setText ("", dontSendNotification);
                    }
                    // Le fichier est valide, vous pouvez continuer avec votre traitement.
                    // N'oubliez pas de fermer le fichier lorsque vous avez terminé avec lui.
                    sf_close(f);

                    // Votre traitement du fichier ici...
                    try {
                        AudioFileProperties afp(file.toStdString());
                        fileWav = file.toStdString();
                        fileLabel.setText(file, juce::dontSendNotification);

                        // Enable processingButton if
                        processingButton.setEnabled(!bpmEditor.getText().isEmpty()
                                                    && !yearEditor.getText().isEmpty()
                                                    && !songEditor.getText().isEmpty()
                                                    && !soundEditor.getText().isEmpty()
                                                    && !fileWav.empty()
                                                    && !rootFolder.empty());

                        channelsLabel.setText("Channels: " + juce::String(afp.getChannels()) , juce::dontSendNotification);
                        sampleRateLabel.setText("Sample rate: " + juce::String(afp.getSampleRate()) + " @ " + juce::String(afp.getPcmBitDepth()) + "bits PCM", juce::dontSendNotification);
                        durationLabel.setText("Duration: " + juce::String(afp.getDuration(), 2) + " seconds", juce::dontSendNotification);
                        break;
                    } catch (const std::runtime_error& e) {
                        // Le fichier n'est pas au format WAV
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          "Error",
                                                          "Can't open file.");
                        fileLabel.setText ("No file loaded...", dontSendNotification);
                        fileWav = "";
                    }
                }
            }
        }
    }
}

bool MainComponent::keyPressed(const KeyPress &key, Component *originatingComponent) {
    if (key == KeyPress::escapeKey) {
        JUCEApplication::getInstance()->systemRequestedQuit();
        return true;  // Indiquer que l'événement de la touche a été traité
    }
    return false;  // Indiquer que l'événement de la touche n'a pas été traité
}

void MainComponent::textEditorTextChanged (TextEditor& editor)
{
    processingButton.setEnabled(!bpmEditor.getText().isEmpty()
                                && !yearEditor.getText().isEmpty()
                                && !songEditor.getText().isEmpty()
                                && !soundEditor.getText().isEmpty()
                                && !fileWav.empty()
                                && !rootFolder.empty()
    );
}

void MainComponent::fileSelectButtonClicked()
{
    FileChooser chooser("Select a .wav file", File{}, "*.wav");

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        StringArray files;
        files.add(file.getFullPathName());
        filesDropped(files, 0, 0); // Vous pouvez ajuster les coordonnées x et y si nécessaire
    }
}

void MainComponent::rootFolderSelectButtonClicked()
{
    FileChooser chooser("Select a root folder for save your process",
                        File{},               // Dossier de départ
                        "",                   // Types de fichiers à afficher (laissez vide pour tous les fichiers)
                        true);                // Use native dialog box?

    if (chooser.browseForDirectory())
    {
        auto folder = chooser.getResult();
        rootFolder = folder.getFullPathName().toStdString();
        String folderPath = "Root folder: " + rootFolder;
        rootLabel.setText(folderPath, juce::dontSendNotification);

        // Enable processingButton if
        processingButton.setEnabled(!bpmEditor.getText().isEmpty()
                                    && !yearEditor.getText().isEmpty()
                                    && !songEditor.getText().isEmpty()
                                    && !soundEditor.getText().isEmpty()
                                    && !fileWav.empty()
                                    && !rootFolder.empty());
    }
}

void MainComponent::processingButtonClicked()
{
    // Save default value
    String rootF = rootFolder;
    props->setValue("root", rootF);
    props->setValue("bpm", bpmEditor.getText());
    props->setValue("tone", toneSelect.getText());
    props->setValue("year", yearEditor.getText());
    props->setValue("song", songEditor.getText());
    props->saveIfNeeded();

    if (bpmEditor.getText().isEmpty()
    || yearEditor.getText().isEmpty()
    || songEditor.getText().isEmpty()
    || soundEditor.getText().isEmpty()
    || fileWav.empty()
    || rootFolder.empty())
        return;

    setMouseCursor(MouseCursor::WaitCursor);
    updateMouseCursor();

    // Create folder
    try {
        if (createDirectories()) {
            // cout << "Répertoires créés avec succès!" << endl;
        } else {
            // cout << "Le répertoire existe déjà!" << endl;
        }
        String fileInput = fileWav;
        player = std::make_unique<WAVPlayer>(fileInput,
                                             folderComplete,
                                             bpmEditor.getText().getFloatValue(),
                                             soundEditor.getText());
        player->setWrite(true);
        player->setBarFraction(WAVPlayer::BarFraction::Bar);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::Half);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::Quarter);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::Eighth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::Sixteenth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::ThirtyTwoSecond);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::SixtyFourth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        player->setBarFraction(WAVPlayer::BarFraction::OneHundredTwentyEighth);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        setMouseCursor(MouseCursor::NormalCursor);
        updateMouseCursor();
        AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Processing complete",
                                          "Files are written to the disk !");
        addAndMakeVisible(oneButton);
        addAndMakeVisible(twoButton);
        addAndMakeVisible(fourButton);
        addAndMakeVisible(eighthButton);
        addAndMakeVisible(sixteenthButton);
        addAndMakeVisible(thirtytwoButton);
        addAndMakeVisible(sixtyfourthButton);
        addAndMakeVisible(onehundredtwentyeigthButton);
        addAndMakeVisible(stoplisteningButton);

        // player->setWrite(false);
    } catch (const fsys::filesystem_error& e) {
        // cerr << "Erreur du système de fichiers: " << e.what() << endl;
        String msg = "Error attempting to create directories: ";
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Erreur", msg);
        msg += e.what();
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Erreur", msg);
    } catch (const exception& e) {
        // cerr << "Erreur du système de fichiers: " << e.what() << endl;
        String msg = "Error attempting to create directories: ";
        msg += e.what();
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon, "Erreur", msg);
    }

    /*
    float bpm = bpmEditor.getText().getFloatValue();
    int bars = barEditor.getText().getIntValue();

    try {
        AudioFileProperties afp(fileLabel.getText().toStdString());
        afp.splitByBars(bpm, bars);
        AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Info",
                                          "File successfully split !");
        durationLabel.setText ("", dontSendNotification);
        sampleRateLabel.setText ("", dontSendNotification);
        channelsLabel.setText ("", dontSendNotification);
        fileLabel.setText ("No file loaded...", dontSendNotification);
    } catch (const std::runtime_error& e) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Can't open file.");
        fileLabel.setText ("No file loaded...", dontSendNotification);
        durationLabel.setText ("", dontSendNotification);
        sampleRateLabel.setText ("", dontSendNotification);
        channelsLabel.setText ("", dontSendNotification);
    }*/
}

void MainComponent::aboutButtonClicked()
{
#ifdef __APPLE__
// Code spécifique à macOS
    // ShowMacOSDialog();
#endif
    AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                      "About",
                                      "This freeware is made by Stephane Bressani\n\nYou can contact him @ www.bressani.dev\nVersion: " + appVersion + "\n\n"
                                      "Thanks to JUCE for the community version of the cross-platform UI, to portaudio, libsnd, as well as these freepik artists for pieces of graphics that helped in the design of the application: GarryKillian and onfocus.");
}

void MainComponent::timerCallback()
{
    update();
}

void MainComponent::loadPNG(const String& path)
{
#ifdef __linux__
    File file(path);
    String p = "";
    // Si l'ouverture du fichier avec un chemin relatif échoue
    if (!file.exists()) {
        p = "/opt/OctaSplit/" + path;
    } else {
        p = path;
    }
    auto inputStream = std::make_unique<FileInputStream>(File(p));
#elif __APPLE__
    File file(path);
    String p = "";
    // Si l'ouverture du fichier avec un chemin relatif échoue
    if (!file.exists()) {
        string pp = GetPath() + "/";
        String ppp = (const char*) pp.c_str();
        p = ppp + path;
    } else {
        p = path;
    }
    auto inputStream = std::make_unique<FileInputStream>(File(p));
#else
    auto inputStream = std::make_unique<FileInputStream>(File(path));
#endif
    pngImage = PNGImageFormat().decodeImage(*inputStream);
}

unique_ptr<Drawable> MainComponent::loadSVG(const String &path) {
#ifdef __linux__
    File file(path);
    String p = "";
    // Si l'ouverture du fichier avec un chemin relatif échoue
    if (!file.exists()) {
        p = "/opt/OctaSplit/" + path;
    } else {
        p = path;
    }
    String path_svg = p;
#elif __APPLE__
    File file(path);
    String p = "";
    // Si l'ouverture du fichier avec un chemin relatif échoue
    if (!file.exists()) {
        string pp = GetPath() + "/";
        String ppp = (const char*) pp.c_str();
        p = ppp + path;
    } else {
        p = path;
    }
    String path_svg = p;
#else
    String path_svg = path;
#endif
    File svgFile{File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile).getSiblingFile(path_svg)};
    if (!svgFile.exists()) {
        DBG("File doesn't exist: " << svgFile.getFullPathName());
        return nullptr;
    }

    auto svgFileContent = svgFile.loadFileAsString();
    XmlDocument xmlDoc(svgFileContent);
    unique_ptr<XmlElement> xml(xmlDoc.getDocumentElement());

    if (xml != nullptr && xml->hasTagName("svg")) {
        return Drawable::createFromSVG(*xml);
    } else {
        DBG("XML is null or not an SVG.");
        return nullptr;
    }
}

bool MainComponent::createDirectories() {
    // Convertir le chemin de base en fs::path
    const string& root = rootFolder;
    fsys::path basePath(root);

    // Construire le chemin complet
    const string& bpm = bpmEditor.getText().toStdString();
    const string& tone = toneSelect.getText().toStdString();
    string sanitizedTone = tone;
    std::replace(sanitizedTone.begin(), sanitizedTone.end(), '/', '-'); // remplace '/' par '-'

    const string& year = yearEditor.getText().toStdString();
    const string& song = songEditor.getText().toStdString();
    fsys::path dirPath = basePath / bpm / sanitizedTone / year / song;
    folderComplete = dirPath.generic_string();

    // Vérifier si le répertoire existe déjà
    if(fsys::exists(dirPath))
    {
        return false;
    }

    // Créer les répertoires (y compris tous les répertoires parents nécessaires)
    return fsys::create_directories(dirPath);
}
