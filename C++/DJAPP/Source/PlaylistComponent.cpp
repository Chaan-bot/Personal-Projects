
#include <JuceHeader.h>
#include "PlaylistComponent.h"

PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2 ,
    AudioFormatManager& _formatManager )
    : player1(_player1), player2(_player2) , formatManager(_formatManager) 
{
    //register formats to read by formatManager
    formatManager.registerBasicFormats();
    //clears the list of trackTitles on startup
    trackTitles.clear();
    //function used to load data into the TableComponent
    loadData();

    //adding columns and headers for columns on application start
    tableComponent.getHeader().addColumn("Track title", 1, 300);
    tableComponent.getHeader().addColumn("Size", 2, 150);
    tableComponent.getHeader().addColumn("Length", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 280);
    tableComponent.getHeader().addColumn("", 5, 280);
    
    //adding the addButton to the GUI and initialising the listener
    addAndMakeVisible(addButton);
    addButton.addListener(this);
    
    //setting up the textEditor used for searching
    textBox.setColour(0x1000200, juce::Colours::black);
    textBox.setTextToShowWhenEmpty("Search the playlist...", juce::Colours::white);
    textBox.setFont(20.0f);

    //adding the textbox to the gui and initialising the listener
    addAndMakeVisible(textBox);
    textBox.addListener(this);
  
    //setting up the tableComponent and adding it to the gui
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

}

PlaylistComponent::~PlaylistComponent()
{
   
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 3);   // draw an outline around the component
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    //adding the tableComponent , textEditor and Addbutton to the gui layout
    double rowH = getHeight() / 18;
    addButton.setBounds(getWidth() / 2, 0, getWidth()/2, rowH*2);
    textBox.setBounds(0, 0, getWidth() / 2, rowH*2);
    tableComponent.setBounds(0, rowH*2, getWidth(), rowH*15);
}

int PlaylistComponent::getNumRows()
{
    //tracktitles.size will equal the amount of rows the tableComponent has 
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        //changes the colour when selecting a row with the mouse
        g.fillAll(juce::Colours::mediumpurple); 
    }
    else {
        g.fillAll(juce::Colours::transparentWhite);
    }
    //checks to see if the searchbox is not empty
    if (!textBox.isEmpty()) {
        //checks to see if the vector TrackTitles contains the text of the searchbox
            if (trackTitles[rowNumber].contains(textBox.getText())) {
               //fills the rows that contain the searched text
                g.fillAll(juce::Colours::lightsalmon);
                //repaints the tableComponent
                repaint();
                //updates the content of the tableComponent
                tableComponent.updateContent();
            }
    }
    //checks to see if the searchbox is empty
    if (textBox.isEmpty()) {
        //sets the colour of all boxes to transperant white when nothing is being searched for
        repaint();
        tableComponent.updateContent();
        g.fillAll(juce::Colours::transparentWhite);
    }
}

