/*
  ==============================================================================

    musicLibrary.h
    Created: 1 Sep 2021 12:56:21pm
    Author:  chaan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class musicLibrary  : public juce::Component
{
public:
    musicLibrary();
    ~musicLibrary() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (musicLibrary)
};
