/*
  ==============================================================================

    PipeComm.h
    Created: 27 Aug 2019 6:24:28am
    Author:  admin

  ==============================================================================
*/
#include "JuceHeader.h"
#define __IPC
//#define PIPENAME "/tmp/InjektorChnPipe"

//#define MSDATAPIPENAME "/tmp/InjektorMsPipe"
#define MSDATAPIPENAME "/tmp/InjektorMsDataPipe"
#define MSMAPLINKPIPENAME "/tmp/InjektorMsMapLinkPipe"
#define MSCFGPIPENAME "/tmp/InjektorMsCfgPipe"
#define MSDATASIZE 512
#define MSMAPLINKSIZE 192
#define MSCFGSIZE 128


//DEBUT DE LA ZONE DES MODIFICATIONS IPC
// ------------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------------
class outputChnPipe : public InterprocessConnection
{

public:
    outputChnPipe() : InterprocessConnection (false) {}
    ~outputChnPipe(){}
    void connectionMade()	{ 
    						//DBG ("Ready to Receive from PIPE");
    						}
    void connectionLost()	{ 
    						//DBG ("Slave ConnectionLost");
    						}

	bool openPipe(int chn);
	bool writePipe(unsigned char *dataBuffer);
	void closePipe(void);
    
    void messageReceived (const MemoryBlock& mb){}

private:
	juce::String chanOutName;

};

// ------------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------------
class outputMsDataPipe : public InterprocessConnection
{

public:
    outputMsDataPipe() : InterprocessConnection (false) {}
    ~outputMsDataPipe(){}
    void connectionMade()	{ 
    						//DBG ("Ready to Receive from PIPE");
    						}
    void connectionLost()	{ 
    						//DBG ("Slave ConnectionLost");
    						}

	bool openPipe(void);
	bool writePipe(unsigned char *dataBuffer);
	void closePipe(void);
    
    void messageReceived (const MemoryBlock& mb){}

private:
	juce::String chanOutName;

};

// ------------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------------
class outputMsMapLinkPipe : public InterprocessConnection
{

public:
    outputMsMapLinkPipe() : InterprocessConnection (false) {}
    ~outputMsMapLinkPipe(){}
    void connectionMade()	{ 
    						//DBG ("Ready to Receive from PIPE");
    						}
    void connectionLost()	{ 
    						//DBG ("Slave ConnectionLost");
    						}

	bool openPipe(void);
	bool writePipe(unsigned char *dataBuffer);
	void closePipe(void);
    
    void messageReceived (const MemoryBlock& mb){}

private:
	juce::String chanOutName;

};

// ------------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------------
class outputMsCfgPipe : public InterprocessConnection
{

public:
    outputMsCfgPipe() : InterprocessConnection (false) {}
    ~outputMsCfgPipe(){}
    void connectionMade()	{ 
    						//DBG ("Ready to Receive from PIPE");
    						}
    void connectionLost()	{ 
    						//DBG ("Slave ConnectionLost");
    						}

	bool openPipe(void);
	bool writePipe(unsigned char *dataBuffer);
	void closePipe(void);
    
    void messageReceived (const MemoryBlock& mb){}

private:
	juce::String chanOutName;

};

#pragma once
