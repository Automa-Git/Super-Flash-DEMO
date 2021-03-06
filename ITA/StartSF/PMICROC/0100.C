//Meters Animation

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define MAXINPUT					16

#define VAR_ANIMATION				1900							

#define INPUT1						1909 
#define STATE_INPUT1				1920
#define INPUT2						1910 
#define STATE_INPUT2				1921
#define INPUT3						1911 
#define STATE_INPUT3				1922

#define OEE_AVAILABILITY			1941
#define OEE_PERFORMANCE			1942
#define OEE_QUALITY				1943

#define BACKGROUND_METER			1932
#define LABEL_METER				1933

static int val[MAXINPUT], inc[MAXINPUT], min[MAXINPUT], max[MAXINPUT];
static int init = FALSE;
static unsigned long timerMeter1  = 0L ;
static unsigned long timerMeter2  = 0L ;
static unsigned long timerMeter3 = 0L ;
static unsigned long timerOEE = 0L ;


main(int ident, int channel, int write, long job, int numval, int arrval[])
{
	int err = FALSE;
	int index = 0;

	if (!write) 
	{
		if (job == 1) 
		{
			if (!init)
				MeterInitialize();

			while (index < MAXINPUT) 
			{
				if (index < 9) 
					StartAnimation(index, 1);
				
				if(index == 9 &&	vardb[STATE_INPUT1]) 
				{
					if (!timerMeter1 || read_tick(timerMeter1) > 5L) 
					{
						StartAnimation(index, 1);
						timerMeter1 = start_tick() ;
					}
				}
				else 
					val[9] = vardb[INPUT1] ;
					
				if(index == 10 &&	vardb[STATE_INPUT2])
				{
					if (!timerMeter2 || read_tick(timerMeter2) > 10L) 
					{
						StartAnimation(index, 5);
						timerMeter2 = start_tick() ;
					}
				}
				else 
					val[10] = vardb[INPUT2] ;
					
				if(index == 11 &&	vardb[STATE_INPUT3])
				{
					if (!timerMeter3 || read_tick(timerMeter3) > 20L) 
					{
						StartAnimation(index, 5);
						timerMeter3 = start_tick() ;
					}
				}
				else
					val[11] = vardb[INPUT2] ;
				
				if(index == 12)
					if (!timerOEE || read_tick(timerOEE) > 30L) 
					{
						SimulatorOEEValue(index);
						timerOEE = start_tick() ;
					}
				
				index++;
			}
		}
		else err = FALSE;
	}
	else 
	{
		if (job == 100)
			ChangeBackgroundMeter(arrval[0]);
		else err = TRUE;
	}

	return (err);
}

MeterInitialize()
{
	inc[0] = inc[2] = inc[4] = inc[6] = inc[8] = inc[9] = inc[10] = inc[11] = inc[12] =  inc[13] =  inc[14] = inc[15] = 1;
	inc[1] = inc[3] = inc[5] = inc[7] = 0;

	val[0] = 70;
	val[1] = val[6] = val[14] =  30;
	val[2] = val[7] = 90;
	val[3] = val[5] = 50;
	val[4] = val[10] = val[13] = 20;
	val[8] = 80;
	val[9] = val[12] = 50;
	val[11] =  5;
	val[15] = 10; 

	min[0] = min[1] = min[2] = min[3] = min [4] = min [5] = min[6] = min[7] = min[9] = min[12] = min[13] = min[14] = min[15] =  0;
	min[8] = 160;
	min[10] = 20;
	min[11] = 30;

	max[0] = max[1] = max[2] = max[3] = max[4] = max[5] = max[6] = max[7] = max[9] = max[12] = max[13] =  max[14] =  max[15] =  100;
	max[8] = 220;
	max[10] = 160;
	max[11] = 240;

	init = TRUE;
}

StartAnimation(int index, int increment)
{
	int  valMeter;

	valMeter = MeterAnimation(index, increment) ;
	vardb[VAR_ANIMATION + index] = valMeter ;
}

MeterAnimation(int index, int increment)
{
	if (inc[index]) 
	{
		val[index] = val[index] + increment ;
		if (val[index] >= max[index])
			if (index <3)
				val[index] = min[index] ;
			else 
				inc[index] = 0;
	}
	else	
	{
		val[index] = val[index] - increment ;
		if (val[index] <= min[index]) 
		{
			if (index <3)
				val[index] = max[index];
			else 
			{
				val[index] = min[index];
				inc[index] = 1;
			}
		}
	}

	return (val[index]) ;
}

SimulatorRandomValue(int index, int minRnd, int maxRnd)
{
	int  diff, rndValue, valMeter;

	diff = (maxRnd - minRnd); 
	rndValue = (rand()%diff) + minRnd;

	if(val[index] > 85) 
	{
		if( rndValue%2 == 0 )
			valMeter = val[index] + rndValue;
		else
			valMeter = val[index] - rndValue;
	}
	else
		valMeter = val[index] + rndValue;

	if(valMeter > max[index])
		valMeter = 90;
	
	if (valMeter < min[index] )
		valMeter = min[index] ;		
	
	vardb[VAR_ANIMATION + index] = val[index] = valMeter;
	
	return valMeter;
}

SimulatorOEEValue(int index)
{
	double valAvailability, valPerformance, valQuality, valOee ;
	
	if(vardb[OEE_AVAILABILITY])
		valAvailability = SimulatorRandomValue(index, 2, 8);
	else
		val[index] = valAvailability = vardb[VAR_ANIMATION + index] ;
	index ++;
	
	if(vardb[OEE_PERFORMANCE])
		valPerformance = SimulatorRandomValue(index, 0, 10);
	else
		val[index] = valPerformance = vardb[VAR_ANIMATION + index] ;
	index ++;
	
	if(vardb[OEE_QUALITY])
		valQuality = SimulatorRandomValue(index, 5, 12);
	else
		val[index] = valQuality = vardb[VAR_ANIMATION + index] ;
	
	valOee = valAvailability * valPerformance * valQuality;
	index ++;
	vardb[VAR_ANIMATION + index] = valOee;
}

ChangeBackgroundMeter(int number) 
{
	utext filename<256>;
	utext strNumber, strUnit;

	dc_int_to_utext(strNumber, number, 1, 0, 0, 1, 0, 0) ;
	filename = U"ICONS\\Meter_Page\\Backgrounds\\background" +  strNumber + U".png"; 

	if(number == 1)
		strUnit = U"DC VOLT";
	else if (number == 2)
		strUnit = U"BAR";
	else
		strUnit = U"VOLT";
	
	uvardb[BACKGROUND_METER] =  filename;
	uvardb[LABEL_METER] =  strUnit;
}
