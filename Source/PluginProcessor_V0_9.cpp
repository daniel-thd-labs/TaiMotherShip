//#define _LOGICSCALE
//#define _PRINTMSG
//#define __STANDALONE
/*
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
#define FRAMERATE 60
//#include "PipeComm.h"
#include "PluginProcessor_V0_9.h"
//#include "Processor_fonctions.h"
#include "PluginEditor_V0_9.h"
int testFunction(void){return 1;}

AudioProcessor* JUCE_CALLTYPE createPluginFilter();
//#define __debug



//==============================================================================
TaiMotherShipPluguinProcessor::TaiMotherShipPluguinProcessor()
    :AudioProcessor (getBusesProperties()),
    parameters(*this, nullptr),
    rxSocket (new DatagramSocket(false)),
    oscReceiver (new OSCReceiver())

{
ininitAll();
startTimerHz(FRAMERATE);
}

//==============================================================================
//
//
//==============================================================================
TaiMotherShipPluguinProcessor::~TaiMotherShipPluguinProcessor()
{
if(PluginConfigState==3)
	{
	txCfgMsg(0,0,0);
	int i;
	doUnRegisterMsChannel();
	i=100;
	while(i--);
	motherShipDataPipe.closePipe();
	motherShipCfgPipe.closePipe();
	motherShipMapLinkPipe.closePipe();
}
stopListeningToUDP();
CloseUdpConnection(0);

//    oscReceiver.stopThread();
//    rxSocket = new DatagramSocket(false);

for(int n=0;n<SystemSize;n++)
	{
    if(VolGestureStateArray[n]==true)
		{
        //AudioProcessorParameterWithID* ParaId1 = parameters.getParameter("V"+String(n));
		if(parameters.getParameter("V"+String(n))!=nullptr)
			parameters.getParameter("V"+String(n))->endChangeGesture();
		VolGestureStateArray[n]=false;
		}
    if(TrimGestureStateArray[n]==true)
		{
        //AudioProcessorParameterWithID* ParaId2 = parameters.getParameter("T"+String(n));
		if(parameters.getParameter("T"+String(n))!=nullptr)
			parameters.getParameter("T"+String(n))->endChangeGesture();
		CutGestureStateArray[n]=false;
		}
	if(CutGestureStateArray[n]==true)
		{
        //AudioProcessorParameterWithID* ParaId3 = parameters.getParameter("CUT"+String(n));
		if(parameters.getParameter("CUT"+String(n))!=nullptr)
			parameters.getParameter("CUT"+String(n))->endChangeGesture();
		CutGestureStateArray[n]=false;
		}
	}

/*
if(PluginConfigState)
	if(LastInitlevel)
        {
        while(LastInitlevel)
			LastInitlevel=CloseUdpConnection(LastInitlevel);
        }
*/
}

//==============================================================================
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::showConnectionErrorMessage (const String& messageText)
{
AlertWindow::showMessageBoxAsync (
								AlertWindow::WarningIcon,
								"Connection error",
								messageText,
								"OK");
}

//--------------------------------------------------------------------------------------
//
// Gestion d'initialisation de la communication du plugin et des changements de canal
//
//--------------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessor::timerCallback()
{
if(ProgStartTime)
	ProgStartTime--;
else
	{
	if((justStartedFlag)&&(PluginConfigState==3))
		{
		refreshFlag=false;
		justStartedFlag=false;
		doRegisterMsChannel();
		playstate=lastPosInfo.isPlaying;
		}
	HeartBeatManager();
	if(PluginConfigState==3)
		{
		playStateManager();
		linkManager();
		gameManager();
#ifdef __STANDALONE
		faderMoveTest();
#endif
		txChannelMsg();
		}
	}
}

//==============================================================================
// Fonction: playStateManager(void)
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::playStateManager(void)
{
if(playstate!=lastPosInfo.isPlaying)
	{
	playstate=lastPosInfo.isPlaying;
	//AudioProcessorParameterWithID* ParaIdTransportstate = parameters.getParameter("TRANSPORTSTATE");
	parameters.getParameter("TRANSPORTSTATE")->setValueNotifyingHost((float)playstate);
	}
}

//==============================================================================
// Fonction: gameManager(void)
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::gameManager(void)
{
if(gameState!=taiPong.getGameState())
	{
	gameState=taiPong.getGameState();
	if(gameState)
		{
		stopTimer();
		startTimerHz(300);
		}
	else
		{
		stopTimer();
		startTimerHz(FRAMERATE);
		}
		
	}
if(taiPong.getGameState())
	{
	taiPong.doTaiPong();
	ballX=taiPong.ball.getXPosWithOffset();
	ballY=taiPong.ball.getYPosWithOffset();
	}
}

//==============================================================================
// Fonction: linkManager(void)
//
//
//==============================================================================
int TaiMotherShipPluguinProcessor::linkManager(void)
{
if(linkChange)
	{
	for(int lnkChn=0;lnkChn<SystemSize;lnkChn++)
		{
		if(changedLinkMap[lnkChn])
			{
			changedLinkMap[lnkChn]=0;
			doChannelMsg(0,lnkChn);
			}
		}
	linkChange--;	
	}
return 0;
}

//==============================================================================
// Fonction: HeartBeatManager(void)
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::HeartBeatManager(void)
{
if(HeartBeatTicks)
	HeartBeatTicks--;
else
	{
	switch(PluginConfigState)
		{
		case 1:
			justStartedFlag=1;
			HeartBeatTicks=30;
			justStartedCnt=30;
			printf("WAITING FOR INJEKTOR:\njustStartedFlag: %d\n PluginConfigState :%d\n%d\n",justStartedFlag,PluginConfigState, heartBeatToggle);
		break;
		case 2:
			justStartedFlag=1;
			justStartedCnt=30;
			HeartBeatTicks=30;
		break;
		case 4:
			HeartBeatTicks=15;
			if(newOscMsg)
				newOscMsg=false;
			else
				PluginConfigState=2;
		break;
		}
	heartBeatToggle=heartBeatToggle^1;
	}
}

//==============================================================================
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::oscMessageReceived (const OSCMessage& message)
{
char dataChangedFlag;
if(message.getAddressPattern()==OSCAddressPattern("/InjektorGlobalMsg")) 
	{
	if(message[0].isBlob())
		{
		oscMsgMemBlck=message[0].getBlob();
		if(PluginConfigState==3)
			{
			if(oscMsgMemBlck[511])
				{
				dataChangedFlag=oscMsgMemBlck[511];	
				if(((dataChangedFlag&0x01)&&(!justStartedFlag))||refreshFlag)
					parseOscFaderData();
				if((dataChangedFlag&0x02)||refreshFlag)
					parseOscAssignData();
				if((dataChangedFlag&0x04)||refreshFlag)
					parseOscAuxData();
				if((dataChangedFlag&0x08)||refreshFlag)
					parseOscCfgData();
				if(refreshFlag)
					refreshFlag=false;			
				}
			newOscMsg=true;
			}	
		else
		if(oscMsgMemBlck[511])
			{
			if(!testMotherShipMapped())
				PluginConfigState=3;
			else
				PluginConfigState=2;
			}
		}
	}
}

