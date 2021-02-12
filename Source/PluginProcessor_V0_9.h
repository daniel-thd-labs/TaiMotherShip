/* matched to PluginProcessor_V0_9w.cpp (build 2021.02.12)
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
#include "PipeComm.h"
#include "taigames.h"
#define SSM_MULTICAST "232.1.1.1"
#define _VERSION "V0.9y 2021-02-12"

//==============================================================================
/**
    As the name suggest, this class does the actual audio processing.
*/
class TaiMotherShipPluguinProcessor  : public AudioProcessor, public AudioProcessorValueTreeState::Listener,
										private OSCReceiver,
                                    	private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::RealtimeCallback>,
										private Timer

{
public:
    //==============================================================================
    TaiMotherShipPluguinProcessor();
    ~TaiMotherShipPluguinProcessor();

   //==============================================================================
    bool hasEditor() const override                                             { return true; }
    AudioProcessorEditor* createEditor() override;
    const String getName() const override                                       { return JucePlugin_Name; }
    bool acceptsMidi() const override                                           { return true; }
    bool producesMidi() const override                                          { return true; }
	void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    double getTailLengthSeconds() const override                                { return 0.0; }
    int getNumPrograms() override                                               { return 0; }
    int getCurrentProgram() override                                            { return 0; }
    void setCurrentProgram (int /*index*/) override                             {}
    const String getProgramName (int /*index*/) override                        { return String(); }
    void changeProgramName (int /*index*/, const String& /*name*/) override     {}
    void getStateInformation (MemoryBlock&) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void updateTrackProperties (const TrackProperties& properties) override;
	bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

	void gameManager(void);
	void playStateManager(void);
	void ininitAll(void);

	void manageSSLFaderDataAutoOff(int n, int localSSLfaderval , bool changedFlag, int touchState);
	void manageSslFaderDataAutoWRITE(int n, int localSSLfaderval,bool changedFlag);
	void manageSslFaderDataAutoREAD(int n,int localSSLfaderval,bool changedFlag,bool cutChangedFlag, int touchState);
	void manageSslFaderDataAutoTRIMWRITE(int n,int localSSLfaderval,bool changedFlag, bool cutChangedFlag,int touchState);
	
	void manageSslCutDataAutoOff(int n, int localSSLCutState,bool cutChangedFlag);
	void manageSslCutDataAutoWRITE(int n, int localSSLCutState, bool changedFlag, bool cutChangedFlag);
	void manageSslCutDataAutoTRIMWRITE(int n,int localSSLCutState,bool changedFlag,bool cutChangedFlag);
	void manageSslCutDataAutoREAD(int n,int localSSLCutState,bool cutChangedFlag);
	
	void endVolumeGestureOnChannel(int channel);
	void startVolumeGestureOnChannel(int channel);
	void endCutGestureOnChannel(int channel);
	void startCutGestureOnChannel(int channel);
	void endTrimGestureOnChannel(int channel);
	void startTrimGestureOnChannel(int channel);
	int testMotherShipMapped(void);
	void updateChnPipeState(int n,int state);
	void parameterChangeManager(unsigned int parametertype,unsigned int chn,float Value);
	int ParseConfigReg(void);
    int configFileRead(void);
    void faderMoveTest(void);
	void manageCutWrites(unsigned char chan);
    int debugvalue1;
    int debugvalue2;
    int debugvalue3;
    int debugvalue4;
    bool newAutoModeStateFlag;
	void DoPente(void);
	unsigned int DataToDB(float data);
	float DBToData(unsigned int data);
	void rwEnableButtonMng(void);
//new UDP code
//---------------------------------------
	int InitUdpConnection(int port);
	int CloseUdpConnection(unsigned int state);
	void HeartBeatManager(void);
	void startListeningToUDP(void);
	void stopListeningToUDP(void);
	
	void doChannelMsg(int type,int channel);
	void txChannelMsg(void);

	void gestureManager(int channel, int SSLAutoState);
	void txCfgMsg(int index,int value,int state);
	int parseOscAssignData(void);
	void parseOscFaderData(void);
	void parseOscAuxData(void);
	void parseOscCfgData(void);
	void configRegMsgParser(int index,int cfgData);
	void doRegisterMsChannel(void);
	void doUnRegisterMsChannel(void);
	void newChnMng(int newchn);
	void clrChnMng(int chn);
	int linkManager(void);

	unsigned char ChannelWriteCut[96];
	int isMappedToSinglechannel[96];

	unsigned char ChannelLinkMap[96];
	bool changedLinkMap[96];
	int linkChange;
	bool refreshStatusSwtPresetFlag;

	//the pipes	

	outputMsDataPipe motherShipDataPipe;
	outputMsMapLinkPipe motherShipMapLinkPipe;
	outputMsCfgPipe motherShipCfgPipe;

	unsigned char pipeMsDataBuffer[MSDATASIZE];
	unsigned char pipeMsMapLinkBuffer[MSMAPLINKSIZE];
	unsigned char pipeMsCfgBuffer[MSCFGSIZE];

	//OwnedArray<outputChnPipe> chnPipe;
	const unsigned char bitIndex[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	int justStartedCnt;
	bool justStartedFlag;
	int PluginConfigState;
	int HeartBeatTicks;
	bool newChannelMsgFlag;
	bool newOscMsg;
	MemoryBlock oscMsgMemBlck;
 	int Initlevel;
	int LastInitlevel;
	int64 ProgStartTime;
	bool globalReadVolAutoState;
	bool globalReadCutAutoState;
 	bool globalWriteVolAutoState;
	bool globalWriteCutAutoState;
	bool globalReadTrimAutoState;
	unsigned char globalRWAutoState;
	int trkLabelRefreshFlag;
	bool refreshFlag;
	int mixOnOffState;
    bool playstate;
	bool heartBeatToggle;
	bool ConfigRegInitFlag;
	unsigned char TxConnectState;
	bool prevTouchState[96];
    unsigned char pluginAutoMode[96];
	unsigned int tempOscVal[96];
	unsigned int bnk;
	//unsigned int iniSistemSys;
	int taiType;
	int SystemSize;
//-------------------------------------------------------
//Start of Easter egg section
//taiPung	
//-------------------------------------------------------
    taiPongGame taiPong;
	float ballX;
	float ballY;
	float pad1X;
	float pad1Y;
	float pad2X;
	float pad2Y;
	float gameHeight;
	float gameWidth;
	float gameleftEndWidth;
	bool demoMode;
	int gameState;
//-------------------------------------------------------
//End of easterEgg section	
//-------------------------------------------------------
	bool shiftKeyState;
	bool ctrlKeyState;
	bool cmdKeyState;
	bool altKeyState;
	bool setAllDisplay;

	bool faderIsDragged[96];
//    unsigned char pluginAutoRWStates;
	unsigned int faderDbRef[96];
	unsigned int trimDeltaDbValue[96];
	unsigned int trimStartDbValue[96];
	unsigned int SSLDbFaderValue[96];
//	unsigned int SSLDbTrimValue[96];
	unsigned int SSLCutState[96];
	bool editorCutstate[96];

	unsigned int CurrentDawFaderValue[96];
	//unsigned int dawDbFaderValue[96];
	//unsigned int dawDbTrimValue[96];
    float SSLScaledFaderValue[96];
	bool SSLTouchStateArray[96];
	unsigned int SSLAutomodeArray[96];
    unsigned int SSLPrevAutomodeArray[96];
    unsigned int DAWAutomodeArray[96];
    unsigned int dawWasInTouch[96];
    unsigned int TxFaderValue[96];
    unsigned int TxTrimValue[96];
    float singleChannelVolValue[96];
    float singleChannelCutValue[96];
    float singleChannelTrimValue[96];
    bool hostVolParameterExist[96];
    bool hostTrimParameterExist[96];    
	int statusSwtPreset;
	bool newStatusSwtPresetFlag;

    bool VolGestureStateArray[96];
	bool TrimGestureStateArray[96];
	bool CutGestureStateArray[96];
	bool GestureStateArray[96];
	int bank;
   	int viewSize;

	unsigned char bankSndFlag[12];
    unsigned char BankRefreshCnt[12];
	int ConfigReg[128];
    int refreshTimOut;

	float Pente_M[14];
	float DAWFaderStep[15];
	int SSLFaderStep[15];
	char *tai_local_address;
    MemoryOutputStream TrackName;
    //Colours Trackcolor;
    String ptTrackName[96];
     //MemoryOutputStream PlayState;

 	int64 timelaps1;
	//int64 timelaps2;
	//int64 timelaps3;
	//int64 timelaps4;
	//int64 time1;
	int64 time2;
	//int64 time3;
	//int64 time4;
	
    String debugtext;
	//AAX_CString bypassID;
//==============================================================================
/*
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (! isUsingDoublePrecision());
        process (buffer, midiMessages, delayBufferFloat);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer& midiMessages) override
    {
        jassert (isUsingDoublePrecision());
        process (buffer, midiMessages, delayBufferDouble);
    }
*/

     AudioPlayHead::CurrentPositionInfo lastPosInfo;

    int lastUIWidth = 400, lastUIHeight = 200;
	//OSCMessage Txmessage;
	
    // Current track colour and name
    TrackProperties trackProperties;
	float ptNewValue;
	int PTIntValue1;
	int PTIntValue2;
private:
    //==============================================================================
	void oscMessageReceived (const OSCMessage& message) override;
	void showConnectionErrorMessage (const String& messageText);
    void timerCallback() override ;
	void parameterChanged(const String &parameterID, float newValue) override;
	//OwnedArray<Parameter> TheSliders;
	//OwnedArray<ImageButton> TheMutes;

    unsigned int TxChan;
    AudioProcessorValueTreeState parameters;
    //OSCSender sender;
    const char* hostname = "tai.local";
    //int AssignedChannel=0;
    //bool AutoAssignFlag;
	//int Txboundedport;
	int udpRxPort;

    std::unique_ptr<DatagramSocket>rxSocket;
    std::unique_ptr<OSCReceiver> oscReceiver;

	
	
	int TxMsgCode=1;
    //int TxConfigIndex;
	//unsigned int ReadyAndConfigState;
    unsigned int HeartBeatCnt;
	
	unsigned char AutoMode;
    unsigned char TxAutoMode[96];
    bool MuteState;
    bool TxMuteState[96];
	void updateCurrentTimeInfoFromHost();
    static BusesProperties getBusesProperties();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TaiMotherShipPluguinProcessor)
};
