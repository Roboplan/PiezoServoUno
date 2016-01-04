#include "Global.h"
#define KnocksSequenceMaxLength 10
#define ServoMinimumPosition	20
#define ServoMaximumPosition	150

int knockSequence[KnocksSequenceMaxLength];
int tempKnockSequence[KnocksSequenceMaxLength];
int knocksSequenceLength  = 0;

void setup() 
{
	Serial.begin(9600);
	servo.attach(SERVO_SIGNAL_PIN);
	servo.write(ServoMinimumPosition);
	Serial.println("start");
	
	//start waiting to new knock sequence, continue till one is found
	while(1)
	{
		Serial.println("record a new knock sequence");
		//record a new knocks sequence to an array:
		knocksSequenceLength = piezo.recorder(knockSequence,KnocksSequenceMaxLength,1000,1); //1000 is the maximum time between knocks in ms, 1 mean listening mode - wait for first knock.
		if(knocksSequenceLength < 2) //one or zero knocks isn't a sequence, try again 
			Serial.println("record failed");
		else	
			break;
	}
	//Send knocks sequence via Serial
	Serial.println("Knocks sequence:");
	for(int i=0;i<knocksSequenceLength;i++)
	{
		Serial.print(knockSequence[i]);
		if(i < knocksSequenceLength - 1) //put a comma between 2 numbers, but not in the last number
			Serial.print(",");
		else
			Serial.println(); //enter when finished 
	}
	
	Serial.println("Start Listening");
}

void loop() 
{	
	if(piezo.readThreshold(30)) //wait to first knock
	{
		int tempKnocksSequenceLength = piezo.recorder(tempKnockSequence,KnocksSequenceMaxLength); //read knock sequence
		if(tempKnocksSequenceLength == knocksSequenceLength) //check length compatibility
		{
		  if(piezo.recordComparison(knockSequence,tempKnockSequence,knocksSequenceLength)) //compare two sequences
		  {
			servo.write(ServoMaximumPosition);
			Serial.println("Hurray - recorded knocks sequence has found");
			delay(1000);
			servo.write(ServoMinimumPosition);
		  }
		}

		if(tempKnocksSequenceLength != 0) //send recorded knocks sequence via Serial
		{
			Serial.println("Knocks sequence recorded:");
			for(int i=0;i<tempKnocksSequenceLength;i++)
			{
				Serial.print(tempKnockSequence[i]);
				if(i < tempKnocksSequenceLength - 1) //put a comma between 2 numbers, but not in the last number
					Serial.print(",");
				else
					Serial.println(); //enter when finished 
			}
		}
	}
}
