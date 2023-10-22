#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "Misc.h"
#include "MainComponent.h"

using namespace std;
using namespace juce;

class MainWindow : public DocumentWindow {
public:
    MainWindow(String name)
            : DocumentWindow(
            name,
            Desktop::getInstance().getDefaultLookAndFeel().findColour(
                    ResizableWindow::backgroundColourId),
            DocumentWindow::allButtons)
    {
        setUsingNativeTitleBar(true);
        setContentOwned(new MainComponent(), true);

        //setResizable(true, true);
        centreWithSize(getWidth(), getHeight());
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }

};

class MyApp : public JUCEApplication {
public:
    MyApp() {
        // Open .env
        Misc misc;
        appName = misc.getAppName();
        appVersion = misc.getAppVersion();
    }
    const String getApplicationName() override { return String(appName); }
    const String getApplicationVersion() override { return String(appVersion); }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override { mainWindow = nullptr; }

    void systemRequestedQuit() override { quit(); }

    void anotherInstanceStarted(const String&) override {}

private:
    unique_ptr<MainWindow> mainWindow;
    string appName;
    string appVersion;
};

START_JUCE_APPLICATION(MyApp)
/*
#include <sndfile.h>
#include <thread>
#include <juce_core/juce_core.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h> // Incluez juce_gui_basics pour JUCEApplication
#include "WAVPlayer.h"

using namespace std;
using namespace juce;

class MyConsoleApp : public JUCEApplication {
public:
    void initialise(const String& commandLine) override {
        juce::Logger::writeToLog("Ceci est une application console JUCE.");

        player = std::make_unique<WAVPlayer>();
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
        player->setWrite(false);
        player->setBarFraction(WAVPlayer::BarFraction::Bar);
        player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
        char input;
        while (true) {
            cin >> input;
            switch (input) {
                case '1':
                    cout << "Bar" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::Bar);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '2':
                    cout << "1/2" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::Half);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '3':
                    cout << "1/4" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::Quarter);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '4':
                    cout << "1/8" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::Eighth);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '5':
                    cout << "1/16" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::Sixteenth);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '6':
                    cout << "1/32" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::ThirtyTwoSecond);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '7':
                    cout << "1/64" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::SixtyFourth);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case '8':
                    cout << "1/128" << endl;
                    player->setBarFraction(WAVPlayer::BarFraction::OneHundredTwentyEighth);
                    player->setPlaybackMode(WAVPlayer::PlaybackMode::ADVANCED);
                    break;
                case 'e':
                    shutdown();
                    quit();
            }
        }
        // Par exemple, attendez 10 secondes (ou la durée du son) avant de quitter
        //juce::Timer::callAfterDelay(10000, [this] { quit(); });
    }

    void shutdown() override {
        player = nullptr;  // Détruit le WAVPlayer
        if (ledThread && ledThread->joinable()) {
            ledThread->join();
        }
    }

    const juce::String getApplicationName() override {
        return "Nom de votre application";
    }

    const juce::String getApplicationVersion() override {
        return "1.0.0";
    }
private:
    std::unique_ptr<WAVPlayer> player;
    std::unique_ptr<std::thread> ledThread;
};

START_JUCE_APPLICATION(MyConsoleApp)
 */