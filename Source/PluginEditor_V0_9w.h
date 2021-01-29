/* PluginEditor_V0_9k.cpp (build 2020_06_26_00)
matched to PluginEditor_V0_96.cpp (build 2020_06_26_00)
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor_V0_9w.h"
#include "taigames.h"

class MyLookAndFeel_V4 : public LookAndFeel_V4

{
public:
    MyLookAndFeel_V4()
    	{
        setColour(Slider::thumbColourId, Colours::red);

		redfaderknob = ImageCache::getFromMemory (BinaryData::red_faderthumb_noshadow_22x52_png,
													BinaryData::red_faderthumb_noshadow_22x52_pngSize);
		greenfaderknob = ImageCache::getFromMemory (BinaryData::green_faderthumb_noshadow_22x52_png,
													BinaryData::green_faderthumb_noshadow_22x52_pngSize);
		trimfaderknob = ImageCache::getFromMemory (BinaryData::TRIMfaderthumb_47x71_png,
													BinaryData::TRIMfaderthumb_47x71_pngSize);
        
		faderknob = ImageCache::getFromMemory (BinaryData::faderthumb_noshadow_22x52_png,
													BinaryData::faderthumb_noshadow_22x52_pngSize);
		/*faderknobTc = ImageCache::getFromMemory (BinaryData::faderthumbTc_noshadow_22x52_png,
													BinaryData::faderthumbTc_noshadow_22x52_pngSize);
		*/
		fadershadow = ImageCache::getFromMemory (BinaryData::faderthumb_shadow_47x80_png,
													BinaryData::faderthumb_shadow_47x80_pngSize);
		linkBar= ImageCache::getFromMemory (BinaryData::Green_silverbar_5_25_no_text_png,
													BinaryData::Green_silverbar_5_25_no_text_pngSize);		
		for(char n=0;n<96;n++)
			{
			MLFTouchStateArray[n]=false;
			MLFAutoStateArray[n]=0;
			MLFChannelMapStateArray[n]=0;
			MLFLinkStateArray[n]=0;			
			}
		}
	~MyLookAndFeel_V4(){}
    //==============================================================================
    //void drawButtonText	(Graphics &,TextButton &,bool isMouseOverButton,bool isButtonDown )override;
	void drawLinearSlider (Graphics& g,
                                    int x, int y,
                                    int width, int height,
                                    float sliderPos,
                                    float minSliderPos,
                                    float maxSliderPos,
                                    const Slider::SliderStyle style,
									Slider& slider)
                        			override;
	
    void drawLinearSliderBackground(Graphics& g,
                                     	int x, int y,
                                       	int width, int height,
                                       	float sliderPos,
                                       	float minSliderPos,
                                       	float maxSliderPos,
                                       	const Slider::SliderStyle style,
                                       	Slider& slider) override;

    void drawLinearSliderThumb (Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         const Slider::SliderStyle style,
                                         Slider& slider) override;
    
    void initDbTable(int interfaceType);
	void MLFPassConfigData(int index,int data);
	void MLFPassAutoState(int index,int state);
	int MLFGetAutoState(int index);
	void MLFPassTouchState(int index,bool state);
	bool MLFGetTouchState(int index);
	
	void MLFPassMapState(int index,bool state);
	void MLFPassLinkState(int index,bool state);
	void MLFPassOnOffOpacitiy(int opacity);
	void MLFPassOnOffState(int state);
	void MLFPassDawAbsDbValue(int chn,int value);
	void MLFPassDawTrimDbValue(int chn,int value);
	void MLFPassSSLFaderDbValue(int chn,int value);
	int MLFGetDawAbsDbValue(int chn);
	int MLFGetDawTrimDbValue(int chn);
	int MLFGetSSLFaderDbValue(int chn);
	float getScaledValue(float faderDbValue);

	//void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
	//                           bool isMouseOverButton, bool isButtonDown) override;
    Image linkBar;
    Image faderknob;
	Image fadershadow;
    Image faderknobTc;
	Image trimfaderknob;
    Image redfaderknob;
    Image greenfaderknob;

   	float insteps[11];
	float outsteps[11];
	float k[11];

private:
	bool MLFTouchStateArray[96];
	int MLFAutoStateArray[96];
	bool MLFChannelMapStateArray[96];
	bool MLFLinkStateArray[96];
	int MLFOnOffOpacity;
	int MLFOnOffState;
	int MLFDawAbsDbValue[96];
	int MLFDawTrimDbValue[96];
	int MLFSSLFaderDbValue[96];
	int MLFConfigRegister[64];
};

//==============================================================================
class cutWriteObject   : public juce::Component
{
public:
    cutWriteObject(const String& name = String())
    {
    switchWidth=40;
    switchHeight=switchWidth;
    radius=1;
    tickness=10;
    setSize (switchWidth, switchHeight);
   	}

	void paint (juce::Graphics& g) override
	{
		g.setColour (juce::Colours::red);
		g.drawRoundedRectangle(0,0,switchWidth,switchHeight,radius,tickness);
   	}

    void resized() override
    {
        setBounds (0, 0, getWidth(),getHeight());
    }
	
