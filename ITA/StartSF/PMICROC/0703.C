// Note: Chance production

#include <microc.h>
#include <sf.h>

#define 	M1_SPEED				1041
#define 	M2_SPEED				1042
#define 	M3_SPEED				1043

#define 	PRODUCTION_TYPE		1046
#define 	PCS54_PROD			1
#define 	PCS96_PROD			2
#define 	PCS150_PROD			3	

main()
{
	int prodType = vardb[PRODUCTION_TYPE] ;

	if(prodType == PCS54_PROD)
	{
		vardb[M1_SPEED] = 180;
		vardb[M2_SPEED] = 250;
		vardb[M3_SPEED] = 130;
	}
	else if(prodType == PCS96_PROD)
	{
		vardb[M1_SPEED] = 100;
		vardb[M2_SPEED] = 170;
		vardb[M3_SPEED] = 80;
	}
	else
	{
		vardb[M1_SPEED] = 60;
		vardb[M2_SPEED] = 100;
		vardb[M3_SPEED] = 50;
	}
}