//==============================================================================
// void TaiChannelPluginProcessor::parseOSCFaderData(void)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::parseOscFaderData(void)
{
int localSSLfaderval;
char localSSLCutState;
//bool newSSLTouchState;
int localSSLAutoMode;

int touchState;
int tempval;
bool changedFlag;
bool cutChangedFlag;
//printf("Fader  #1 value: %d \n",touchState);

for(int n=0;n<SystemSize;n++)
	{
	changedFlag=0;
	cutChangedFlag=0;
	//printf("New OSC message\n");
	tempval= ((unsigned char)oscMsgMemBlck[n*2]<<8)+((unsigned char)oscMsgMemBlck[(n*2)+1]);
	if(tempOscVal[n]!=tempval)
		{
		tempOscVal[n] = tempval;
		localSSLfaderval=tempval&0x03ff;									//bits 0 a 9
		localSSLCutState=(tempval&0x0400)>>10;								//bit 10
		localSSLAutoMode = (tempval&0x3800)>>11;							//bit 11,12,13 
		touchState= (tempval&0x4000)>>14;									//bit 14
		if(taiPong.getGameState())
			{
			if((n>=SystemSize-8)&&(n<=SystemSize))
				{
				if(SSLDbFaderValue[n]!=localSSLfaderval)
					{
					SSLDbFaderValue[n]=localSSLfaderval;
					taiPong.setPadYPos(n,(float)SSLDbFaderValue[n],SystemSize);
					}
				}
			}
		else
			{
			if(SSLCutState[n]!=localSSLCutState)
				{
				SSLCutState[n]=localSSLCutState;
				cutChangedFlag=true;
				}			
			if(prevTouchState[n]!=touchState)
				{
				prevTouchState[n]=touchState;
				}
			if(pluginAutoMode[n]!=localSSLAutoMode)
				{
				pluginAutoMode[n]=localSSLAutoMode;
				parameterChangeManager(3,n,localSSLAutoMode);
				if(localSSLAutoMode==TRIMWRITE)
					{
					trimStartDbValue[n]=DataToDB(parameters.getParameter("T"+String(n))->getValue());	//get last DAW Trim value from envelopp
					faderDbRef[n]=localSSLfaderval;
					}
				changedFlag=true;
				gestureManager(n, localSSLAutoMode);
				parameters.getParameter("A"+String(n))->setValueNotifyingHost((float)localSSLAutoMode/10);
				}
//------------------------------------------------------------------------
//OSC MESSAGE-------- fader and CUTS data Management ---------------------		
//------------------------------------------------------------------------
			switch (localSSLAutoMode)
				{
				case OFF://AUTO OFF
					dawWasInTouch[n]=0;
					//printf("SSL fader data received\n");
					manageSSLFaderDataAutoOff(n,localSSLfaderval,changedFlag,touchState);	//MANAGE OFF SSL FADER DATA
					manageSslCutDataAutoOff(n,localSSLCutState,cutChangedFlag);				//MANAGE OFF SSL CUT DATA
				break;
				case WRITE://AUTO WRITE
					if(dawWasInTouch[n]==1)
						dawWasInTouch[n]=2;
					manageSslFaderDataAutoWRITE(n,localSSLfaderval,changedFlag);			//MANAGE WRITE SSL FADER DATA
					manageSslCutDataAutoWRITE(n,localSSLCutState,changedFlag,cutChangedFlag);//MANAGE WRITE SSL CUT DATA
				break;
				case READ://AUTO READ
				case TRIMREAD://AUTO TRIM READ:
					dawWasInTouch[n]=0;
					manageSslFaderDataAutoREAD(n,localSSLfaderval,changedFlag,cutChangedFlag,touchState);	//MANAGE READ TRIMREAD SSL FADER DATA
					manageSslCutDataAutoREAD(n,localSSLCutState,cutChangedFlag);							//MANAGE READ TRIMREAD SSL CUT DATA
				break;
				case TRIMWRITE:
					dawWasInTouch[n]=2;
					manageSslFaderDataAutoTRIMWRITE(n,localSSLfaderval,changedFlag,cutChangedFlag,touchState);	//MANAGE TRIMWRITE SSL FADER DATA
					manageSslCutDataAutoTRIMWRITE(n,localSSLCutState,changedFlag,cutChangedFlag);				//MANAGE TRIM SSL CUT DATA
				break;
				case TOUCH_LATCH:
				case TRIMTOUCH_LATCH:
					dawWasInTouch[n]=1;
				break;
				default:break;
				}
			}
		}
	}
}

//==============================================================================
//
//	void parameterChanged(const String & parameterID, float	newValue)
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::parameterChanged(const String & parameterID, float	newValue)
{
int chn;
if((parameterID.substring(0,1))=="V")
	{
	hostVolParameterExist[parameterID.substring(1).getIntValue()]=1;
	chn=parameterID.substring(1).getIntValue();
	if(globalReadVolAutoState)
		parameterChangeManager(0,chn,newValue);
	
	}
	
if((parameterID.substring(0,3))=="CUT")
	{
	chn=parameterID.substring(1).getIntValue();
	if(globalReadCutAutoState)
		parameterChangeManager(1,chn,newValue);
	}

if((parameterID.substring(0,1))=="T")
	{
	chn=parameterID.substring(1).getIntValue();
	//if(globalReadTrimAutoState)
	parameterChangeManager(2,chn,newValue);
	}

if((parameterID.substring(0,1))=="A")
	{
	//parameterChangeManager(3,(parameterID.substring(1).getIntValue()),newValue);
	}

if((parameterID.substring(0,3))=="RWS")
	{
	parameterChangeManager(4,(parameterID.substring(3).getIntValue()),newValue);
	//txCfgMsg(0x24,newValue,1);

	}
if(parameterID=="TRANSPORTSTATE")
	#ifdef _PRINTMSG 
	printf("PLay state changed\n");
	#endif
	if(newValue)
		parameterChangeManager(5,0,newValue);

if(parameterID=="STATUSPRESET")
	{
	#ifdef _PRINTMSG 
	//printf("Status preset changed index:%d  Value:%d\n",34,ConfigReg[34]);
	#endif
	txCfgMsg(34,newValue,1);
	printf("Status preset changed index:%d  Value:%f\n",34,newValue);
	}
if(parameterID=="CONFIGREGISTER")
	{
	if(ConfigReg[0])
		{
		//printf("Config message index:%d  Value:%d\n",ConfigReg[0],(int)newValue);
		txCfgMsg(ConfigReg[0],newValue,1);
		ConfigReg[0]=0;
		}
	}
if(parameterID=="MIXONOFFSTATE")
		{
	//ConfigReg[0x20]=int(newValue);
	//mixOnOffState=ConfigReg[0x20];
	txCfgMsg(0x20,newValue,1);
	}
}

