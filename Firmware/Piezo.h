#ifndef RGBLED_H
#define RGBLED_H

#include "arduino.h"

class Piezo
{
	public:
		Piezo(int pin, int threshold = 50);
		int read(); 
		bool readThreshold(int threshold = -1);
		int recorder(int type[], int maxLenght, int timeOut = 1000, int listeningMode = 0);
		bool recordComparison(int typeA[], int typeB[],int size,int maxDiff = 15);
		
	private:
		int signalPin;
		int piezoThreshold;
};
#endif //__RGBLED_H