void PlaylistComponent::paintCell(Graphics& g ,int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{    
    //checks if the column id = 1
    if (columnId == 1) {
        //draws the vector tracktitles contents to the tableComponent
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    //checks if the column id = 2
    if (columnId == 2) {
        //draws the vector trackSize's conents to the tableComponent
        g.drawText(trackSize[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    //checks if the column id = 3
    if (columnId == 3) {
        //draws the vector tracklegth's contents to the tableComponent
        g.drawText(trackLength[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component* existingComponentToUpdate)
{
    //checks if the columnId = 4
    if (columnId == 4)
    {
        //checks if component is empty
        if (existingComponentToUpdate == nullptr)
        {
            //creates a textButton to be drawn in all rows of the tableComponent
            TextButton* LoadPlayer1Btn = new TextButton{ "LoadPlayer1" };
            String id{ std::to_string(rowNumber) };
            LoadPlayer1Btn->setComponentID(id);
            LoadPlayer1Btn->addListener(this);
            existingComponentToUpdate = LoadPlayer1Btn;
            buttonClicked(LoadPlayer1Btn);
        }
    }
    //checks if the columnId = 5
    if (columnId == 5)
    {
        //checks if component is empty
        if (existingComponentToUpdate == nullptr)
        {
            //creates a textButton to be drawn in all rows of the tableComponent
            TextButton* LoadPlayer2Btn = new TextButton{ "LoadPlayer2" };
            String id{ std::to_string(rowNumber) };
            LoadPlayer2Btn->setComponentID(id);
            LoadPlayer2Btn->addListener(this);
            existingComponentToUpdate = LoadPlayer2Btn;
            buttonClicked(LoadPlayer2Btn);
        }
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    //sets variable dir = current working directory
    auto dir = juce::File::getCurrentWorkingDirectory();

    int numTries = 0;

    while (!dir.getChildFile("Resources").exists() && numTries++ < 30)
        dir = dir.getParentDirectory();

    //sets tableFile equal to the directory of tracks which can be found in resources
    auto tableFile = dir.getChildFile("Resources").getChildFile("tracks");

    //checks if tablefile exists
    if (tableFile.exists())
    {
        //loops through all entries in tablefile
        for (DirectoryEntry entry : RangedDirectoryIterator(tableFile, false)) {

            //sets the integer id eequal to the component id
            int id = std::stoi(button->getComponentID().toStdString());

            //checks if an entry is not hidden
            if (!entry.isHidden()) {

                //checks if the filename is equal to the id of vector tracktitles
                if (entry.getFile().getFileName() == trackTitles[id]) {

                    //creates a file
                    File fileToLoad = entry.getFile();
                    //creates a url file
                    URL audioURL = URL{ fileToLoad };
                    //checks if button name is equal to loadPlayer1
                    if (button->getButtonText() == "LoadPlayer1") {
                        //loads the url into player 1
                        player1->loadURL(audioURL);
                    }
                    //checks if button name is equal to loadPlayer2
                    if (button->getButtonText() == "LoadPlayer2") {
                        //loads the url into player 2
                        player2->loadURL(audioURL);
                    }
                }
            }
            //sets id of addbutton = 0
            addButton.setComponentID("0");
            if (button == &addButton) {
                //creates a file chooser if addbutton selected
                FileChooser chooser{ "Select a file..." };
                //browse for files to save to tableComponent
                if (chooser.browseForFileToSave(false))
                {
                    //sets a file equal to the file search result
                    File fileToAdd = chooser.getResult();
                    //gets the id of the button in this case the addbutton
                   // int id = std::stoi(button->getComponentID().toStdString());
                    //pushes back the track name to tracktitles
                    trackTitles.push_back(fileToAdd.getFileName());

                    //gets the file size and saves it as a string in trackSize vector
                    double size = fileToAdd.getSize() * 0.00000095367432;
                    std::string stringSize = std::to_string(size);
                    stringSize.resize(4);
                    trackSize.push_back(stringSize + " " + "MB");

                    //gets the song length of the file to add
                    getlength(fileToAdd);

                    //updates the tableComponent with new file added
                    tableComponent.updateContent();
                    break;
                    
                }
            }
        }
    }
}

void PlaylistComponent::loadData()
{
    //sets variable dir = current working directory
    auto dir = juce::File::getCurrentWorkingDirectory();

    int numTries = 0;

    while (!dir.getChildFile("Resources").exists() && numTries++ < 30)
        dir = dir.getParentDirectory();

    //sets tableFile equal to the directory of tracks which can be found in resources
    auto tableFile = dir.getChildFile("Resources").getChildFile("tracks");

    //checks if tablefile exists
    if (tableFile.exists())
    {
        //loops through all entries in tablefile
        for (DirectoryEntry entry : RangedDirectoryIterator(tableFile, false)) {

            //checks if an entry is not hidden
            if (!entry.isHidden()) {
                //pushes back the track name to tracktitles
                trackTitles.push_back(entry.getFile().getFileName());

                //gets the file size and saves it as a string in trackSize vector
                double size = entry.getFileSize()*0.00000095367432;
                std::string stringSize = std::to_string(size);
                stringSize.resize(4);
                trackSize.push_back(stringSize + " " + "MB");

                //gets the song length of the file to add
                getlength(entry.getFile());

            }
        }          
    }
}

void PlaylistComponent::getlength(File something)
{
        //sets the argument equal to type file
        File fileToLoad = something;
        //creates a url type
        URL audioURL = URL{ fileToLoad };
        //initializing variables to use to calculate song length
        double lengthSeconds = 0;
        int lengthMinutes;
        int seconds;
       
        //creates a audioFormatReader to read the created audioURL
        AudioFormatReader* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
        if (reader != nullptr) // good file!
        {
            //calculates seconds and minutes
            lengthSeconds = reader->lengthInSamples / reader->sampleRate;
            seconds = int( (reader->lengthInSamples / reader->sampleRate)) %60;
            lengthMinutes = lengthSeconds / 60;
            
            //changes type to type string
            std::string songmins = std::to_string(lengthMinutes);
            std::string songLength = std::to_string(seconds);
            songLength.resize(2);

            //adds the calculated length to the vvector trackLength
            trackLength.push_back("0" + songmins + ":" + songLength );
            //deletes the audioFormatReader
            delete reader;
        }
}


    