//--------------------------------------------------------------------------------------
//
// Gestion des changement de parametre des faders switch et autes
//
//--------------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessor::parameterChangeManager(unsigned int parametertype,unsigned int chn,float Value)
{
switch(parametertype)
	{
	//----CHANNEL VOLUME
	case 0:
       	if(!ChannelLinkMap[chn])
			{
			TxFaderValue[chn] =  DataToDB(*parameters.getRawParameterValue("V"+String(chn)));
       		ptNewValue=Value;
       		TxTrimValue[chn] = DataToDB(*parameters.getRawParameterValue("T"+String(chn)));
			doChannelMsg(0,chn);
       		//#ifdef _PRINTMSG 
			//printf("parameterChangeManager    FADER #%d Value:%d\n",chn,TxFaderValue[chn]);
       		//#endif
       		}
		else
       	if(ChannelLinkMap[chn])
       		{
			if(!(chn%2))
				{
				TxFaderValue[chn] =  DataToDB(Value);
				TxFaderValue[chn+1] =  DataToDB(Value);
		       	if(pluginAutoMode[chn]==TRIMREAD)
					{
					TxTrimValue[chn] = DataToDB(*parameters.getRawParameterValue("T"+String(chn)));
					TxTrimValue[chn+1] = DataToDB(*parameters.getRawParameterValue("T"+String(chn)));
					}
				doChannelMsg(0,chn);
				doChannelMsg(0,chn+1);
       			}
			}
	break;
    //------------------------------------------CHANNEL CUT
    case 1:

       	if(!ChannelLinkMap[chn])
			{
			if(Value)
				TxMuteState[chn] = 1;
			else
				TxMuteState[chn] = 0;
			doChannelMsg(0,chn);
			}
		else

       	if(ChannelLinkMap[chn])
       		{
			if(!(chn%2))
				{
				if(Value)
					TxMuteState[chn] = 1;
				else
					TxMuteState[chn] = 0;
				TxMuteState[chn+1]=TxMuteState[chn];
				doChannelMsg(0,chn);
				doChannelMsg(0,chn+1);
				}
			}
	break;
    //------------------------------------------CHANNEL TRIM
	case 2:
       	if(!ChannelLinkMap[chn])
       		{
   			TxFaderValue[chn] = DataToDB(*parameters.getRawParameterValue("V"+String(chn)));
			if(pluginAutoMode[chn]==TRIMWRITE)
				TxTrimValue[chn] = SSLDbFaderValue[chn];
			else
				TxTrimValue[chn] = DataToDB(*parameters.getRawParameterValue("T"+String(chn)));
			
			doChannelMsg(0,chn);
			}
	break;
	//------------------------------------------Automode change
	case 3: 
		TxAutoMode[chn] = Value;
		TxFaderValue[chn] = DataToDB(*parameters.getRawParameterValue("V"+String(chn)));
		TxTrimValue[chn]= DataToDB(*parameters.getRawParameterValue("T"+String(chn)));
		doChannelMsg(0,chn);
	break;
    //-------------------------------------------Read/write enable switches
	case 4:
		globalRWAutoState=globalReadCutAutoState|(globalReadVolAutoState<<1)|(globalWriteCutAutoState<<2)|(globalWriteVolAutoState<<3);
        txCfgMsg(0x24,globalRWAutoState,1);
        //ConfigReg[0x24]=globalRWAutoState;
		if(!(globalRWAutoState&0x01))
			{
			for(int i=0;i<SystemSize;i++)
				{
				TxMuteState[i] = 0;
				parameters.getParameter("CUT"+String(i))->setValueNotifyingHost(0);
				}
			}
	break;
	case 5://New play
			for(int chn=0;chn<SystemSize;chn++)
				{
				TxFaderValue[chn] =  DataToDB(*parameters.getRawParameterValue("V"+String(chn)));
       			TxTrimValue[chn] = DataToDB(*parameters.getRawParameterValue("T"+String(chn)));
				TxMuteState[chn]= (bool)(*parameters.getRawParameterValue("CUT"+String(chn)));
				doChannelMsg(1,chn);
				}
			doChannelMsg(2,chn);
 	break;
   }
}



//==============================================================================
// void TaiChannelPluginProcessor::doUnRegisterMsChannel(void)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::doUnRegisterMsChannel(void)
{
for(int chn=0;chn<SystemSize;chn++)
	pipeMsMapLinkBuffer[chn<<2]=pipeMsMapLinkBuffer[chn<<2]&0xfd;
motherShipMapLinkPipe.writePipe(pipeMsMapLinkBuffer);
}

//==============================================================================
// void TaiChannelPluginProcessor::updateChnPipeState(int n,int state)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::updateChnPipeState(int n,int state)
{
/*
if(!state)
	chnPipe[n]->openChnPipe(n);//open local writes from MS to channel pipe it is free to write
else
	chnPipe[n]->closeChnPipe();//close local writes from MS to channel pipe since it is assigned to a SingleChannel
*/
}

//==============================================================================
// void TaiChannelPluginProcessor::doRegisterMsChannel(void)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::doRegisterMsChannel(void)
{
for(int chn=0;chn<SystemSize;chn++)
	pipeMsMapLinkBuffer[chn<<2]=0x02|isMappedToSinglechannel[chn];
motherShipMapLinkPipe.writePipe(pipeMsMapLinkBuffer);
}

//==============================================================================
// void TaiChannelPluginProcessor::doChannelMsg(int channel)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::doChannelMsg(int type,int channel)
{
int pipeChnIdx =channel<<2;
if(type!=2)
	{
	pipeMsDataBuffer[pipeChnIdx]=TxFaderValue[channel]&0x00ff;			//who? 
	pipeMsDataBuffer[pipeChnIdx+1]=(TxFaderValue[channel]>>8)|(TxMuteState[channel]&0x01)<<2|(TxAutoMode[channel]&0x07)<<3;
	pipeMsDataBuffer[pipeChnIdx+2]=TxTrimValue[channel]&0x00ff;
	pipeMsDataBuffer[pipeChnIdx+3]=TxTrimValue[channel]>>8;;
	if(type==0)
		{
		if(channel<64)
			pipeMsDataBuffer[510]=pipeMsDataBuffer[510]|bitIndex[channel/8];
		else
			pipeMsDataBuffer[511]=pipeMsDataBuffer[511]|bitIndex[(channel-32)/8];
		newChannelMsgFlag=true;
		}
	else
	if(type==1)
		{
		if(channel<64)
			pipeMsDataBuffer[510]=pipeMsDataBuffer[510]|bitIndex[channel/8];
		else
			pipeMsDataBuffer[511]=pipeMsDataBuffer[511]|bitIndex[(channel-32)/8];
		}
	}
else
	newChannelMsgFlag=true;
}

