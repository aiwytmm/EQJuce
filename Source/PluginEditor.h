/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct LookAndFeel : juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider) override;
};

struct RotarySliderWithLabels : juce::Slider {
    RotarySliderWithLabels(juce::RangedAudioParameter& rap, const juce::String& unitSuffix) : 
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
        parameter(&rap),
        suffix(unitSuffix) 
    {
        setLookAndFeel(&lnf);
    }

    ~RotarySliderWithLabels() {
        setLookAndFeel(nullptr);
    }

    struct labelPosition {
        float position;
        juce::String label;
    };
    juce::Array<labelPosition> labels;

    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    juce::String getDisplayString() const;

private: 
    LookAndFeel lnf;
    juce::RangedAudioParameter* parameter;
    juce::String suffix;
};

struct ResponseCurveComponent : juce::Component, 
    juce::AudioProcessorParameter::Listener, juce::Timer {
    ResponseCurveComponent(EQAudioProcessor&);
    ~ResponseCurveComponent();

    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override { }
    void timerCallback() override;
    void paint(juce::Graphics& g) override;
private: 
    EQAudioProcessor& audioProcessor;
    juce::Atomic<bool> parametersChanged{ false };

    MonoChain monoChain;
    void updateChain();
};

//==============================================================================
/**
*/
class EQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    EQAudioProcessorEditor (EQAudioProcessor&);
    ~EQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EQAudioProcessor& audioProcessor;

    RotarySliderWithLabels peakFreqSlider, 
        peakGainSlider, 
        peakQualitySlider, 
        lowCutFreqSlider, 
        highCutFreqSlider,
        lowCutSlopeSlider,
        highCutSlopeSlider;

    ResponseCurveComponent responseCurveComponent;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;
    Attachment peakFreqSliderAttachment,
        peakGainSliderAttachment,
        peakQualitySliderAttachment,
        lowCutFreqSliderAttachment,
        highCutFreqSliderAttachment,
        lowCutSlopeSliderAttachment,
        highCutSlopeSliderAttachment;

    std::vector<juce::Component*> getComponents();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQAudioProcessorEditor)
};
