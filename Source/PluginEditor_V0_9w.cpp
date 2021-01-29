//Date (build 2020.10.29)

// good fader VCA scalling
/* ==============================================================================

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
#define __MODKEYDEV
#include "PluginProcessor_V0_9w.h"
#include "PluginEditor_V0_9w.h"

#define OFF 0
#define WRITE 1
#define TOUCH_LATCH 2
#define READ 3
#define TRIMWRITE 5
#define TRIMTOUCH_LATCH 6
#define TRIMREAD 7


//===============================================================================================
//
//
//===============================================================================================
TaiMotherShipPluguinProcessorEditor::TaiMotherShipPluguinProcessorEditor (TaiMotherShipPluguinProcessor& p,
																		AudioProcessorValueTreeState& params)
: AudioProcessorEditor (&p),
	debugDisplay ("Debug Display"),
 	processor (p),
	//transportlabel(String()),
	parameters(params)
{
editorMixOnOffState=-1;
MyLookAndFeel.initDbTable(processor.taiType);
editorJustStartedFlag=false;
debugDisplayState=0;
debugDisplayHeight=100;
savedViewSize=processor.viewSize;
scalefactor=1.0f;
EditorSystemSize=0;
SystemSizeRcvdFlag=0;

setLookAndFeel (&MyLookAndFeel);
tooltipWindow.setMillisecondsBeforeTipAppears(10);
inputChnCnt=processor.SystemSize-8;
onoffOpacity=1.0f;
MyLookAndFeel.MLFPassOnOffOpacitiy(onoffOpacity);
onoffHoverOpacity=0.8f;
onoffClickedOpacity=0.8f;

processor.trkLabelRefreshFlag=1;
processor.linkChange=0;
viewSizeButtonYPos=173;
bankViewButtonYPos=197;

//----------------------------------------------------
// sw object H,W sizes
    
sw1w=40;
sw2w=sw1w;
sw3w=sw1w;
sw4w=sw1w;

sw1h=sw1w;
sw2h=sw1w;
sw3h=sw1w;
sw4h=sw1w;

//----------------------------------------------------
//images loading to be used in plugin
//----------------------------------------------------

blankWideFaderPlate = ImageCache::getFromMemory (BinaryData::fader_plate_462_110_full_png, BinaryData::fader_plate_462_110_full_pngSize);
singleChnBkgnd = ImageCache::getFromMemory (BinaryData::blank_plate_44_462_png, BinaryData::blank_plate_44_462_pngSize);

mixOff=ImageCache::getFromMemory(BinaryData::classic_red_100x33_mix_off_png,BinaryData::classic_red_100x33_mix_off_pngSize);
mixOn=ImageCache::getFromMemory(BinaryData::classic_green_100x33_MIX_ON_png,BinaryData::classic_green_100x33_MIX_ON_pngSize);

ShiftFirstOn= ImageCache::getFromMemory (BinaryData::classic_green_button_100_100_first_png, BinaryData::classic_green_button_100_100_first_pngSize);
ShiftDownOn= ImageCache::getFromMemory (BinaryData::classic_green_button_100_100_down_png, BinaryData::classic_green_button_100_100_down_pngSize);
ShiftUpOn= ImageCache::getFromMemory (BinaryData::classic_green_button_100_100_up_png, BinaryData::classic_green_button_100_100_up_pngSize);
ShiftLastOn= ImageCache::getFromMemory (BinaryData::classic_green_button_100_100_last_png, BinaryData::classic_green_button_100_100_last_pngSize);

ShiftFirstOff= ImageCache::getFromMemory (BinaryData::classic_grey_button_100_100_first_png, BinaryData::classic_grey_button_100_100_first_pngSize);
ShiftDownOff= ImageCache::getFromMemory (BinaryData::classic_grey_button_100_100_down_png, BinaryData::classic_grey_button_100_100_down_pngSize);
ShiftUpOff= ImageCache::getFromMemory (BinaryData::classic_grey_button_100_100_up_png, BinaryData::classic_grey_button_100_100_up_pngSize);
ShiftLastOff= ImageCache::getFromMemory (BinaryData::classic_grey_button_100_100_last_png, BinaryData::classic_grey_button_100_100_last_pngSize);

bank8_on = ImageCache::getFromMemory (BinaryData::classic_green_button_8_png, BinaryData::classic_green_button_8_pngSize);
bank16_on = ImageCache::getFromMemory (BinaryData::classic_green_button_16_png, BinaryData::classic_green_button_16_pngSize);
bank24_on = ImageCache::getFromMemory (BinaryData::classic_green_button_24_png, BinaryData::classic_green_button_24_pngSize);
bank32_on = ImageCache::getFromMemory (BinaryData::classic_green_button_32_png, BinaryData::classic_green_button_32_pngSize);

bank8_off = ImageCache::getFromMemory (BinaryData::classic_grey_button_8_png, BinaryData::classic_grey_button_8_pngSize);
bank16_off = ImageCache::getFromMemory (BinaryData::classic_grey_button_16_png, BinaryData::classic_grey_button_16_pngSize);
bank24_off = ImageCache::getFromMemory (BinaryData::classic_grey_button_24_png, BinaryData::classic_grey_button_24_pngSize);
bank32_off = ImageCache::getFromMemory (BinaryData::classic_grey_button_32_png, BinaryData::classic_grey_button_32_pngSize);


/*
ShiftUp= ImageCache::getFromMemory (BinaryData::clasic_green_button_UP_s_png, BinaryData::clasic_green_button_UP_s_pngSize);
ShiftUp2= ImageCache::getFromMemory (BinaryData::clasic_green_button_UP2_s_png, BinaryData::clasic_green_button_UP2_s_pngSize);
ShiftDown= ImageCache::getFromMemory (BinaryData::clasic_green_button_DOWN_s_png, BinaryData::clasic_green_button_DOWN_s_pngSize);
ShiftDown2= ImageCache::getFromMemory (BinaryData::clasic_green_button_DOWN2_s_png, BinaryData::clasic_green_button_DOWN2_s_pngSize);
*/
CutEnableOn= ImageCache::getFromMemory (BinaryData::classic_green_button_cuts_s_png, BinaryData::classic_green_button_cuts_s_pngSize);
CutEnableOff= ImageCache::getFromMemory (BinaryData::classic_grey_button_cuts_s_png, BinaryData::classic_grey_button_cuts_s_pngSize);
VolEnableOn= ImageCache::getFromMemory (BinaryData::classic_green_button_fader_s_png, BinaryData::classic_green_button_fader_s_pngSize);
VolEnableOff= ImageCache::getFromMemory (BinaryData::classic_grey_button_fader_s_png, BinaryData::classic_grey_button_fader_s_pngSize);

//----------------------------------------------------------------------------------------------------------------------------------
setAllAbsImg[0]= ImageCache::getFromMemory (BinaryData::classic_black_off_105x30_png, BinaryData::classic_black_off_105x30_pngSize);
setAllAbsImg[1]= ImageCache::getFromMemory (BinaryData::classic_black_abs_write_105x30_png, BinaryData::classic_black_abs_write_105x30_pngSize);
if(!processor.taiType)
	setAllAbsImg[2]= ImageCache::getFromMemory (BinaryData::classic_black_abs_latch_105x30_png, BinaryData::classic_black_abs_latch_105x30_pngSize);
else
	setAllAbsImg[2]= ImageCache::getFromMemory (BinaryData::classic_black_abs_touch_105x30_png, BinaryData::classic_black_abs_touch_105x30_pngSize);
setAllAbsImg[3]= ImageCache::getFromMemory (BinaryData::classic_black_abs_read_105x30_png, BinaryData::classic_black_abs_read_105x30_pngSize);
setAllAbsImg[4]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllAbsImg[5]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
//----------------------------------------------------------------------------------------------------------------------------------

setAllTrimImg[0]= ImageCache::getFromMemory (BinaryData::classic_black_off_105x30_png, BinaryData::classic_black_off_105x30_pngSize);
setAllTrimImg[1]= ImageCache::getFromMemory (BinaryData::classic_black_trim_write_105x30_png, BinaryData::classic_black_trim_write_105x30_pngSize);
if(!processor.taiType)
	setAllTrimImg[2]= ImageCache::getFromMemory (BinaryData::classic_black_trim_latch_105x30_png, BinaryData::classic_black_trim_latch_105x30_pngSize);
else
	setAllTrimImg[2]= ImageCache::getFromMemory (BinaryData::classic_black_trim_touch_105x30_png, BinaryData::classic_black_trim_touch_105x30_pngSize);
setAllTrimImg[3]= ImageCache::getFromMemory (BinaryData::classic_black_trim_read_105x30_png, BinaryData::classic_black_trim_read_105x30_pngSize);
setAllTrimImg[4]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllTrimImg[5]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);

setAll= ImageCache::getFromMemory (BinaryData::classic_black_set_all_105x30_png, BinaryData::classic_black_set_all_105x30_pngSize);

setAllAutomodeTxt[0]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllAutomodeTxt[1]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllAutomodeTxt[2]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllAutomodeTxt[3]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllAutomodeTxt[4]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);
setAllAutomodeTxt[5]= ImageCache::getFromMemory (BinaryData::classic_black_105x30_png, BinaryData::classic_black_105x30_pngSize);

//addAndMakeVisible(leftSection);

FaderPlatex8 = ImageCache::getFromMemory (BinaryData::blank_fader8_plate_352_462_png, BinaryData::blank_fader8_plate_352_462_pngSize);
vcaFaderScalex8 = ImageCache::getFromMemory (BinaryData::new_vca_db_scale_x8_png, BinaryData::new_vca_db_scale_x8_pngSize);
ufFaderScalex8 = ImageCache::getFromMemory (BinaryData::new_uf_db_scale_x8_png, BinaryData::new_uf_db_scale_x8_pngSize);
mute_on = ImageCache::getFromMemory (BinaryData::lightbutton_on_cut_png, BinaryData::lightbutton_on_cut_pngSize);
mute_off = ImageCache::getFromMemory (BinaryData::lightbutton_off_cut_png, BinaryData::lightbutton_off_cut_pngSize);
//	ConnectUDP= ImageCache::getFromMemory (BinaryData::clasic_grey_button_fader_s_png, BinaryData::clasic_grey_button_fader_s_pngSize);

play=ImageCache::getFromMemory(	BinaryData::classic_green_100x33_play_png,
								BinaryData::classic_green_100x33_play_pngSize);

stop=ImageCache::getFromMemory(	BinaryData::classic_red_100x33_stop_png,
								BinaryData::classic_red_100x33_stop_pngSize);

pause=ImageCache::getFromMemory(BinaryData::classic_orange_100x33_pause_png,
								BinaryData::classic_orange_100x33_pause_pngSize);

reset=ImageCache::getFromMemory(BinaryData::classic_grey_100x33_png,
								BinaryData::classic_grey_100x33_pngSize);

resume=ImageCache::getFromMemory(	BinaryData::classic_grey_100x33_resume_png,
									BinaryData::classic_grey_100x33_resume_pngSize);

newgame=ImageCache::getFromMemory(	BinaryData::classic_black_100x33_newgame_png,
									BinaryData::classic_black_100x33_newgame_pngSize);




autoOffImg= ImageCache::getFromMemory (BinaryData::classic_grey_100x33_png, BinaryData::classic_grey_100x33_pngSize);
autoWriteImg= ImageCache::getFromMemory (BinaryData::classic_red_100x33_png, BinaryData::classic_red_100x33_pngSize);
autoReadImg= ImageCache::getFromMemory (BinaryData::classic_green_100x33_png, BinaryData::classic_green_100x33_pngSize);
autoTouchImg= ImageCache::getFromMemory (BinaryData::classic_orange_100x33_png, BinaryData::classic_orange_100x33_pngSize);

ImgLinkOff = ImageCache::getFromMemory (BinaryData::silverball_40_L_png, BinaryData::silverball_40_L_pngSize);
ImgLinkOn = ImageCache::getFromMemory(BinaryData::Green_silverball_40_L_png, BinaryData::Green_silverball_40_L_pngSize);

automodesw_off[0]= ImageCache::getFromMemory (BinaryData::swt_preview_ow_off_s_png, BinaryData::swt_preview_ow_off_s_pngSize);
automodesw_off[1]= ImageCache::getFromMemory (BinaryData::swt_review_or_off_s_png, BinaryData::swt_review_or_off_s_pngSize);
automodesw_off[2]= ImageCache::getFromMemory (BinaryData::swt_abs_rw_off_s_png, BinaryData::swt_abs_rw_off_s_pngSize);
if(!processor.taiType)
	{
	automodesw_off[3]= ImageCache::getFromMemory (BinaryData::swt_abs_rl_off_s_png, BinaryData::swt_abs_rl_off_s_pngSize);
	automodesw_off[5]= ImageCache::getFromMemory (BinaryData::swt_Trim_rl_off_s_png, BinaryData::swt_Trim_rl_off_s_pngSize);
	}
else
	{
	automodesw_off[3]= ImageCache::getFromMemory (BinaryData::swt_abs_rt_off_s_png, BinaryData::swt_abs_rt_off_s_pngSize);
	automodesw_off[5]= ImageCache::getFromMemory (BinaryData::swt_Trim_rt_off_s_png, BinaryData::swt_Trim_rt_off_s_pngSize);
	}
automodesw_off[4]= ImageCache::getFromMemory (BinaryData::swt_Trim_rw_off_s_png, BinaryData::swt_Trim_rw_off_s_pngSize);

automodesw_on[0]= ImageCache::getFromMemory (BinaryData::swt_preview_ow_on_s_png, BinaryData::swt_preview_ow_on_s_pngSize);
automodesw_on[1]= ImageCache::getFromMemory (BinaryData::swt_review_or_on_s_png, BinaryData::swt_review_or_on_s_pngSize);
automodesw_on[2]= ImageCache::getFromMemory (BinaryData::swt_abs_rw_on_s_png, BinaryData::swt_abs_rw_on_s_pngSize);
if(!processor.taiType)
	automodesw_on[3]= ImageCache::getFromMemory (BinaryData::swt_abs_rl_on_s_png, BinaryData::swt_abs_rt_on_s_pngSize);
else
	automodesw_on[3]= ImageCache::getFromMemory (BinaryData::swt_abs_rt_on_s_png, BinaryData::swt_abs_rt_on_s_pngSize);
automodesw_on[4]= ImageCache::getFromMemory (BinaryData::swt_Trim_rw_on_s_png, BinaryData::swt_Trim_rw_on_s_pngSize);
automodesw_on[5]= ImageCache::getFromMemory (BinaryData::swt_Trim_rl_on_s_png, BinaryData::swt_Trim_rt_on_s_pngSize);



//----------------------------------------------------
// Objects sizes
//----------------------------------------------------
largeFaderW= blankWideFaderPlate.getWidth();
largeFaderH= blankWideFaderPlate.getHeight();
sFaderW=singleChnBkgnd.getWidth();
sFaderH=singleChnBkgnd.getHeight();

//----------------------------------------------------
// Objects X, Y Positions
//----------------------------------------------------
sw1x=(largeFaderW/4)-(sw1w/2);
sw2x=(3*(largeFaderW/4))-(sw1w/2);
sw3x=sw1x;
sw4x=sw2x;

sw1y= 65;
sw2y=sw1y;
sw3y=sw2y+(sw1h/2)+25;
sw4y=sw3y;

//----------------------------------------------------
// visual object initialisation
//----------------------------------------------------
if(debugDisplayState)
    {
    debugDisplay.setMultiLine (true);
    debugDisplay.setReturnKeyStartsNewLine (false);
    debugDisplay.setReadOnly (false);
    debugDisplay.setScrollbarsShown (true);
    debugDisplay.setCaretVisible (true);
    debugDisplay.setPopupMenuEnabled (false);
    debugDisplay.setText (String());
    addAndMakeVisible (debugDisplay);
    }

