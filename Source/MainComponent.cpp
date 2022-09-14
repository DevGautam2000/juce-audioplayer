#include "MainComponent.h"

//==============================================================================
void MainComponent::openButtonClicked(){
    
    //choose a file
    //files alllowed are wav and mp3
    juce::FileChooser chooser("Choose an audio file.",juce::File::getSpecialLocation(juce::File::userDesktopDirectory),"*.wav,*.mp3",true,false);
    
    
    
    if (chooser.browseForFileToOpen()) { // if the file window is open
        
        juce::File file;
        file = chooser.getResult(); //grab the file chosed
        
        //create a pointer to the audio format reader
        juce::AudioFormatReader* reader = formatManager.createReaderFor(file);
        
        
        //this creates a unique pointer so that when th audio is playing
        //then the temp source can be replaced with the current source
        //and the plaback has no hiccups
        std::unique_ptr<juce::AudioFormatReaderSource> src{ new juce::AudioFormatReaderSource(reader,true) };
        playSource.reset(src.release());
        
        //debig the file format
        DBG(reader->getFormatName());
    }
}

MainComponent::MainComponent() : openButton("open")
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (200, 100);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
    
    openButton.onClick = [this]{ openButtonClicked(); };
    addAndMakeVisible(&openButton);
    
    formatManager.registerBasicFormats();
    
    
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    openButton.setBounds(10, 10, getWidth()-20, 30);
}
