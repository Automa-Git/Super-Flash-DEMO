// Execute Demo Trend Application

#include <microc.h>
#include <sf.h>

main()
{
	system_command("","..\\Trend\\wrunfile.exe","  /STRETCH_DIM_MONITOR /STRETCH_ALG_BASIC /STRETCH_KEY_DIM:F1 /STRETCH_KEY_ALG:F2 /STRETCH_KEY_INFO:F3","..\\Trend",SYSTEM_COMMAND_SHOW);
}