globalAutoRWLabel.setColour (Label::textColourId, Colours::black);
globalAutoRWLabel.setFont (Font (Font::getDefaultMonospacedFontName(), 14.0f, Font::bold));
globalAutoRWLabel.setJustificationType(Justification::centred);
globalAutoRWLabel.setText ("ENABLE", dontSendNotification);
addAndMakeVisible (globalAutoRWLabel);
/*
AutoReadlabel.setColour (Label::textColourId, Colours::black);
AutoReadlabel.setFont (Font (Font::getDefaultMonospacedFontName(), 14.0f, Font::bold));
AutoReadlabel.setJustificationType(Justification::centred);
AutoReadlabel.setText ("READ", dontSendNotification);
addAndMakeVisible (AutoReadlabel);

AutoWritelabel.setColour (Label::textColourId, Colours::black);
AutoWritelabel.setFont (Font (Font::getDefaultMonospacedFontName(), 14.0f, Font::bold));
AutoWritelabel.setJustificationType(Justification::centred);
AutoWritelabel.setText ("WRITE", dontSendNotification);
addAndMakeVisible (AutoWritelabel);

AutoCutWriteLabel.setColour (Label::textColourId, Colours::black);
AutoCutWriteLabel.setFont (Font (Font::getDefaultMonospacedFontName(), 14.0f, Font::bold));
AutoCutWriteLabel.setJustificationType(Justification::centred);
AutoCutWriteLabel.setText ("CUTWRT", dontSendNotification);
addAndMakeVisible (AutoCutWriteLabel);
*/
statSwtLabel.setColour (Label::textColourId, Colours::black);
statSwtLabel.setFont (Font (Font::getDefaultMonospacedFontName(), 14.0f, Font::bold));
statSwtLabel.setJustificationType(Justification::centred);
statSwtLabel.setText ("Status Switch\nToggle Modes", dontSendNotification);
addAndMakeVisible (statSwtLabel);

//-------------------------------------------------------------------	
//EDITOR INIT---------------IMAGE BUTTONS----------------------------  
//-------------------------------------------------------------------	

//---------------mix on on switch-----------------------  
ImgButMixOffOn.setTopLeftPosition(0,0);
if(processor.mixOnOffState)
	ImgButMixOffOn.setImages(true, true, true,mixOn, 1.0f, Colours::transparentBlack, mixOn, 0.8f, Colours::transparentBlack, mixOn, 0.8f,Colours::transparentBlack);
else
	ImgButMixOffOn.setImages(true, true, true,mixOff, 1.0f, Colours::transparentBlack, mixOff, 0.8f, Colours::transparentBlack, mixOff, 0.8f,Colours::transparentBlack);

ImgButMixOffOn.setBounds ((largeFaderW/2)-(105/2),5,105,29);
addAndMakeVisible (ImgButMixOffOn);
ImgButMixOffOn.setTooltip ("TURN SSL ""ON"" OR ""OFF"" AUTOMATION MIX MODE");

ImgButMixOffOn.setVisible(true);

//READ CUT ENALBES SWT
ImgButAutoCutRead.setTopLeftPosition(0,0);
if(processor.globalReadCutAutoState)
	ImgButAutoCutRead.setImages(true, true, true,CutEnableOn, onoffOpacity, Colours::transparentBlack, CutEnableOn, onoffHoverOpacity, Colours::transparentBlack, CutEnableOn, onoffClickedOpacity,Colours::transparentBlack);
else
	ImgButAutoCutRead.setImages(true, true, true,CutEnableOff, onoffOpacity, Colours::transparentBlack, CutEnableOff, onoffHoverOpacity, Colours::transparentBlack, CutEnableOff, onoffClickedOpacity,Colours::transparentBlack);

//READ VOLUME ENABLE SWT
ImgButAutoVolRead.setTopLeftPosition(0,0);
if(processor.globalReadVolAutoState)
	ImgButAutoVolRead.setImages(true, true, true,VolEnableOn, onoffOpacity, Colours::transparentBlack, VolEnableOn, onoffHoverOpacity, Colours::transparentBlack, VolEnableOn, onoffClickedOpacity,Colours::transparentBlack);
 else
	ImgButAutoVolRead.setImages(true, true, true,VolEnableOff, onoffOpacity, Colours::transparentBlack, VolEnableOff, onoffHoverOpacity, Colours::transparentBlack, VolEnableOff, onoffClickedOpacity,Colours::transparentBlack);
    
//WRITE CUTS ENABLE SWT
ImgButAutoCutWrite.setTopLeftPosition(0,0);
if(processor.globalWriteCutAutoState)
	ImgButAutoCutWrite.setImages(true, true, true,CutEnableOn, onoffOpacity, Colours::transparentBlack, CutEnableOn, onoffHoverOpacity, Colours::transparentBlack, CutEnableOn, onoffClickedOpacity,Colours::transparentBlack);
else
	ImgButAutoCutWrite.setImages(true, true, true,CutEnableOff, onoffOpacity, Colours::transparentBlack, CutEnableOff, onoffHoverOpacity, Colours::transparentBlack, CutEnableOff, onoffClickedOpacity,Colours::transparentBlack);

//WRITE VOLUME ENABLE SWT
ImgButAutoVolWrite.setTopLeftPosition(0,0);
if(processor.globalWriteVolAutoState)
	ImgButAutoVolWrite.setImages(true, true, true,VolEnableOn, onoffOpacity, Colours::transparentBlack, VolEnableOn, onoffHoverOpacity, Colours::transparentBlack, VolEnableOn, onoffClickedOpacity,Colours::transparentBlack);
else
	ImgButAutoVolWrite.setImages(true, true, true,VolEnableOff, onoffOpacity, Colours::transparentBlack, VolEnableOff, onoffHoverOpacity, Colours::transparentBlack, VolEnableOff, onoffClickedOpacity,Colours::transparentBlack);

addAndMakeVisible(ImgButAutoVolRead);
addAndMakeVisible(ImgButAutoVolWrite);
addAndMakeVisible(ImgButAutoCutRead);
addAndMakeVisible(ImgButAutoCutWrite);
    

ImgButAutoVolRead.setTooltip ("Globaly enable or disable READ of FADER automation\n (Hold <ALT> for more)");
ImgButAutoCutRead.setTooltip ("Globaly enable or disable READ of CUT automation\n (Hold <ALT> for more)");
ImgButAutoVolWrite.setTooltip ("Globaly enable or disable WRITE of FADER automation\n (Hold <ALT> for more)");
ImgButAutoCutWrite.setTooltip ("Globaly enable or disable WRITE of CUT automation\n (Hold <ALT> for more)");

imgButtonBank8.setTopLeftPosition(0,0);
imgButtonBank8.setTooltip("Set 8 channel view");
imgButtonBank8.setImages(true, true, true,bank8_off, onoffOpacity, Colours::transparentBlack, bank8_off, 0.8f, Colours::transparentBlack, bank8_on, 0.8f,Colours::transparentBlack);
addAndMakeVisible(imgButtonBank8);

imgButtonBank16.setTopLeftPosition(0,0);
imgButtonBank16.setTooltip("Set 16 channel view");
imgButtonBank16.setImages(true, true, true,bank16_off, onoffOpacity, Colours::transparentBlack, bank16_off, 0.8f, Colours::transparentBlack, bank16_on, 0.8f,Colours::transparentBlack);
addAndMakeVisible(imgButtonBank16);

imgButtonBank24.setTopLeftPosition(0,0);
imgButtonBank24.setTooltip("Set 24 channel view");
imgButtonBank24.setImages(true, true, true,bank24_off, onoffOpacity, Colours::transparentBlack, bank24_off, 0.8f, Colours::transparentBlack, bank24_on, 0.8f,Colours::transparentBlack);
addAndMakeVisible(imgButtonBank24);

imgButtonBank32.setTopLeftPosition(0,0);
imgButtonBank32.setTooltip("Set 32 channel view");
imgButtonBank32.setImages(true, true, true,bank32_off, onoffOpacity, Colours::transparentBlack, bank32_off, 0.8f, Colours::transparentBlack, bank32_on, 0.8f,Colours::transparentBlack);
addAndMakeVisible(imgButtonBank32);
imgButtonBank8.setVisible(false);
imgButtonBank16.setVisible(false);
imgButtonBank24.setVisible(false);
imgButtonBank32.setVisible(false);


addAndMakeVisible(imgButtonShiftFirst);
imgButtonShiftFirst.setTopLeftPosition(0,0);
imgButtonShiftFirst.setTooltip  ("MOVE DOWN TO VIEW FIRST BANK");
imgButtonShiftFirst.setImages(true, true, true,ShiftFirstOff, 1.0f, Colours::transparentBlack, ShiftFirstOff, 0.8f, Colours::transparentBlack, ShiftFirstOn, 0.8f,Colours::transparentBlack);
//imgButtonShiftFirst.setBounds (1,bankViewButtonYPos,23,23);

addAndMakeVisible(imgButtonShiftDown);
imgButtonShiftDown.setTopLeftPosition(0,0);
imgButtonShiftDown.setTooltip ("MOVE DOWN 1 BANK");
imgButtonShiftDown.setImages(true, true, true,ShiftDownOff, onoffOpacity, Colours::transparentBlack, ShiftDownOff, 0.8f, Colours::transparentBlack, ShiftDownOn, 0.8f,Colours::transparentBlack);
//imgButtonShiftDown.setBounds (29,bankViewButtonYPos,23,23);

addAndMakeVisible(imgButtonShiftUp);
imgButtonShiftUp.setTopLeftPosition(0,0);
imgButtonShiftUp.setTooltip ("MOVE UP 1 BANK");
imgButtonShiftUp.setImages(true, true, true,ShiftUpOff, onoffOpacity, Colours::transparentBlack, ShiftUpOff, 0.8f, Colours::transparentBlack, ShiftUpOn, 0.8f,Colours::transparentBlack);
//imgButtonShiftUp.setBounds (56,bankViewButtonYPos,23,23);

addAndMakeVisible(imgButtonShiftLast);
imgButtonShiftLast.setTopLeftPosition(0,0);
imgButtonShiftLast.setTooltip ("MOVE UP TO VIEW LAST BANK");
imgButtonShiftLast.setImages(true, true, true,ShiftLastOff, onoffOpacity, Colours::transparentBlack, ShiftLastOff, 0.8f, Colours::transparentBlack, ShiftLastOn, 0.8f,Colours::transparentBlack);
//imgButtonShiftLast.setBounds (83,bankViewButtonYPos,23,23);

for(int n=0;n<6;n++)
    {
	addAndMakeVisible(faderStatusSwt[n]);
	faderStatusSwt[n].setTopLeftPosition(0,0);
	faderStatusSwt[n].setImages(false, true, false,automodesw_off[n], onoffOpacity, Colours::transparentBlack, automodesw_off[n], onoffHoverOpacity, Colours::transparentBlack, automodesw_off[n], onoffClickedOpacity,Colours::transparentBlack);
	faderStatusSwt[n].setVisible(true);
	}	

//taiPung button
//---------------mix on on switch-----------------------  
ImgButGameCtrl1.setTopLeftPosition(0,0);
ImgButGameCtrl1.setImages(false, true, true,stop, onoffOpacity, Colours::transparentBlack, stop, onoffHoverOpacity, Colours::transparentBlack, stop, onoffClickedOpacity,Colours::transparentBlack);
ImgButGameCtrl1.setBounds ((largeFaderW/2)-(105/2),45,105,29);
addAndMakeVisible (ImgButGameCtrl1);
//ImgButGameCtrl1.setTooltip ("STOP the Game");

ImgButGameCtrl2.setTopLeftPosition(0,0);
ImgButGameCtrl2.setImages(false, true, true,play, onoffOpacity, Colours::transparentBlack, play, onoffHoverOpacity, Colours::transparentBlack, play, onoffClickedOpacity,Colours::transparentBlack);
ImgButGameCtrl2.setBounds ((largeFaderW/2)-(105/2),45,105,29);
addAndMakeVisible (ImgButGameCtrl2);
//ImgButGameCtrl2.setTooltip ("START the Game");

ImgButGameCtrl3.setTopLeftPosition(0,0);
ImgButGameCtrl3.setImages(false, true, true,pause, onoffOpacity, Colours::transparentBlack, pause, onoffHoverOpacity, Colours::transparentBlack, pause, onoffClickedOpacity,Colours::transparentBlack);
ImgButGameCtrl3.setBounds ((largeFaderW/2)-(105/2),85,105,29);
addAndMakeVisible (ImgButGameCtrl3);
//ImgButGameCtrl3.setTooltip ("PAUSE the Game");

ImgButGameCtrl4.setTopLeftPosition(0,0);
ImgButGameCtrl4.setImages(false, true, true,resume, onoffOpacity, Colours::transparentBlack, resume, onoffHoverOpacity, Colours::transparentBlack, resume, onoffClickedOpacity,Colours::transparentBlack);
ImgButGameCtrl4.setBounds ((largeFaderW/2)-(105/2),85,105,29);
addAndMakeVisible (ImgButGameCtrl4);
//ImgButGameCtrl4.setTooltip ("RESUME the Game");

ImgButGameCtrl5.setTopLeftPosition(0,0);
ImgButGameCtrl5.setImages(false, true, true,newgame, onoffOpacity, Colours::transparentBlack, newgame, onoffHoverOpacity, Colours::transparentBlack, newgame, onoffClickedOpacity,Colours::transparentBlack);
ImgButGameCtrl5.setBounds ((largeFaderW/2)-(105/2),45,105,29);
addAndMakeVisible (ImgButGameCtrl5);
//ImgButGameCtrl5.setTooltip ("new Game");
//----------------------------------------------------------------------------------------------
ImgButGameCtrl1.addListener (this);
ImgButGameCtrl2.addListener (this);
ImgButGameCtrl3.addListener (this);
ImgButGameCtrl4.addListener (this);
ImgButGameCtrl5.addListener (this);
setLeftHandGameButtonVisibleState(false);


versionNumber.setColour(juce::Label::textColourId,Colours::orange);
versionNumber.setText(_VERSION, dontSendNotification);
versionNumber.setCentrePosition(versionNumber.getWidth()/2,0);
versionNumber.setFont (Font (9.00f, Font::bold));
addAndMakeVisible (versionNumber);


AlertLabel.setColour (Label::backgroundColourId, Colours::green);
AlertLabel.setFont(Font (15.0f, Font::bold));
AlertLabel.setJustificationType (Justification::centred);
AlertLabel.setColour(Label::textColourId,Colours::yellow);
AlertLabel.setText("THE INJEKTOR APPLICATION MUST BE RUNNING TO USE THE MOTHERSHIP\n\nLOOKS LIKE IT IS NOT...\n\n Please start the INJEKTOR application", dontSendNotification);
AlertLabel.setSize(300, 200);
AlertLabel.setAlwaysOnTop(true);
AlertLabel.setBounds((getWidth()/2)-(AlertLabel.getWidth()/2), 
							(getHeight()/2)-(AlertLabel.getHeight()/2), 
							AlertLabel.getWidth(),
							AlertLabel.getHeight());
addAndMakeVisible (AlertLabel);
AlertLabel.setVisible(false);


//addAndMakeVisible(cutWrite);

vcaIndex=1;
//----------------------------------------------------------------------------------------------
// RIGHT SIDE SECTION
//----------------------------------------------------------------------------------------------