//==============================================================================
// void TaiChannelPluginProcessor::txChannelMsg(int channel)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::txChannelMsg(void)
{
PTIntValue1=(pipeMsDataBuffer[0]|(pipeMsDataBuffer[1]<<8))&0x03ff;
if(pipeMsDataBuffer[510]||pipeMsDataBuffer[511])
	{
	motherShipDataPipe.writePipe(pipeMsDataBuffer);
	pipeMsDataBuffer[510]=0;
	pipeMsDataBuffer[511]=0;
	newChannelMsgFlag=false;
	PTIntValue2=TxFaderValue[0];
	//printf("data sent\n");
	}
}

//==============================================================================
// void TaiChannelPluginProcessor::txCfgMsg(int channel,int state)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::txCfgMsg(int index,int value,int state)
{
if(!index)
	{
	//printf("heartbeat message from config 0 sent\n");
	pipeMsCfgBuffer[0]=index;
	pipeMsCfgBuffer[1]=0;
	}
else
	{
	pipeMsCfgBuffer[0]=index;
	if(index<64)
		pipeMsCfgBuffer[index]=value;
	else
		pipeMsCfgBuffer[64]=value;
	}
pipeMsCfgBuffer[15]=state;
//#ifndef __IPC	
//taiPipe.openMsCfgPipe();
//taiPipe.writeMsCfgPipe(pipeCfgDataBuffer);
//taiPipe.closeMsCfgPipe();
//#else
//motherShipCfgPipe.openMsCfgPipe();
motherShipCfgPipe.writePipe(pipeMsCfgBuffer);
//motherShipCfgIPC.closeCfgPipeIpc();
//#endif
pipeMsCfgBuffer[0]=0;
ConfigReg[0]=0;
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::testMotherShipMapped(void)
//
//
//
//==============================================================================
int TaiMotherShipPluguinProcessor::testMotherShipMapped(void)
{
for(int n=0;n<SystemSize;n++)
	{
	if(oscMsgMemBlck[n+192]&0x02)
		return 1;
	}
return 0;
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::parseOscAssignData(void)
//
//
//
//==============================================================================
int TaiMotherShipPluguinProcessor::parseOscAssignData(void)
{
int hiChn=0;
if(justStartedFlag)
	{
	for(int n=0;n<SystemSize;n++)
		isMappedToSinglechannel[n]=oscMsgMemBlck[n+192];
	}
else
	{
	for(int n=0;n<SystemSize;n++)
		{
		int mapcode=oscMsgMemBlck[n+192];
		if(isMappedToSinglechannel[n]!=(mapcode&0x01))
			{
			isMappedToSinglechannel[n]=(mapcode&0x01);
			trkLabelRefreshFlag=1;
			updateChnPipeState(n,isMappedToSinglechannel[n]);
			}
		if(isMappedToSinglechannel[n]&0x01)//ChannelAssgMap
			if(hiChn<n)
				hiChn=n;
		}
	if(hiChn)
		return hiChn+1;
	return hiChn;
	}
return hiChn;
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::parseOscCfgData(MemoryBlock oscMemBlck)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::parseOscCfgData(void)
{
for(int n=1;n<64;n++)
	{
	if(ConfigReg[n]!=oscMsgMemBlck[n+384])
		configRegMsgParser(n,oscMsgMemBlck[n+384]);
	}
	
if(oscMsgMemBlck[384])
	configRegMsgParser(oscMsgMemBlck[384],oscMsgMemBlck[384+oscMsgMemBlck[384]]);
	
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::parseOscAuxData(const OSCMessage& message)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::parseOscAuxData(void)
{
for(int chan=0;chan<SystemSize;chan++)
	{
	if(ChannelLinkMap[chan]!=(oscMsgMemBlck[chan+288]&0x01))// test link bit0
		ChannelLinkMap[chan]=oscMsgMemBlck[chan+288]&0x01;
	if(ChannelWriteCut[chan]!=((oscMsgMemBlck[chan+288]&0x02)>>1))// test WRITE CUT bit1
		{
		ChannelWriteCut[chan]=(oscMsgMemBlck[chan+288]&0x02)>>1;
		manageCutWrites(chan);
		}
	}
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::manageCutWrites(unsigned char chan)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::manageCutWrites(unsigned char chan)
{
if(ChannelWriteCut[chan])			//WRITE CUTS IS ENABLED
	{
	if(globalReadCutAutoState==true)//GLOBAL CUT ENABLE ON
		{
		parameters.removeParameterListener("CUT"+String(chan), this);	//STOP LISTENING TO HOST
		startCutGestureOnChannel(chan);
		}
	else
		{							//GLOBAL CUT ENABLE OFF
		parameters.addParameterListener("CUT"+String(chan), this);
		endCutGestureOnChannel(chan);
		}
	}
else
	{								//WRITE CUTS IS DISABLED
	switch(pluginAutoMode[chan])
		{
		case OFF://PLUG-IN AUTOMODE OFF
			parameters.removeParameterListener("CUT"+String(chan), this);
			endCutGestureOnChannel(chan);
		break;
		case WRITE:
			parameters.addParameterListener("CUT"+String(chan), this);		//Get parameters changes from host
			endCutGestureOnChannel(chan);	
		break;
		}
	}
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::configRegMsgParser(int index,int cfgData)
//
//
//
//==============================================================================
void TaiMotherShipPluguinProcessor::configRegMsgParser(int index,int cfgData)
{
//printf("New Congig register: #%d  Value %x \n",index,(char)ConfigReg[index]);
switch (index)
	{
	case 0:		break;
	case 14:
		ConfigReg[14]=cfgData;
		statusSwtPreset=ConfigReg[14]&0x0f;
		newStatusSwtPresetFlag=true;
		//refreshStatusSwtPresetFlag=true;
		#ifdef _PRINTMSG
		printf("New register #14 Switch Preset : %d\n",statusSwtPreset);
		#endif
	break;
	case 6://PVCA count and bank location
			ConfigReg[index]=cfgData;
	break;
	case 10://mix state and interface type
		ConfigReg[index]=cfgData;
    	switch(ConfigReg[10]&0xf0)
        	{
        	case 0:taiType=0;break;		//SSL VCA
			case 0x80:taiType=1;break;	//SSL ULTIMATION
			case 0xc0:taiType=2;break;	//FlyingFader
			}
		#ifdef _PRINTMSG
		printf("mix on/off register\n");
		#endif
		mixOnOffState=ConfigReg[10]&0x01;
	break;
	case 21://Master Vader Location
			ConfigReg[index]=cfgData;
	break;
	case 31:
		//printf("INJEKTOR heartbeat state: %x\n",cfgData);
		txCfgMsg(0,0,1); //MotherShip hearthbeat message
		break;

	case 032://mix on off switch
		#ifdef _PRINTMSG
		printf("mix on/off switch state\n");
		#endif
		ConfigReg[index]=cfgData;
 //		mixOnOffState=ConfigReg[10]&0x01;
	break;
	case 033://mix on off switch
		#ifdef _PRINTMSG
		printf("Level Match on/off state\n");
		#endif
		ConfigReg[index]=cfgData;
 //		mixOnOffState=ConfigReg[10]&0x01;
	break;

	case 34:
		if(PluginConfigState==2)
			ConfigReg[index]=ConfigReg[14];
		else
			ConfigReg[index]=cfgData;
		statusSwtPreset=ConfigReg[34]&0x0f;
		newStatusSwtPresetFlag=true;
		//refreshStatusSwtPresetFlag=true;
		#ifdef _DEBUG
		//printf("New status Switch Preset : %d\n",statusSwtPreset);
		#endif
	break;
	default:ConfigReg[index]=cfgData;
	break;
	}
//printf("cfg index: %d   cfg Data %d  					Plugin config state :%d \n",index,ConfigReg[index],PluginConfigState);
}

//==============================================================================
bool TaiMotherShipPluguinProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
	// Only mono/stereo and input/output must have same layout
	const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();
	const AudioChannelSet& mainInput  = layouts.getMainInputChannelSet();
	// input and output layout must either be the same or the input must be disabled altogether
	if (! mainInput.isDisabled() && mainInput != mainOutput)
		return false;
	// do not allow disabling the main buses
	if (mainOutput.isDisabled())
		return false;
	// only allow stereo and mono
	if (mainOutput.size() > 2)
		return false;
	return true;
}

//===============================================================================================
AudioProcessor::BusesProperties TaiMotherShipPluguinProcessor::getBusesProperties()
{
	return BusesProperties().withInput  ("Input",  AudioChannelSet::stereo(), true)
							.withOutput ("Output", AudioChannelSet::stereo(), true);
}



//==============================================================================
void TaiMotherShipPluguinProcessor::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

//===============================================================================================
void TaiMotherShipPluguinProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

//===============================================================================================
void TaiMotherShipPluguinProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	const int totalNumInputChannels  = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	    buffer.clear (i, 0, buffer.getNumSamples());
	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	updateCurrentTimeInfoFromHost();
	/*for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
	float* channelData = buffer.getWritePointer (channel);

	// ..do something to the data...
	}
*/
}

//==============================================================================
AudioProcessorEditor* TaiMotherShipPluguinProcessor::createEditor()
{
    return new TaiMotherShipPluguinProcessorEditor (*this, parameters);
}

//===============================================================================================
void TaiMotherShipPluguinProcessor::updateTrackProperties (const TrackProperties& properties)
{
    trackProperties = properties;
	if (auto* editor = dynamic_cast<TaiMotherShipPluguinProcessorEditor*> (getActiveEditor()))
        editor->updateTrackProperties ();
    TrackName<<properties.name;
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TaiMotherShipPluguinProcessor();
}

//==============================================================================
void TaiMotherShipPluguinProcessor::updateCurrentTimeInfoFromHost()
{
if (AudioPlayHead* ph = getPlayHead())
	{
	AudioPlayHead::CurrentPositionInfo newTime;
	if (ph->getCurrentPosition (newTime))
		{
		lastPosInfo = newTime;  // Successfully got the current time from the host..
		return;
        }
    }
// If the host fails to provide the current time, we'll just reset our copy to a default..
lastPosInfo.resetToDefault();
}

//------------------------------------------------------------------------------------
//	FONCTIONS:	float TaiChannelPluginProcessor::InitUdpConnection(void)
//	Notes:	Reçois le data du AD et fait la mise a la mise a l'échelle pour le DAW
//		
//
//
//------------------------------------------------------------------------------------
int TaiMotherShipPluguinProcessor::InitUdpConnection(int port)
{
//oscReceiver= new OSCReceiver();
//rxSocket = new DatagramSocket(false);
//Make this socket re-usable
rxSocket->setEnablePortReuse(true);
// specify here on which UDP port number to receive incoming OSC messages
if (! rxSocket->bindToPort(port))//,SSMMULTICAST);
	{
	DBG("Could not bind to port: %d."+String(port));
	return -1;
	}
//
if (!rxSocket->joinMulticast(SSM_MULTICAST))
	{
	DBG("Could not join multicast.");
	return -2;
	}

if (! oscReceiver->connectToSocket (*rxSocket))                   // [3]
	{
	DBG("Error: could not connect to UDP port:"+String(port));
	return -3;
	}
return 1;
}

//------------------------------------------------------------------------------------
//	FONCTIONS:void TaiChannelPluginProcessor::startListeningToUDP(void)
//
//
//------------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessor::startListeningToUDP(void)
{
oscReceiver->addListener(this, "/InjektorGlobalMsg");
}

//------------------------------------------------------------------------------------
//	FONCTIONS:void TaiChannelPluginProcessor::startListeningToUDP(void)
//
//
//------------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessor::stopListeningToUDP(void)
{
oscReceiver->removeListener(this);
}

//------------------------------------------------------------------------------------
//	FONCTIONS:float TaiChannelPluginProcessor::CloseUdpConnection(unsigned int data)
//	Notes:	Reçois le data du AD et fait la mise a la mise a l'échelle pour le DAW
//		
//
//
//------------------------------------------------------------------------------------
int TaiMotherShipPluguinProcessor::CloseUdpConnection(unsigned int state)
{
rxSocket->leaveMulticast(SSM_MULTICAST);
rxSocket->shutdown();
oscReceiver->disconnect();
return 0;
}

//------------------------------------------------------------------------------------
//	FONCTIONS:	int TaiMotherShipPluguinProcessor::ParseConfigReg(void);
//	Notes:	Reçois le data du AD et fait la mise a la mise a l'échelle pour le DAW
//		
//
//
//------------------------------------------------------------------------------------
int TaiMotherShipPluguinProcessor::ParseConfigReg(void)
{
#ifdef _PRINTMSG
if(ConfigReg[10]&0x80)
	printf("SYSTEM TYPE :ULTIMATION \n");
else
	printf("SYSTEM TYPE :VCA \n");
#endif
mixOnOffState=ConfigReg[10]&0x01;
statusSwtPreset=ConfigReg[14]&0x0f;
newStatusSwtPresetFlag=true;
return 1;
}

//------------------------------------------------------------------------------------
//	FONCTIONS:	unsigned int VCA_Scale_It(unsigned int data)
//	Notes:	Reçois le data du AD et fait la mise a la mise a l'échelle pour le DAW
//		
//
//
//------------------------------------------------------------------------------------
float TaiMotherShipPluguinProcessor::DBToData(unsigned int data)
{
    unsigned int TempData;
    
    if (data<=(SSLFaderStep[0]+1))							//fader a 10db
        return (DAWFaderStep[0]);
    if (data<=SSLFaderStep[1])								//fader a 10db a 5db
    {
        TempData=data-SSLFaderStep[0];
        return (float)DAWFaderStep[0]-(TempData*Pente_M[0]);
    }
    if (data<=SSLFaderStep[2])								//fader a 5db a 0db
    {
        TempData=data-SSLFaderStep[1];
        return (float)DAWFaderStep[1]-(TempData*Pente_M[1]);
    }
    if (data<=SSLFaderStep[3])								//fader a 0db a -5db
    {
        TempData=data-SSLFaderStep[2];
        return (float)DAWFaderStep[2]-(TempData*Pente_M[2]);
    }
    if (data<=SSLFaderStep[4])								//fader a -5db a -10db
    {
        TempData=data-SSLFaderStep[3];
        return (float)DAWFaderStep[3]-(TempData*Pente_M[3]);
    }
    if (data<=SSLFaderStep[5])//5								//fader a -10db a -15db
    {
        TempData=data-SSLFaderStep[4];
        return (float)DAWFaderStep[4]-(TempData*Pente_M[4]);
    }
    if (data<=SSLFaderStep[6])//6								//fader a -15db a -20db
    {
        TempData=data-SSLFaderStep[5];
        return (float)DAWFaderStep[5]-(TempData*Pente_M[5]);
    }
    if (data<=SSLFaderStep[7])//7								//fader a -20db a -25db
    {
        TempData=data-SSLFaderStep[6];
        return (float)DAWFaderStep[6]-(TempData*Pente_M[6]);
    }
    if (data<=SSLFaderStep[8])//8								//fader a -25db a -30dbF
    {
        TempData=data-SSLFaderStep[7];
        return (float)DAWFaderStep[7]-(TempData*Pente_M[7]);
    }
    if (data<=SSLFaderStep[9])//8								//fader a -30db a -50dbF
    {
        TempData=data-SSLFaderStep[8];
        return (float)DAWFaderStep[8]-(TempData*Pente_M[8]);
    }
    if (data<=SSLFaderStep[10])									//fader a -50db a -infdb
    {
        TempData=data-SSLFaderStep[9];
        return (float)DAWFaderStep[9]-(TempData*Pente_M[9]);
    }
    if (data<=SSLFaderStep[11])									//fader a -50db a -infdb
    {
        TempData=data-SSLFaderStep[10];
        return (float)DAWFaderStep[10]-(TempData*Pente_M[10]);
    }
    if (data<=SSLFaderStep[12])									//fader a -50db a -infdb
    {
        TempData=data-SSLFaderStep[11];
        return (float)DAWFaderStep[11]-(TempData*Pente_M[11]);
    }
    if (data<=SSLFaderStep[13])									//fader a -50db a -infdb
    {
        TempData=data-SSLFaderStep[12];
        return (float)DAWFaderStep[12]-(TempData*Pente_M[12]);
    }
    if (data<=SSLFaderStep[14])									//fader a -INFdb
    {
        TempData=data-SSLFaderStep[13];
        return (float)DAWFaderStep[13]-(TempData*Pente_M[13]);
    }
    return 0;
}

//-----------------------------------------------------------------------------------
// FONCTIONS:	DBToData(unsigned int data)
// Notes:	Recois le data du AD et fait la mise a la mise a l'échelle pour le Ptools
// Codes de sortie
//	1 OK
//
//	-1	timeout
//	-2 buffer avec plus de 1
//-----------------------------------------------------------------------------------
unsigned int TaiMotherShipPluguinProcessor::DataToDB(float data)
{
    if (data>DAWFaderStep[0])											//test si fader PLus de 10db Test (si fader a 12db)
        return (SSLFaderStep[0]);
    if (data>=DAWFaderStep[1])												//test DAW fader 5 a 10db)
        return(SSLFaderStep[0]+((DAWFaderStep[0]-data)/Pente_M[0]));			//REAPER = 	16383 a 14368
    if (data>=DAWFaderStep[2])												//test DAW fader 0 a 5db
        return(SSLFaderStep[1]+((DAWFaderStep[1]-data)/Pente_M[1]));			//REAPER = 14368 a 12541
    if (data>=DAWFaderStep[3])												//test DAW fader  de 0 a -5db
        return(SSLFaderStep[2]+((DAWFaderStep[2]-data)/Pente_M[2]));			//REAPER = 12451 a 10682
    if (data>=DAWFaderStep[4])												//test DAW fader  -5 a -10db
        return(SSLFaderStep[3]+((DAWFaderStep[3]-data)/Pente_M[3]));			//REAPER = 10682 a 9076
    if (data>=DAWFaderStep[5])												//test DAW fader -10 a -20db
        return(SSLFaderStep[4]+((DAWFaderStep[4]-data)/Pente_M[4]));			//REAPER = 9076 a 6340
    if (data>=DAWFaderStep[6])												//test DAW fader  de -20 a -30db
        return(SSLFaderStep[5]+((DAWFaderStep[5]-data)/Pente_M[5]));			//REAPER = 6340 a 4407
    if (data>=DAWFaderStep[7])												//test DAW fader  de -30 a -40db
        return(SSLFaderStep[6]+((DAWFaderStep[6]-data)/Pente_M[6]));			//REAPER = 4407 a 3064
    if (data>=DAWFaderStep[8])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[7]+((DAWFaderStep[7]-data)/Pente_M[7]));			//REAPER = 3064 a 2097
    if (data>=DAWFaderStep[9])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[8]+((DAWFaderStep[8]-data)/Pente_M[8]));			//REAPER = 2097 a 0
    if (data>=DAWFaderStep[10])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[9]+((DAWFaderStep[9]-data)/Pente_M[9]));			//REAPER = 2097 a
    if (data>=DAWFaderStep[11])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[10]+((DAWFaderStep[10]-data)/Pente_M[10]));			//REAPER = 2097 a 0
    if (data>=DAWFaderStep[12])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[11]+((DAWFaderStep[11]-data)/Pente_M[11]));			//REAPER = 2097 a 0
    if (data>=DAWFaderStep[13])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[12]+((DAWFaderStep[12]-data)/Pente_M[12]));			//REAPER = 2097 a 0
    if (data>=DAWFaderStep[14])												//test DAW fader  de -40 a -50db
        return(SSLFaderStep[13]+((DAWFaderStep[13]-data)/Pente_M[13]));			//REAPER = 2097 a 0
    
    
    return 915;
}

