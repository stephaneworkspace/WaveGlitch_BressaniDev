//
// Created by Stéphane on 23.10.23.
//
#include "CustomLookAndFeel.h"

void CustomLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    // Ne faites rien ici. Cela empêchera JUCE de positionner et de dessiner le label par défaut.
}

void CustomLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                     int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box)
{
    juce::Rectangle<int> boxBounds(0, 0, width, height);

    // Dessinez l'arrière-plan
    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.fillRect(boxBounds);

    // Dessinez la bordure
    g.setColour(juce::Colours::grey); // Choisissez la couleur que vous préférez pour la bordure
    g.drawRect(boxBounds, 1); // Le deuxième paramètre est l'épaisseur de la bordure

    juce::Rectangle<int> textArea = boxBounds.reduced(4, 0);
    g.setColour(box.findColour(juce::ComboBox::textColourId));

    if (box.isTextEditable())
    {
        g.drawText(box.getText(), textArea, juce::Justification::centred, false);
    }
    else
    {
        g.setFont(juce::jlimit(0.0f, static_cast<float>(height), 30.0f) * 1.0f);
        g.drawText(box.getText(), textArea, juce::Justification::centred, false);
    }
}