for(int i = 0; i < processor.SystemSize; i++)
	{
	TheSliders.add(new Slider(String(i)));
	TheSliders.getLast()->setTopLeftPosition(i*sFaderW,0);
	TheSliders.getLast()->setSliderStyle(Slider::SliderStyle::LinearVertical);
	TheSliders.getLast()->setColour(Slider::ColourIds::backgroundColourId,Colours::black);
	TheSliders.getLast()->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 0,0);
    TheSliders.getLast()->setRange(0.000f, 1.000f,0.001f);
    TheSliders.getLast()->setSkewFactor(1.0f);
    TheSliders.getLast()->setValue(0.0f);
	addAndMakeVisible(TheSliders.getLast());
	if(i<inputChnCnt)
		{
		ImgButLink.add(new ImageButton(String(i)));
		ImgButLink.getLast()->setComponentID("L"+String(i+1));
		ImgButLink.getLast()->setAlwaysOnTop(true);
		ImgButLink.getLast()->setImages(false, true, true,
										ImgLinkOff, 1.0f, Colours::transparentBlack,
										ImgLinkOff, 1.0f, Colours::transparentWhite,
										ImgLinkOff, 1.0f,Colours::transparentBlack,
										0.0f);
		ImgButLink.getLast()->setBounds(int((largeFaderW+(i*sFaderW)+35)/scalefactor),
   	                                   int((sFaderH-385)/scalefactor),
   	                                   16,
   	                                   16);
		addAndMakeVisible (ImgButLink.getLast());
		ImgButLink.getLast()->setVisible(false);
		}
	TheMutes.add(new ImageButton(String(i)));
    TheMutes.getLast()->setTopLeftPosition((i*sFaderW)+(sFaderW/2)-((mute_on.getWidth()-mutebtntrim)/2),20);
	TheMutes.getLast()->setImages(false, true, false,mute_off, 1.0f, Colours::transparentBlack, mute_off, 0.8f, Colours::transparentBlack, mute_off, 0.8f,Colours::transparentBlack);
    addAndMakeVisible(TheMutes.getLast());
	TheMutes.getLast()->setVisible(false);

	cutWrite.add(new cutWriteObject(String(i)));
    addAndMakeVisible(cutWrite.getLast());
	cutWrite.getLast()->setVisible(false);

    sslTrackNumber.add(new Label(String(i)));
    if(i<inputChnCnt)
       	{
       	sslTrackNumber.getLast()->setText (String(i+1), dontSendNotification);
		sslTrackNumber.getLast()->setFont (Font (20.0f, Font::bold));
		sslTrackNumber.getLast()->setMinimumHorizontalScale(1.0f);
		}
	else
    	{
       	sslTrackNumber.getLast()->setText (String(vcaIndex), dontSendNotification);
		sslTrackNumber.getLast()->setFont (Font (16.0f, Font::bold));
		sslTrackNumber.getLast()->setMinimumHorizontalScale(1.0f);
		}

	sslTrackNumber.getLast()->setColour (Label::textColourId, Colours::black);
	sslTrackNumber.getLast()->setJustificationType (Justification::centred);
    addAndMakeVisible (sslTrackNumber.getLast());
	
	if(i>=inputChnCnt)
		{
 		sslVCAlabel.add(new Label(String(vcaIndex-1)));
    	sslVCAlabel.getLast()->setFont (Font (16.0f, Font::bold));
    	sslVCAlabel.getLast()->setMinimumHorizontalScale(1.0f);
   		sslVCAlabel.getLast()->setText ("VCA", dontSendNotification);
		sslVCAlabel.getLast()->setColour (Label::textColourId, Colours::black);
		sslVCAlabel.getLast()->setJustificationType (Justification::centred);
    	addAndMakeVisible (sslVCAlabel.getLast());
		vcaIndex++;
		}

    tNameLabel.add(new Label(String(i)));
    tNameLabel.getLast()->setFont (Font (12.0f, Font::bold));
	tNameLabel.getLast()->setMinimumHorizontalScale(0.5f);
	tNameLabel.getLast()->setText (String(i+1), dontSendNotification);
	tNameLabel.getLast()->setColour (Label::textColourId, Colours::black);
	tNameLabel.getLast()->setColour (Label::backgroundColourId, Colours::transparentWhite);
	tNameLabel.getLast()->setJustificationType (Justification::centred);
    addAndMakeVisible (tNameLabel.getLast());

    TheValueLabels.add(new Label(String(i)));
    TheValueLabels.getLast()->setFont (Font (12.0f, Font::bold));
	TheValueLabels.getLast()->setText (String(10-((1000-TheSliders.getLast()->getValue())/10)),dontSendNotification);
	TheValueLabels.getLast()->setColour (Label::textColourId, Colours::cyan);
	TheValueLabels.getLast()->setJustificationType (Justification::centred);
    addAndMakeVisible (TheValueLabels.getLast());

    ThedbLabels.add(new Label(String(i)));
    ThedbLabels.getLast()->setFont (Font (12.0f, Font::bold));
	ThedbLabels.getLast()->setText ("db", dontSendNotification);
	ThedbLabels.getLast()->setColour (Label::textColourId, Colours::black);
	ThedbLabels.getLast()->setJustificationType (Justification::centred);
    addAndMakeVisible (ThedbLabels.getLast());
    
    TheAutoLabels.add(new Label(String(i)));
    TheAutoLabels.getLast()->setFont (Font (10.0f, Font::bold));
	switch(processor.pluginAutoMode[i])
		{
		case 0:	TheAutoLabels.getLast()->setText ("OFF", dontSendNotification);
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::grey);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::white);	
				break;
		case 1:	TheAutoLabels.getLast()->setText ("WRITE", dontSendNotification);
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::red);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::white);	
				break;
		case 2:	if(processor.ConfigReg[10]&0x80)
					TheAutoLabels.getLast()->setText ("TOUCH", dontSendNotification);
				else	
					TheAutoLabels.getLast()->setText ("LATCH", dontSendNotification);
				
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::orange);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::white);	
				break;
		case 3:	TheAutoLabels.getLast()->setText ("READ", dontSendNotification);
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::green);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::white);	
				break;
		case 5:	TheAutoLabels.getLast()->setText ("TRIM WRITE", dontSendNotification);
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::red);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::yellow);	
				break;
		case 6:	if(processor.ConfigReg[10]&0x80)
					TheAutoLabels.getLast()->setText ("TRIM TOUCH", dontSendNotification);
				else
					TheAutoLabels.getLast()->setText ("TRIM TOUCH", dontSendNotification);
				
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::orange);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::white);	
				break;
		case 7:	TheAutoLabels.getLast()->setText ("TRIM READ", dontSendNotification);
				TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::green);
		    	TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::yellow);	
				break;
		}
	TheAutoLabels.getLast()->setColour (Label::backgroundColourId, Colours::grey);
    TheAutoLabels.getLast()->setColour (Label::textColourId, Colours::white);
	TheAutoLabels.getLast()->setJustificationType (Justification::centred);
    addAndMakeVisible (TheAutoLabels.getLast());       

	}

//---
updateTrackProperties();
//---
for(int i = 0; i < processor.SystemSize; i++)
	TheSliders[i]->addListener(this);

for(int i = 0; i < inputChnCnt; i++)
	ImgButLink[i]->addListener(this);

ImgButMixOffOn.addListener (this);
ImgButAutoCutRead.addListener (this);
ImgButAutoVolRead.addListener (this);
ImgButAutoCutWrite.addListener (this);
ImgButAutoVolWrite.addListener (this);

imgButtonShiftFirst.addListener (this);
imgButtonShiftDown.addListener (this);
imgButtonShiftUp.addListener (this);
imgButtonShiftLast.addListener (this);

imgButtonBank8.addListener (this);
imgButtonBank16.addListener (this);
imgButtonBank24.addListener (this);
imgButtonBank32.addListener (this);
for(int n=0; n<6;n++)
	{
	faderStatusSwt[n].addListener (this);
	faderStatusSwt[n].addMouseListener(this, false);
	}

if(processor.viewSize>processor.SystemSize)
	processor.viewSize=processor.SystemSize;

setLeftHandButtonVisibleState(false);
//bank=processor.bank;

int tmpviewsize = processor.viewSize;
setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
					largeFaderW+(sFaderW*tmpviewsize),sFaderH);
setResizable(true,true);
setSize (largeFaderW+(sFaderW*8),sFaderH);

startTimerHz(30);
resized();
}

