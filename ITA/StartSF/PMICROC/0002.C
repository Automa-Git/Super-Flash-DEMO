// Note: Alarms simulation

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define	WORD1			1703
#define 	WORD2			1704

main()
{
	int indall = 16;
	int index = 0;
	unsigned long timer = 0L;

	timer = start_tick();
	vardb[WORD2] = 3;
	while(indall > 0)
	{
		if(index <= 3)
			while(read_tick(timer) <= 18L) ; //1 second
		else if(index >= 4 && index <= 10)
			while(read_tick(timer) <= 91L); //5 second
		else
			while(read_tick(timer) <= 180L); //10 second
		
		index ++;
		timer = start_tick();
		vardb[WORD1] |= 0x01 << (indall -1);
		indall--;
	}
	
}