//-----------------------------------------------------------------------------------
// FONCTIONS:	unsigned int DoPente(void)
// Notes:	Recois le data du AD et fait la mise a la mise a l'échelle pour le Ptools
// Codes de sortie
//	1 OK
//
//	-1	timeout
//	-2 buffer avec plus de 1
//-----------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessor::DoPente(void)
{
#ifdef _LOGICSCALE
DAWFaderStep[0]=1;     	//10db
DAWFaderStep[1]=0.945;	//5db
DAWFaderStep[2]=0.709;	//0db
DAWFaderStep[3]=0.531;	//-5db
DAWFaderStep[4]=0.396;  //-10db
DAWFaderStep[5]=0.299;  //-15db
DAWFaderStep[6]=0.224;  //-20db
DAWFaderStep[7]=0.168;  //-25db
DAWFaderStep[8]=0.126;  //-30db
DAWFaderStep[9]=0.094;  //-35db
DAWFaderStep[10]=0.071;	//-40db ok
DAWFaderStep[11]=0.040;	//-50db ok
DAWFaderStep[12]=0.022; //-60db ok
DAWFaderStep[13]=0.013; //-70db ok
DAWFaderStep[14]=0.004; //-90db
#else    
    DAWFaderStep[0]=0.96;      //10db
    DAWFaderStep[1]=0.853;      //5db
    DAWFaderStep[2]=0.75;       //0db
    DAWFaderStep[3]=0.655;      //-5db
    DAWFaderStep[4]=0.56;       //-10db
    DAWFaderStep[5]=0.465;      //-15db
    DAWFaderStep[6]=0.37;       //-20db
    DAWFaderStep[7]=0.31;       //-25db
    DAWFaderStep[8]=0.25;       //-30db
    DAWFaderStep[9]=0.19;       //-35db
    DAWFaderStep[10]=0.129;      //-40db ok
    DAWFaderStep[11]=0.099;      //-50db ok
    DAWFaderStep[12]=0.068;      //-60db ok
    DAWFaderStep[13]=0.038;      //-70db ok
    DAWFaderStep[14]=0.0073;      //-90db
#endif
    
    SSLFaderStep[0]=0;			//10db
    SSLFaderStep[1]=50;			//5db
    SSLFaderStep[2]=100;		//0db
    SSLFaderStep[3]=150;		//-5db
    SSLFaderStep[4]=200;		//-10db
    SSLFaderStep[5]=250;		//-15db
    SSLFaderStep[6]=300;		//-20db
    SSLFaderStep[7]=350;		//-25db
    SSLFaderStep[8]=400;		//-30db
    SSLFaderStep[9]=450;		//-35db
    SSLFaderStep[10]=500;		//-40db
    SSLFaderStep[11]=600;		//-50db
    SSLFaderStep[12]=700;		//-60db
    SSLFaderStep[13]=800;		//-70db
    SSLFaderStep[14]=1010;		//-infdb
    
    Pente_M[0]=(double)(DAWFaderStep[0]-DAWFaderStep[1])/(SSLFaderStep[1]-SSLFaderStep[0]);		// 10db
    Pente_M[1]=(double)(DAWFaderStep[1]-DAWFaderStep[2])/(SSLFaderStep[2]-SSLFaderStep[1]);		// 5db
    Pente_M[2]=(double)(DAWFaderStep[2]-DAWFaderStep[3])/(SSLFaderStep[3]-SSLFaderStep[2]);		// 0db
    Pente_M[3]=(double)(DAWFaderStep[3]-DAWFaderStep[4])/(SSLFaderStep[4]-SSLFaderStep[3]);		// -5db
    Pente_M[4]=(double)(DAWFaderStep[4]-DAWFaderStep[5])/(SSLFaderStep[5]-SSLFaderStep[4]);		// -10db
    Pente_M[5]=(double)(DAWFaderStep[5]-DAWFaderStep[6])/(SSLFaderStep[6]-SSLFaderStep[5]);		// -15db
    Pente_M[6]=(double)(DAWFaderStep[6]-DAWFaderStep[7])/(SSLFaderStep[7]-SSLFaderStep[6]);		// -20db
    Pente_M[7]=(double)(DAWFaderStep[7]-DAWFaderStep[8])/(SSLFaderStep[8]-SSLFaderStep[7]);		// -25db
    Pente_M[8]=(double)(DAWFaderStep[8]-DAWFaderStep[9])/(SSLFaderStep[9]-SSLFaderStep[8]);		// -30db
    Pente_M[9]=(double)(DAWFaderStep[9]-DAWFaderStep[10])/(SSLFaderStep[10]-SSLFaderStep[9]);	// -35db
    Pente_M[10]=(double)(DAWFaderStep[10]-DAWFaderStep[11])/(SSLFaderStep[11]-SSLFaderStep[10]);// -40db
    Pente_M[11]=(double)(DAWFaderStep[11]-DAWFaderStep[12])/(SSLFaderStep[12]-SSLFaderStep[11]);// -50db
    Pente_M[12]=(double)(DAWFaderStep[12]-DAWFaderStep[13])/(SSLFaderStep[13]-SSLFaderStep[12]);// -60db
    Pente_M[13]=(double)(DAWFaderStep[13]-DAWFaderStep[14])/(SSLFaderStep[14]-SSLFaderStep[13]);// -70db
}