//===============================================================================================
//~TaiMotherShipPluguinProcessorEditor()
//
//===============================================================================================
TaiMotherShipPluguinProcessorEditor::~TaiMotherShipPluguinProcessorEditor()
{
setLookAndFeel (nullptr);
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::paint (Graphics& g)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::paint (Graphics& g)
{
if(!processor.taiPong.getGameState())
	{
	if (SystemSizeRcvdFlag)
		{
		g.fillAll(Colours::black);
		g.drawImage (blankWideFaderPlate,
				-2,2,																		//Position
				int(largeFaderW/scalefactor),int(largeFaderH/scalefactor),					//Dimension of space to draw
				0,0,																		//Source x,y
				largeFaderW,largeFaderH,													//Dimension of source images
				false);

	  	g.setColour (Colours::black);
  		g.setOpacity (1.0f);
  		g.drawRoundedRectangle(2,38,103,115,5,1);
  		g.drawRoundedRectangle(2,158,103,65,5,1);
  		g.drawRoundedRectangle(2,229,103,211,5,1);
	  	g.setFont((Font(Font::getDefaultMonospacedFontName(), 13.0f, Font::bold)));
  		g.setColour (Colours::black);
		g.drawSingleLineText("ENABLE",37,50,Justification::left);
		g.drawSingleLineText("READ",14,61,Justification::left);
		g.drawSingleLineText("WRITE",65,61,Justification::left);

//paint===============================================================================================
		g.drawSingleLineText("VIEW SIZE",25,170,Justification::left);
		if(processor.taiType!=2)
			{
			if(!processor.shiftKeyState)
				{
				g.setFont((Font(Font::getDefaultMonospacedFontName(), 15.0f, Font::bold)));
  				g.drawSingleLineText("FADER STATUS",7,253,Justification::left);
				//g.drawSingleLineText("TOGGLE MODES",11,253,Justification::left);
				}
			else
				{
				g.setFont((Font(Font::getDefaultMonospacedFontName(), 15.0f, Font::bold)));
  				g.drawSingleLineText("SET ALL",28,253,Justification::left);
				//g.drawSingleLineText("CHANNELS TO",17,253,Justification::left);
				}
			}
    	int xoffset=0;
    
 	//paint===============================================================================================
 	//Fader Mix view section
 	//t===============================================================================================
		for(int i = 0; i < processor.viewSize; i=i+8)
			{
      		g.setOpacity (1.0f);
      		g.drawImage (FaderPlatex8,
    					int((int(largeFaderW/scalefactor)+((xoffset)*FaderPlatex8.getWidth())/scalefactor)),0,			//Position
    					int(FaderPlatex8.getWidth()/scalefactor),int(FaderPlatex8.getHeight()/scalefactor),									//dimension of space to draw
    					0,0,																												//source x,y
    					FaderPlatex8.getWidth(),FaderPlatex8.getHeight(),
    					false);
  			if(processor.taiType==0)
  				{
  				g.drawImage (vcaFaderScalex8,
  					int((int(largeFaderW/scalefactor)+((xoffset)*vcaFaderScalex8.getWidth())/scalefactor)),			//x Position
  					10,																													//y Position
  					int(vcaFaderScalex8.getWidth()/scalefactor),int(vcaFaderScalex8.getHeight()/scalefactor),							//dimension of space to draw
  					0,0,																												//source x,y
  					vcaFaderScalex8.getWidth(),vcaFaderScalex8.getHeight(),
  					false);
  				}
 	 		else
  			if(processor.taiType==1)
  				{
  				g.drawImage (ufFaderScalex8,
  						int((int(largeFaderW/scalefactor)+((xoffset)*ufFaderScalex8.getWidth())/scalefactor)),13,			//Position
  						int(ufFaderScalex8.getWidth()/scalefactor),int(ufFaderScalex8.getHeight()/scalefactor),									//dimension of space to draw
  						0,0,																												//source x,y
  						ufFaderScalex8.getWidth(),ufFaderScalex8.getHeight(),
   	 					false);
   		 		}
	 		else
  			if(processor.taiType==2)
  				{
  				g.drawImage (ufFaderScalex8,
  						int((int(largeFaderW/scalefactor)+((xoffset)*ufFaderScalex8.getWidth())/scalefactor)),13,			//Position
  						int(ufFaderScalex8.getWidth()/scalefactor),int(ufFaderScalex8.getHeight()/scalefactor),									//dimension of space to draw
  						0,0,																												//source x,y
  						ufFaderScalex8.getWidth(),ufFaderScalex8.getHeight(),
   	 					false);
   		 		}
			
			if(!processor.mixOnOffState)
			    {
			    g.setColour (Colours::darkgrey);
			    g.setOpacity(0.3);
    			g.fillRect (int((int(largeFaderW/scalefactor)+((xoffset)*FaderPlatex8.getWidth())/scalefactor)),0,
    					int(ufFaderScalex8.getWidth()/scalefactor),int(ufFaderScalex8.getHeight()/scalefactor));
				}
    		xoffset++;
			}
		}
  	}
else
	paintPung(g);
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::resized()
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::resized()
{
versionNumber.setBounds (15, getHeight()-15,180, 15);

const int margin = 10;
if(SystemSizeRcvdFlag)
	{
//-------------------------------------------------------------------	
//resized()--------------  MIX ON-OFF SWITCH  -----------------------  
//-------------------------------------------------------------------
    if(processor.mixOnOffState)
       	ImgButMixOffOn.setImages(true, true, true,mixOn, 1.0f, Colours::transparentBlack, mixOn, 0.8f, Colours::transparentBlack, mixOn, 0.8f,Colours::transparentBlack);
	else
    	ImgButMixOffOn.setImages(true, true, true,mixOff, 1.0f, Colours::transparentBlack, mixOff, 0.8f, Colours::transparentBlack, mixOff, 0.8f,Colours::transparentBlack);
	ImgButMixOffOn.setBounds ((largeFaderW/2)-(105/2),5,105,29);


//-------------------------------------------------------------------	
//resized()-----------------DEBUG WINDOWS----------------------------  
//-------------------------------------------------------------------	
	if(debugDisplayState==1)
		debugDisplay.setVisible(true);
	else
		debugDisplay.setVisible(false);


	debugDisplay.setBounds (margin, getHeight()-debugDisplayHeight, getWidth() - (2*margin), getHeight() - margin);

//-------------------------------------------------------------------	
//resized()---------------  ALERT WINDOW  ---------------------------  
//-------------------------------------------------------------------	
 	AlertLabel.setBounds((getWidth()/2)-(AlertLabel.getWidth()/2), 
							(getHeight()/2)-(AlertLabel.getHeight()/2), 
							AlertLabel.getWidth(),
							AlertLabel.getHeight());
//-------------------------------------------------------------------	
//resized()-------------- LEFT HAND BUTTON  -------------------------  
//-------------------------------------------------------------------

	if(!processor.taiPong.getGameState())
		
		{	
		//-------------------------------------------------------------	
		//resized()-----  READ/WRITE ENEABLE SWITCH  ------------------ 
		//-------------------------------------------------------------
		//READ CUT ENALBES SWT
		if(processor.globalReadCutAutoState)
			ImgButAutoCutRead.setImages(true, true, true,CutEnableOn, onoffOpacity, Colours::transparentBlack, CutEnableOn, onoffHoverOpacity, Colours::transparentBlack, CutEnableOn, onoffClickedOpacity,Colours::transparentBlack);
		else
			ImgButAutoCutRead.setImages(true, true, true,CutEnableOff, onoffOpacity, Colours::transparentBlack, CutEnableOff, onoffHoverOpacity, Colours::transparentBlack, CutEnableOff, onoffClickedOpacity,Colours::transparentBlack);	

		//READ VOLUME ENABLE SWT
		if(processor.globalReadVolAutoState)
			ImgButAutoVolRead.setImages(true, true, true,VolEnableOn, onoffOpacity, Colours::transparentBlack, VolEnableOn, onoffHoverOpacity, Colours::transparentBlack, VolEnableOn, onoffClickedOpacity,Colours::transparentBlack);
		else
			ImgButAutoVolRead.setImages(true, true, true,VolEnableOff, onoffOpacity, Colours::transparentBlack, VolEnableOff, onoffHoverOpacity, Colours::transparentBlack, VolEnableOff, onoffClickedOpacity,Colours::transparentBlack);
    
		//WRITE CUTS ENABLE SWT
		if(processor.globalWriteCutAutoState)
			ImgButAutoCutWrite.setImages(true, true, true,CutEnableOn, onoffOpacity, Colours::transparentBlack, CutEnableOn, onoffHoverOpacity, Colours::transparentBlack, CutEnableOn, onoffClickedOpacity,Colours::transparentBlack);
		else
			ImgButAutoCutWrite.setImages(true, true, true,CutEnableOff, onoffOpacity, Colours::transparentBlack, CutEnableOff, onoffHoverOpacity, Colours::transparentBlack, CutEnableOff, onoffClickedOpacity,Colours::transparentBlack);

		//WRITE VOLUME ENABLE SWT
		if(processor.globalWriteVolAutoState)
			ImgButAutoVolWrite.setImages(true, true, true,VolEnableOn, onoffOpacity, Colours::transparentBlack, VolEnableOn, onoffHoverOpacity, Colours::transparentBlack, VolEnableOn, onoffClickedOpacity,Colours::transparentBlack);
		else
			ImgButAutoVolWrite.setImages(true, true, true,VolEnableOff, onoffOpacity, Colours::transparentBlack, VolEnableOff, onoffHoverOpacity, Colours::transparentBlack, VolEnableOff, onoffClickedOpacity,Colours::transparentBlack);


		ImgButAutoVolRead.setBounds (sw1x,sw1y,sw1w,sw1w);
   	 	ImgButAutoVolWrite.setBounds (sw2x,sw2y,sw2w,sw2h);
   		ImgButAutoCutRead.setBounds (sw3x,sw3y,sw3w,sw3h);
		ImgButAutoCutWrite.setBounds (sw4x,sw4y,sw4w,sw4h);
 
//------------------------------------------------------------	
//resized()---------  BANK VIEW SWITCHS  ---------------------  
//------------------------------------------------------------

		imgButtonBank8.setImages(true, true, true,bank8_off, onoffOpacity, Colours::transparentBlack, bank8_off, onoffHoverOpacity, Colours::transparentBlack, bank8_on, onoffClickedOpacity,Colours::transparentBlack);
		imgButtonBank16.setImages(true, true, true,bank16_off, onoffOpacity, Colours::transparentBlack, bank16_off, onoffHoverOpacity, Colours::transparentBlack, bank16_on, onoffClickedOpacity,Colours::transparentBlack);
		imgButtonBank24.setImages(true, true, true,bank24_off, onoffOpacity, Colours::transparentBlack, bank24_off, onoffHoverOpacity, Colours::transparentBlack, bank24_on, onoffClickedOpacity,Colours::transparentBlack);
		imgButtonBank32.setImages(true, true, true,bank32_off, onoffOpacity, Colours::transparentBlack, bank32_off, onoffHoverOpacity, Colours::transparentBlack, bank32_on, 0.8f,Colours::transparentBlack);
		switch (processor.viewSize)
			{
  			case 8:	imgButtonBank8.setImages(true, true, true,bank8_on, onoffOpacity, Colours::transparentBlack, bank8_on, onoffHoverOpacity, Colours::transparentBlack, bank8_on, onoffClickedOpacity,Colours::transparentBlack);
			break;
  			case 16:imgButtonBank16.setImages(true, true, true,bank16_on, onoffOpacity, Colours::transparentBlack, bank16_on, onoffHoverOpacity, Colours::transparentBlack, bank16_on, onoffClickedOpacity,Colours::transparentBlack);
  			break;
  			case 24:imgButtonBank24.setImages(true, true, true,bank24_on, onoffOpacity, Colours::transparentBlack, bank24_on, onoffHoverOpacity, Colours::transparentBlack, bank24_on, onoffClickedOpacity,Colours::transparentBlack);
  			break;
  			case 32:imgButtonBank32.setImages(true, true, true,bank32_on, onoffOpacity, Colours::transparentBlack, bank32_on, onoffHoverOpacity, Colours::transparentBlack, bank32_on, onoffClickedOpacity,Colours::transparentBlack);
  			break;
	 		default:
    		break;
			}

		imgButtonBank8.setBounds(4,viewSizeButtonYPos,23,23);
		imgButtonBank16.setBounds(30,viewSizeButtonYPos,23,23);
		imgButtonBank24.setBounds(56,viewSizeButtonYPos,23,23);
		imgButtonBank32.setBounds(82,viewSizeButtonYPos,23,23);
		
		imgButtonShiftFirst.setImages(true, true, true,ShiftFirstOff, onoffOpacity, Colours::transparentBlack, ShiftFirstOff, onoffHoverOpacity, Colours::transparentBlack, ShiftFirstOn, onoffClickedOpacity,Colours::transparentBlack);
		imgButtonShiftDown.setImages(true, true, true,ShiftDownOff, onoffOpacity, Colours::transparentBlack, ShiftDownOff, onoffHoverOpacity, Colours::transparentBlack, ShiftDownOn, onoffClickedOpacity,Colours::transparentBlack);
		imgButtonShiftUp.setImages(true, true, true,ShiftUpOff, onoffOpacity, Colours::transparentBlack, ShiftUpOff, onoffHoverOpacity, Colours::transparentBlack, ShiftUpOn, onoffClickedOpacity,Colours::transparentBlack);
		imgButtonShiftLast.setImages(true, true, true,ShiftLastOff, onoffOpacity, Colours::transparentBlack, ShiftLastOff, onoffHoverOpacity, Colours::transparentBlack, ShiftLastOn, onoffClickedOpacity,Colours::transparentBlack);

		imgButtonShiftFirst.setBounds (4,bankViewButtonYPos,23,23);
		imgButtonShiftDown.setBounds (30,bankViewButtonYPos,23,23);
		imgButtonShiftUp.setBounds (56,bankViewButtonYPos,23,23);
		imgButtonShiftLast.setBounds (82,bankViewButtonYPos,23,23);

//---------------------------------------------------------------
//resized()------  FADER STATUS PRESET SWITCHS  -----------------------
//---------------------------------------------------------------	
		if(!autoToggleSwtHover)
			doFaderStatus();
//resized()===============================================================================================
		else
			{
			if(processor.shiftKeyState)
				doSetAll();
			else
				doFaderStatus();
			}
		setLeftHandButtonVisibleState(true);
		repaint();
		rebuildMixView();
		
		}
	else
		{
		setLeftHandButtonVisibleState(false);
		ImgButGameCtrl1.setImages(false, true, true,stop, onoffOpacity, Colours::transparentBlack, stop, onoffHoverOpacity, Colours::transparentBlack, stop, onoffClickedOpacity,Colours::transparentBlack);
		ImgButGameCtrl2.setImages(false, true, true,play, onoffOpacity, Colours::transparentBlack, play, onoffHoverOpacity, Colours::transparentBlack, play, onoffClickedOpacity,Colours::transparentBlack);
		ImgButGameCtrl3.setImages(false, true, true,pause, onoffOpacity, Colours::transparentBlack, pause, onoffHoverOpacity, Colours::transparentBlack, pause, onoffClickedOpacity,Colours::transparentBlack);
		ImgButGameCtrl4.setImages(false, true, true,resume, onoffOpacity, Colours::transparentBlack, resume, onoffHoverOpacity, Colours::transparentBlack, resume, onoffClickedOpacity,Colours::transparentBlack);
		ImgButGameCtrl5.setImages(false, true, true,newgame, onoffOpacity, Colours::transparentBlack, newgame, onoffHoverOpacity, Colours::transparentBlack, newgame, onoffClickedOpacity,Colours::transparentBlack);

		switch(processor.taiPong.getGameState())
			{
			case STOP:
			case RESET:
				ImgButGameCtrl1.setVisible(false);	
				ImgButGameCtrl2.setVisible(true);
				ImgButGameCtrl3.setVisible(false);
				ImgButGameCtrl4.setVisible(false);
				ImgButGameCtrl5.setVisible(false);
			break;
			case PLAY:
				ImgButGameCtrl1.setVisible(true);
				ImgButGameCtrl2.setVisible(false);
				ImgButGameCtrl3.setVisible(true);
				ImgButGameCtrl4.setVisible(false);
				ImgButGameCtrl5.setVisible(false);
			break;
			case PAUSE:
				ImgButGameCtrl1.setVisible(true);
				ImgButGameCtrl2.setVisible(false);
				ImgButGameCtrl3.setVisible(false);
				ImgButGameCtrl4.setVisible(true);
				ImgButGameCtrl5.setVisible(false);
			break;
			case WINNER:
				ImgButGameCtrl1.setVisible(false);
				ImgButGameCtrl2.setVisible(false);
				ImgButGameCtrl3.setVisible(false);
				ImgButGameCtrl4.setVisible(false);
				ImgButGameCtrl5.setVisible(true);
			break;
			case DEMO:
				ImgButGameCtrl1.setVisible(false);
				ImgButGameCtrl2.setVisible(false);
				ImgButGameCtrl3.setVisible(false);
				ImgButGameCtrl4.setVisible(false);
				ImgButGameCtrl5.setVisible(true);
			break;
			}
		}
	resizeTrigger=false;
	}
}

//------------------------------------------------------------------------------
//void TaiMotherShipPluguinProcessorEditor::doFaderStatus(void)
//
//
//------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessorEditor::doFaderStatus(void)
{
	for(char n=0;n<6;n++)
		{
		faderStatusSwt[n].setImages(false, true, false,automodesw_off[n], onoffOpacity, Colours::transparentBlack, automodesw_off[n], onoffHoverOpacity, Colours::transparentBlack, automodesw_off[n], onoffClickedOpacity,Colours::transparentBlack);
		faderStatusSwt[n].setBounds (3,392-(n*27),101,27);
		if(processor.taiType==2)
			faderStatusSwt[n].setVisible(false);
		else
			faderStatusSwt[n].setVisible(true);

		}
	if(processor.mixOnOffState)
		{
		if((processor.statusSwtPreset)&&(processor.statusSwtPreset<7))
			{
			faderStatusSwt[processor.statusSwtPreset-1].setImages(false, true, false,
																	automodesw_on[processor.statusSwtPreset-1], onoffOpacity,
																	Colours::transparentBlack,
																	automodesw_on[processor.statusSwtPreset-1], onoffHoverOpacity,
																	Colours::transparentBlack,
																	automodesw_on[processor.statusSwtPreset-1], onoffClickedOpacity,
																	Colours::transparentBlack);
			}
		}
}

//------------------------------------------------------------------------------
//void TaiMotherShipPluguinProcessorEditor::doSetAll(void)
//
//
//------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessorEditor::doSetAll(void)
{
	for(char n=0;n<6;n++)
		{
		if(!processor.ctrlKeyState)
			{
			faderStatusSwt[n].setImages(false, true, false,setAllAbsImg[n], onoffOpacity, Colours::transparentBlack, setAllAbsImg[n], onoffHoverOpacity, Colours::transparentBlack, setAllAbsImg[n], onoffClickedOpacity,Colours::transparentBlack);
			faderStatusSwt[n].setBounds (3,228+((n+1)*27),101,27);
			faderStatusSwt[n].setVisible(true);
			}
		else
			{
			faderStatusSwt[n].setImages(false, true, false,setAllTrimImg[n], onoffOpacity, Colours::transparentBlack, setAllTrimImg[n], onoffHoverOpacity, Colours::transparentBlack, setAllTrimImg[n], onoffClickedOpacity,Colours::transparentBlack);
			faderStatusSwt[n].setBounds (3,228+((n+1)*27),101,27);
			faderStatusSwt[n].setVisible(true);
			}
		}
}

//------------------------------------------------------------------------------
//void TaiMotherShipPluguinProcessorEditor::rebuildMixView(void)
//
//
//------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessorEditor::rebuildMixView(void)
{
if(!processor.taiPong.getGameState())
	{
	for(int n=0;n<inputChnCnt;n++)
		{
	    ImgButLink[n]->setVisible(false);
	    MyLookAndFeel.MLFPassLinkState(n,processor.ChannelLinkMap[n]);
	    }
	int vcaIndex=0;
	for(int n=0;n<processor.SystemSize;n++)
		{
		
		TheMutes[n]->setVisible(false);
		sslTrackNumber[n]->setVisible(false);
		if(n>=(inputChnCnt))
			sslVCAlabel[vcaIndex++]->setVisible(false);
		tNameLabel[n]->setVisible(false);
		TheAutoLabels[n]->setVisible(false);
		TheSliders[n]->setVisible(false);
		TheValueLabels[n]->setVisible(false);
		}
		
//rebuildMixView(void)-----------------------------------------------------------------------------

  	vcaIndex=0;
	int chnidx=0;
	int vsize=processor.viewSize;
	for(int i = 0; i < vsize; i++)
		{
		chnidx=i+(processor.bank*8);
		if(i<processor.viewSize)
			{
			
			if(processor.editorCutstate[chnidx])
				TheMutes[chnidx]->setImages(false, true, false,mute_on, onoffOpacity, Colours::transparentBlack, mute_on, onoffHoverOpacity, Colours::transparentBlack, mute_on, onoffClickedOpacity,Colours::transparentBlack);
			else
				TheMutes[chnidx]->setImages(false, true, false,mute_off, onoffOpacity, Colours::transparentBlack, mute_off, onoffHoverOpacity, Colours::transparentBlack, mute_off, onoffClickedOpacity,Colours::transparentBlack);
			TheMutes[chnidx]->setVisible(true);
			TheMutes[chnidx]->setBounds( int((largeFaderW+(i*sFaderW)+(sFaderW/2)-((mute_off.getWidth()-mutebtntrim)/2))/scalefactor),
											int(5/scalefactor),
											int((mute_off.getWidth()-mutebtntrim)/scalefactor),
											int((mute_off.getWidth()-mutebtntrim)/scalefactor));
           //the write cut indicators --------------------------------------------------------------------------------
            cutWrite[chnidx]->setBounds(int(largeFaderW+(i*sFaderW)+(sFaderW/2))-int((cutWrite[chnidx]->getWidth())/2),
											5,
											cutWrite[chnidx]->getWidth(),
											cutWrite[chnidx]->getWidth());  

			//printf("processor.ChannelWriteCut[%d]= %d \n",processor.ChannelWriteCut[chnidx],chnidx);
			if(processor.ChannelWriteCut[chnidx])
				cutWrite[chnidx]->setVisible(true);
			else
				cutWrite[chnidx]->setVisible(false);
			cutWrite[chnidx]->toFront(true);
			//-----------------------------------------------------------------------------------------------------------
			TheAutoLabels[chnidx]->setVisible(true);
			TheAutoLabels[chnidx]->setBounds(int((largeFaderW+(i*sFaderW)+(sFaderW/2)-((sFaderW-10)/2))/scalefactor),
  									int(53/scalefactor),
  									int((sFaderW-10)/scalefactor),
  									int(20/scalefactor));
  		
			TheSliders[chnidx]->setVisible(true);
			TheSliders[chnidx]->setBounds(
              						int((largeFaderW+(i*sFaderW)+(sFaderW/2)-(50/2))/scalefactor),//x pos
  									int((103-15/*25*/)/scalefactor),//y pos
  									int(50/scalefactor),
  									int(340/scalefactor));
  			
			TheValueLabels[chnidx]->setVisible(true);
			TheValueLabels[chnidx]->setBounds(int((largeFaderW
              											+(i*sFaderW)
              											+(sFaderW/2)
              											-18)/scalefactor),
  									int((sFaderH-41)/scalefactor),
  									int(37/scalefactor),
  									int(18/scalefactor));
  									
          	sslTrackNumber[chnidx]->setVisible(true);
   			if((processor.isMappedToSinglechannel[chnidx]&0x1))
				{
				tNameLabel[chnidx]->setColour (Label::backgroundColourId, Colours::orange);
				tNameLabel[chnidx]->setText (processor.ptTrackName[chnidx].substring(0, 5), dontSendNotification);
				}
			else
				{
				tNameLabel[chnidx]->setText (processor.ptTrackName[chnidx].substring(0, 5), dontSendNotification);
				tNameLabel[chnidx]->setColour (Label::backgroundColourId, Colours::transparentWhite);
				}

//rebuildMixView(void)-----------------------------------------------------------------------------
			if((chnidx)<(inputChnCnt))
				{
				sslTrackNumber[chnidx]->setBounds(int((largeFaderW+(i*sFaderW))/scalefactor),
                                                  int((sFaderH-387)/scalefactor),
                                                  int(sFaderW/scalefactor),
                                                  int(20/scalefactor));
  				//-----------------------------------------------------------------------------
  				//---------------------------- LINK SWITCH DISPLAY  ---------------------------
  				//-----------------------------------------------------------------------------
   				if((!(i%2))&&(!processor.taiType))
					{
              		int btnidx=chnidx;
  					if(!processor.isMappedToSinglechannel[btnidx])
  						{
  						if(!processor.ChannelLinkMap[btnidx])
  							ImgButLink[btnidx]->setImages(false, true, true,
  														ImgLinkOff, onoffOpacity, Colours::transparentBlack,
  														ImgLinkOff, onoffHoverOpacity, Colours::transparentBlack,
  														ImgLinkOn, onoffClickedOpacity,Colours::transparentBlack
  														,0.0f);   	
  						else
  							ImgButLink[btnidx]->setImages(false, true, true,
  														ImgLinkOn, onoffOpacity, Colours::transparentBlack,
  														ImgLinkOn, onoffHoverOpacity, Colours::transparentBlack,
  														ImgLinkOff, onoffClickedOpacity,Colours::transparentBlack
  														,0.0f);
  						ImgButLink[btnidx]->setBounds(int((largeFaderW+(i*sFaderW)+35)/scalefactor),
                      		            	          int((sFaderH-385)/scalefactor),
  														16,
                              	              			16);
  						ImgButLink[btnidx]->setVisible(true);
  						//printf("btnidx: %d\n",btnidx);
  						}
  					}
  				}
//rebuildMixView(void)-----------------------------------------------------------------------------
			else
              	{
              	sslVCAlabel[vcaIndex]->setVisible(true);
              	sslVCAlabel[vcaIndex++]->setBounds(int((largeFaderW+(i*sFaderW))/scalefactor),
  										int((sFaderH-390)/scalefactor),
  										int(sFaderW/scalefactor),//width
  										int(20/scalefactor)); //height
              	
              	sslTrackNumber[chnidx]->setBounds(int((largeFaderW+(i*sFaderW))/scalefactor),
                                                  int((sFaderH-375)/scalefactor),
                                                  int(sFaderW/scalefactor),//width
                                                  int(20/scalefactor)); //height
              	
				}
			tNameLabel[chnidx]->setVisible(true);
			tNameLabel[chnidx]->setBounds(int((2+largeFaderW+(i*sFaderW))/scalefactor),
                                                  int((sFaderH-17)/scalefactor),
                                                  int((sFaderW-2)/scalefactor),
                                                  int(16/scalefactor));
			}
      	}
	}
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::timerCallback()
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::timerCallback()
{
if(processor.PluginConfigState)
	{
	if(EditorSystemSize!=processor.SystemSize)
		{
		EditorSystemSize=processor.SystemSize;									//dont comme back here
		SystemSizeRcvdFlag=EditorSystemSize;
		doDebugWindow();
		resizeTrigger=true;
		}
	}

if(processor.PluginConfigState>2)
	{
    AlertLabel.setVisible(false);
	if(editorJustStartedFlag!=processor.justStartedFlag)
 		{
		editorJustStartedFlag=processor.justStartedFlag;
		resizeTrigger=true;
		}
	if(processor.SystemSize!=EditorSystemSize)
		{
		EditorSystemSize=processor.SystemSize;									//dont comme back here
		SystemSizeRcvdFlag=1;
		doDebugWindow();
		resizeTrigger=true;
		}

//--------------------------------------------------------------------
//timerCallback()----  Debug window section  -------------------------  
//--------------------------------------------------------------------
	if(debugDisplayState)
		{
        doDebugWindow();
		doDebugWindowContent();
		}

//--------------------------------------------------------------------
//timerCallback()----  Mix On/Off swt manager  -----------------------  
//--------------------------------------------------------------------
    if(editorMixOnOffState!=processor.mixOnOffState)
    	{
        if(processor.mixOnOffState)
        	{
        	onoffOpacity=1.0f;
       		onoffHoverOpacity=0.8f;
       		onoffClickedOpacity=0.8f;
       		ImgButMixOffOn.setImages(true, true, true,mixOn, 1.0f, Colours::transparentBlack, mixOn, 0.8f, Colours::transparentBlack, mixOn, 0.8f,Colours::transparentBlack);
       		}
        else
        	{
       		onoffOpacity=0.3f;
    		onoffHoverOpacity=0.3f;
       		onoffClickedOpacity=0.3f;
       		ImgButMixOffOn.setImages(true, true, true,mixOff, 1.0f, Colours::transparentBlack, mixOff, 0.8f, Colours::transparentBlack, mixOff, 0.8f,Colours::transparentBlack);
       		}
        editorMixOnOffState=processor.mixOnOffState;
		MyLookAndFeel.MLFPassOnOffState(editorMixOnOffState);
		MyLookAndFeel.MLFPassOnOffOpacitiy(onoffOpacity);
		resizeTrigger=true;
        }

//--------------------------------------------------------------------
//timerCallback()----  Status Swt Preset  ----------------------------  
//--------------------------------------------------------------------
	if(processor.newStatusSwtPresetFlag)
		{
		processor.newStatusSwtPresetFlag=0;
		resizeTrigger=true;
        }

 	if(!processor.taiPong.getGameState())
    	{
   		//--------------------------------------------------------------------
    	//timerCallback()----  manage parameters  ----------------------------  
    	//--------------------------------------------------------------------
    	for(int i = 0; i < processor.SystemSize; i++)
			ParameterManager(i);
		
    	//--------------------------------------------------------------------
    	//timerCallback()----  manage tracks labels colors for single channel 
    	//--------------------------------------------------------------------
    	if(processor.trkLabelRefreshFlag)
			{
      		processor.trkLabelRefreshFlag=0;
        	resizeTrigger=true;
        	}
      	}
	else
    	//--------------------------------------------------------------------
    	//timerCallback() ---  Manage pong paddles input vca1 and 8  ---------  
    	//--------------------------------------------------------------------
 		{
		bool repaintflag=false;
		if((ballX!=processor.ballX)||(ballY!=processor.ballY))
			{
			ballX=processor.ballX;//ball X position+Offset
			ballY=processor.ballY;//ball Y position+Offset
			ball.setXPos(ballX);
			ball.setYPos(ballY);
			player1Score.setScore(processor.taiPong.player1Score.getScore());
			player2Score.setScore(processor.taiPong.player2Score.getScore());
			repaintflag=true;
			}
		if((pad1X!=processor.taiPong.getPadXPos(1))||(pad1Y!=processor.taiPong.getPadYPos(1)))
			{
			pad1X=processor.taiPong.getPadXPos(1);
			pad1Y=processor.taiPong.getPadYPos(1);
			pad1.setXPos(pad1X);
			pad1.setYPos(pad1Y);
			repaintflag=true;
			}

		if((processor.pad2X!=processor.taiPong.getPadXPos(2))||(pad2Y!=processor.taiPong.getPadYPos(2)))
			{
			pad2X=processor.taiPong.getPadXPos(2);
			pad2Y=processor.taiPong.getPadYPos(2);
			pad2.setXPos(pad2X);
			pad2.setYPos(pad2Y);
			repaintflag=true;
			}

		if(repaintflag)
			repaint();
		}
	}
//--------------------------------------------------------------------
//timerCallback()-------------  taiPong  -----------------------------  
//--------------------------------------------------------------------
 
if(processor.taiPong.gameStateChangedFlag)
	{
	if(!processor.taiPong.getGameState())
  		{
  		rebuildMixView();
 		setLeftHandButtonVisibleState(true);
  		}
  	else
  		{
  		int vcaIdx=0;
  		for(int n=0;n<processor.SystemSize;n++)
			{
			TheMutes[n]->setVisible(false);
			sslTrackNumber[n]->setVisible(false);
			if(n>=(inputChnCnt))
				sslVCAlabel[vcaIdx++]->setVisible(false);
			tNameLabel[n]->setVisible(false);
			TheAutoLabels[n]->setVisible(false);
			TheSliders[n]->setVisible(false);
			TheValueLabels[n]->setVisible(false);
			}
		setLeftHandButtonVisibleState(false);
		}
	processor.taiPong.gameStateChangedFlag=false;
	repaint();
	}

//--------------------------------------------------------------------
//timerCallback()-------------  ALERTS   -----------------------------  
//--------------------------------------------------------------------
if(processor.PluginConfigState!=3)
	alertMsgManager();
	
if(resizeTrigger)
	resized();
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::alertMsgManager(void)
//
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::alertMsgManager(void)
{
switch(processor.PluginConfigState)
	{
	case 0:
		AlertLabel.setText("NO UDP CONECTION ESTABLISHED! \n\n", dontSendNotification);
	break;
	case 1:
		AlertLabel.setText("THE INJEKTOR APPLICATION MUST BE RUNNING TO USE THE MOTHERSHIP\n\nLOOKS LIKE IT IS NOT...\n\n Please start the INJEKTOR application", dontSendNotification);
	break;
	case 2:
		AlertLabel.setText("Only one TaiMotherShip plug-in \n\n can be running in a session. \n\n Remove this Plugin", dontSendNotification);
	break;
	}
	
AlertLabel.setVisible(true);
if(heartBeatToggle!=processor.heartBeatToggle)
	{
	heartBeatToggle=processor.heartBeatToggle;
	if(heartBeatToggle)
		AlertLabel.setFont(Font (15.0f, Font::bold));
	else
		AlertLabel.setFont(Font (16.0f, Font::bold));
	if(heartBeatToggle)
		resizeTrigger=true;
	}

}
	
//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::setLeftHandButtonVisible(bool state)
//
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::setLeftHandButtonVisibleState(bool state)
	{
	if(state)
		{
		if(processor.SystemSize>=8)
			imgButtonBank8.setVisible(state);
		if(processor.SystemSize>=16)
			imgButtonBank16.setVisible(state);
		if(processor.SystemSize>=24)
			imgButtonBank24.setVisible(state);
		if(processor.SystemSize>=32)
			imgButtonBank32.setVisible(state);
		}
	else
		{
		imgButtonBank8.setVisible(state);
		imgButtonBank16.setVisible(state);
		imgButtonBank24.setVisible(state);
		imgButtonBank32.setVisible(state);
		}
	imgButtonShiftFirst.setVisible(state);
	imgButtonShiftDown.setVisible(state);
	imgButtonShiftUp.setVisible(state);
	imgButtonShiftLast.setVisible(state);

	ImgButAutoVolRead.setVisible(state);
	ImgButAutoVolWrite.setVisible(state);
	ImgButAutoCutRead.setVisible(state);
	ImgButAutoCutWrite.setVisible(state);
   	if(processor.taiType==2)
		state=false;
	for(char n=0;n<6;n++)
    	faderStatusSwt[n].setVisible(state);
	}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::setLeftHandGameButtonVisibleState(bool state);
//
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::setLeftHandGameButtonVisibleState(bool state)
	{
	ImgButGameCtrl1.setVisible(state);
	ImgButGameCtrl2.setVisible(state);
	ImgButGameCtrl3.setVisible(state);
	ImgButGameCtrl4.setVisible(state);
	ImgButGameCtrl5.setVisible(state);
	}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::doDebugWindow (void)
//
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::doDebugWindow (void)
{
if(debugDisplayState)
	{
	setResizable(true,true);
    setSize (largeFaderW+(sFaderW*((processor.viewSize))),
			sFaderH+debugDisplayHeight);
	setResizeLimits (largeFaderW+(sFaderW*8), sFaderH+debugDisplayHeight,
				 largeFaderW+(sFaderW*processor.viewSize), sFaderH+debugDisplayHeight);
	resized();
	}
else
	{
	setResizable(true,true);
	setSize (largeFaderW+(sFaderW*((processor.viewSize))),
			sFaderH);
	setResizeLimits (largeFaderW+(sFaderW*8), sFaderH,
				 largeFaderW+(sFaderW*processor.viewSize), sFaderH);
	resized();
	}
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::doDebugWindowContent (void)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::doDebugWindowContent (void)
{
debugDisplay.setText("host Vol parmeter: "+String((float)processor.ptNewValue)+ 
					"      host INT Vol1 parmeter:"+String((int)processor.PTIntValue1)+
					"      host INT Vol2 parmeter:"+String((int)processor.PTIntValue2)+
					/*"     host Vol parmeter:"+String((int)processor.hostVolParameterExist[3])+
					"     host Vol parmeter:"+String((int)processor.hostVolParameterExist[4])+*/"\n");

debugDisplay.insertTextAtCaret("line2\n");
debugDisplay.insertTextAtCaret("line3\n");
debugDisplay.insertTextAtCaret("line4\n");



}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::sliderValueChanged (Slider* slider)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::sliderValueChanged (Slider* slider)
{
AudioProcessorParameterWithID* volId;
String faderNum =slider->getName();
String paramId=String("V"+faderNum);
volId=parameters.getParameter(paramId.toUTF8());

if(processor.faderIsDragged[faderNum.getIntValue()])
	{
	switch(processor.pluginAutoMode[faderNum.getIntValue()])
		{
		case OFF:
			volId->setValue(TheSliders[faderNum.getIntValue()]->getValue());	
			printf("FADER moved by mouse\n");
   	 	break;
		case WRITE:
			volId->setValueNotifyingHost(TheSliders[faderNum.getIntValue()]->getValue());

//		if(processor.pluginAutoRWStates&WRITEVOL)
//			{
//			else
//				volId->setValue((float) sliderThatchanged->getValue());
//			}
//		else
//			volId->setValue((float) sliderThatchanged->getValue());	
		break;
		}
	}
}
	
//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::sliderDragStarted (Slider* theSlider)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::sliderDragStarted (Slider* theSlider)
{
String faderNum = theSlider->getName();
processor.faderIsDragged[faderNum.getIntValue()]=true;
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::sliderDragEnded (Slider* theSlider)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::sliderDragEnded (Slider* theSlider)
{
String faderNum = theSlider->getName();
processor.faderIsDragged[faderNum.getIntValue()]=false;
}
/** Called when the button's state changes. */


//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::ParameterManager(unsigned int chan
//
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::ParameterManager(unsigned int chan)
{
float paramval;
float trimparamval;
//AudioProcessorParameterWithID* ParaIdVol;
//Do label update
//Do fader update
//Do automation mode update
MyLookAndFeel.MLFPassAutoState(chan,processor.pluginAutoMode[chan]);
MyLookAndFeel.MLFPassTouchState(chan,processor.SSLTouchStateArray[chan]);
MyLookAndFeel.MLFPassDawAbsDbValue(chan,processor.TxFaderValue[chan]);
MyLookAndFeel.MLFPassDawTrimDbValue(chan,processor.TxTrimValue[chan]);
MyLookAndFeel.MLFPassSSLFaderDbValue(chan,processor.SSLDbFaderValue[chan]);


Mute_manager(chan);
//if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
//if(chan==0)
//	printf("togglautomode= %d\n",processor.pluginAutoMode[chan]);

switch(processor.pluginAutoMode[chan])
	{
	case OFF:  if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
                {
                processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				TheAutoLabels[chan]->setText ("OFF", sendNotification);
				TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::grey);
				TheAutoLabels[chan]->setColour (Label::textColourId, Colours::white);
				TheValueLabels[chan]->setColour (Label::textColourId, Colours::white);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::black);
				}

			if(processor.CurrentDawFaderValue[chan]!=processor.SSLDbFaderValue[chan])
				{
				//DBG("Fader value" <<float(processor.CurrentDawFaderValue[chan])<<"channel:"<<float(chan));
                processor.CurrentDawFaderValue[chan]=processor.SSLDbFaderValue[chan];
                //printf("Editor Side  Fader: #%d Value: %d\n",chan,processor.SSLDbFaderValue[chan]);
				TheSliders[chan]->setValue(processor.SSLScaledFaderValue[chan]);
				paramval=(((100-(float)(processor.SSLDbFaderValue[chan]))/10));
                //    10-((float)processor.SSLDbFaderValue[chan]/10);
                if(paramval<-90)
                    TheValueLabels[chan]->setText ("-inf", sendNotification);
                else
                    TheValueLabels[chan]->setText (String(paramval), sendNotification);
				}
	break;
    case WRITE:
			if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
				{
				processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				TheAutoLabels[chan]->setText ("WRITE", sendNotification);
                TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::red);
            	TheAutoLabels[chan]->setColour (Label::textColourId, Colours::white);
            	TheValueLabels[chan]->setColour (Label::textColourId, Colours::white);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::red);
			    }
			
//Fader data			
			paramval=(float)*parameters.getRawParameterValue("V"+String(chan));
			if(paramval!=TheSliders[chan]->getValue())
                {
				TheSliders[chan]->setValue(paramval);
				TheValueLabels[chan]->setText (String(((100-(float)(processor.SSLDbFaderValue[chan]))/10)), sendNotification);
				}
	break;
	case TOUCH_LATCH:
			if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
				{
				processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				if(processor.ConfigReg[10]&0x80)
                	TheAutoLabels[chan]->setText ("TOUCH", sendNotification);
				else
    				TheAutoLabels[chan]->setText ("LATCH", sendNotification);
				TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::darkorange);
    			TheAutoLabels[chan]->setColour (Label::textColourId, Colours::white);
				TheValueLabels[chan]->setColour (Label::textColourId, Colours::cyan);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::black);
    			}

//Fader data			
            paramval=(float)*parameters.getRawParameterValue("V"+String(chan));
            if(paramval!=TheSliders[chan]->getValue())
                {
                TheSliders[chan]->setValue(paramval);
                TheValueLabels[chan]->setText (String(((100-(float)(processor.TxFaderValue[chan]))/10)), sendNotification);
				}
	break;
	case READ:
			if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
				{
				processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				TheAutoLabels[chan]->setText ("READ", sendNotification);
    			TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::green);
        		TheAutoLabels[chan]->setColour (Label::textColourId, Colours::white);
				TheValueLabels[chan]->setColour (Label::textColourId, Colours::cyan);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::black);
				}

