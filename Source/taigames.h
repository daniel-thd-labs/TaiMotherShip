	/*
  ==============================================================================

    mycomponent.h
    Created: 1 Apr 2018 12:42:28pm
    Author:  admin

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define PAD1XFRONT 5
#define PAD1XBACK 6
#define PAD1YTOP 7
#define PAD1YBOTTOM 8

#define INITIALISED 0 //stop
#define STOP 1 //stop game, after 30 seconde go to demo
#define PLAY 2 //start a new game
#define PAUSE 3 //WAIT for action, pause ball pads are active
#define RESET 4 //RESET GAME and got to start
#define DEMO 5 //DEMO mode
#define WINNER 6 //Winner mode

#define PAD2XFRONT 9
#define PAD2XBACK 10
#define PAD2YTOP 11
#define PAD2YBOTTOM 12


//==============================================================================
//
//
//==============================================================================
class taiPadGraphics : public Component
{
public:

taiPadGraphics()
	{
	setXYPos(0,0);
	setXYDim(1,1);
	}
		
~taiPadGraphics(){}

void paint (Graphics& g) override
	{
    g.setColour (Colours::orange);
    g.fillRect (posX,posY,dimX,dimY);
	//g.drawSingleLineText(String(padSpeed),posX,posY+20,Justification::left);
	}

void resized() override
	{
	
	}

void setXYPos(float x,float y)
	{
	posX=x;
	posY=y;
	}

void setXPos(float x){posX=x;}
void setYPos(float y){posY=y;}
float getXPos(void){return posX;}
float getYPos(void){return posY;}

void setPadSpeed(float speed){padSpeed=speed;}
float getPadSpeed(void){return padSpeed;}
	
void setXYDim(float x,float y)
	{
	dimX=x;
	dimY=y;
	}


void setXDim(float x)	{dimX=x;}
void setYDim(float y)	{dimY=y;}
float getXDim(void)		{return dimX;}
float getYDim(void)		{return dimY;}


private:
float posX;
float posY;
float dimX;
float dimY;
float padSpeed;
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiPadGraphics)
};

//==============================================================================
//
//
//==============================================================================
class taiPadObject : public Component
{
public:

taiPadObject()
	{
	xPos=1;
	yPos=1;
	xSize=1;
	ySize=1;
	}
		
~taiPadObject(){}

void setXPos(float x){xPos=x;}
void setYPos(float y){yPos=y;}
void setXYPos(float x,float y)
	{
	xPos=x;
	yPos=y;
	}
float getXPos(void){return xPos;}
float getYPos(void){return yPos;}

void setPadSpeed(float speed){padSpeed=speed;}
float getPadSpeed(void){return padSpeed;}

void setXDim(float x)	{xSize=x;}
void setYDim(float y)	{ySize=y;}
void setXYDim(float x,float y)
	{
	xSize=x;
	ySize=y;
	}
float getXDim(void)	{return xSize;}
float getYDim(void)	{return ySize;}

float doPadSpeed()
	{
	thisTime=Time::getMillisecondCounterHiRes();
	padSpeed=int(((lastYPos-yPos)/(thisTime-lastTime))*100)/100;
	lastTime=thisTime;
	lastYPos=yPos;
	return padSpeed;
	}

private:

float xPos;
float yPos;
float xSize;
float ySize;
double lastTime;
double thisTime;
float lastXPos;
float lastYPos;
float padSpeed;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiPadObject)
};



//==============================================================================
//
//
//==============================================================================
class taiBallGraphics : public Component
{
public:

taiBallGraphics()
	{
	xOffset=0;
	yOffset=0;
	posX=1;
	posY=1;
	dimX=15;
	dimY=15;
//	ballImg=ImageCache::getFromMemory(BinaryData::Green_silverball_40_L_png,BinaryData::Green_silverball_40_L_pngSize);
	taiBall=ImageCache::getFromMemory(BinaryData::tai_crest_64_png,BinaryData::tai_crest_64_pngSize);
	}
		
~taiBallGraphics(){}

void paint (Graphics& g) override
	{
/*    g.drawImage (ballImg,											//source
				posX,posY,											//Position
    			dimX,dimY,											//dimension of space to draw
    			0,0,												//source x,y
    			40,40,
    			false);
*/
    g.drawImage (taiBall,											//source
				posX+xOffset,posY+yOffset,											//Position
    			dimX,dimY,											//dimension of space to draw
    			0,0,												//source x,y
    			64,64,
    			false);
	}

