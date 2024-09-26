

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"

class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;
     /** implement Button::Listener */
    void buttonClicked (Button *) override;
    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 
    void timerCallback() override; 

private:

    //adds various textButton objects
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton pauseButton{ "PAUSE" };
    TextButton loadButton{"LOAD"};
    TextButton loopButton{ "LOOP" };
    
    //adds various Slider objects
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    //adds various label objects
    juce::Label  volumeLabel;
    juce::Label  speedLabel;
    juce::Label  posLabel;

    //waveformdisplay object added
    WaveformDisplay waveformDisplay;
    
    //adds a DjAudioPlayer object
    DJAudioPlayer* player; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