//Fader data			
            if(!processor.isMappedToSinglechannel[chan])
				{
				if(chan%2)
					{
					if(!processor.ChannelLinkMap[chan])
						paramval=(float)*parameters.getRawParameterValue("V"+String(chan));
					else
						paramval=(float)*parameters.getRawParameterValue("V"+String(chan-1));
					}
				else
					paramval=(float)*parameters.getRawParameterValue("V"+String(chan));
				
            	}
            else
            	paramval=processor.singleChannelVolValue[chan];
            
           	if(processor.globalReadVolAutoState)
           		{
           		if(paramval!=TheSliders[chan]->getValue())
               		{
                   	TheSliders[chan]->setValue(paramval);
                   	TheValueLabels[chan]->setText (String(((100-(float)(processor.TxFaderValue[chan]))/10)), sendNotification);
                   	}
				}
			else
				{
				paramval=(float)*parameters.getRawParameterValue("V"+String(chan));
				if(paramval!=TheSliders[chan]->getValue())
					{
					TheSliders[chan]->setValue(processor.SSLScaledFaderValue[chan]);
					paramval=(((100-(float)(processor.SSLDbFaderValue[chan]))/10));
    	            if(paramval<-90)
            	        TheValueLabels[chan]->setText ("-inf", sendNotification);
                	else
                    	TheValueLabels[chan]->setText (String(paramval), sendNotification);
					}
    			}
    break;
	case TRIMWRITE:
			if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
				{
				processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				TheAutoLabels[chan]->setText ("TRIM WRITE", dontSendNotification);
				TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::red);
        		TheAutoLabels[chan]->setColour (Label::textColourId, Colours::yellow);
				TheValueLabels[chan]->setColour (Label::textColourId, Colours::yellow);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::red);
                }
