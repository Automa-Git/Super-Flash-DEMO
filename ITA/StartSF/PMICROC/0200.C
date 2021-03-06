// Animate Page Dosing 1                                           									

#include <microc.h>
#include <sf.h>

static int valsilo[6], load[6], maxsilo[6] ;
static int init = 0 ;

main(int ident, int channel, int write, int car, long address, int numval, int arrval[])
{
	int err;
	long silo = address ;

	if (!init)
		initialize_data_page() ;
	if (!write)  
	{
		if (silo >= 1 && silo <= 6)
			err = read_data(car, silo, arrval) ;
		else
			err = 1 ;
	}
	else
		err = write_data(car, silo, arrval[0]) ;

	return (err) ;
}

int read_data(int car, long silo, int val[])
{
	int err = 0;
	long index = silo - 1 ;

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

int manage_level_silo(long silo)
{
	long index = silo - 1 ;

	if (load[index])  
	{
		if (silo == 1 || silo == 2 || silo == 3 || !load[0]) 
		{
			if (silo == 1)
				valsilo[index] += 25 ;
			else if (silo == 5)
				valsilo[index] += 3 ;
			else
				valsilo[index] += 10 ;
			
			if (valsilo[index] >= maxsilo[index])
				load[index] = 0 ;
		}
	}
	else
	{
		if ((silo == 1 && (load[3] || load[4] || load[5])) || silo != 1)  
		{
			if (silo == 1)
				valsilo[index] -= 4 ;
			else if (silo == 5)
				valsilo[index] -= 3 ;
			else
				valsilo[index] -= 10 ;
			
			if (valsilo[index] <= 0)
			{
				valsilo[index] = 0 ;
				load[index] = 1 ;
			}
		}
	}

	return (valsilo[index]) ;
}

int write_data(int car, long ind, int val)
{
	if (car == 73) // I
		initialize_data_page() ;

	return (0) ;
}

initialize_data_page()
{
	load[0] = load[1] = load[2] = 1 ;
	load[3] = load[4] = load[5] = 1 ;
	valsilo[0] = 10 ;
	valsilo[1] = 100 ;
	valsilo[2] = 54 ;
	valsilo[3] = 512 ;
	valsilo[4] = 48 ;
	valsilo[5] = 128 ;
	maxsilo[0] = 1000 ;
	maxsilo[4] = 128 ;
	maxsilo[1] = maxsilo[2] = 512 ;
	maxsilo[3] = maxsilo[5] = 800 ;
	init = 1 ;
}
