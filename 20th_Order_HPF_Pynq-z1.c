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

#define numcoeff 5

#define XADC_DEVICE_ID   XPAR_XADCPS_0_DEVICE_ID
static XAdcPs XAdcInst;      /* XADC driver instance */
XAdcPs *XAdcInstPtr = &XAdcInst;

/************************** GPIO Constant Definitions *****************************/

#define Pulse 0x1   /* Assumes bits 0-1 of GPIO0 is connected to LEDs  */
#define sws 0x3   /* Assumes bits 0-1 of GPIO0 is connected to SWs  */
#define ins2 0x03   /* Assumes bit 0-1 of GPIO is input  */
#define outs1 0x01   /* Assumes bit 0 of GPIO is output  */
#define outs16 0xFF   /* Assumes bits 0-15 of GPIO is output  */

/*
 * The following constant maps to the name of the hardware instances that
 * were created in the EDK XPS system.
 */

#define GPIO_PulseSw_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
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
short int fir_coeff[numcoeff]={//coefficients signed fixed points
 0x1999,0x1999,0x1999,0x1999,0x1999//5 point running avg filter
 /*0x0277,0xF32A,0x1D88,0x59AE,0x1D88*/

};                                   //each point 0.2d


int main()
{
    u32 sw,swNew,led,flag;
    short int /*double*/ j[numcoeff]={4095,
      2680,
      391,
      391,
      2680
};
    short int AUX00RawData;
    short int DtoAData;
    short int data_out;
    int Status,i = 0;
    int fir_mac;
    int fir_out;

    XAdcPs_Config *ConfigPtr;

    init_platform();

    /*
    * Initialize the GPIO drivers
    */
    Status = XGpio_Initialize(&Gpio0, GPIO_PulseSw_DEVICE_ID);
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
    XGpio_SetDataDirection(&Gpio0, CH1, ~Pulse);//setup pins as leds out
    XGpio_SetDataDirection(&Gpio0, CH2, sws);//setup pins as switches in
    XGpio_SetDataDirection(&Gpio1, CH1, ins2);//setup pin as DA2Done bit0, busy in bit1,
    XGpio_SetDataDirection(&Gpio1, CH2, ~outs1);//setup pin as DA2Start out
    XGpio_SetDataDirection(&Gpio2, CH1, ~outs16);//setup pins a DA2Data out
    XGpio_SetDataDirection(&Gpio2, CH2, ~outs16);//setup pins as counter out


    print("Hello World\n\r");

    swNew=XGpio_DiscreteRead(&Gpio0, CH2);//read switches

   // XGpio_DiscreteWrite(&Gpio0, CH1, led);//write switches to leds
    /*
     * Initialize the XAdc driver.
     */
    ConfigPtr = XAdcPs_LookupConfig(XADC_DEVICE_ID);
           if (ConfigPtr == NULL) {
              return XST_FAILURE;
           }
    XAdcPs_CfgInitialize(XAdcInstPtr, ConfigPtr,ConfigPtr->BaseAddress);

    XGpio_DiscreteWrite(&Gpio1, CH2, 0x00);//DA2 Refcomp is stopped
    i=0;

   while(1){
      swNew=XGpio_DiscreteRead(&Gpio0, CH2);
      XGpio_DiscreteWrite(&Gpio0, CH1, swNew);
      if (swNew != sw){
       sw=swNew;
      switch (sw)
      {
         case 0:
         XGpio_DiscreteWrite(&Gpio2, CH2, 12500); //counter at 16KHz
         break;
         case 1:
         XGpio_DiscreteWrite(&Gpio2, CH2, 12500);//counter at 8KHz
            break;
         case 2:
         XGpio_DiscreteWrite(&Gpio2, CH2, 2267);//counter at 88.22Khz
          break;
         case 3:
         XGpio_DiscreteWrite(&Gpio2, CH2, 1000);//counter at 200Khz
            break;
//          case 4:
//          XGpio_DiscreteWrite(&Gpio2, CH2, 500);//counter at 100Khz
//             break;
//          case 5:
//          XGpio_DiscreteWrite(&Gpio2, CH2, 200);//counter at 250Khz, max fs allowable because of code overhead
//           break;
         default:
         XGpio_DiscreteWrite(&Gpio2, CH2, 12500);//default counter at 8Khz

      } //end of case
      } //end of if


    while((XGpio_DiscreteRead(&Gpio1, CH1) & 0x02)==0x02){
     flag=1; //wait until data valid and set flag
    }//end of while

if (flag==1){
 AUX00RawData = XAdcPs_GetAdcData(XAdcInstPtr, XADCPS_AUX00_OFFSET);//read XAdc
 DtoAData=(AUX00RawData)>>4;//move top 12-bits to right
 flag=0;//reset flag

for(i=numcoeff-1; i>0; i--)
{
 j[i] = j[i-1]; //shift oldest data out to make room for new one
}

j[0] = DtoAData; // get new data
fir_mac = 0; //clear output for new data
XGpio_DiscreteWrite(&Gpio0, CH1, Pulse);//write switches to leds
for(i=0; i<numcoeff; i++)
{
 fir_mac = fir_mac + fir_coeff[i]*(int)j[i]; //multiply coeffients with data and sum all for output
}

 //adjust binary point to convert back from fixed point with offset for DA2
XGpio_DiscreteWrite(&Gpio0, CH1, Pulse);
       fir_out = (fir_mac>>15)+2047;//should shift 15 places to adjust for data location

       data_out = (short int)fir_out;
       XGpio_DiscreteWrite(&Gpio2, CH1, data_out);//write DA data to DA
       XGpio_DiscreteWrite(&Gpio1, CH2, 0x01);//Toggle DA2 start input
       XGpio_DiscreteWrite(&Gpio1, CH2, 0x00);//

}//end of if

    }//end of infinite while

   //following code never executed
    cleanup_platform();
    return 0;
}//end of main