//Fader data			
			paramval=(float)*parameters.getRawParameterValue("T"+String(chan));
			if(paramval!=TheSliders[chan]->getValue())
                {
				//printf("slider TRIM value %f\n",paramval);
				TheSliders[chan]->setValue(paramval);
                TheValueLabels[chan]->setText (String(((100-(float)(processor.SSLDbFaderValue[chan]))/10)), sendNotification);
				}
	break;
	case TRIMTOUCH_LATCH:
			if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
				{
				processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				if(processor.ConfigReg[10]&0x80)
					TheAutoLabels[chan]->setText ("TRIM TOUCH", dontSendNotification);
				else
					TheAutoLabels[chan]->setText ("TRIM LATCH", dontSendNotification);
				
				TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::darkorange);
        		TheAutoLabels[chan]->setColour (Label::textColourId, Colours::yellow);
				TheValueLabels[chan]->setColour (Label::textColourId, Colours::cyan);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::black);
                }
	break;
	case TRIMREAD:
			if(processor.SSLPrevAutomodeArray[chan]!=processor.pluginAutoMode[chan])
				{
				processor.SSLPrevAutomodeArray[chan]=processor.pluginAutoMode[chan];
				TheAutoLabels[chan]->setText ("TRIM READ", sendNotification);
				TheAutoLabels[chan]->setColour (Label::backgroundColourId, Colours::green);
    			TheAutoLabels[chan]->setColour (Label::textColourId, Colours::yellow);
				TheValueLabels[chan]->setColour (Label::textColourId, Colours::cyan);
				TheValueLabels[chan]->setColour (Label::backgroundColourId, Colours::black);
                }

//Fader data			
			trimparamval=(float)*parameters.getRawParameterValue("T"+String(chan));
			if(trimparamval!=TheSliders[chan]->getValue())
                {
                TheSliders[chan]->setValue(trimparamval);
                TheValueLabels[chan]->setText (String(((100-(float)(processor.TxFaderValue[chan]))/10)), sendNotification);
				}
		break;
	}
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::Mute_manager(unsigned int chan)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::Mute_manager(unsigned int chan)
{
int automode =processor.pluginAutoMode[chan];
bool mutestate=false;

switch(automode)
	{
	case WRITE:
		if(processor.dawWasInTouch[chan])
			mutestate= (bool)*parameters.getRawParameterValue("CUT"+String(chan));
	break;
	case TRIMTOUCH_LATCH:
	case OFF:
	case TOUCH_LATCH:
	case TRIMWRITE:
	case TRIMREAD:
	case READ:	
		if(!processor.isMappedToSinglechannel[chan])
			mutestate= (bool)*parameters.getRawParameterValue("CUT"+String(chan));
	   else
			mutestate= processor.singleChannelCutValue[chan];
	break;
	default:break;
	}

mutestate=mutestate|processor.SSLCutState[chan];

if(processor.editorCutstate[chan]!=mutestate)
	{
    TheMutes[chan]->setVisible(false);
	processor.editorCutstate[chan]=mutestate;
	if(mutestate)
		{
		TheMutes[chan]->setImages(false, true, false,mute_on, onoffOpacity, Colours::transparentBlack, mute_on, onoffHoverOpacity, Colours::transparentBlack, mute_on, onoffClickedOpacity,Colours::transparentBlack);
		}
	else
		{
		TheMutes[chan]->setImages(false, true, false,mute_off, onoffOpacity, Colours::transparentBlack, mute_off, onoffHoverOpacity, Colours::transparentBlack, mute_off, onoffHoverOpacity,Colours::transparentBlack);
		}
    if((chan<=((processor.bank*8)+processor.viewSize))
       &&(chan>=(processor.bank*8)))
        {
//        int offset =processor.viewSize-chan;
        //printf("Mute channel: %d  bank number: %d offset: %d\n",chan,processor.bank,offset);
        TheMutes[chan]->setBounds(	int((largeFaderW+((chan-(processor.bank*8))*sFaderW)+(sFaderW/2)-((mute_off.getWidth()-mutebtntrim)/2))/scalefactor),
                                int(5/scalefactor),
                                int((mute_off.getWidth()-mutebtntrim)/scalefactor),
                                int((mute_off.getWidth()-mutebtntrim)/scalefactor));

        TheMutes[chan]->setVisible(true);
        }
    }

cutWrite[chan]->setBounds(int(largeFaderW+(chan*sFaderW)+(sFaderW/2))-int((cutWrite[chan]->getWidth())/2),
							5,
							cutWrite[chan]->getWidth(),
							cutWrite[chan]->getWidth());  
switch(processor.taiType)
	{
	case 0:
	case 1:
		if(automode==WRITE&&((processor.globalWriteCutAutoState==true)&&!processor.dawWasInTouch[chan]))
			cutWrite[chan]->setVisible(true);
		else
			cutWrite[chan]->setVisible(false);
		cutWrite[chan]->toFront(true);
		
	break;
	case 2:
		if(processor.ChannelWriteCut[chan])
			cutWrite[chan]->setVisible(true);
		else
			cutWrite[chan]->setVisible(false);
		cutWrite[chan]->toFront(true);
	break;
	
	}
}


//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor:: buttonStateChanged (Button* buttonstatechange)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor:: buttonStateChanged (Button* buttonstatechange)
{
#ifdef __MODKEYDEV
if(processor.mixOnOffState)
	{
	if(faderStatusSwt[0].isOver()
		||faderStatusSwt[1].isOver()
		||faderStatusSwt[2].isOver()
		||faderStatusSwt[3].isOver()
		||faderStatusSwt[4].isOver()
		||faderStatusSwt[5].isOver())
		{
		autoToggleSwtHover=true;
		resizeTrigger=true;
		}
	else
		{
		autoToggleSwtHover=false;
		resizeTrigger=true;
		}
#endif
	}
}

void TaiMotherShipPluguinProcessorEditor::mouseMove(const MouseEvent &event)
{
//DBG("X:"<<event.getPosition().getX()<<"   Y:"<<event.getPosition().getY());
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::modifierKeysChanged(const ModifierKeys& modifiers)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::modifierKeysChanged(const ModifierKeys& modifiers)
{
#ifdef __MODKEYDEV
  //Component* hit = getComponentAt (e.getMouseDownPosition());
	{
	if(modifiers.ModifierKeys::shiftModifier)
  		{
 	 	if(modifiers.isShiftDown())
  			processor.shiftKeyState=true;
  		else
  			processor.shiftKeyState=false;
  		}
  		
	if(modifiers.ModifierKeys::ctrlModifier)
	  	{
  		if(modifiers.isCtrlDown())
  			processor.ctrlKeyState=true;
  		else
  			processor.ctrlKeyState=false;
  		}
  	
    if(modifiers.ModifierKeys::commandModifier)
		{
  		if(modifiers.isCommandDown())
  			processor.cmdKeyState=true;
  		else
  			processor.cmdKeyState=false;
  		}
  		
	if(modifiers.ModifierKeys::altModifier)
		{
		if(modifiers.isAltDown())
  			processor.altKeyState=true;
  		else
  			processor.altKeyState=false;
  		}
  	}
resizeTrigger=true;
#endif
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::updateTrackProperties ()
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::updateTrackProperties ()
{
backgroundColour = getProcessor().trackProperties.colour;
//TrkNambutton.setButtonText(getProcessor().trackProperties.name);
}
//buttonClicked---------------------------------------------------------------------------------
//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
	if(buttonThatWasClicked==&ImgButMixOffOn)
		{
		if(processor.shiftKeyState&&processor.ctrlKeyState)//test if MIX ON-OFF button clicked with shift + ctrl key
			{
			if(processor.mixOnOffState && (!processor.taiPong.getGameState()))
				{
				savedViewSize=processor.viewSize;			
				processor.viewSize=16;
				setSize (largeFaderW+(sFaderW*processor.viewSize),sFaderH);
    	    	          setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
                             largeFaderW+(sFaderW*processor.viewSize), sFaderH);
				
				taiPungInit();
				processor.taiPong.gameInit();
				processor.taiPong.gameStopMode();
				resized();
				repaint();
				}
			}
		else			//if not, test in game mode
			{
			if(!processor.taiPong.getGameState())//if in game mode, exit game mode
				{
				int mixState=(int)parameters.getParameter("MIXONOFFSTATE")->getValue();
				mixState=mixState^1;
				parameters.getParameter("MIXONOFFSTATE")->setValueNotifyingHost((float)mixState);
				resized();
				}
			else		//if not, do mix on<->off
				{
				setLeftHandGameButtonVisibleState(false);
				processor.taiPong.gameInitMode();
				processor.viewSize=savedViewSize;
				setSize (largeFaderW+(sFaderW*processor.viewSize),sFaderH);
				setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
    	                         largeFaderW+(sFaderW*processor.viewSize), sFaderH);
				processor.taiPong.gameStateChangedFlag=true;
				resized();
				repaint();
				}
			}
		}
//buttonClicked---------------------------------------------------------------------------------
	else
		{	
		int i=buttonThatWasClicked->getComponentID().substring(1).getIntValue();
		if((i<(inputChnCnt))&&(i))
			{
			printf("clicked index: %d\n",i);
			processor.ChannelLinkMap[i-1]^=1;
			processor.ChannelLinkMap[i]=processor.ChannelLinkMap[i-1];

/*		if(processor.ChannelLinkMap[i])
			printf("Channel %d and %d linked\n",i,i+1);
		else
			printf("Channel %d and %d onlinked\n",i,i+1);
*/
			processor.changedLinkMap[i-1]=1;
			processor.changedLinkMap[i]=1;
//			MyLookAndFeel.MLFPassLinkState(i-1,processor.ChannelLinkMap[i-1]);
//			MyLookAndFeel.MLFPassLinkState(i,processor.ChannelLinkMap[i]);
		
			processor.linkChange++;
			processor.linkChange++;
			resized();
			}
//buttonClicked---------------------------------------------------------------------------------
		else
		if(processor.mixOnOffState)
			{
    			if(buttonThatWasClicked==&imgButtonShiftUp)
				{
     			int size=processor.SystemSize;
     			if(((processor.bank*8)+processor.viewSize)<size)
					{
					processor.bank=processor.bank+1;
        				resized();
        				}
				}
    			else
    			if(buttonThatWasClicked==&imgButtonShiftDown)
     			{
    				if(processor.bank)
        				{
       			 	processor.bank=processor.bank-1;
       			 	resized();
        				}
    				}
//buttonClicked---------------------------------------------------------------------------------
			else
  			if(buttonThatWasClicked==&imgButtonShiftFirst)
				{
				processor.bank=0;
				resized();
				}
			else
    			if(buttonThatWasClicked==&imgButtonShiftLast)
				{
				int size=processor.SystemSize;
				if(((processor.bank*8)+processor.viewSize)<size)
					{
					processor.bank=(size-processor.viewSize)/8;
					resized();
					}
    				}
//buttonClicked---------------------------------------------------------------------------------
  			else
  			if(buttonThatWasClicked==&imgButtonBank32)
    				{
    				processor.viewSize=32;
					while(((processor.bank*8)+processor.viewSize)>processor.SystemSize)
    	   				{
       				if(processor.bank)
     				    processor.bank--;
       				}
				setResizable(true,true);
				setSize (largeFaderW+(sFaderW*processor.viewSize),sFaderH);
   			 	setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
                         largeFaderW+(sFaderW*processor.viewSize), sFaderH);
    				resized();
   				}
//buttonClicked---------------------------------------------------------------------------------
  			else
    			if(buttonThatWasClicked==&imgButtonBank24)
    				{
        			processor.viewSize=24;
      			while(((processor.bank*8)+processor.viewSize)>processor.SystemSize)
      				{
      				if(processor.bank)
      					processor.bank--;
       	 			}
				setResizable(true,true);
    				setSize (largeFaderW+(sFaderW*processor.viewSize),sFaderH);
      			setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
                             largeFaderW+(sFaderW*processor.viewSize), sFaderH);
      			resized();
      			}
 //buttonClicked---------------------------------------------------------------------------------
   			else
			if(buttonThatWasClicked==&imgButtonBank8)
				{
       			processor.viewSize=8;
			  	while(((processor.bank*8)+processor.viewSize)>processor.SystemSize)
        				{
      				if(processor.bank)
				   		processor.bank--;
				   	}
			  	setResizable(true,true);
			  	setSize (largeFaderW+(sFaderW*processor.viewSize),sFaderH);
			  	setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
    	                         largeFaderW+(sFaderW*processor.viewSize), sFaderH);
			  	resized();
			  	}
