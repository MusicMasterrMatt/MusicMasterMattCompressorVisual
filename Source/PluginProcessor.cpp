/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MusicMasterMattCompressorVisualAudioProcessor::MusicMasterMattCompressorVisualAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameter", createParameters())
#endif
{
    threshold = apvts.getRawParameterValue("THRESHOLD");
    ratio = apvts.getRawParameterValue("RATIO");
    attack = apvts.getRawParameterValue("ATTACK");
    release = apvts.getRawParameterValue("RELEASE");
    
    apvts.addParameterListener("THRESHOLD", this);
    apvts.addParameterListener("RATIO", this);
    apvts.addParameterListener("ATTACK", this);
    apvts.addParameterListener("RELEASE", this);
}

MusicMasterMattCompressorVisualAudioProcessor::~MusicMasterMattCompressorVisualAudioProcessor()
{
}

//==============================================================================
const juce::String MusicMasterMattCompressorVisualAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MusicMasterMattCompressorVisualAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MusicMasterMattCompressorVisualAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MusicMasterMattCompressorVisualAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MusicMasterMattCompressorVisualAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MusicMasterMattCompressorVisualAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MusicMasterMattCompressorVisualAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MusicMasterMattCompressorVisualAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MusicMasterMattCompressorVisualAudioProcessor::getProgramName (int index)
{
    return {};
}

void MusicMasterMattCompressorVisualAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MusicMasterMattCompressorVisualAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
        spec.numChannels = getTotalNumInputChannels();
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;

     compressor.prepare(spec);

   

    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    const int numInputerChannels = getTotalNumInputChannels();
    const int compressorBufferSize = 2 * (sampleRate * samplesPerBlock);
    mCompressorBuffer.setSize(numInputerChannels, compressorBufferSize);
}

void MusicMasterMattCompressorVisualAudioProcessor::releaseResources()
{
    compressor.reset();
    
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MusicMasterMattCompressorVisualAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MusicMasterMattCompressorVisualAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // I created an Audio Block here
    juce::dsp::AudioBlock<float> block(buffer);
    compressor.process(juce::dsp::ProcessContextReplacing<float>(block));

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool MusicMasterMattCompressorVisualAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MusicMasterMattCompressorVisualAudioProcessor::createEditor()
{
    return new MusicMasterMattCompressorVisualAudioProcessorEditor (*this);
}

//==============================================================================
void MusicMasterMattCompressorVisualAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MusicMasterMattCompressorVisualAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MusicMasterMattCompressorVisualAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
MusicMasterMattCompressorVisualAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold",-60.f, 0.f, 0.f)); // There is no SKEW option though :(
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RATIO", "Ratio",1.f, 40.f, 1.f)); //
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack",0.f, 200.f, 13.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release",5.f, 5000.f, 1.f)); ////check that final section is the SKEW and INTERVALS  are  in the Slider clas
    
    //added  20Aug

    // If any of these assertions are hit, the parameter ID string specified above doesn't match any existing parameter
//    jassert (thresholdRaw != nullptr);
  //  jassert (ratioRaw != nullptr);

    
    return { params.begin(), params.end() };
}

void MusicMasterMattCompressorVisualAudioProcessor::parameterChanged
(const juce::String& parameterID, float newValue)
{
    compressor.setThreshold(*threshold);
    compressor.setRatio(*ratio);
    compressor.setAttack(*attack);
    compressor.setRelease(*release);
    //etc. for all of your parameters
};

