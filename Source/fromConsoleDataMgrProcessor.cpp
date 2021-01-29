/*
  ==============================================================================

    Processor_fonctions.cpp
    Created: 12 Jul 2020 7:51:15pm
    Author:  Daniel

  ==============================================================================
*/

//==============================================================================
//
//	void manageSSLFaderDataAutoOff
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSSLFaderDataAutoOff(int n, int localSSLfaderval , bool changedFlag, int touchState)
{
if((SSLDbFaderValue[n]!=localSSLfaderval)
     ||changedFlag)
     {
     if(!ChannelLinkMap[n])																		//loop back value
     	{
     	SSLDbFaderValue[n]=localSSLfaderval;														//SSL fader db value as changed, save it
     	SSLScaledFaderValue[n]=DBToData(SSLDbFaderValue[n]);
     	//parameters.getParameter("V"+String(n))->setValueNotifyingHost(SSLScaledFaderValue[n]);
     	parameters.getParameter("V"+String(n))->setValue(SSLScaledFaderValue[n]);
     	if(touchState)
     		parameterChangeManager(0,n,SSLScaledFaderValue[n]);
     	}
     else	//if(ChannelLinkMap[n])
     	{	
     	if(!(n%2))
     		{
     		#ifdef _PRINTMSG
     		printf("Auto OFF: linked ssl fader #%d message value %d\n",n,localSSLfaderval);
     		#endif
     		SSLDbFaderValue[n]=localSSLfaderval;							//SSL fader db value as changed, save it
     		SSLScaledFaderValue[n]=DBToData(SSLDbFaderValue[n]);
     		parameters.getParameter("V"+String(n))->setValue(SSLScaledFaderValue[n]);
     		
     		SSLDbFaderValue[n+1]=localSSLfaderval;							//SSL fader db value as changed, save it
     		SSLScaledFaderValue[n+1]=DBToData(SSLDbFaderValue[n]);
     		parameters.getParameter("V"+String(n+1))->setValue(SSLScaledFaderValue[n]);
     		parameterChangeManager(0,n,SSLScaledFaderValue[n]);
     		parameterChangeManager(0,n+1,SSLScaledFaderValue[n]);
     		}
     	}
     }
}

//==============================================================================
//
//	manageSslCutDataAutoOff
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSslCutDataAutoOff(int n,int localSSLCutState,bool cutChangedFlag)
{
if(cutChangedFlag
     &&globalWriteCutAutoState
	&&ChannelWriteCut[n])
	{
	if(!ChannelLinkMap[n])	
		parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
	else
	if(!(n%2))
		{
		parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
		parameters.getParameter("CUT"+String(n+1))->setValueNotifyingHost((float)localSSLCutState);		//WRITE CUT IS enabled
		}					
	}
else	
if(!ChannelLinkMap[n])													//loop back CUT State
	parameterChangeManager(1,n,localSSLCutState);
else
if(!(n%2))
	{
	SSLCutState[n+1]=localSSLCutState;
	parameterChangeManager(1,n,SSLCutState[n]);
	parameterChangeManager(1,n+1,SSLCutState[n]);
	}
}

//==============================================================================
//
//	vmanageSslFaderDataAutoWRITE
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSslFaderDataAutoWRITE(int n,int localSSLfaderval,bool changedFlag)
{
if((SSLDbFaderValue[n]!=localSSLfaderval)
     ||changedFlag)
     {
     SSLDbFaderValue[n]=localSSLfaderval;
     SSLScaledFaderValue[n]=DBToData(SSLDbFaderValue[n]);
     if((globalWriteVolAutoState)
     	&&(!faderIsDragged[n]))
     	{
     	if(!ChannelLinkMap[n])	
     		parameters.getParameter("V"+String(n))->setValueNotifyingHost(SSLScaledFaderValue[n]);			//WRITE VOLUME IS enabled
     	else
     	if(!(n%2))
     		{
     		SSLDbFaderValue[n+1]=localSSLfaderval;
     		#ifdef _PRINTMSG
     		printf("Auto WRITE: linked ssl fader #%d message value %d\n",n,localSSLfaderval);
     		#endif
     		SSLScaledFaderValue[n+1]=DBToData(SSLDbFaderValue[n]);
     		parameters.getParameter("V"+String(n))->setValueNotifyingHost(SSLScaledFaderValue[n]);			//WRITE VOLUME IS enabled
     		parameters.getParameter("V"+String(n+1))->setValueNotifyingHost(SSLScaledFaderValue[n]);			//WRITE VOLUME IS enabled
     		}
     	}
     }
}

//==============================================================================
//
//	manageSslCutDataAutoWRITE
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSslCutDataAutoWRITE(int n, int localSSLCutState, bool changedFlag, bool cutChangedFlag)
{
switch(taiType)
     {
     case 0:
     case 1:
     	if((cutChangedFlag||changedFlag)&&(globalWriteCutAutoState))
     		{
     		if(!ChannelLinkMap[n])
     			{	
				if(!dawWasInTouch[n])
     				parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
				else
     				parameters.getParameter("CUT"+String(n))->setValue((float)localSSLCutState);						//no WRITE CUT
				
				}
     		else
     		if(!(n%2))
     			{
     			parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
     			parameters.getParameter("CUT"+String(n+1))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
     			}					
     		}
     break;
     case 2:
     	if((cutChangedFlag||changedFlag)
     	    &&globalWriteCutAutoState
     		&&ChannelWriteCut[n])
     		{
     		if(!ChannelLinkMap[n])	
     			parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
     		else
     		if(!(n%2))
     			{
     			parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
     			parameters.getParameter("CUT"+String(n+1))->setValueNotifyingHost((float)localSSLCutState);			//WRITE CUT IS enabled
     			}					
     		}
     break;
     default:break;
     }
}