void resized() override
	{
	}

void setXPosWithOffset(float x)	{posX=x+xOffset;}
void setYPosWithOffset(float y)	{posY=y+yOffset;}
void setXYPosWithOffset(float x,float y)
	{
	posX=x+xOffset;
	posY=y+yOffset;
	}
float getXPosWithOffset(void){return posX+xOffset;}
float getYPosWithOffset(void){return posY+yOffset;}

float getXPos(void) {return posX;}
float getYPos(void)	{return posY;}

void setXPos(float x)	{posX=x;}
void setYPos(float y)	{posY=y;}
void setXYPos(float x,float y)
	{
	posX=x;
	posY=y;
	}

void setXOffset(float x)	{xOffset=x;}
void setYOffset(float y)	{yOffset=y;}
float getXOffset(void){return xOffset;}
float getYOffset(void){return yOffset;}
	
void setXDim(float x)	{dimX=x;}
void setYDim(float y)	{dimY=y;}
void setXYDim(float x,float y)
	{
	dimX=x;
	dimY=y;
	}
float getXDim(void) 	{return dimX;}
float getYDim(void)		{return dimY;}

//--------------------------------------------------------------
private:

float posX,xOffset;
float posY,yOffset;
float dimX;
float dimY;
//Image ballImg;
Image taiBall;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiBallGraphics)

};

//==============================================================================
//
//
//==============================================================================
class taiBallObject : public Component
{
public:

taiBallObject()
	{
	xOffset=0;
	yOffset=0;
	posX=0;
	posY=0;
	dimX=24;
	dimY=24;	}
		
~taiBallObject(){}

void setXPosWithOffset(float x)	{posX=x+xOffset;}
void setYPosWithOffset(float y)	{posY=y+yOffset;}
void setXYPosWithOffset(float x,float y)
	{
	posX=x+xOffset;
	posY=y+yOffset;
	}
float getXPosWithOffset(void){return posX+xOffset;}
float getYPosWithOffset(void){return posY+yOffset;}

float getXPos(void) {return posX;}
float getYPos(void)	{return posY;}

void setXPos(float x)	{posX=x;}
void setYPos(float y)	{posY=y;}
void setXYPos(float x,float y)
	{
	posX=x;
	posY=y;
	}

void setXOffset(float x)	{xOffset=x;}
void setYOffset(float y)	{yOffset=y;}
float getXOffset(void){return xOffset;}
float getYOffset(void){return yOffset;}
	
void setXDim(float x)	{dimX=x;}
void setYDim(float y)	{dimY=y;}
void setXYDim(float x,float y)
	{
	dimX=x;
	dimY=y;
	}
	
float getXDim(void) 	{return dimX;}
float getYDim(void)		{return dimY;}


private:

float posX,xOffset;
float posY,yOffset;
float dimX;
float dimY;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiBallObject)
};

//==============================================================================
//
//
//==============================================================================
class taiPongScoreGraphics : public Component
{
public:

taiPongScoreGraphics()
	{
	xpos= 10;
	ypos= 10;
	xdim= 10;
	ydim= 10;
	score=0;
	gameWins=0;
	highScore=0;
	}
		
~taiPongScoreGraphics(){}

void paint (Graphics& g) override
	{
   	g.setFont((Font(Font::getDefaultMonospacedFontName(), 45.0f, Font::bold)));
  	g.setColour (Colours::orange);
    g.drawSingleLineText(String(score),xpos,ypos,Justification::left);
	}

void resized() override
	{
	}

void setXYPos(float x,float y)
	{
	xpos=x;
	ypos=y;
	}

void setXPos(float x){xpos=x;}
void setYPos(float y){ypos=y;}
	
void setXYDim(int x,int y)
	{
	xdim=x;
	ydim=y;
	}

void setScore(int scr)	{score=scr;}
void setHighScore(int scr) {highScore=scr;}

int getHighScore(void)	{return highScore;}
int getScore(void)	{return score;}

void setGameWins(int wins)	{gameWins=wins;}
int getGameWins(void)	{return gameWins;}

float getXPos(void)	{return xpos;}
float getYPos(void)	{return ypos;}
//-------------------------------------------------------
private:
int score; 
int highScore;
int gameWins;

float xpos;
float ypos;
int xdim;
int ydim;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiPongScoreGraphics)

};

