/*
  ==============================================================================

    PipeComm.cpp
    Created: 27 Aug 2019 6:23:45am
    Author:  admin

  ==============================================================================
*/

#include "PipeComm.h"
//==============================================================================
//
//-----------------------------  CFG IPC SECTION  ----------------------------------
//
//==============================================================================
bool outputMsDataPipe::openPipe()
{
if(connectToPipe(MSDATAPIPENAME, -1))
	{
	//DBG("pipe OPEN: "+chanOutName);
	return true;
	}
//else
	//DBG("pipe NOT OPEN: "+chanOutName);
return false;
}

//==============================================================================
//
//
//==============================================================================
bool outputMsDataPipe::writePipe(unsigned char *dataBuffer)
{
const MemoryBlock msg(dataBuffer, MSDATASIZE);
if(sendMessage (msg))     /* Block until jojoIPC2. */
	{
//	DBG("Message sent");
	return true;
	}
//else
//	DBG("ERROR Message not sent");
return false;
}

//==============================================================================
//
//
//==============================================================================
void outputMsDataPipe::closePipe()
{
disconnect();
}

//==============================================================================
//
//-----------------------------  CFG IPC SECTION  ----------------------------------
//
//==============================================================================
bool outputMsCfgPipe::openPipe()
{
if(connectToPipe(MSCFGPIPENAME, -1))
	{
	//DBG("pipe OPEN: "+chanOutName);
	return true;
	}
//else
	//DBG("pipe NOT OPEN: "+chanOutName);
return false;
}

//==============================================================================
//
//
//==============================================================================
bool outputMsCfgPipe::writePipe(unsigned char *dataBuffer)
{
const MemoryBlock msg(dataBuffer, MSCFGSIZE);
if(sendMessage (msg))     /* Block until jojoIPC2. */
	{
//	DBG("Message sent");
	return true;
	}
//else
//	DBG("ERROR Message not sent");
return false;
}

//==============================================================================
//
//
//==============================================================================
void outputMsCfgPipe::closePipe()
{
disconnect();
}

//==============================================================================
//
//-----------------------------  MS IPC SECTION  ----------------------------------
//
//==============================================================================
bool outputMsMapLinkPipe::openPipe()
{
if(connectToPipe(MSMAPLINKPIPENAME, -1))
	{
	//DBG("pipe OPEN: "+chanOutName);
	return true;
	}
//else
	//DBG("pipe NOT OPEN: "+chanOutName);
return false;
}

//==============================================================================
//
//
//==============================================================================
bool outputMsMapLinkPipe::writePipe(unsigned char *dataBuffer)
{
const MemoryBlock msg(dataBuffer, MSMAPLINKSIZE);
if(sendMessage (msg))     /* Block until jojoIPC2. */
	{
//	DBG("Message sent");
	return true;
	}
//else
//	DBG("ERROR Message not sent");
return false;
}

//==============================================================================
//
//
//==============================================================================
void outputMsMapLinkPipe::closePipe()
{
disconnect();
}
