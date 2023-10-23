//
// Created by Stéphane on 23.10.23.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel() = default;
    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override;
    void drawComboBox(juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox& box) override;
};