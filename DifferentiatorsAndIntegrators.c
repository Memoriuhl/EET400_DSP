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
#include "xscutimer.h"

/************************** Private Timer  Constant Definitions *****************************/
#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_LOAD_VALUE	0xFFFFFFFF


//#define numcoeff 6
#define XADC_DEVICE_ID 		XPAR_XADCPS_0_DEVICE_ID
static XAdcPs XAdcInst; /* XADC driver instance */
XAdcPs *XAdcInstPtr = &XAdcInst;

/************************** GPIO Constant Definitions *****************************/

#define lds 0xF   /* Assumes bits 0-3 of GPIO0 is connected to LEDs  */
#define sws 0xF   /* Assumes bits 0-3 of GPIO0 is connected to SWs  */
#define ins2 0x03   /* Assumes bit 0-1 of GPIO is input  */
#define outs1 0x01   /* Assumes bit 0 of GPIO is output  */
#define outs16 0xFF   /* Assumes bits 0-15 of GPIO is output  */

/*
 * The following constant maps to the name of the hardware instances that
 * were created in the EDK XPS system.
 */
#define GPIO_LedSw_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
#define GPIO_Cntl_DEVICE_ID  XPAR_GPIO_1_DEVICE_ID
#define GPIO_Cntr_DA2_Dat_DEVICE_ID  XPAR_GPIO_2_DEVICE_ID
/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */

/*
 * The following constants is used to determine which channel of the GPIO is
 * used.
 */
#define CH1 1
#define CH2 2
/***************************End GPIO Constant Definitions***************************/
//void print(char *str);
/************************** GPIO Variable Definitions *****************************/
/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XGpio Gpio0; /* The Instance of the GPIO Driver */
XGpio Gpio1; /* The Instance of the GPIO Driver */
XGpio Gpio2; /* The Instance of the GPIO Driver */

/************************End GPIO Variable Definitions*****************************/

/************************** Private Timer Variable Definitions *****************************/
/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */

XScuTimer Timer; /* The Instance of the Private Timer */

/************************End Private Timer Variable Definitions*****************************/