//==============================================================================
int TaiMotherShipPluguinProcessor::configFileRead(void)
{
File injektorcfg("/tmp/injektor.cfg");
int injektorSize=0;
if(!injektorcfg.existsAsFile())
	{
	#ifdef _PRINTMSG
	printf("No config file");
	#endif
	return 0;
	}
if (injektorcfg.exists())
    {
    XmlDocument xmlDoc(injektorcfg);
    std::unique_ptr<XmlElement> mainElement = xmlDoc.getDocumentElement();
    if (mainElement != nullptr)
        {
        // check the main tag
        if (mainElement->getTagName() == "injektorDataTag")
            {
            if (mainElement->getChildByName("SYSTEMSIZE"))
                {
                XmlElement* sizeElement = mainElement->getChildByName("SYSTEMSIZE");
                injektorSize = sizeElement->getIntAttribute("Size");
                }
            if (mainElement->getChildByName("SYSTEMTYPE"))
                {
                XmlElement* typeElement = mainElement->getChildByName("SYSTEMTYPE");
                taiType = typeElement->getIntAttribute("Type");
                }
            if (mainElement->getChildByName("UDPTXPORT"))
                {
                XmlElement* udpPortElement = mainElement->getChildByName("UDPTXPORT");
                udpRxPort = udpPortElement->getIntAttribute("Port");
                }

			return injektorSize;
            }
	    }
	}
else
	{
	taiType =0;
	return 0;
	
	
	
	}
return injektorSize;
}
        
