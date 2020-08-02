/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"

#include "xparameters.h"
#include "xgpio.h"

#include "xadcps.h"
#include "xstatus.h"
#include "stdio.h"

/************************** Constant Definitions *****************************/

#define BitIO_0 0x01   /* Assumes bit 0 of GPIO 0 is an input, rest output  */
#define BitIO_1 0x00   /* Assumes bit 0-15 of GPIO 1 is an output  */

/*
 * The following constant maps to the name of the hardware instances that
 * were created in the EDK XPS system.
 */
#define GPIO_DEVICE_ID0  XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_DEVICE_ID1  XPAR_AXI_GPIO_1_DEVICE_ID
#define XADC_DEVICE_ID 	XPAR_XADCPS_0_DEVICE_ID

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define GPIO_CHANNEL1 1
#define GPIO_CHANNEL2 2

/**************************** End of Definitions *******************************/

/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XGpio Gpio0,Gpio1; /* The Instance of the GPIO Driver */
static XAdcPs XAdcInst;      /* XADC driver instance */
XAdcPs_Config *ConfigPtr;

/*****************************************************************************/

//void print(char *str);

int main()
{
    init_platform();
    XAdcPs *XAdcInstPtr = &XAdcInst;
    int Status,i,Flag;
    short int AUX08RawData;
    short int AUX00RawData;
    short int AtoD08Data[100];
    short int AtoD00Data[100];

    print("Hello World\n\r");

	/*
	 * Initialize the GPIO drivers
	 */
	Status = XGpio_Initialize(&Gpio0, GPIO_DEVICE_ID0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XGpio_Initialize(&Gpio1, GPIO_DEVICE_ID1);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

    /*
    	 * Set the direction for signal to be input
    	 *
    	 */
    XGpio_SetDataDirection(&Gpio0, GPIO_CHANNEL1, BitIO_0);//Ch1, bit 0 is input
    XGpio_SetDataDirection(&Gpio0, GPIO_CHANNEL2, ~BitIO_0);//Ch2, bit 1 is output
    XGpio_SetDataDirection(&Gpio1, GPIO_CHANNEL1, BitIO_1);//Ch1, bits 0-15 output
    XGpio_SetDataDirection(&Gpio1, GPIO_CHANNEL2, BitIO_1);//Ch2, bits 0-15 output

	/*
	 * Initialize the XAdc driver.
	 */
	ConfigPtr = XAdcPs_LookupConfig(XADC_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr,
				ConfigPtr->BaseAddress);

	/*
	 * Self Test the XADC/ADC device
	 */
	Status = XAdcPs_SelfTest(XAdcInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//XAdcPs_SetSequencerMode(XAdcInstPtr, XADCPS_SEQ_MODE_SIMUL_SAMPLING);
i=0;Flag=0;
    while (1) {
    		/*
    		 * Read the Busy signal, wait for it to be a 1
    		 *
    		 */
    		while ((XGpio_DiscreteRead(&Gpio0, GPIO_CHANNEL1)&0x01)==0x01){
    			Flag=1;
    		}
    		if (Flag==1){
    	    	AUX00RawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_AUX00_OFFSET);
    	    	AUX08RawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_AUX08_OFFSET);

    			AtoD00Data[i]=AUX00RawData >> 4;
    			AtoD08Data[i]=AUX08RawData >> 4;
    			XGpio_DiscreteWrite(&Gpio1, GPIO_CHANNEL1,AtoD00Data[i]);//write to channel 1 of DA2
    			XGpio_DiscreteWrite(&Gpio1, GPIO_CHANNEL2,AtoD08Data[i]);//write to channel 2 of DA2
    			XGpio_DiscreteWrite(&Gpio0, GPIO_CHANNEL2,0x01);//toggle start bit of DA2 RefComp
    			XGpio_DiscreteWrite(&Gpio0, GPIO_CHANNEL2,0x00);//
    			Flag = 0;
    		}

    }
    cleanup_platform();
    return 0;
}
