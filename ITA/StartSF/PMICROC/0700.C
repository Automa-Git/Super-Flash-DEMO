// Note: Driver: Dynamic colouring

#include <microc.h>
#include <sf.h>

#define 	TRUE 				1
#define 	FALSE 				0

#define 	RED_VALUE			1031
#define	GREEN_VALUE			1032
#define	BLUE_VALUE			1033
#define	STATE_PLAY			1034


static int red = 0 ;
static int green = 0 ;
static int blue = 0 ;
static int current = 0 ;
static int last = 0 ;
static int redFinishStatus  = FALSE;
static int greenFinishStatus  = FALSE;

main(int ident, int channel, int write, long job, int numval,long arrval[])
{
	int err = FALSE;

	if (!write) 	
	{
		if (job == 1) 
		{	//calculate color
			red = vardb[RED_VALUE] ;
			green = vardb[GREEN_VALUE] ;
			blue = vardb[BLUE_VALUE] ;
		
			if(vardb[STATE_PLAY])
			{
				arrval[0] = make_rgbcolor(red, green, blue) ;
				red += 10 ;
				//change color gradient red
				if (redFinishStatus == TRUE || red > 255) 
				{
					red = 50;
					redFinishStatus = TRUE;
					green += 10 ;
				}
			
				//change color gradient green
				if (greenFinishStatus == TRUE || green > 255) 
				{
					green = 60;
					greenFinishStatus = TRUE;
					blue += 10 ;
				}
			
				//change color gradient blue 
				if (blue > 255) 
				{
					blue = 0 ;
					current += 5 ;
					if (current > 25) 
					{
						current = 0 ;
					}
				}
				
				if (current != last) 
				{
					red = 0 ;
					blue = 0 ;
					green = 0 ;
					last = current ;
					redFinishStatus = FALSE;
					greenFinishStatus  = FALSE;
				}
			
				vardb[RED_VALUE] = red ;
				vardb[GREEN_VALUE] = green ;
				vardb[BLUE_VALUE] = blue ;
			}
			else
			{
				arrval[0] = make_rgbcolor(red, green, blue) ;
			}
		}
		else 
			err = TRUE ;
	}
	else 
		err = TRUE;

	return (err) ;
}
