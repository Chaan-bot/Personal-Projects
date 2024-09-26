
#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

#include <vector>
#include <string>
#include <fstream>
#include <cmath>

class PlaylistComponent  :  public juce::Component , 
                            public TableListBoxModel,
                            public Button::Listener , 
                            TextEditor::Listener  
                           
{
public:
    //adds 2 djAudioPlayer objects and audioFromatReader to constructor
    PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2 , AudioFormatManager& _formatManager);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;

    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component* existingComponentToUpdate)override;
    //button listener when mouse is clicked adds functionality
    void buttonClicked(Button* button)override;
    //loads the data into the tableComponent
    void loadData();
    //returns the track length of a audio file(parameter)
    void getlength(File something);

    
private:
    //adds a tableListBox object
    TableListBox tableComponent;

    //adds 3 vectors to read from of type string 
    std::vector<juce::String > trackTitles;
    std::vector<juce::String > trackSize;
    std::vector<juce::String > trackLength;
    
    //adds a addButton for adding files
    TextButton addButton{ "Add File" };
    //adds a textEditor to search for files
    TextEditor textBox;

    //adds 2 djAudioPlayer objects
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;

    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
