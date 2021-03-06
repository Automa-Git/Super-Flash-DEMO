// Motor Animation                                        

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define ALARM_LEVEL						450 
#define STATE_MOTOR_STARTED				1200
#define ALARM_MOTOR_SPIKING				1202

static int level = 500, onpmp2 ;
static unsigned int pospmp, tubeIn, tubeOut ;
static unsigned long timer  = 0L ;

main(int handle, int channel, int write, long ind, int numval, int val[])
{
	int start = vardb[STATE_MOTOR_STARTED] ;

	if (!write) 
	{
		if (ind == 1L)
			val[0] = animate_pos_pumpRotate(start) ;
		else if (ind == 2L)
			val[0] = animate_pos_level(start) ;
		else if (ind == 3L)
			val[0] = animate_pos_tubeIn(start) ;
		else if (ind == 4L)
			val[0] = animate_pos_tubeOut(start) ;
	}
	else 
	{
		if (ind == 2L) {
			level = val[0] ;
		}
	}

	return (0) ;
}
int animate_pos_pumpRotate(int start)
{
	if (start)
		pospmp++ ;
	else
		pospmp = 0;


	return (pospmp % 7) ;
}
int animate_pos_tubeOut(int start)
{
	if (start)
		tubeOut++ ;
	else
		tubeOut = 0;

	return (tubeOut % 3) ;
}
int animate_pos_tubeIn(int start)
{
	if (start && onpmp2)
		tubeIn++ ;
	else
		tubeIn = 0;

	return (tubeIn % 3) ;
}
int animate_pos_level(int start)
{
	int levelTest ;

	if (start) 
	{
		levelTest = level <= ALARM_LEVEL ;
	
		if (levelTest)
			level -= 12 ;
		else
			level -= 53 ;
		
		if (onpmp2)
			onpmp2 = level <= 1250 ;
		else
			onpmp2 = level < 200 ;
		
		if (onpmp2)
		{
			if (levelTest)
				level += 60 ;
			else
				level += 150 ;
		}
	}

	playAlarm(level  <= ALARM_LEVEL) ;
	
	return (level) ;
}
playAlarm(int alarmOn)
{
	if(alarmOn)
	{
		if (!timer || read_tick(timer) > 73L) 
		{
			playBalcon(PLAY_TEXT, "Warning, low gasoline level") ;
			vardb[ALARM_MOTOR_SPIKING]  = TRUE ;
			timer = start_tick() ;
		}
	}
	else if (vardb[ALARM_MOTOR_SPIKING])
	{
		playBalcon(STOP_ACTIVE, "") ;
		vardb[ALARM_MOTOR_SPIKING]  = FALSE ;
	}
}
