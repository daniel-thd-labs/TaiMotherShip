/*
  ==============================================================================

    Processor_fonctions.cpp
    Created: 12 Jul 2020 7:51:15pm
    Author:  Daniel

  ==============================================================================
*/
#include "PluginProcessor_V0_9w.h"

#define _JUCE_5
#define OFF 0
#define WRITE 1
#define TOUCH_LATCH 2
#define READ 3
#define TRIMWRITE 5
#define TRIMTOUCH_LATCH 6
#define TRIMREAD 7

#define WRITECUT 0x04
#define WRITEVOL 0x08
#define READCUT	0x01
#define READVOL 0x02
#define ULTIMATION 0x01

//#include "Processor_fonctions.h"
void TaiMotherShipPluguinProcessor::ininitAll(void)
{
SystemSize=configFileRead();
if(!SystemSize)
	SystemSize=8;
viewSize=8;
shiftKeyState=false;
ctrlKeyState=false;
cmdKeyState=false;
altKeyState=false;
shiftKeyState=false;
refreshFlag=false;

globalReadVolAutoState=true;
globalReadCutAutoState=true;
globalWriteVolAutoState=true;
globalWriteCutAutoState=true;
globalReadTrimAutoState=true;
globalRWAutoState=0x0f;
linkChange=0;
bank=0;

justStartedFlag=true;
newOscMsg=false;

HeartBeatTicks=30;
if(InitUdpConnection(udpRxPort)>0)
	{
	PluginConfigState=1;
	justStartedCnt=5;
	startListeningToUDP();
	//if(!taiPipe.openMsCfgPipe())
	motherShipDataPipe.openPipe();
	motherShipCfgPipe.openPipe();
	motherShipMapLinkPipe.openPipe();
	txCfgMsg(0,0,1);
	}
else
	{
	PluginConfigState=0;
	justStartedCnt=1;
	}
#ifdef _PRINTMSG
else
	{
	printf("ERROR opening MotherShip config pipe file avaliable\n");		
	}
#endif

	
int maxSystemSize=96;
for(char n=0;n<maxSystemSize;n++)
	{
  	SSLDbFaderValue[n]=0;
    SSLScaledFaderValue[n]=0;
	CurrentDawFaderValue[n]=0;
	pipeMsDataBuffer[n]=0;
	pluginAutoMode[n]=0;
	SSLPrevAutomodeArray[n]=0;
    DAWAutomodeArray[n]=0;
    dawWasInTouch[n]=0;
	
	editorCutstate[n]=false;
    SSLCutState[n]=false;
	
	GestureStateArray[n]=false;
	VolGestureStateArray[n]=false;
    TrimGestureStateArray[n]=false;
	CutGestureStateArray[n]=false;
	
	SSLTouchStateArray[n]=false;
	ptTrackName[n]=String(n+1);
	
	isMappedToSinglechannel[n]=0;
    trkLabelRefreshFlag=1;
 	
	singleChannelVolValue[n]=0;
    singleChannelCutValue[n]=0;
    singleChannelTrimValue[n]=0;
    
	hostVolParameterExist[n]=0;
    hostTrimParameterExist[n]=0; 
   	faderIsDragged[n]=false;
   	changedLinkMap[n]=false;
   	ChannelLinkMap[n]=0;
	ChannelWriteCut[n]=0;
	TxFaderValue[n]=0;
	TxTrimValue[n]=0;
	TxMuteState[n]=0;
	TxAutoMode[n]=0;
	}
	
for(int n=0;n<MSDATASIZE;n++)
	pipeMsDataBuffer[n]=0;
for(int n=0;n<MSMAPLINKSIZE;n++)
	pipeMsMapLinkBuffer[n]=0;
for(int n=0;n<MSCFGSIZE;n++)
	pipeMsCfgBuffer[n]=0;

//for(char n=0;n<maxSystemSize;n++)
//	chnPipe.add(new outputChnPipe());
	
for(char bnk=0;bnk<maxSystemSize/8;bnk++)
    BankRefreshCnt[bnk]=0;

statusSwtPreset=0;
mixOnOffState=0;
refreshTimOut=30;
ConfigRegInitFlag=false;
linkChange=0;
playstate=true;
DoPente();
lastPosInfo.resetToDefault();
TxConnectState =false;
//AssignedChannel=TxChan;
PluginConfigState=0;
Initlevel=0;
ProgStartTime=60;
bnk=0;
String tname;
newChannelMsgFlag=false;

using Parameter = AudioProcessorValueTreeState::Parameter;
for(int i = 0; i < SystemSize; i++)
	{
	if(i<(SystemSize-8))
		tname="FADER "+String(i+1);
	else
		tname="VCA"+String((i+1)-(SystemSize-8));
	
	parameters.createAndAddParameter(std::make_unique<Parameter> ("V"+String(i), 
									 tname,
									"db",
									NormalisableRange<float>(0.000f, 1.000f, 0.001f,1.0f,false),//NormalisableRange
									0.001f,//default value
									[this](float value){return String((float)((float)100-DataToDB(value))/10)+"db";},//value to text function
									nullptr,//text to value function
									false,//bool isMetaParameter
									true,//bool isAutomatableParameter
									false));//bool is discreet
    parameters.addParameterListener("V"+String(i), this);
	}
										
for(int i = 0; i < SystemSize; i++)
	{
	if(i<(SystemSize-8))
		tname="CUT "+String(i+1);
	else
		tname="CUT VCA"+String((i+1)-(SystemSize-8));

	parameters.createAndAddParameter(std::make_unique<Parameter> ("CUT"+String(i),
									tname,
									String(),
									NormalisableRange<float>(0.000f, 1.000f),//NormalisableRange
									0.00f,//default value
									nullptr,//value to text function
									nullptr,//text to value function
									false,//bool isMetaParameter
									true,//bool isAutomatableParameter
									false));//bool is discreet
	parameters.addParameterListener("CUT"+String(i), this);
	}

for(int i = 0; i < SystemSize; i++)
	{
	if(i<(SystemSize-8))
		tname="TRIM "+String(i+1);
	else
		tname="TRIM VCA"+String((i+1)-(SystemSize-8));

	parameters.createAndAddParameter(std::make_unique<Parameter> ("T"+String(i),
									tname,
									String(),
									NormalisableRange<float>(0.000f, 1.000f, 0.001f,1.0f,false),
									0.75f,//default value
									[this](float value){return String((float)((float)100-DataToDB(value))/10)+"db";},//value to text function
									nullptr,//text to value function
									false,//bool isMetaParameter
									true,//bool isAutomatableParameter
									false));//bool is discreet
    parameters.addParameterListener("T"+String(i), this);

	}

for(int i = 0; i < SystemSize; i++)
	{
	parameters.createAndAddParameter(std::make_unique<Parameter> ("A"+String(i), "A"+String(i),String(),
								NormalisableRange<float>(0.00f, 8.00f), 0.00f, nullptr, nullptr,false,false,false));
    parameters.addParameterListener("A"+String(i), this);
	}

parameters.createAndAddParameter(std::make_unique<Parameter> ("RWS1", "FaderReadSwt",String(),
                                     NormalisableRange<float>(0.000f, 1.000f), 1.00f, nullptr, nullptr,false,false,false));
parameters.createAndAddParameter(std::make_unique<Parameter> ("RWS3", "CutReadSwt",String(),
								NormalisableRange<float>(0.000f, 1.000f), 1.00f, nullptr, nullptr,false,false,false));
parameters.createAndAddParameter(std::make_unique<Parameter> ("RWS2", "FaderWriteSwt",String(),
								NormalisableRange<float>(0.000f, 1.000f), 1.00f, nullptr, nullptr,false,false,false));
parameters.createAndAddParameter(std::make_unique<Parameter> ("RWS4", "CutWriteSwt",String(),
                                     NormalisableRange<float>(0.000f, 1.000f), 1.00f, nullptr, nullptr,false,false,false));

parameters.createAndAddParameter(std::make_unique<Parameter> ("STATUSPRESET", "statusPreset", String(),
                                     NormalisableRange<float>(0.000f, 7.000f), 0.00f, nullptr, nullptr,false,false,false));

parameters.createAndAddParameter(std::make_unique<Parameter> ("CONFIGREGISTER", "configRegister", String(),
                                     NormalisableRange<float>(0.0f, 255.0f), 0.00f, nullptr, nullptr,false,false,false));

parameters.createAndAddParameter(std::make_unique<Parameter>("MIXONOFFSTATE", "MixOnOffState", String(),
                                    NormalisableRange<float>(0.000f, 1.000f), 0.00f, nullptr, nullptr,false,false,false));
parameters.createAndAddParameter(std::make_unique<Parameter>("TRANSPORTSTATE", "transportState", String(),
                                    NormalisableRange<float>(0.000f, 1.000f), 0.00f, nullptr, nullptr,false,false,false));


parameters.addParameterListener("RWS1", this);
parameters.addParameterListener("RWS2", this);
parameters.addParameterListener("RWS3", this);
parameters.addParameterListener("RWS4", this);
parameters.addParameterListener("TRANSPORTSTATE", this);
parameters.addParameterListener("STATUSPRESET", this);
parameters.addParameterListener("CONFIGREGISTER", this);
parameters.addParameterListener("MIXONOFFSTATE", this);

parameters.state = ValueTree(Identifier("taiMotherShipPlugin"));

/*
for(int i = 0; i < SystemSize; i++)
	{
//	parameters.getParameter("V"+String(i))->endChangeGesture();
	parameters.getParameter("V"+String(i))->beginChangeGesture();
	parameters.getParameter("V"+String(i))->setValueNotifyingHost(0.001f);
	parameters.getParameter("V"+String(i))->endChangeGesture();
	}
*/

}