int main() {
	u32 swNew, led, flag, bool_cal,neg;

float j[3] = {
			0,
			0,
			0
};

float out[3] = {
			0,
			0,
			0
};

	short int AUX00RawData;
	short int DtoAData;
	short int data_out;
	int Status, i = 0;
	float FloatAtoD;
	float Resolution;
	float rect_comp;
	float rect_out;
	XAdcPs_Config *ConfigPtr;

	init_platform();

	XScuTimer_Config *TimerConfigPtr; //Assign the Config Pointer to the Timer _Config
	XScuTimer *TimerInstancePtr = &Timer;  // points to Timer struct

	//Initialize the Scu Private Timer so that it is ready to use.
	TimerConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
	Status = XScuTimer_CfgInitialize(TimerInstancePtr, TimerConfigPtr,
			TimerConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XScuTimer_LoadTimer(TimerInstancePtr, TIMER_LOAD_VALUE);//Load the timer to the counter register.
	XScuTimer_EnableAutoReload(TimerInstancePtr); //Enable AutoReload so the timer can run multiple times, in our case, infinite.
	XScuTimer_Start(TimerInstancePtr); //Start the Timer
/*

	 * Initialize the GPIO drivers
	 */
	Status = XGpio_Initialize(&Gpio0, GPIO_LedSw_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XGpio_Initialize(&Gpio1, GPIO_Cntl_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XGpio_Initialize(&Gpio2, GPIO_Cntr_DA2_Dat_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*


	 * Set the direction for all signals to be inputs except the
	 * LED, counter out and DA2Data output
	 */
	XGpio_SetDataDirection(&Gpio0, CH1, ~lds);          //setup pins as leds out
	XGpio_SetDataDirection(&Gpio0, CH2, sws);        //setup pins as switches in
	XGpio_SetDataDirection(&Gpio1, CH1, ins2); //setup pin as DA2Done bit0, busy in bit1,
	XGpio_SetDataDirection(&Gpio1, CH2, ~outs1);     //setup pin as DA2Start out
	XGpio_SetDataDirection(&Gpio2, CH1, ~outs16);     //setup pins a DA2Data out
	XGpio_SetDataDirection(&Gpio2, CH2, ~outs16);    //setup pins as counter out

	//   print("Hello World\n\r");

	swNew = XGpio_DiscreteRead(&Gpio0, CH2);                     //read switches
	led = swNew;
	XGpio_DiscreteWrite(&Gpio0, CH1, led);              //write switches to leds
	/*
	 * Initialize the XAdc driver.
	 */
	ConfigPtr = XAdcPs_LookupConfig(XADC_DEVICE_ID);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}
	XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr, ConfigPtr->BaseAddress);

	XGpio_DiscreteWrite(&Gpio1, CH2, 0x00);             //DA2 Refcomp is stopped
	i = 0;

	Resolution = (float)(1.0 / 4096.0);

	while (1) {

		XGpio_DiscreteWrite(&Gpio2, CH2, 6250); //counter at 100Khz

		while ((XGpio_DiscreteRead(&Gpio1, CH1) & 0x02) == 0x02) {
			flag = 1; //wait until data valid and set flag
		} //end of while

		if (flag == 1) {
			AUX00RawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_AUX00_OFFSET); //read XAdc
			DtoAData = (AUX00RawData) >> 4; //move top 12-bits to right
			flag = 0; //reset flag

			/***************************************************************Calculations Started******************************************************/



			FloatAtoD = Resolution * (float)DtoAData;


			for (i = 2; i > 0; i--)
				j[i] = j[i - 1];
			j[0] = FloatAtoD;
		//	j[0] = DtoAData;
			if(j[0]>=0 && j[1]<0)
/*			 * Refresh the array with the newest sample point
			 * Clear the multiply-accumulate variable
*/			{
			bool_cal = 1;
			}

			if(bool_cal==1){
		// j[0] = FloatAtoD;
				neg=0;

			for (i = 2; i > 0; i--)
				out[i] = out[i - 1];

			rect_comp = (j[0]) + out[1];// adds new sample to last

			out[0] = rect_comp;// adds output back to array

/*			**************************************************************Calculations End****************************************************
*/
			rect_out = (rect_comp) / Resolution; //should shift 15 places for unity gain abd at the offset
		//  rect_out = rect_comp;
			data_out = (short int)rect_out;



			XGpio_DiscreteWrite(&Gpio2, CH1, data_out); //write DA data to DA

			XGpio_DiscreteWrite(&Gpio1, CH2, 0x01); //Toggle DA2 start input
			XGpio_DiscreteWrite(&Gpio1, CH2, 0x00);
			}
			if(j[0]<0 && j[1]>=0){
							out[0]=0;
							out[1]=0;
							out[2]=0;
							bool_cal = 0;
							neg=1;
						}
			if(neg==1){
				for (i = 2; i > 0; i--)
								out[i] = out[i - 1];

							rect_comp = (j[0]) + out[1];// adds new sample to last

							out[0] = rect_comp;// adds output back to array

				/*			**************************************************************Calculations End****************************************************
				*/
							rect_out = (rect_comp) / Resolution; //should shift 15 places for unity gain abd at the offset
						//  rect_out = rect_comp;
							data_out = (short int)rect_out;



							XGpio_DiscreteWrite(&Gpio2, CH1, data_out); //write DA data to DA

							XGpio_DiscreteWrite(&Gpio1, CH2, 0x01); //Toggle DA2 start input
							XGpio_DiscreteWrite(&Gpio1, CH2, 0x00);
							}

		} //end of if

	} //end of infinite while

	//following code never executed
	cleanup_platform();
	return 0;
} //end of main
