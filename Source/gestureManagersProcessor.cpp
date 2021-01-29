/*
  ==============================================================================

    gestureManagersFunctions.cpp
    Created: 12 Jul 2020 7:51:15pm
    Author:  Daniel

  ==============================================================================
*/

void TaiMotherShipPluguinProcessor::endVolumeGestureOnChannel(int channel)
{
if(VolGestureStateArray[channel]==true)
	{
	parameters.getParameter("V"+String(channel))->endChangeGesture();
	VolGestureStateArray[channel]=false;
	}
}
//==============================================================================
void TaiMotherShipPluguinProcessor::endCutGestureOnChannel(int channel)
{
if(CutGestureStateArray[channel]==true)
	{
	parameters.getParameter("CUT"+String(channel))->endChangeGesture();
	CutGestureStateArray[channel]=false;
	}
}
//==============================================================================
void TaiMotherShipPluguinProcessor::endTrimGestureOnChannel(int channel)
{
if(TrimGestureStateArray[channel]==true)
	{
	parameters.getParameter("T"+String(channel))->endChangeGesture();
	TrimGestureStateArray[channel]=false;
	}
}
//==============================================================================
void TaiMotherShipPluguinProcessor::startVolumeGestureOnChannel(int channel)
{
if(VolGestureStateArray[channel]==false)
	{
	parameters.getParameter("V"+String(channel))->beginChangeGesture();
	VolGestureStateArray[channel]=true;
	}
}
//==============================================================================
void TaiMotherShipPluguinProcessor::startCutGestureOnChannel(int channel)
{
if(CutGestureStateArray[channel]==false)
	{
	parameters.getParameter("CUT"+String(channel))->beginChangeGesture();
	CutGestureStateArray[channel]=true;
	}
}
//==============================================================================
void TaiMotherShipPluguinProcessor::startTrimGestureOnChannel(int channel)
{
if(TrimGestureStateArray[channel]==false)
	{
	parameters.getParameter("T"+String(channel))->beginChangeGesture();
	TrimGestureStateArray[channel]=true;
	}
}

