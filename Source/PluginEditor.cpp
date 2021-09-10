/*/*
 ==============================================================================

   This file contains the basic framework code for a JUCE plugin editor.

 ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MusicMasterMattCompressorVisualAudioProcessorEditor::MusicMasterMattCompressorVisualAudioProcessorEditor (MusicMasterMattCompressorVisualAudioProcessor& p)
   : AudioProcessorEditor (&p), audioProcessor (p)
{
   ThresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
   ThresholdSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 25);
   ThresholdSlider.setRange(-60, 0, 0.25);
   ThresholdSlider.setValue(-20);//set the defualt setting
   addAndMakeVisible(ThresholdSlider);
    
 
   ThresholdSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "THRESHOLD", ThresholdSlider);
   
  RatioSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  //RatioSlider.setVelocityModeParameters(1.0,1,0.0,true); //
  RatioSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 25);
  RatioSlider.setRange(1, 40, 1); // experiment with .5 intervals :)
  RatioSlider.setValue(3);//set the defualt setting
   addAndMakeVisible(RatioSlider);
   
   RatioSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATIO", RatioSlider);
   
    AttackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //AttackSlider.setVelocityModeParameters(1.0,1,0.0,true);
    AttackSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 25);
    AttackSlider.setRange(0, 200, 1); //
    AttackSlider.setValue(13);
    addAndMakeVisible(AttackSlider);
    AttackSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", AttackSlider);
   
    
    
    ReleaseSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    ReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 50, 25);
    ReleaseSlider.setRange(0, 5000, 1); //
    ReleaseSlider.setValue(30);
    addAndMakeVisible(ReleaseSlider);
    ReleaseSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", ReleaseSlider);
    
    
   // Make sure that before the constructor has finished, you've set the
   // editor's size to whatever you need it to be.
   setSize (600, 450);
}

MusicMasterMattCompressorVisualAudioProcessorEditor::~MusicMasterMattCompressorVisualAudioProcessorEditor()
{
}

//==============================================================================
void MusicMasterMattCompressorVisualAudioProcessorEditor::paint (juce::Graphics& g)
{
   // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::gold);
    g.setFont (14.0f);
    
    g.fillAll (juce::Colours::darkgrey);
    
    //goldenrod for dial  darkgoldenrod
    auto bounds = getLocalBounds();

    {
    g.drawFittedText ("AMusicMasterMattCompressor", bounds.removeFromTop (450), juce::Justification::bottomRight, 600);
    }
    // changed variable name to bounds 2 in order to add more words
    auto bounds2 = getLocalBounds();
    {
    
    g.setColour (juce::Colours::gold);
    g.setFont (14.0f);
    g.drawFittedText ("Ratio", bounds2.removeFromLeft (474), juce::Justification::centred, 1);
   
    }
    auto bounds3 = getLocalBounds();
    {
    
    g.setColour (juce::Colours::gold);
    g.setFont (14.0f);
    g.drawFittedText ("Release", bounds3.removeFromRight (204), juce::Justification::centred, 1);
   
    }
    auto bounds4 = getLocalBounds();
    {
    
    g.setColour (juce::Colours::gold);
    g.setFont (14.0f);
    g.drawFittedText ("Attack", bounds4.removeFromRight (465), juce::Justification::centred, 1);
   
    }
    auto bounds5 = getLocalBounds();
    {
    
    g.setColour (juce::Colours::gold);
    g.setFont (14.0f);
    g.drawFittedText ("Threshold", bounds5.removeFromLeft (216), juce::Justification::centred, 1);
   
    }

}

void MusicMasterMattCompressorVisualAudioProcessorEditor::resized()
{
   //ThresholdSlider.setBounds(getWidth() / 450, getHeight() / 300, 200, 100);
   //RatioSlider.setBounds(getWidth() / 10, getHeight() / 500, 200, 100);
   //AttackSlider.setBounds(getWidth() / 130, getHeight() /  10, 200, 100);
   // This is generally where you'll want to lay out the positions of any
   // subcomponents in your editor..
    // x, y , width, height
    ThresholdSlider.setBounds(10, 150, 200, 130);
    RatioSlider.setBounds(140, 150, 200, 130);
    AttackSlider.setBounds(270, 150, 200, 130);
    ReleaseSlider.setBounds(400, 150, 200, 130);
    
}