//buttonClicked---------------------------------------------------------------------------------
			else
			if(buttonThatWasClicked==&imgButtonBank16)
				{
				processor.viewSize=16;
				while(((processor.bank*8)+processor.viewSize)>processor.SystemSize)
					{
   	     			if(processor.bank)
    	     	 			processor.bank--;
    	      			}
				setSize (largeFaderW+(sFaderW*processor.viewSize),sFaderH);
   	    		 	setResizeLimits (	largeFaderW+(sFaderW*8), sFaderH,
               	             largeFaderW+(sFaderW*processor.viewSize), sFaderH);
        			resized();
        			}
//buttonClicked---------------------------------------------------------------------------------
			else
			if(buttonThatWasClicked==&ImgButAutoVolRead)
				{
				processor.globalReadVolAutoState=processor.globalReadVolAutoState^1;
				if(!processor.globalReadVolAutoState)
					ImgButAutoVolRead.setImages(true, true, true,	VolEnableOff, onoffOpacity, Colours::transparentBlack,
																	VolEnableOff, onoffHoverOpacity, Colours::transparentBlack,
																	VolEnableOff, onoffClickedOpacity,Colours::transparentBlack);
				else
					ImgButAutoVolRead.setImages(true, true, true,	VolEnableOn, onoffOpacity, Colours::transparentBlack,
																	VolEnableOn, onoffHoverOpacity, Colours::transparentBlack,
																	VolEnableOn, onoffClickedOpacity,Colours::transparentBlack);
				parameters.getParameter("RWS1")->setValueNotifyingHost((float)processor.globalReadVolAutoState);
				resized();
				}
			else	
    			if(buttonThatWasClicked==&ImgButAutoVolWrite)
				{
		        	processor.globalWriteVolAutoState=processor.globalWriteVolAutoState^1;
				if(!processor.globalWriteVolAutoState)
     		       	ImgButAutoVolWrite.setImages(true, true, true,	VolEnableOff, onoffOpacity, Colours::transparentBlack,
     		       													VolEnableOff, onoffHoverOpacity, Colours::transparentBlack,
     		       													VolEnableOff, onoffClickedOpacity,Colours::transparentBlack);
				else
					ImgButAutoVolWrite.setImages(true, true, true,VolEnableOn, onoffOpacity, Colours::transparentBlack, VolEnableOn, onoffHoverOpacity, Colours::transparentBlack, VolEnableOn, onoffClickedOpacity,Colours::transparentBlack);
				parameters.getParameter("RWS2")->setValueNotifyingHost((float)processor.globalWriteVolAutoState);
       			resized();
        			}
//buttonClicked---------------------------------------------------------------------------------
			else
			if(buttonThatWasClicked==&ImgButAutoCutRead)
    			{
 				processor.globalReadCutAutoState=processor.globalReadCutAutoState^1;
    			if(!processor.globalReadCutAutoState)
 					ImgButAutoCutRead.setImages(true, true, true,	CutEnableOff, onoffOpacity, Colours::transparentBlack,
 																	CutEnableOff, onoffHoverOpacity, Colours::transparentBlack, 
 																	CutEnableOff, onoffClickedOpacity,Colours::transparentBlack);
				else
					ImgButAutoCutRead.setImages(true, true, true,	CutEnableOn, onoffOpacity, Colours::transparentBlack,
																	CutEnableOn,  onoffHoverOpacity,Colours::transparentBlack, 
																	CutEnableOn, onoffClickedOpacity,Colours::transparentBlack);
				parameters.getParameter("RWS3")->setValueNotifyingHost((float)processor.globalReadCutAutoState);
				resized();
				}
//buttonClicked---------------------------------------------------------------------------------
			else
			if(buttonThatWasClicked==&ImgButAutoCutWrite)	
  		  		{
				processor.globalWriteCutAutoState=processor.globalWriteCutAutoState^1;
				if(!processor.globalWriteCutAutoState)
					ImgButAutoCutWrite.setImages(true, true, true,	CutEnableOff, onoffOpacity, Colours::transparentBlack,
																	CutEnableOff, onoffHoverOpacity, Colours::transparentBlack,
																	CutEnableOff, onoffClickedOpacity,Colours::transparentBlack);
				else
 					ImgButAutoCutWrite.setImages(true, true, true,	CutEnableOn, onoffOpacity, Colours::transparentBlack,
 																	CutEnableOn, onoffHoverOpacity, Colours::transparentBlack,
 																	CutEnableOn, onoffClickedOpacity,Colours::transparentBlack);
				parameters.getParameter("RWS4")->setValueNotifyingHost((float)processor.globalWriteCutAutoState);
        			resized();
        			}
//buttonClicked---------------------------------------------------------------------------------
			else
    			if(buttonThatWasClicked==&faderStatusSwt[0]
				||buttonThatWasClicked==&faderStatusSwt[1]
				||buttonThatWasClicked==&faderStatusSwt[2]
				||buttonThatWasClicked==&faderStatusSwt[3]
				||buttonThatWasClicked==&faderStatusSwt[4]
				||buttonThatWasClicked==&faderStatusSwt[5])
				{
				if(!processor.shiftKeyState)
					{
					processor.ConfigReg[0]=34;
					if(buttonThatWasClicked==&faderStatusSwt[0])
						parameters.getParameter("STATUSPRESET")->setValueNotifyingHost((float)1/7);
					else
					if(buttonThatWasClicked==&faderStatusSwt[1])
						parameters.getParameter("STATUSPRESET")->setValueNotifyingHost((float)2/7);
					else
					if(buttonThatWasClicked==&faderStatusSwt[2])
						parameters.getParameter("STATUSPRESET")->setValueNotifyingHost((float)3/7);
					else
					if(buttonThatWasClicked==&faderStatusSwt[3])
						parameters.getParameter("STATUSPRESET")->setValueNotifyingHost((float)4/7);
					else
					if(buttonThatWasClicked==&faderStatusSwt[4])
						parameters.getParameter("STATUSPRESET")->setValueNotifyingHost((float)5/7);
					else
					if(buttonThatWasClicked==&faderStatusSwt[5])
						parameters.getParameter("STATUSPRESET")->setValueNotifyingHost((float)6/7);
					}
				else
//buttonClicked---------------------------------------------------------------------------------
					{
					if(buttonThatWasClicked==&faderStatusSwt[0])
						{
						//processor.ConfigReg[0x60]=0;//Set all OFF
						processor.ConfigReg[0]=0x60;
						parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)0/255);
						}
					else
					if(buttonThatWasClicked==&faderStatusSwt[1])
						{
						processor.ConfigReg[0]=0x60;
						if(!processor.ctrlKeyState)
							parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)1/255);//Set all WRITE
						else
							parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)5/255);//Set all TRIM WRITE
						}
					else
//buttonClicked---------------------------------------------------------------------------------
					if(buttonThatWasClicked==&faderStatusSwt[2])
						{
						processor.ConfigReg[0]=0x60;
						if(!processor.ctrlKeyState)
							parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)2/255);//Set all TOUCH
						else
							parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)6/255);//Set all TRIMTOUCH
						}
					else
					if(buttonThatWasClicked==&faderStatusSwt[3])
						{
						processor.ConfigReg[0]=0x60;
						if(!processor.ctrlKeyState)
							parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)3/255);//Set all READ
						else
							parameters.getParameter("CONFIGREGISTER")->setValueNotifyingHost((float)7/255);//Set all TRIMREAD
						}
					}		
				resized();
				}
			}
		if(processor.taiPong.getGameState())
			{
			if(buttonThatWasClicked==&ImgButGameCtrl1)		//En stop... on peu just jouer
				{
				processor.taiPong.gameStopMode();
				resized();
				repaint();
				}
			else
			if(buttonThatWasClicked==&ImgButGameCtrl2)	//En play... on peu arreter ou faire une pause
               	{
               	processor.taiPong.gamePlayMode();
               	resized();
               	repaint();
               	}
			else
			if(buttonThatWasClicked==&ImgButGameCtrl3)	//En pause... on peux continuer ou arreter
               	{
               	processor.taiPong.gamePauseMode();
               	resized();
               	repaint();
               	}
			else
			if(buttonThatWasClicked==&ImgButGameCtrl4)	//En pause... on peux continuer ou arreter
               	{
               	processor.taiPong.gamePlayMode();
               	resized();
               	repaint();
               	}
			else
			if(buttonThatWasClicked==&ImgButGameCtrl5)	//En pause... on peux continuer ou arreter
               	{
               	processor.taiPong.gameResetMode();
               	resized();
               	repaint();
               	}
			}			
		}
}

//===============================================================================================
//bool TaiMotherShipPluguinProcessorEditor::GetTransportState (AudioPlayHead::CurrentPositionInfo pos)
//
//===============================================================================================
bool TaiMotherShipPluguinProcessorEditor::GetTransportState (AudioPlayHead::CurrentPositionInfo pos)
{
	if (pos.isPlaying==true)
    	return true;
    return false;
   
}

//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::taiPungInit(void)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::taiPungInit(void)
{
processor.taiPong.setGameHeight(sFaderH);
processor.taiPong.setGameWidth(largeFaderW+(sFaderW*processor.viewSize));
processor.taiPong.setGameOffset(largeFaderW);

processor.gameleftEndWidth=largeFaderW;
processor.gameWidth=largeFaderW+(sFaderW*processor.viewSize);
processor.gameHeight=sFaderH;

ballHeight=20;
ballWidth=20;

padHeight=100;
padWidth=10;
ballX=processor.gameleftEndWidth+((int)(processor.gameWidth-processor.gameleftEndWidth)/2)-(ballWidth/2);
ballY=(int)sFaderH/2;
pad1X=processor.gameleftEndWidth+50;
pad1Y=sFaderH/2;
pad2X=processor.gameWidth-50-padWidth;
pad1Y=sFaderH/2;

processor.taiPong.ball.setXYPos(ballX,ballY);
processor.taiPong.ball.setXYDim(ballWidth,ballHeight);

processor.taiPong.pad1.setXYPos(pad1X,pad1Y);
processor.taiPong.pad1.setXYDim(padWidth,padHeight);

processor.taiPong.pad2.setXYPos(pad2X,pad2Y);
processor.taiPong.pad2.setXYDim(padWidth,padHeight);

ball.setXYPos(ballX,ballY);
ball.setXYDim(ballWidth,ballHeight);

pad1.setXYPos(pad1X,pad1Y);
pad1.setXYDim(padWidth,padHeight);

pad2.setXYPos(pad2X,pad2Y);
pad2.setXYDim(padWidth,padHeight);

player1Score.setXYPos(processor.gameleftEndWidth+((int)((processor.gameWidth-processor.gameleftEndWidth)/6)*2)-30,40);
player2Score.setXYPos(processor.gameleftEndWidth+((int)((processor.gameWidth-processor.gameleftEndWidth)/6)*4),40);

}
//===============================================================================================
//void TaiMotherShipPluguinProcessorEditor::paintPung (Graphics& g)
//
//===============================================================================================
void TaiMotherShipPluguinProcessorEditor::paintPung (Graphics& g)
{
if(SystemSizeRcvdFlag)
	{
	g.fillAll(Colours::black);
	//g.drawImageAt (blankbackground,0,0);
	g.drawImage (blankWideFaderPlate,
			-2,2,																		//Position
			int(largeFaderW/scalefactor),int(largeFaderH/scalefactor),					//Dimension of space to draw
			0,0,																		//Source x,y
			largeFaderW,largeFaderH,													//Dimension of source images
			false);
  	g.setColour (Colours::black);
  	g.setOpacity (1.0f);
  	g.drawRoundedRectangle(2,38,104,210,5,1);
	g.setColour (Colours::orange);

	//	if(processor.taiPong.getGameState()==STOP)
	//		doSnowflakes(g);
	if(processor.taiPong.getGameState()==WINNER)
		{
		player1Score.paint(g);
		player2Score.paint(g);
		g.setFont((Font(Font::getDefaultMonospacedFontName(), 40.0f, Font::bold)));
		g.setColour (Colours::white);
		g.drawSingleLineText("PLAYER "+String(processor.taiPong.getWinner())
							+" WINS!", (815/2)-largeFaderW,largeFaderH/2,Justification::left);
		}
 	else
		{
	  	for(int dashNumber=0;dashNumber<getHeight()/20;dashNumber++)
			{
			if(dashNumber%2)
				g.fillRect(largeFaderW+((getWidth()-largeFaderW)/2)-5,dashNumber*(getHeight()/20),5,getHeight()/20);
			}
		player1Score.paint(g);
		player2Score.paint(g);
		if(!editorJustStartedFlag)
			{
			if(processor.taiPong.getGameState()!=STOP)
				ball.paint(g);
			if(processor.taiPong.getGameState()!=DEMO)
				{
				pad1.paint(g);
				pad2.paint(g);
				}
			}
		}
	}
}