//==============================================================================
//
//
//==============================================================================
class taiPongScoreObject : public Component
{
public:

taiPongScoreObject()
	{
	xpos= 10;
	ypos= 10;
	xdim= 10;
	ydim= 10;
	score=0;
	gameWins=0;
	highScore=0;
	winnerScore=15;
	}
		
~taiPongScoreObject(){}

void setXYPos(float x,float y)
	{
	xpos=x;
	ypos=y;
	}

void setXPos(float x){xpos=x;}
void setYPos(float y){ypos=y;}
	
void setXYDim(int x,int y)
	{
	xdim=x;
	ydim=y;
	}
int setScore(int scr)
	{
	score=scr;
	if(score > winnerScore)
		return 2;
	else
		return 1;
	}
void clrScore(){score=0;}
	
void setHighScore(int scr) {highScore=scr;}

int getScore(void)	{return score;}
int getHighScore(void)	{return highScore;}

void setGameWins(int wins)	{gameWins=wins;}
int getGameWins(void)	{return gameWins;}

float getXPos(void)	{return xpos;}
float getYPos(void)	{return ypos;}

//-------------------------------------------------------
private:
int score; 
int highScore;
int gameWins;
int winnerScore;

float xpos;
float ypos;
int xdim;
int ydim;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiPongScoreObject)

};

//==============================================================================
//
//
//==============================================================================
class taiPongGameGraphics : public Component
{
public:

taiPongGameGraphics()
	{
	gameState =0;	
	}
		
~taiPongGameGraphics()
{
}


//---------------------------------------------------	
void paint (Graphics& g) override
	{
	pad1.paint(g);
	pad2.paint(g);
	ball.paint(g);
	player1Score.paint(g);
	player2Score.paint(g);
	}

//---------------------------------------------------	
void resized() override
	{
	}
	
public:
int gameState;
taiPadGraphics pad1;
taiPadGraphics pad2;
taiBallGraphics ball;
taiPongScoreGraphics player1Score;
taiPongScoreGraphics player2Score;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiPongGameGraphics)

};

