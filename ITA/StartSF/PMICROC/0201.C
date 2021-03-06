// Animate Dosing Page 2

#include <microc.h>
#include <sf.h>
#include "pmicroc\general.h"

static int valsilo[3], load[3], maxsilo[3] ;
static int init = FALSE ;

main(int ident, int channel, int write, int car, long address, int numval, int arrval[])
{
	int err, silo = address ;

	if (!init)
		initialize_data_page() ;
	if (!write) 
	{
		if (silo >= 1 && silo <= 3)
			err = read_data(car, silo, arrval) ;
		else
			err = TRUE ;
	}
	else
		err = write_data(car, silo, arrval[0]) ;

	return (err) ;
}

int read_data(int car, int silo, int val[])
{
	int err = FALSE, index = silo - 1 ;

	if (car == 83) // S
		val[0] = manage_level_silo(silo) ;
	else if (car == 80) // P
		val[0] = (valsilo[index] * 100L) / maxsilo[index] ;
	else if (car == 67) // C
		val[0] = load[index] ;
	else if (car == 69) // E
		val[0] = !load[index] ;
	else
		err = 2 ;

	return (err) ;
}

int manage_level_silo(int silo)
{
	int index = silo - 1 ;

	if (load[index]) 
	{
		valsilo[index] += 12 ;
		if (valsilo[index] >= maxsilo[index])
			load[index] = 0 ;
	}
	else 
	{
		valsilo[index] -= 12 ;
		if (valsilo[index] <= 0)
			load[index] = 1 ;
	}

	return (valsilo[index]) ;
}

int write_data(int car, int ind, int val)
{
	if (car == 73) // I
		initialize_data_page() ;

	return (0) ;
}

initialize_data_page()
{
	load[2]  = 1 ;
	valsilo[2]  = 100 ;
	maxsilo[2] = 1000 ;
	init = 1 ;
}