//===============================================================================================
//void MyLookAndFeel_V4::initDbTable(int interfaceType)
//
//===============================================================================================
void MyLookAndFeel_V4::initDbTable(int interfaceType)
{
if(interfaceType==0)//le range qui va placer
	{
/*
	insteps[0]=24.54f;	//10db <> 5db
	insteps[1]=58.50f;	//10db <> 5db
	insteps[2]=91.0f;	//5db <> 0db
	insteps[3]=121.0f;	//0 <> -5db
	insteps[4]=151.0f;	//-5db <> -10db 
	insteps[5]=181.0f;	//-10db <> -15db
	insteps[6]=211.0f;	//-15db <> -20db
	insteps[7]=230.0f;	//-20db <> -25db
	insteps[8]=249.0f;	//-25db <> -30db
	insteps[9]=296.0f;	//-30db <> -50db
	insteps[10]=328.0f;
*/
	insteps[0]=0.0f;	//10db <> 5db
	insteps[1]=5.0f;	//10db <> 5db
	insteps[2]=10.0f;	//5db <> 0db
	insteps[3]=15.0f;	//0 <> -5db
	insteps[4]=20.0f;	//-5db <> -10db 
	insteps[5]=25.0f;	//-10db <> -15db
	insteps[6]=30.0f;	//-15db <> -20db
	insteps[7]=35.0f;	//-20db <> -25db
	insteps[8]=40.0f;	//-25db <> -30db
	insteps[9]=60.0f;	//-30db <> -50db
	insteps[10]=101.0f;

/*	---------20 steps scale--------
	insteps[0]=0.0f;	// > 9db
	insteps[1]=2.5f;	//9db <> 7.5db
	insteps[2]=5.0f;	//7.5db <> 5db
	insteps[3]=7.5f;	//5db <> 2.55db
	insteps[4]=10.0f;	//2.5db <> 0db 
	insteps[5]=12.5f;	//0db <> -2.5db
	insteps[6]=15.0f;	//-2.5db <> -5db
	insteps[7]=17.5f;	//-5db <> -7.5db
	insteps[8]=20.0f;	//-7.5db <> -10db
	insteps[9]=22.5f;	//-10db <> -12.5db
	insteps[10]=25.0f;	//12.5 <> -15.0db
	insteps[11]=27.5f;	//-15db <> -17.5db
	insteps[12]=30.0f;	//-17.5db <> -20db
	insteps[13]=32.5f;	//-20db <> -22.5db
	insteps[14]=35.0f;	//-22.5db <> -25db 
	insteps[15]=37.5f;	//-25db <> -27.5db
	insteps[16]=40.0f;	//-27.5db <> -30db
	insteps[17]=45.0f;	//-20db <> -25db
	insteps[18]=50.0f;	//-25db <> -30db
	insteps[19]=60.0f;	//-30db <> -50db
*/


	outsteps[0]=16.30f;		//10db
	outsteps[1]=46.25f;		//5db
	outsteps[2]=73.5f;		//0d
	outsteps[3]=97.5f;		//-5
	outsteps[4]=124.5f;		//-10
	outsteps[5]=151.0f;		//-15
	outsteps[6]=176.0f;		//-20
	outsteps[7]=193.5f;		//-25
	outsteps[8]=210.5f;		//-30
	outsteps[9]=250.0f;		//-50
	outsteps[10]=275.0f;	//-inf
	}
else
if(interfaceType==1)
	{
	insteps[0]=0.0f;	//10db <> 5db
	insteps[1]=5.0f;	//10db <> 5db
	insteps[2]=10.0f;	//5db <> 0db
	insteps[3]=15.0f;	//0 <> -5db
	insteps[4]=20.0f;	//-5db <> -10db 
	insteps[5]=30.0f;	//-10db <> -20db
	insteps[6]=40.0f;	//-20db <> -30db
	insteps[7]=50.0f;	//-30db <> -40db
	insteps[8]=60.0f;	//-40db <> -50db
	insteps[9]=90.0f;	//-50db <> -90db
	insteps[10]=101.0f;//-90db <> -infdb

/*
	insteps[0]=26.5373f;	//25.0f; 10db
	insteps[1]=58.45f; 		//5db
	insteps[2]=91.632f;		//0db
	insteps[3]=121.02;		//121.0f;	-5db
	insteps[4]=151.04f;		//151.0f;	-10db
	insteps[5]=211.39f;		//211.0f;	-20db
	insteps[6]=249.0f;		//249.0f;	-30db
	insteps[7]=286.77f;		//287.0f; 	-40db
	insteps[8]=297.0f;		//296.0f;	-50db
	insteps[9]=327.0f;		//328.0f; 	-90
	insteps[10]=328.0f;		//328.0f; 	-inf
*/
	outsteps[0]=20.8247f;	//10.0f;	10db
	outsteps[1]=51.269f;	//64.5f;	5db
	outsteps[2]=82.30f;		//99.5f;	0db
	outsteps[3]=115.658f;	//142.5f;	-5db
	outsteps[4]=149.39f;	//181.7f;	-10db
	outsteps[5]=179.53f;	//218.6f;	-20db
	outsteps[6]=213.782f;	//258.8f;	-30db
	outsteps[7]=235.996f;	//290.0f;	-40db
	outsteps[8]=256.04f;	//309.9f;	-50db
	outsteps[9]=274.658f;	//325.0f;	-inf
	outsteps[10]=275.0f;		//325.0f;	-inf
	}
else
	{
	insteps[0]=0.0f;	//> 9db
	insteps[1]=5.0f;	//9db <> 5db
	insteps[2]=10.0f;	//5db <> 0db
	insteps[3]=15.0f;	//0 <> -5db
	insteps[4]=20.0f;	//-5db <> -10db 
	insteps[5]=30.0f;	//-10db <> -20db
	insteps[6]=40.0f;	//-20db <> -30db
	insteps[7]=50.0f;	//-30db <> -40db
	insteps[8]=60.0f;	//-40db <> -50db
	insteps[9]=90.0f;	//-50db <> -90db
	insteps[10]=101.0f;//-90db <> -infdb
/*
	insteps[0]=26.5373f;	//25.0f; 10db
	insteps[1]=58.45f; 		//5db
	insteps[2]=91.632f;		//0db
	insteps[3]=121.02;		//121.0f;	-5db
	insteps[4]=151.04f;		//151.0f;	-10db
	insteps[5]=211.39f;		//211.0f;	-20db
	insteps[6]=249.0f;		//249.0f;	-30db
	insteps[7]=286.77f;		//287.0f; 	-40db
	insteps[8]=297.0f;		//296.0f;	-50db
	insteps[9]=323.0f;		//328.0f; 	-inf
	insteps[10]=328.0f;		//328.0f; 	-inf
*/
	outsteps[0]=20.8247f;	//10.0f;	10db
	outsteps[1]=51.269f;	//64.5f;	5db
	outsteps[2]=82.30f;		//99.5f;	0db
	outsteps[3]=115.658f;	//142.5f;	-5db
	outsteps[4]=149.39f;	//181.7f;	-10db
	outsteps[5]=179.53f;	//218.6f;	-20db
	outsteps[6]=213.782f;	//258.8f;	-30db
	outsteps[7]=235.996f;	//290.0f;	-40db
	outsteps[8]=256.04f;	//309.9f;	-50db
	outsteps[9]=274.658f;	//325.0f;	-inf
	outsteps[10]=275.0f;		//325.0f;	-inf
	}
	
for (int n=0;n<10;n++)
	k[n]=(outsteps[n+1]-outsteps[n])/(insteps[n+1]-insteps[n]);
}

//===============================================================================================
//void MyLookAndFeel_V4::drawLinearSlider (Graphics& g,
//
//===============================================================================================
void MyLookAndFeel_V4::drawLinearSlider (Graphics& g,
                                    int x, int y,
                                    int width, int height,
                                    float sliderPos,
                                    float minSliderPos,
                                    float maxSliderPos,
                                    const Slider::SliderStyle style,
									Slider& slider)

{
	drawLinearSliderBackground (g, x, y, width, height,
								sliderPos, minSliderPos, maxSliderPos,
                                    style, slider);
	drawLinearSliderThumb(g, x, y, width, height,
                            	sliderPos, minSliderPos+50, maxSliderPos-50,
                                style, slider);
}

//===============================================================================================
//void MyLookAndFeel_V4::drawLinearSliderBackground(Graphics& g,
//
//===============================================================================================
void MyLookAndFeel_V4::drawLinearSliderBackground(Graphics& g,
                                        	int x, int y,
                                        	int width, int height,
                                         	float sliderPos,
                                         	float minSliderPos,
                                         	float maxSliderPos,
                                         	const Slider::SliderStyle style,
                                         	Slider& slider)
{
    const float sliderRadius = (float) 6;
	const Colour trackColour = Colours::black ;
	auto ix = x + width * 0.5f - sliderRadius * 0.5f;
	Path indent;
	indent.addRoundedRectangle (ix, y+10, sliderRadius , height -25, 5.0f);
    g.setColour (trackColour);
    g.fillPath (indent);
    g.setOpacity(MLFOnOffOpacity);
    g.strokePath (indent, PathStrokeType (0.5f));
}

///===============================================================================================
//void MyLookAndFeel_V4::drawLinearSliderThumb (Graphics& g,
//
//===============================================================================================
void MyLookAndFeel_V4::drawLinearSliderThumb (Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         const Slider::SliderStyle style,
                                         Slider& slider)
{
	float altSliderPos;
    float scalefactor =1.0f;
	float kx,ksx,ksy,ky,Ksyoffest;
    
    kx = int((x+15)/scalefactor);// - (width	 * 0.5f);
    ksx = int((x+3)/scalefactor);// - (width	 * 0.5f);
    
    int chn=slider.getName().getIntValue();
    switch(MLFAutoStateArray[chn])
    	{
    	case OFF:
    	case TRIMWRITE:
    	case WRITE:
    			altSliderPos=getScaledValue((float)MLFDawAbsDbValue[chn]/10); 
    			sliderPos=getScaledValue((float)MLFSSLFaderDbValue[chn]/10);
    	break;
    	case TRIMTOUCH_LATCH:
    	case TRIMREAD: altSliderPos=getScaledValue((float)MLFDawTrimDbValue[chn]/10);
    	case TOUCH_LATCH:
    	case READ:sliderPos=getScaledValue((float)MLFDawAbsDbValue[chn]/10);
    	break;
		}
    			
    if(chn==39)
		printf("    channel: %d   value:  %f \n",chn,sliderPos);

Ksyoffest=(sliderPos-(height/2))/(height/2)*5;

ky = sliderPos;//(faderknob.getHeight()/2);
ksy=sliderPos-(fadershadow.getHeight()/4)-(Ksyoffest)+5;

//Draw thumb shadow
g.setOpacity(0.9f);
g.drawImage (fadershadow,
			ksx,ksy,			//Position
			int(fadershadow.getWidth()/scalefactor),int(fadershadow.getHeight()/scalefactor),	//dimension of space to draw
			0,0,																				//source x,y
			fadershadow.getWidth(),fadershadow.getHeight(),
			false);

if(MLFOnOffState)
	g.setOpacity(1.0f);
else
	g.setOpacity(0.8f);

switch(MLFAutoStateArray[chn])
	{
	case WRITE:
	case TRIMWRITE:
		g.drawImage (redfaderknob,
			kx,ky,																				//Position
			int(redfaderknob.getWidth()/scalefactor),int(redfaderknob.getHeight()/scalefactor),	//dimension of space to draw
			0,0,																				//source x,y
			redfaderknob.getWidth(),redfaderknob.getHeight(),
			false);
	break;
	default:
		g.drawImage (faderknob,
			kx,ky,																				//Position
			int(faderknob.getWidth()/scalefactor),int(faderknob.getHeight()/scalefactor),		//dimension of space to draw
			0,0,																				//source x,y
			faderknob.getWidth(),faderknob.getHeight(),
			false);
	break;
	}

if(MLFLinkStateArray[chn])	
	{
	if(!(chn%2))
		g.drawImage (linkBar,
					kx,ky+23,			//Position
					int((linkBar.getWidth()+15)/scalefactor),int(linkBar.getHeight()/scalefactor),			//dimension of space to draw
					0,0	,																				//source x,y
					linkBar.getWidth(),linkBar.getHeight(),
					false);
	else
		g.drawImage (linkBar,
					kx-18,ky+23,			//Position
					int((linkBar.getWidth()+15)/scalefactor),int(linkBar.getHeight()/scalefactor),			//dimension of space to draw
					0,0,																				//source x,y
					linkBar.getWidth(),linkBar.getHeight(),
					false);
	}

}

//===============================================================================================
//void MyLookAndFeel_V4::getScaledValue(int faderDbValue)
//
//===============================================================================================
float MyLookAndFeel_V4::getScaledValue(float faderDbValue)
{
if(faderDbValue<=insteps[0]+1)
	return outsteps[0];						// >= 9db
else
if(faderDbValue<=insteps[1])
	return outsteps[0]+(k[0]*(faderDbValue-insteps[0]));	// 9db a 5db
else
if(faderDbValue<=insteps[2])
	return outsteps[1]+(k[1]*(faderDbValue-insteps[1]));// 5db a 0db
else
if(faderDbValue<=insteps[3])
	return outsteps[2]+(k[2]*(faderDbValue-insteps[2]));	// 0db a -5db
else
if(faderDbValue<=insteps[4])
	 return outsteps[3]+(k[3]*(faderDbValue-insteps[3]));  // -5db a -10db
else
if(faderDbValue<=insteps[5])
	return outsteps[4]+(k[4]*(faderDbValue-insteps[4]));	// -10db a -20db
else
if(faderDbValue<=insteps[6])
	return outsteps[5]+(k[5]*(faderDbValue-insteps[5]));	// -20db a -30db
else
if(faderDbValue<=insteps[7])
	return outsteps[6]+(k[6]*(faderDbValue-insteps[6]));	// -30db a -40db
else
if(faderDbValue<=insteps[8])
	return outsteps[7]+(k[7]*(faderDbValue-insteps[7]));	// -40db a -50db
else
if(faderDbValue<=insteps[9])
	return outsteps[8]+(k[8]*(faderDbValue-insteps[8]));	// -50db a -infdb
	
return outsteps[9]+(k[9]*(faderDbValue-insteps[9]));	// -50db a -infdb
}


//===============================================================================================
//void MyLookAndFeel_V4::MLFPassConfigData(int index,int data)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassConfigData(int index,int data)
{
	MLFConfigRegister[index]=data;
}

//===============================================================================================
//void MyLookAndFeel_V4::MLFPassOnOffState(int state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassOnOffState(int state)
{
	MLFOnOffState=state;
}

//===============================================================================================
//void MyLookAndFeel_V4::MLFPassOnOffOpacitiy(int opacity)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassOnOffOpacitiy(int opacity)
{
	MLFOnOffOpacity=opacity;
}

//===============================================================================================
//void MyLookAndFeel_V4::MLFPassAutoState(int index,int state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassAutoState(int index,int state)
{
	MLFAutoStateArray[index]=state;
}

//===============================================================================================
//int MyLookAndFeel_V4::MLFGetAutoState(int index)
//
//===============================================================================================
int MyLookAndFeel_V4::MLFGetAutoState(int index)
{
	return MLFAutoStateArray[index];
}

//===============================================================================================
//void MyLookAndFeel_V4::MLFPassTouchState(int index,bool state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassTouchState(int index,bool state)
{
	MLFTouchStateArray[index]=state;
}

//===============================================================================================
//void MyLookAndFeel_V4::MLFPassMapState(int index,bool state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassMapState(int index,bool state)
{
	MLFChannelMapStateArray[index]=state;
}
//===============================================================================================
//void MyLookAndFeel_V4::MLFPassLinkState(int index,bool state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassLinkState(int index,bool state)
{
	MLFLinkStateArray[index]=state;
}

//===============================================================================================
//bool MyLookAndFeel_V4::MLFGetTouchState(int index)
//
//===============================================================================================
bool MyLookAndFeel_V4::MLFGetTouchState(int index)
{
	return MLFTouchStateArray[index];
}

//===============================================================================================
//void MyLookAndFeel_V4::MLFPassDawAbsDbValue(int index,int state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassDawAbsDbValue(int chn,int value)
{
	MLFDawAbsDbValue[chn]=value;
}
//===============================================================================================
//void MyLookAndFeel_V4::MLFPassDawTrimDbValue(int index,int state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassDawTrimDbValue(int chn,int value)
{
	MLFDawTrimDbValue[chn]=value;
}
//===============================================================================================
//void MyLookAndFeel_V4::MLFPassDawAbsDbValue(int index,int state)
//
//===============================================================================================
void MyLookAndFeel_V4::MLFPassSSLFaderDbValue(int chn,int value)
{
	MLFSSLFaderDbValue[chn]=value;
}

//===============================================================================================
//bool MyLookAndFeel_V4::MLFGetDawAbsDbValue(int chn)
//
//===============================================================================================
int MyLookAndFeel_V4::MLFGetDawAbsDbValue(int chn)
{
	return MLFDawAbsDbValue[chn];
}

//===============================================================================================
//bool MyLookAndFeel_V4::MLFGetDawTrimDbValue(int chn)
//
//===============================================================================================
int MyLookAndFeel_V4::MLFGetDawTrimDbValue(int chn)
{
	return MLFDawTrimDbValue[chn];
}

//===============================================================================================
//bool MyLookAndFeel_V4::MLFGetSSLFaderDbValue(int chn)
//
//===============================================================================================
int MyLookAndFeel_V4::MLFGetSSLFaderDbValue(int chn)
{
	return MLFSSLFaderDbValue[chn];
}

