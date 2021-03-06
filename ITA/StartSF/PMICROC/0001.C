// General Command

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

#define	ALARM_MOTOR_SPIKING		1202
#define 	IS_SPIKING				1709
#define	IS_SPIKING_HELP			1722

main()
{
	if( vardb[ALARM_MOTOR_SPIKING])
	{
		playBalcon(STOP_OTHER,"");
		vardb[ALARM_MOTOR_SPIKING]  = FALSE;
	}

	if( vardb[IS_SPIKING] || vardb[IS_SPIKING_HELP] )
	{
		playBalcon(STOP_OTHER,"");
		vardb[IS_SPIKING_HELP]  = FALSE;
		vardb[IS_SPIKING] = FALSE;
	}
}
