 /*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class VSTAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VSTAudioProcessor();
    ~VSTAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
// JUCE uses an object called AudioProcessorValueTreeState to sync the GUI and the parameters in the code
    
// Can be kept static since doesn't use member variables (is the same no matter how many instances)
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts {*this,nullptr,"Parameters",createParameterLayout()};
private:
    // each filter has 12db we want 48db so we need to use a processor chain of 4 filters.
    using Filter=juce::dsp::IIR::Filter<float>;
    
    using CutFilter=juce::dsp::ProcessorChain<Filter,Filter,Filter,Filter>;
    
    
    // LowCut, Parametric, HighCut - we need two of these for stereo processing
    using MonoChain=juce::dsp::ProcessorChain<CutFilter,Filter,CutFilter>;
    
    MonoChain leftChain,rightChain;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTAudioProcessor)
};