//==============================================================================
void TaiMotherShipPluguinProcessor::getStateInformation (MemoryBlock& destData)
{

XmlElement xml ("TAIMotherShipPlugin");
    // add some attributes to it..
xml.setAttribute ("GLOBALRWAUTOSTATE",globalRWAutoState);
xml.setAttribute ("BANK",bank);
xml.setAttribute ("VIEWSIZE",viewSize);
xml.setAttribute("READVOL",globalReadVolAutoState);
xml.setAttribute("READCUT",globalReadCutAutoState);
xml.setAttribute("WRITEVOL",globalWriteVolAutoState);
xml.setAttribute("WRITECUT",globalWriteCutAutoState);
xml.setAttribute("READTRIM",globalReadTrimAutoState);
copyXmlToBinary (xml, destData);

}

//===============================================================================================
void TaiMotherShipPluguinProcessor::setStateInformation (const void* data, int sizeInBytes)
{

std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
if (xmlState != nullptr)
	{
        // make sure that it's actually our type of XML object..
	if (xmlState->hasTagName ("TAIMotherShipPlugin"))
		{
		// ok, now pull out our last window size..
		globalRWAutoState= jmax (xmlState->getIntAttribute ("GLOBALRWAUTOSTATE",globalRWAutoState), 0);
		bank= jmax (xmlState->getIntAttribute ("BANK",bank), 0);
		viewSize= jmax (xmlState->getIntAttribute ("VIEWSIZE",viewSize), 0);
		globalReadVolAutoState=jmax (xmlState->getIntAttribute ("READVOL",1), 0);
		globalReadCutAutoState=jmax (xmlState->getIntAttribute ("READCUT",1), 0);
		globalWriteVolAutoState=jmax (xmlState->getIntAttribute ("WRITEVOL",1), 0);
		globalWriteCutAutoState=jmax (xmlState->getIntAttribute ("WRITECUT",1), 0);
		globalReadTrimAutoState=jmax (xmlState->getIntAttribute ("READTRIM",1), 0);

		//SystemSize = jmax (xmlState->getIntAttribute ("SYSTEMESIZE", SystemSize), 0);
		//taiType = jmax (xmlState->getIntAttribute ("TAITYPE", taiType), 0);
		/*
		for (auto* param : getParameters())
            if (auto* p = dynamic_cast<AudioProcessorParameterWithID*> (param))
                p->setValue ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
		*/
		}
	}
}

