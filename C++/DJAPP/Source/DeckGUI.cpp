

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"


DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    //initialising the buttons to show on the gui
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loopButton);
    //initialising the sliders to show on the gui
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    //initialising the waveformdisplay to show on the gui
    addAndMakeVisible(waveformDisplay);

    //setting the volume slider style and size
    volSlider.setSliderStyle(juce::Slider::Rotary);
    volSlider.setSize(100, 100);

    //setting the speed slider style and size
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSize(100, 100);

    //setting the position slider style and size
    posSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setSize(100, 100);

    //setting the textbox style containing the values of each slider
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);

    //setting labels to add names to the slider textboxes
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.attachToComponent(&volSlider, false);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);

    //adding event listeners to the buttons
    pauseButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
  
    //adding event listeners to the sliders
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    //setting the range of each slider
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 3.0);
    posSlider.setRange(0.0, 1.0);

    //setting the timer to start
    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    //stoping the timer
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour (Colours::white);
    g.setFont (14.0f);

    //setting rowH so that rectangles can be drawn around components and buttons
    double rowH = getHeight() / 13;

    //drawing rectangles to place sliders and buttons
    g.drawRect(0, 0, getWidth() / 4, rowH * 3, 1);
    g.drawRect((getWidth() / 4), 0, getWidth() / 4, rowH * 3, 1);
    g.drawRect((getWidth() / 4)*2, 0, getWidth() / 4, rowH * 3, 1);
    g.drawRect((getWidth() / 4)*3, 0, getWidth() / 4, rowH * 3, 1);
    g.drawRect(0, rowH * 4, getWidth() / 3 , rowH *6, 1);
    g.drawRect(getWidth() /3, rowH * 4, getWidth() / 3 , rowH * 6, 1);
    g.drawRect((getWidth() /3)*2, rowH * 4, getWidth() / 3, rowH *6, 1);
}

//resising the deckGUI components
void DeckGUI::resized()
{
    double rowH = getHeight() / 13; 
    playButton.setBounds(0, 0, getWidth() / 4, rowH * 3);
    stopButton.setBounds((getWidth() / 4), 0, (getWidth() / 4), rowH * 3);
    pauseButton.setBounds((getWidth() / 4)*2,0, (getWidth() / 4), rowH * 3);
    loopButton.setBounds((getWidth() / 4)*3, 0,(getWidth() / 4), rowH * 3);

    volSlider.setBounds(0, rowH * 5, 200, rowH*4);
    speedSlider.setBounds(getWidth() / 3, rowH * 5, 200, rowH*4);
    posSlider.setBounds((getWidth() / 3) * 2, rowH *5, 200, rowH*4);
    waveformDisplay.setBounds(0, rowH *10, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 12, getWidth(), rowH);

}

//function that checks if a button was clicked on the deckGUI
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
        //looping set to false when playing starts 
        player->loopTrack(false);
    }
    if (button == &stopButton)
    {
        player->setPosition(0.0);
        player->stop();
        //looping set to false when playing stops
        player->loopTrack(false);
    }
    if (button == &pauseButton)
    {
        player->stop();
    }
    if (button == &loopButton)
    {
        //looping set to true when looping is activated
        player->loopTrack(true);
    }
    if (button == &loadButton)
    {
        //looping set to false when loading a new audiofile 
        player->loopTrack(false);

        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});
        }
    }
}

//function that checks if slider value changed
void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

//checking if fileDragandDrop is accepted
bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}


    

