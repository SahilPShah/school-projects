//io_handler.c
#include "io_handler.h"
#include "alt_types.h"
#include "system.h"
#include <stdio.h>




void IO_init(void)
{
	*otg_hpi_reset = 1;
	*otg_hpi_cs = 1;
	*otg_hpi_r = 1;
	*otg_hpi_w = 1;
	*otg_hpi_address = 0;
	*otg_hpi_data = 0;
	// Reset OTG chip
	*otg_hpi_cs = 0;
	*otg_hpi_reset = 0;
	*otg_hpi_reset = 1;
	*otg_hpi_cs = 1;
}

void IO_write(alt_u8 Address, alt_u16 Data)
{
//*************************************************************************//
//									TASK								   //
//*************************************************************************//
//							Write this function							   //
//*************************************************************************//

	*otg_hpi_address = Address;			//Setting the address into the proper hpi register

	*otg_hpi_cs = 0;

	*otg_hpi_w = 0;
	*otg_hpi_data = Data;				//Writing the data into the previous selected register
	*otg_hpi_w = 1;

	*otg_hpi_cs = 1;


}

alt_u16 IO_read(alt_u8 Address)
{
	alt_u16 temp;

	*otg_hpi_address = Address;			//Setting the address into the proper hpi register

	*otg_hpi_cs = 0;

	*otg_hpi_r = 0;
	temp = *otg_hpi_data;				//Reading the data from the proper memory location. the location is automatically synchronized
	*otg_hpi_r = 1;

	*otg_hpi_cs = 1;

	//printf("%x\n",temp);				//printing the data register value to the console
	return temp;
}