//==============================================================================
//
//	manageSslFaderDataAutoREAD
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSslFaderDataAutoREAD(int n,int localSSLfaderval,bool changedFlag,bool cutChangedFlag,int touchState)
{
if((!(globalReadVolAutoState))	 		//if no volume read, SSL fader is active
	&&(changedFlag||cutChangedFlag||touchState))						//on ultimation fader motor should be off if not grouped
    {
    if(SSLDbFaderValue[n]!=localSSLfaderval)
		{
        if(!ChannelLinkMap[n])
        	{
          	SSLDbFaderValue[n]=localSSLfaderval;
          	SSLScaledFaderValue[n]=DBToData(SSLDbFaderValue[n]);
          	if(READ)
          		parameterChangeManager(0,n,*parameters.getRawParameterValue("V"+String(n)));
          	else
          		parameterChangeManager(2,n,*parameters.getRawParameterValue("T"+String(n)));
          	}
		else
        	{
         	if(READ)
         		{
         		if(!(n%2))
         			{
         			SSLDbFaderValue[n]=localSSLfaderval;
     				SSLDbFaderValue[n+1]=localSSLfaderval;														//SSL fader db value as changed, save it
 
         			SSLScaledFaderValue[n]=DBToData(SSLDbFaderValue[n]);
     				SSLScaledFaderValue[n+1]=DBToData(SSLDbFaderValue[n]);
 
     				parameterChangeManager(0,n,*parameters.getRawParameterValue("V"+String(n)));
     				parameterChangeManager(0,n+1,*parameters.getRawParameterValue("V"+String(n)));
     				}
     			}
     		else
     			{
				if(!(n%2))
     				{
     				SSLDbFaderValue[n]=localSSLfaderval;
     				SSLDbFaderValue[n+1]=localSSLfaderval;

     				SSLScaledFaderValue[n]=DBToData(SSLDbFaderValue[n]);
     				SSLScaledFaderValue[n+1]=DBToData(SSLDbFaderValue[n]);

     				parameterChangeManager(2,n,*parameters.getRawParameterValue("T"+String(n)));
     				parameterChangeManager(2,n+1,*parameters.getRawParameterValue("T"+String(n)));
     				}
     			}
     		}
    	}
	}
}


//==============================================================================
//
//	manageSslFaderDataAutoTRIMWRITE
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSslFaderDataAutoTRIMWRITE(int n,int localSSLfaderval, bool changedFlag, bool cutChangedFlag,int touchState)
{
    int trimFaderFinalValue;
    if(globalWriteVolAutoState)
	{
	if((SSLDbFaderValue[n]!=localSSLfaderval)
		||changedFlag)
		{
		SSLDbFaderValue[n]=localSSLfaderval;
		trimDeltaDbValue[n]=faderDbRef[n]-localSSLfaderval;
		trimFaderFinalValue=trimStartDbValue[n]-trimDeltaDbValue[n];
		#ifdef _PRINTMSG 
		//printf("TRIM START VALUE:%d     TRIM DELTA: %d    FINAL VALUE %d\n",trimStartDbValue[n],trimDeltaDbValue[n],trimFaderFinalValue);
		#endif	
		if(trimFaderFinalValue<0)		
			trimFaderFinalValue=0;	

		if(!faderIsDragged[n])
			{
			if(!ChannelLinkMap[n])	
				parameters.getParameter("T"+String(n))->setValueNotifyingHost(DBToData(trimFaderFinalValue));//devrait générer un parameter change
			else
			if(!(n%2))
				{	
				#ifdef _PRINTMSG 
				printf("Auto TRIM WRITE: linked ssl trim fader #%d message value %d\n",n,trimFaderFinalValue);
				#endif
				parameters.getParameter("T"+String(n))->setValueNotifyingHost(DBToData(trimFaderFinalValue));//devrait générer un parameter change
				parameters.getParameter("T"+String(n+1))->setValueNotifyingHost(DBToData(trimFaderFinalValue));//devrait générer un parameter change
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
void TaiMotherShipPluguinProcessor::manageSslCutDataAutoTRIMWRITE(int n,int localSSLCutState,bool changedFlag,bool cutChangedFlag)
{					
if((cutChangedFlag||changedFlag)&&(globalWriteCutAutoState))
	{
	if(!ChannelLinkMap[n])	
		parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);
	else
	if(!(n%2))
		{
		parameters.getParameter("CUT"+String(n))->setValueNotifyingHost((float)localSSLCutState);
		parameters.getParameter("CUT"+String(n+1))->setValueNotifyingHost((float)localSSLCutState);
		}
	}				
}
//==============================================================================
//
//	void parameterChanged(const String & parameterID, float	newValue)
//	this is what we receive from the DAW
//==============================================================================
void TaiMotherShipPluguinProcessor::manageSslCutDataAutoREAD(int n,int localSSLCutState,bool cutChangedFlag)

{
if(cutChangedFlag)
	{
	parameters.getParameter("CUT"+String(n))->setValue((float)localSSLCutState);			//WRITE CUT IS enabled
	parameterChangeManager(1,n,(float)localSSLCutState);
	}
}
