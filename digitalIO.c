#include <stdio.h>
#include <unistd.h>
#include <ftdi.h>
#include <stdint.h>
#include <string.h>

struct ftdi_context ftdic;
typedef enum ftdi_interface ftdi_interface;
 
typedef struct _Inputoptions {
	uint8_t inputByte;
	ftdi_interface interface;
}Inputoptions;

void Usleep(int uSec)
{
	volatile uint32_t i =0;
	for(;i<uSec*100000;i++);
}

int OutputByte(uint8_t output_b)
{
    int result=0; // returned error code
    ftdi_write_data(&ftdic, &output_b, 1);
    return result;
}

uint8_t InputByte()
{
    uint8_t buf;
    ftdi_read_pins(&ftdic,&buf);
    //printf("%2x\t",buf);
    return buf;
}

int initializeHardware(ftdi_interface interface)
{
    int f;

    if (ftdi_init(&ftdic) < 0)
    {
        fprintf(stderr, "ftdi_init failed\n");
        return EXIT_FAILURE;
    }

    f = ftdi_usb_open(&ftdic, 0x0403, 0x6014);
    if (f < 0 && f != -5)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(&ftdic));
        exit(-1);
    }
	
    f = ftdi_set_interface(&ftdic,interface);
    printf("Setting interface: %d\n", f);
    ftdi_set_bitmode(&ftdic, 0xfb, BITMODE_BITBANG);
    //OutputByte(0x01);
    return 0;
}

int freeHardwareResources()
{
    ftdi_disable_bitbang(&ftdic);
    ftdi_usb_close(&ftdic);
    ftdi_deinit(&ftdic);
    return 0;
}

void print_usage()
{
	printf("--------------Usage------------\n");
	printf("digitalIO -options\n");
	printf("Options: -i -o -f\n");
	printf("-------------------------------\n");
}

Inputoptions parseArguments(int argc, char* argv[])
{
	int flag;
	Inputoptions Inflags;
	Inflags.interface = 0;
	while ((flag = getopt(argc, argv,"i:f:")) != -1) {
		switch (flag) {
             case 'i' : Inflags.inputByte =(uint8_t)atoi(optarg);
                 break;
             //case 'o' : continue;
             //    break;
             case 'f' : 
						if(strlen(optarg)>1)
							continue;
						Inflags.interface = optarg[0] - 'A' + 1; 
                 break;
             default: print_usage(); 
                 exit(EXIT_FAILURE);
			 }
   }
   return Inflags;
}

int main(int argc, char* argv[])
{
	Inputoptions inputs;
	inputs = parseArguments(argc, argv);
	printf("InputOptions:%x\t%d\n",inputs.inputByte,inputs.interface);
	initializeHardware(inputs.interface);
	OutputByte(inputs.inputByte);
	Usleep(100000);
	printf("Freeing HW resources");
	freeHardwareResources();
	return 0;
}