//==============================================================================
// void TaiMotherShipPluguinProcessor::gestureManager(int autovalue);
// Note: Get here if automation mode as changed
//
//==============================================================================
void TaiMotherShipPluguinProcessor::gestureManager(int channel, int SSLAutoState)
{
switch (SSLAutoState)
	{
	case 0://OFF
		if(!ChannelLinkMap[channel])
			{
			parameters.removeParameterListener("V"+String(channel), this);
			parameters.removeParameterListener("CUT"+String(channel), this);
			parameters.removeParameterListener("T"+String(channel), this);
			endVolumeGestureOnChannel(channel);
			endCutGestureOnChannel(channel);
			endTrimGestureOnChannel(channel);
			}
		else
		if(!(channel%2))
			{
			parameters.removeParameterListener("V"+String(channel), this);
			parameters.removeParameterListener("CUT"+String(channel), this);
			parameters.removeParameterListener("T"+String(channel), this);
			parameters.removeParameterListener("V"+String(channel+1), this);
			parameters.removeParameterListener("CUT"+String(channel+1), this);
			parameters.removeParameterListener("T"+String(channel+1), this);
			endVolumeGestureOnChannel(channel);
			endVolumeGestureOnChannel(channel+1);
			endCutGestureOnChannel(channel);
			endCutGestureOnChannel(channel+1);
			endTrimGestureOnChannel(channel);
			endTrimGestureOnChannel(channel+1);
			}
			
	break;
	case 2://TOUCH
	case 3://READ
	case 4://LATCH
		if(!ChannelLinkMap[channel])
			{
			parameters.addParameterListener("V"+String(channel), this);
			parameters.addParameterListener("CUT"+String(channel), this);
			parameters.removeParameterListener("T"+String(channel), this);
			endVolumeGestureOnChannel(channel);
			endCutGestureOnChannel(channel);
			endTrimGestureOnChannel(channel);
			}
		else
		if(!(channel%2))
			{
			parameters.addParameterListener("V"+String(channel), this);
			parameters.addParameterListener("CUT"+String(channel), this);
			parameters.addParameterListener("V"+String(channel+1), this);
			parameters.addParameterListener("CUT"+String(channel+1), this);
			parameters.removeParameterListener("T"+String(channel), this);
			parameters.removeParameterListener("T"+String(channel+1), this);
			endVolumeGestureOnChannel(channel);
			endVolumeGestureOnChannel(channel+1);
			endCutGestureOnChannel(channel);
			endCutGestureOnChannel(channel+1);
			endTrimGestureOnChannel(channel);
			endTrimGestureOnChannel(channel+1);
			}
	break;
	case 1://WRITE
			//fader 
			if(!ChannelLinkMap[channel])
				{
				//volumes in WRITE MODE
				if(globalWriteVolAutoState==true)
					startVolumeGestureOnChannel(channel);
				else
					{//In READ since WRITE is disabled
					parameters.addParameterListener("V"+String(channel), this);
					endVolumeGestureOnChannel(channel);
					}	
				//CUTS in WRITE MODE (no link channels)
				switch(taiType)
					{
					case 2:	if(!ChannelWriteCut[channel])
								break;
					case 0:
					case 1:
						if((globalWriteCutAutoState==true)&&!dawWasInTouch[channel])
							startCutGestureOnChannel(channel);
						else
							{
							//not writing cuts so get parameter changing from host
							endCutGestureOnChannel(channel);
							parameters.addParameterListener("CUT"+String(channel), this);
							}
					break;
					}			
				//TRIMS in WRITE MODE
				parameters.removeParameterListener("T"+String(channel), this);
				endTrimGestureOnChannel(channel);
				}
			else
			if(!(channel%2))
				{
				//volumes in WRITE MODE
				if(globalWriteVolAutoState==true)
					{
					parameters.addParameterListener("V"+String(channel), this);
					parameters.addParameterListener("V"+String(channel+1), this);
					startVolumeGestureOnChannel(channel);
					startVolumeGestureOnChannel(channel+1);
					}
				else
					{
					//printf("WRITE MODE: add listener on chan: %d\n",channel);
					parameters.addParameterListener("V"+String(channel), this);
					parameters.addParameterListener("V"+String(channel+1), this);
					endVolumeGestureOnChannel(channel);
					endVolumeGestureOnChannel(channel+1);	
						
					}
				//CUTS in WRITE MODE ON LINKED CHANNELS
				switch(taiType)
					{
					case 2:	if(!ChannelWriteCut[channel])
								break;
					case 0:
					case 1:
						if((globalWriteCutAutoState==true)&&!dawWasInTouch[channel])
							{
							startCutGestureOnChannel(channel);
							startCutGestureOnChannel(channel+1);
							}
						else
							{//not writing cuts so get parameter changing from host
							endCutGestureOnChannel(channel);
							endCutGestureOnChannel(channel+1);
							parameters.addParameterListener("CUT"+String(channel), this);
							parameters.addParameterListener("CUT"+String(channel+1), this);
							}
					break;
					}			
				//TURN OFF TRIM DATA LISTENER FROM HOST IN WRITE MODE
				parameters.removeParameterListener("T"+String(channel), this);
				parameters.removeParameterListener("T"+String(channel+1), this);
				endTrimGestureOnChannel(channel);
				endTrimGestureOnChannel(channel+1);	
				}
	break;
	//==========================================
	
//------------------------------------------------------------------------------------------------
//manage FADERS in TRIM MODE (and MUTES...)
//------------------------------------------------------------------------------------------------
	case 5://TRIM WRITE

		if(!ChannelLinkMap[channel])
			{
			//TRIM in WRITE MODE
			if(globalWriteVolAutoState==true)
				{
				parameters.addParameterListener("V"+String(channel), this);
				parameters.removeParameterListener("T"+String(channel), this);
				startTrimGestureOnChannel(channel);
				endVolumeGestureOnChannel(channel);
				}
			else
				{
				//printf("WRITE MODE: add listener on chan: %d\n",channel);
				parameters.addParameterListener("V"+String(channel), this);
				parameters.addParameterListener("T"+String(channel), this);
				endVolumeGestureOnChannel(channel);
				endTrimGestureOnChannel(channel);
				}
			//NO CUTS WRTIE in TRIM WRITE MODE
/*
			if(globalReadCutAutoState==true)
				{
				parameters.addParameterListener("CUT"+String(channel), this);
				endCutGestureOnChannel(channel);
				}
			else
				{
				parameters.removeParameterListener("CUT"+String(channel), this);
				endCutGestureOnChannel(channel);
				}
*/
			endCutGestureOnChannel(channel);
			parameters.addParameterListener("CUT"+String(channel), this);
			}
		else
		if(!(channel%2))
			{
			//volumes in TRIM WRITE MODE
			if(globalWriteVolAutoState==true)
				{
				parameters.addParameterListener("V"+String(channel), this);
				parameters.addParameterListener("V"+String(channel+1), this);
				parameters.removeParameterListener("T"+String(channel), this);
				parameters.removeParameterListener("T"+String(channel+1), this);
				endVolumeGestureOnChannel(channel);			
				endVolumeGestureOnChannel(channel+1);
				startTrimGestureOnChannel(channel);
				startTrimGestureOnChannel(channel+1);
				}
			else
				{
				//printf("WRITE MODE: add listener on chan: %d\n",channel);
				parameters.addParameterListener("V"+String(channel), this);
				parameters.addParameterListener("V"+String(channel+1), this);
				parameters.addParameterListener("T"+String(channel), this);
				parameters.addParameterListener("T"+String(channel+1), this);
				endVolumeGestureOnChannel(channel);			
				endVolumeGestureOnChannel(channel+1);
				}
/*
			//NO CUTS WRITE in TRIM WRITE MODE
			if(globalReadCutAutoState==true)
				{
				parameters.addParameterListener("CUT"+String(channel), this);
				parameters.addParameterListener("CUT"+String(channel+1), this);
				endCutGestureOnChannel(channel);	
				endCutGestureOnChannel(channel+1);	
				}
			else
				{
				parameters.removeParameterListener("CUT"+String(channel), this);
				parameters.removeParameterListener("CUT"+String(channel+1), this);
				endCutGestureOnChannel(channel);
				endCutGestureOnChannel(channel+1);
				}
*/
			endCutGestureOnChannel(channel);	
			endCutGestureOnChannel(channel+1);	
			parameters.addParameterListener("CUT"+String(channel), this);
			parameters.addParameterListener("CUT"+String(channel+1), this);
			}
	break;
	case 6://TRIM LATCH or LATCH
	case 7://TRIM READ

		if(!ChannelLinkMap[channel])
			{
			parameters.addParameterListener("V"+String(channel), this);
			parameters.addParameterListener("CUT"+String(channel), this);
			parameters.addParameterListener("T"+String(channel), this);
			endVolumeGestureOnChannel(channel);
			endCutGestureOnChannel(channel);
			endTrimGestureOnChannel(channel);
			}
		else
		if(!(channel%2))
			{
			parameters.addParameterListener("V"+String(channel), this);
			parameters.addParameterListener("CUT"+String(channel), this);
			parameters.addParameterListener("V"+String(channel+1), this);
			parameters.addParameterListener("CUT"+String(channel+1), this);
			parameters.addParameterListener("T"+String(channel), this);
			parameters.addParameterListener("T"+String(channel+1), this);
			endVolumeGestureOnChannel(channel);
			endCutGestureOnChannel(channel);
			endTrimGestureOnChannel(channel);
			endVolumeGestureOnChannel(channel+1);
			endCutGestureOnChannel(channel+1);
			endTrimGestureOnChannel(channel+1);			}

	break;
	default:break;
	}
}

