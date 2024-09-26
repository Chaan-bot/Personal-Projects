/*
  ==============================================================================

    CustomDeckControl.h
    Created: 1 Sep 2021 12:56:05pm
    Author:  chaan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomDeckControl  : public juce::Component
{
public:
    CustomDeckControl();
    ~CustomDeckControl() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomDeckControl)
};