//==============================================================================
//
//
//==============================================================================
class taiPongGame : public Component,Timer
{
public:

taiPongGame()
	{
	gameState =0;	
	}
		
~taiPongGame()
{
}

//---------------------------------------------------	
//---------------------------------------------------	
void gameInit()
{
minSpeed=1;
maxSpeed=3;
hitCount=0;
stop=false;
moveCnt=0;
effect=0;
speed=minSpeed;
gameWinScore=6;
angle =45.0f;
roundWinner=0;
xmove=(std::cos(MathConstants<float>::pi*(angle/180)));
ymove=(std::sin(MathConstants<float>::pi*(angle/180)));
		
pad1xdim= pad1.getXDim();
pad1ydim= pad1.getYDim();

pad2xdim= pad2.getXDim();
pad2ydim= pad2.getYDim();

ballxdim=ball.getXDim();
ballydim=ball.getYDim();

pad1xpos= gameOffset+50;
pad1ypos= (gameHeight/2)-(pad1.getXDim()/2);;

pad2xpos= gameWidth-50;
pad2ypos= (gameHeight/2)-(pad2.getXDim()/2);

/*	
pad1.setXYPos(pad1xpos,pad1ypos);
pad1.setXYDim(pad1xdim,pad1ydim);

pad2.setXYPos(pad2xpos,pad2ypos);
pad2.setXYDim(pad2xdim,pad2ydim);
*/
resetBall();
//ball.setXYDim(ballxdim,ballydim);

setRandomDirection();

player1Score.clrScore();
player2Score.clrScore();

player1Score.setXYPos(0,0);
player2Score.setXYPos(0,0);

xdir=1;
ydir=1;

pad1Index=0;
pad2Index=7;	
gameInitMode();
}

void resetBall()
	{
	ballxpos=gameOffset+((gameWidth-gameOffset)/2)-(ballxdim/2);//((pad1xpos+pad2xpos)/2)+pad1xpos;
	ballypos=(gameHeight/2)-(ballydim/2);
	ball.setXYPos(ballxpos,ballypos);
	}

void timerCallback() override
{
if(countDown)
	countDown--;
}

//---------------------------------------------------	
//---------------------------------------------------	
void setGameHeight(int height)	{gameHeight=height;}
void setGameWidth(int Width)	{gameWidth=Width;}
void setGameOffset(int Offset)	{gameOffset=Offset;}
float getGameHeight(void)	{return gameHeight;}
float getGameWidth(void)	{return gameWidth;}
float getGameOffset(void)	{return gameOffset;}

void setRandomDirection(void)
	{
	if (Random::getSystemRandom().nextInt (Range<int> (0, 1)))
		ballXDir =LEFT;
	else
		ballXDir =RIGHT;
	if (Random::getSystemRandom().nextInt (Range<int> (0, 1)))
		ballYDir =UP;
	else
		ballYDir =DOWN;
	}


int setPadYPos(int n, int yPostion,int SystemSize)
{
int chn=n-(SystemSize-8);
switch(chn)
	{
	case 0:
		if(yPostion<0)
			yPostion=0;
		if(yPostion>(gameHeight-pad1.getYDim()))
			yPostion=gameHeight-pad1.getYDim();
		//yPostion=yPostion-(pad1.getYDim()/2);
		pad1ypos=450*float(yPostion)/450;
		pad1.setYPos(pad1ypos);
	break;
	case 7:
		if(yPostion<0)
			yPostion=0;
		if(yPostion>(gameHeight-pad2.getYDim()))
			yPostion=gameHeight-pad2.getYDim();
		//yPostion=yPostion-(pad2.getYDim()/2);
		pad2ypos=450*float(yPostion)/450;
		pad2.setYPos(pad2ypos);
	break;
	default:return 0;break;
	}
return 0;
}

float getPadXPos(int n)
	{
	if(n==1)
		return pad1.getXPos();
	if(n==2)
		return pad2.getXPos();
	return 0;
	}
	
float getPadYPos(int n)
	{
	if(n==1)
		return pad1.getYPos();
	if(n==2)
		return pad2.getYPos();
	return 0;
	}

//---------------------------------------------------	
//---------------------------------------------------	
void gameInitMode(void){gameState=0; stopTimer(); gameStateChangedFlag=true;}
void gameStopMode(void){gameState=STOP; startTimer(10);gameStateChangedFlag=true;stopFlag=true;}
void gamePlayMode(void){gameState=PLAY;gameStateChangedFlag=true;}
void gamePauseMode(void){gameState=PAUSE;gameStateChangedFlag=true;}
void gameResetMode(void){gameState=RESET;gameStateChangedFlag=true;}
void gameDemoMode(void){gameState=DEMO;gameStateChangedFlag=true;}
void gameWinnerMode(void){gameState=WINNER;gameStateChangedFlag=true;}

void clearGameStateChangedFlag(){gameStateChangedFlag=false;};
bool getGameStateChangedFlag(){return gameStateChangedFlag;};
int getWinner(void){return gameWinner;}


//---------------------------------------------------	
//---------------------------------------------------	
int getGameState()
{
return gameState;
}

//---------------------------------------------------	
//---------------------------------------------------	
void setGameState(int state)
{
gameState=state;
}

//---------------------------------------------------	
//---------------------------------------------------	
void doTaiPong()
	{
	switch(gameState)
		{
		case RESET: gameReset();break;
		case STOP:	runStop();	break; 
		case PAUSE:				break;
		case PLAY:
				pad1.doPadSpeed();
				pad2.doPadSpeed();
				//DBG("PAD 1 Speed"<<pad1.getPadSpeed());
				runPlay();
	 	break;
		case WINNER:runWinner();break;
		case DEMO: 	runDemo(); 	break;
		}
	}

//---------------------------------------------------	
void gameReset(void)
	{
	ballXDir =LEFT;
	ballYDir =UP;
	player1Score.clrScore();
	player2Score.clrScore();
	if(roundWinner)
		readyForNextRound(roundWinner);
	matchState=0;
	roundWinner=0;
	setCountDown(3);
	gamePlayMode();
	}
	
//---------------------------------------------------	
int runStop()
	{
	if(stopFlag)
		{
		stopFlag=false;
		setRandomDirection();
		player1Score.clrScore();
		player2Score.clrScore();
		stop=true;
		matchState=0;
		roundWinner=0;
		resetBall();
		}
	setCountDown(2);
	return 0;
	}

//---------------------------------------------------	
int runPlay()
	{
	if(stop==false)
		{
		moveBall(1);
		matchState=crashTest(PLAY);
		if(!matchState)
			return 0;
		stop=true;
		if(matchState<gameWinScore)
			{
			readyForNextRound(roundWinner);
			setCountDown(2);
			}
		else
			{
			gameWinner=roundWinner;
			gameWinnerMode();
			setCountDown(15);
			}
		}
	else
		{
		if(!getCountDown())
			stop=false;
		else
			moveBall(0);
		}
	return 0;
	}

//---------------------------------------------------	
void setCountDown(int cnt)
	{
	countDown=cnt*100;
	}

//---------------------------------------------------	
int getCountDown(void)
	{
	return countDown;
	}

//---------------------------------------------------	
void runWinner()
	{
	if(!getCountDown())
		{
		stop=false;
		gameDemoMode();
		}
	}
	
//---------------------------------------------------	
void runDemo()
	{
	moveBall(1);
	matchState=crashTest(DEMO);
	}
	
//---------------------------------------------------	
void readyForNextRound(int Winner)
{
if(Winner==1)
	{
	ball.setXPos(pad1.getXPos()+pad1.getXDim());
	ball.setYPos((pad1.getYPos()+(pad1.getYDim()/2))-(ball.getYDim()/2));
	}
else
if(Winner==2)
	{
	ball.setXPos(pad2.getXPos()-ball.getXDim());
	ball.setYPos((pad2.getYPos()+(pad2.getYDim()/2))+(ball.getYDim()/2));
	}
}

//---------------------------------------------------	
//---------------------------------------------------	
void startNewGame(){}

//---------------------------------------------------	
//---------------------------------------------------	
void newSpeed(float newAngle)
	{
	xmove=(std::cos(MathConstants<float>::pi*(newAngle/180)));
	ymove=(std::sin(MathConstants<float>::pi*(newAngle/180)));
	}

//---------------------------------------------------	
//---------------------------------------------------	
void moveBall(int mode)
	{
	float bxpos=ball.getXPos();
	float bypos=ball.getYPos();
	float xval;
	float yval;
	if(mode)
		{
		if(!stop)
			{
			xval=(float)xmove*speed*xdir;
			yval=(float)ymove*speed*ydir;
			xval=bxpos+xval;
			yval=bypos+yval;
			ball.setXOffset(effect*(std::cos(MathConstants<float>::pi*((xval*5)/180))));
			ball.setYOffset(effect*(std::sin(MathConstants<float>::pi*((yval*10)/180))));
			ball.setXPos(xval);
			ball.setYPos(yval);
			}
		}
	else
		{
		if(roundWinner==1)
			ball.setYPos((pad1.getYPos()+(pad1.getYDim()/2))-(ball.getYDim()/2));
		else
		if(roundWinner==2)
			ball.setYPos((pad2.getYPos()+(pad2.getYDim())/2)-(ball.getYDim()/2));
		}	
	}	
	
//---------------------------------------------------	
//---------------------------------------------------	
int crashTest(int mode)
	{
	float pad1Front = pad1.getXPos()+pad1.getXDim()+(ball.getXDim()/2);//-(pad1.getXDim()/2)+(ball.getXDim()/2);
	float pad1Back = pad1.getXPos()-(ball.getXDim()/2);///2)+(ball.getXDim()/2);
	float pad1Top =	pad1.getYPos()-(ball.getYDim()/2);
	float pad1Bottom = pad1.getYPos()+pad1.getYDim()+(ball.getYDim()/2);

	float pad2Front = pad2.getXPos()-(ball.getXDim()/2);
	float pad2Back = pad2.getXPos()+(pad2.getXDim()/2)+(ball.getXDim()/2);
	float pad2Top =	pad2.getYPos()-(ball.getYDim()/2);
	float pad2Bottom = pad2.getYPos()+pad2.getYDim()+(ball.getYDim()/2);
	
	float ballx=ball.getXPosWithOffset()+(ball.getXDim()/2);
	float bally=ball.getYPosWithOffset()+(ball.getYDim()/2);
	float gameLeftlimit=gameOffset+(ball.getXDim()/2);
	float gameRightlimit=gameWidth-(ball.getXDim()/2);
	float gameBottomlimit=gameHeight;
	float gameToplimit=0;
	
//--------LEFT WALL COLLISION TEST----------------
	if(xdir<0)
		{
		if(ballx<=gameLeftlimit)
			{
			hitCount=0;
			speed=minSpeed;
			angle=45;					
			newSpeed(angle);
			//DBG("ANGLE: "<<angle);
			if(mode==PLAY)
				{
				int scr=player2Score.getScore();
				scr++;
				roundWinner=2;
				player2Score.setScore(scr);
				return scr;
				}
			else
				xdir=1;
			return 0;
			}

//--------PAD1 COLLISIONS TESTS----------------
		if(mode==PLAY)
			{
			if((bally>pad1Top)&&(bally<pad1Bottom)
				&&(ballx<pad1Front)&&(ballx>pad1Back))
				{
				//if(pad1.getPadSpeed())
					DBG("HIT        ------------------- PAD 1 Speed"<<pad1.getPadSpeed());
				hitCount++;
				//edge tests-----------------------
				if(((bally>pad1Top)&&(bally<pad1Top+5))
					||((bally<pad1Bottom)&&(bally>pad1Bottom-5)))
					{
					angle=60;					
					newSpeed(angle);
					//DBG("ANGLE: "<<angle);
					xdir=1;
					ydir=ydir*(-1);
					}
				else
					{
					float hit=(pad2Bottom-bally)/(pad2Bottom-pad2Top);
					if(hit<=0.25)
						angle=52;
					else
					if(hit<=0.45)
						angle=45;
					else
					if(hit<=0.75)
						angle=45;
					else
					if(hit<=1.0)
						angle=52;					
					newSpeed(angle);
					//DBG("ANGLE: "<<angle);
					xdir=1;
					}
				if(hitCount)
					if(speed<maxSpeed)
						if(hitCount%2)
							speed=speed+0.25f;
				return 0;
				}		
			}
		}

//--------RIGHT WALL COLLISION TEST----------------
	if(xdir>0)
		{
		if(ballx>=gameRightlimit)
			{
			hitCount=0;
			speed=minSpeed;
			angle=45;					
			//DBG("ANGLE: "<<angle);
			newSpeed(angle);
			if(mode==PLAY)
				{
				int scr=player1Score.getScore();
				scr++;
				roundWinner=1;
				player1Score.setScore(scr);
				return scr;
				}
			else
				{
				xdir=-1;
				}
			return 0;
			}
//--------PAD2 COLLISIONS TESTS----------------
		if(mode==PLAY)
			{
			if((bally>pad2Top)&&(bally<pad2Bottom)
				&&((ballx>pad2Front)&&(ballx<pad2Back)))
				{
				hitCount++;
				if(pad2.getPadSpeed())
					DBG("PAD 2 Speed"<<pad2.getPadSpeed());
				//edge tests-----------------------
				if(((bally>pad2Top)&&(bally<pad2Top+5))
					||((bally<pad2Bottom)&&(bally>pad2Bottom-5)))
					{
					angle=60;					
					newSpeed(angle);
					//DBG("ANGLE: "<<angle);
					xdir=-1;
					ydir=ydir*(-1);
					}
				else
					{
					float hit=(pad2Bottom-bally)/(pad2Bottom-pad2Top);
					if(hit<=0.25)
						angle=52;
					else
					if(hit<=0.45)
						angle=45;
					else
					if(hit<=0.55)
						angle=0;
					else
					if(hit<=0.75)
						angle=45;
					else
					if(hit<=1.0)
						angle=52;					
					newSpeed(angle);
					//DBG("ANGLE: "<<angle);
					xdir=-1;
					}
				if(hitCount)
					if(speed<maxSpeed)
						if(hitCount%2)
							speed=speed+0.25f;
				return 0;
				}
			}
		}
		
//--------WALL Y COLLISIONS TESTS----------------
	if(bally<=gameToplimit)
		{
		ydir=1;
		return 0;
		}
	if(bally>=gameBottomlimit)
		{
		ydir=-1;
		return 0;
		}
	return 0;
   }


//---------------------------------------------------	
//---------------------------------------------------	
int taiGameType;

taiPadObject pad1;
taiPadObject pad2;
taiBallObject ball;
taiPongScoreObject player1Score;
taiPongScoreObject player2Score;

int gameState;
bool gameStateChangedFlag;
int roundWinner;
int gameWinner;
int matchState;
bool stop;
int countDown;
int gameWinScore;
int hitCount;
int effect;
//------------------
private:

int pad1Index;
int pad2Index;
bool stopFlag;
bool newGameFlag;
int gameHeight;
int gameWidth;
int gameOffset;
float xmove;
float ymove;
int moveCnt;
float angle;
float speed;
float maxSpeed;
float minSpeed;

int xdir;
int ydir;
float pad1xpos,pad2xpos;
int pad1xdim,pad2xdim;

float pad1ypos,pad2ypos;
int pad1ydim,pad2ydim;

float ballxpos,ballypos;
int	ballxdim,ballydim;
int ballXDir,ballYDir;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (taiPongGame)

};
