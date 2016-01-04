#include "Piezo.h"

Piezo::Piezo(int pin, int threshold) 
{
	signalPin = pin;
	piezoThreshold = threshold; //default threshold
}
int Piezo::read() //return Piezo reading
{
	return analogRead(signalPin);
}
bool Piezo::readThreshold(int threshold) //return true when reading above certain level
{
	return (analogRead(signalPin)>= max(piezoThreshold,threshold)); //user default threshold if threshold parameters stayed unfilled
}
int Piezo::recorder(int type[], int maxLenght, int timeOut, int listeningMode) //record knock sequence
{
	int maxTime,i, startTime;
	if(listeningMode)
	{
		maxTime = millis() + 5000; //5 second timeOut when listen to first knock
		while(1)
		{
			if(millis()>maxTime) //if timeOut return 0
				return 0;
			if(readThreshold())	//if knock detected start recording
				break;
		}
	}
	
	startTime = millis();		//all time compared to the startTime;
	for(i=0; i<maxLenght; i++)	//record no more than maxLenght knocks
	{
		maxTime = millis() + timeOut; //define timeOut
		while(1)
		{
			if(millis()>maxTime)	//if timeOut return knock sequence length
				return i;
			if(readThreshold())		//if knock detected break the while loop
				break;
		}
		type[i] = millis() - startTime;	//define the time of the knock relative to the start time
		delay(30); //avoiding sticky knocks, wait for 30 ms
	}
	return i; //return knock sequence length
}

//compare between to recordings, they have to  
//they have to be in the same lenght
//maxDiff define the allowed change in percentage  - 0 to 100
bool Piezo::recordComparison(int sequence[], int comparedSequence[],int lenght,int maxDiff) 
{
	if(lenght < 2) //can't compare one single knock
		return false;
	for(int i=1;i<lenght;i++) // start from the second knock, the first one is to sticky
	{
		//calc the comparedSequence difference from sequence in percentage
		if(100*abs(sequence[i] - comparedSequence[i]) > sequence[i] * maxDiff) //if it is too different return false;
			return false; 
	}
	return true;
}