//------------------------------------------------------------------------------------------
//
//
//
//
//------------------------------------------------------------------------------------------
void TaiMotherShipPluguinProcessor::faderMoveTest(void)
{
    AudioProcessorParameterWithID* ParaIdtestvol;
    AudioProcessorParameterWithID* ParaIdtesttrim;
    AudioProcessorParameterWithID* ParaIdtestcut;
    static float scanvalue[96];
    static float scancut[96];

//    int bank=2;
    for (int ch=0;ch<SystemSize;ch++)
        {
        if(pluginAutoMode[ch]==READ)
            {
	        ParaIdtestcut=parameters.getParameter("CUT"+String(ch));
	        ParaIdtestvol=parameters.getParameter("V"+String(ch));
	        scanvalue[ch]=*parameters.getRawParameterValue("V"+String(ch));
	        scanvalue[ch]=scanvalue[ch]+0.01f;
	        if(scanvalue[ch]>=0.96)
                {
	            scanvalue[ch]=0;
	            if(scancut[ch])
	                scancut[ch]=0;
	            else
	                scancut[ch]=1;
                }
	        ParaIdtestcut->setValueNotifyingHost(scancut[ch]);
	        ParaIdtestvol->setValueNotifyingHost(scanvalue[ch]);
	    	}
	    if(pluginAutoMode[ch]==TRIMREAD)
            {
            ParaIdtestvol=parameters.getParameter("V"+String(ch));
	        ParaIdtesttrim=parameters.getParameter("T"+String(ch));
	        scanvalue[ch]=*parameters.getRawParameterValue("V"+String(ch));
	        scanvalue[ch]=scanvalue[ch]+0.01f;
	        if(scanvalue[ch]>=0.96)
	            {
                scanvalue[ch]=0;
            	if(ParaIdtesttrim->getValue()>=0.5)
            		ParaIdtesttrim->setValueNotifyingHost(0.25f);
                else
            		ParaIdtesttrim->setValueNotifyingHost(0.76f);
            	}
            ParaIdtestvol->setValueNotifyingHost(scanvalue[ch]);
	        }
	    
	    if(pluginAutoMode[ch]==WRITE)
            {
	        ParaIdtestcut=parameters.getParameter("CUT"+String(ch));
	        scancut[ch]=*parameters.getRawParameterValue("CUT"+String(ch));
	        
	        ParaIdtestvol=parameters.getParameter("V"+String(ch));
	        scanvalue[ch]=*parameters.getRawParameterValue("V"+String(ch));
	        if(scanvalue[ch]>0)
	        	{
	            scanvalue[ch]=scanvalue[ch]-0.01f;
	            if(scanvalue[ch]<0)
	                scanvalue[ch]=0;
	        	}
	        else
	        	{
	            scanvalue[ch]=0.96f;
	            if(scancut[ch])
	                scancut[ch]=0;
	            else
	                scancut[ch]=1;
	        	}
	        ParaIdtestcut->setValueNotifyingHost(scancut[ch]);
	        ParaIdtestvol->setValueNotifyingHost(scanvalue[ch]);
	    	}
        if(pluginAutoMode[ch]==TRIMWRITE)
            {
	        ParaIdtestvol=parameters.getParameter("V"+String(ch));
	        scanvalue[ch]=*parameters.getRawParameterValue("V"+String(ch));
	        scanvalue[ch]=scanvalue[ch]+0.01f;
	        if(scanvalue[ch]>=0.96)
                scanvalue[ch]=0;
	        ParaIdtestvol->setValueNotifyingHost(scanvalue[ch]);
	    	}
    	}

}


#include "fromConsoleDataMgrProcessor.cpp"
#include "gestureManagersProcessor.cpp"