	void changeSize(int size)
	{
		switchWidth=size;switchHeight=switchWidth;
	}

	void changeTickness(int size)
	{
		tickness=size;
	}
	void changeRadius(int size)
	{
		radius=size;
	}
	
private:

int switchWidth,switchHeight,tickness,radius;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cutWriteObject)
};



//==============================================================================
class cutWriteGraphics   : public juce::Component
{
public:
	cutWriteGraphics() {}

	void paint (juce::Graphics& g) override
		{
		g.setColour (juce::Colours::red);
		g.drawRoundedRectangle(0,0,25,25,1,3);
    	}
	
	void resized() override
	    {
		int a;
		a=0;
		}

private:
   //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cutWriteGraphics)
};

class cutWriteobject   : public juce::Component
{
public:
	cutWriteobject()
		{
		addAndMakeVisible(cutWriteRect);
		}

	~cutWriteobject(){}
	void paint (juce::Graphics& g) override
		{
		g.setColour (juce::Colours::red);

		}
	void resized() override
	    {
    	setSize(40,40);
   		cutWriteRect.setBounds (0,0, 40,40);
   		}

private:
cutWriteGraphics cutWriteRect; 
//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cutWriteobject)
};


//==============================================================================
/** This is the editor component that our filter will display.
*/
class TaiMotherShipPluguinProcessorEditor  :public AudioProcessorEditor,
													Button::Listener, Slider::Listener,
											private Timer
{
public:
    TaiMotherShipPluguinProcessorEditor (TaiMotherShipPluguinProcessor&,AudioProcessorValueTreeState&);
    ~TaiMotherShipPluguinProcessorEditor();
 
//    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
//    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void buttonClicked (Button* button) override;
	void sliderValueChanged (Slider* slider) override;
    void sliderDragStarted (Slider*) override;
    void sliderDragEnded (Slider*) override;
	void timerCallback() override;
    void updateTrackProperties();
	void labelScaling (unsigned char n);
	void ParameterManager(unsigned int chan);
	void Mute_manager(unsigned int chan);
	void doDebugWindowContent (void);
	void doDebugWindow (void);
	void modifierKeysChanged(const ModifierKeys& modifiers) override;
	void buttonStateChanged (Button* buttonstatechange) override;
	void rebuildMixView(void);
	void doFaderStatus(void);
	void doSetAll(void);
	void alertMsgManager(void);
	void setLeftHandButtonVisibleState(bool state);
	unsigned char TxChan;
    unsigned char RxChan;
	//void mouseEnter(const MouseEvent &event)override;
	void mouseMove(const MouseEvent &event)override;
	//leftHandBloc leftSection;

//------------------------------------------------------------------------------------------
//START: Easter eggs...section
	void taiPungInit(void);
	void setLeftHandGameButtonVisibleState(bool state);
	void paintPung (Graphics& g);
    Image play;
    Image stop;
    Image pause;
    Image reset;
    Image resume;
    Image newgame;
	Image taiBall;
	taiPadGraphics pad1;
	taiPadGraphics pad2;
	taiBallGraphics ball;
	taiPongScoreGraphics player1Score;
	taiPongScoreGraphics player2Score;
	bool mousePad1Flag;
	bool mousePad2Flag;
	float ballX;
	float ballY;
	float pad1X;
	float pad1Y;
	float pad2X;
	float pad2Y;
	float ballHeight;
	float ballWidth;
	float padHeight;
	float padWidth;

	ImageButton ImgButGameCtrl1;
	ImageButton ImgButGameCtrl2;
	ImageButton ImgButGameCtrl3;
	ImageButton ImgButGameCtrl4;
	ImageButton ImgButGameCtrl5;
	
 //------------------------------------------------------------------------------------------
 
    int savedViewSize;
//END: Easter eggs...section

	TooltipWindow tooltipWindow;
	
	ComboBox InputMenu;
    ComboBox VcaMenu;
    ConcertinaPanel mpanel;
	MyLookAndFeel_V4 MyLookAndFeel;
	ImageButton ImgButMute;
    TextButton mybutton;
    TextButton TrkNambutton;
    TextButton AutoCutButton;
    TextButton AutoVolButton;
	
	Image CutEnableOn;
	Image CutEnableOff;
	Image VolEnableOn;
	Image VolEnableOff;
	Image ConnectUDP;
    
    //Image ShiftUp;
    //Image ShiftUp2;
    //Image ShiftDown;
    //Image ShiftDown2;
	
	Image ShiftFirstOn;
    Image ShiftDownOn;
    Image ShiftUpOn;
    Image ShiftLastOn;
    
    Image ShiftFirstOff;
    Image ShiftDownOff;
    Image ShiftUpOff;
    Image ShiftLastOff;
    
    Image bank8_on;	
    Image bank16_on;	
    Image bank24_on;	
    Image bank32_on;

    Image bank8_off;	
    Image bank16_off;	
    Image bank24_off;	
    Image bank32_off;
    
    ImageButton imgButtonShiftUp;
    ImageButton imgButtonShiftFirst;
    ImageButton imgButtonShiftDown;
    ImageButton imgButtonShiftLast;
    
    ImageButton imgButtonBank8;
    ImageButton imgButtonBank16;
    ImageButton imgButtonBank24;
	ImageButton imgButtonBank32;	

	int viewSizeButtonYPos;
	int bankViewButtonYPos;	
	
	
	Image ImgLinkOff;
	Image ImgLinkOn;

	ImageButton ImgButLinkOff;
	ImageButton ImgButLinkOn;

	ImageButton ImgButUDPConnect;
	ImageButton ImgButAutoCutRead;
	ImageButton ImgButAutoVolRead;
	ImageButton ImgButAutoCutWrite;
    ImageButton ImgButAutoVolWrite;
    ImageButton ImgButMixOffOn;
    //==================================
    //cutWriteObject cutWrite;
	//MainContentComponent triangle;
/*
    ImageButton imgButtonShiftUp;
    ImageButton imgButtonShiftUp2;
    ImageButton imgButtonShiftDown;
    ImageButton imgButtonShiftDown2;
*/    
    ImageButton faderStatusSwt[8];
	int AutoModeRemap[7];
	int inputChnCnt;
	bool autoToggleSwtHover;
	bool resizeTrigger;

	int EditorSystemSize;
	unsigned char SystemSizeRcvdFlag;
    Image singleChnBkgnd;
    Image FaderPlatex8;
	Image vcaFaderScalex8;
	Image ufFaderScalex8;
    Image blankWideFaderPlate;
    Image mixOn;
    Image mixOff;
    Image mute_off;
    Image mute_on;
    Image image1;
    Image image2;
	Image automodesw_off[6];
	Image automodesw_on[6];
	Image setAllAbsImg[6];
	Image setAllTrimImg[6];
	
	Image setAll;
	Image setAllAutomodeTxt[6];


	int ActiveAutoModeState;
    ImageComponent ImageCmp1;
    ImageComponent ImageCmp2;
   	float scalefactor;
    int debugDisplayState;
    //void paintButton(Graphics &, bool isMouseOver,bool isButtonDown)override;
	struct SliderWithAttachment
		{
		std::unique_ptr<Slider> slider;
		std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
		};
    int editorMixOnOffState;
    
    int viewSize;
	float onoffOpacity;
	float onoffHoverOpacity;
	float onoffClickedOpacity;
	OwnedArray<Label> sslTrackNumber;
	OwnedArray<Label> 	sslVCAlabel;



private:
    //==============================================================================
    TaiMotherShipPluguinProcessor& getProcessor() const
    	{
        return static_cast<TaiMotherShipPluguinProcessor&> (processor);
    	}
    Image autoOffImg;
    Image autoWriteImg;
    Image autoTouchImg;
    Image autoReadImg;
    
	bool GetTransportState (AudioPlayHead::CurrentPositionInfo);
//===================================
//The labels
//===================================
	Label globalAutoRWLabel;
    Label AutoReadlabel;
	Label AutoWritelabel;
	Label AutoCutWriteLabel;
	Label statSwtLabel;

    Label timerlabel1;
    Label timerlabel2;
    Label timerlabel3;

	Label AlertLabel;
	Label versionNumber;
    //mycomponent mycomp;
//    Viewport myviewport;
	TextEditor debugDisplay;

	int debugDisplayHeight;

	int sw1x;
	int sw2x;
	int sw3x;
	int sw4x;
	int sw1y;
	int sw2y;
	int sw3y;
	int sw4y;

	int sw1w;
	int sw1h;
	int sw2w;
	int sw2h;
	int sw3w;
	int sw3h;
	int sw4w;
	int sw4h;
	int largeFaderW;
	int largeFaderH;
	int sFaderW;
	int sFaderH;
	
	bool heartBeatToggle;
    int mutebtntrim=10;
    
    Colour backgroundColour;
    TaiMotherShipPluguinProcessor& processor;
	AudioProcessorValueTreeState& parameters;
    
/*
    TextButton bank8;
    TextButton bank16;
    TextButton bank24;
    TextButton bank32;
    TextButton bankall;
    TextButton bankup;
    TextButton lastbank;
    TextButton bankdwn;
    TextButton firstbank;
*/
    int vcaIndex;
    int bank;
    TextButton mute_button;
    TextButton TxtButConnectUdp;
	OwnedArray<Slider> TheSliders;
	
	OwnedArray<Slider> TheTrimSliders;
    
	OwnedArray<ImageButton> TheMutes;
	OwnedArray<cutWriteObject> cutWrite;
	
	OwnedArray<ImageButton> ImgButLink;
    bool editorJustStartedFlag;
    OwnedArray<TextButton> TheCutButtons;
   	OwnedArray<Label> tNameLabel;
//  OwnedArray<Label> sslTrackNumber;
    OwnedArray<Label> TheValueLabels;
    OwnedArray<Label> ThedbLabels;
 	OwnedArray<Label> TheAutoLabels;
//	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> TSA;
//    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> TTA;
//    OwnedArray<AudioProcessorValueTreeState::ButtonAttachment> TBA;
    
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TaiMotherShipPluguinProcessorEditor)
};
