#include "me_init.h"

/********************************************************************
 *             Global variables and function declarations           *
 ********************************************************************/

int gearValue, doorStatus, fuelLevel, engineCool, fuelLevelWarning, engWarning, lightControl =0, ecuAlive;
int gearValueShift[8] = {1,2,4,8,16,32,64,128};
int i = 1;
char errorMessage;

/* Switches and buttons variable to be used to receive signals from board */
int SW1 = 0;
int SW2 = 0;
int SW3 = 0;
int SW4= 0;
int BT5= 0;
int BT6= 0;




/********************************************************************
 *                         Application entry point                  *
 ********************************************************************/
int main(void) {

  /* Board and modules initialization */
		ME_Init();

		/* Timer configuration */
		PIT_ConfigureTimer(1,200);
		PIT_ConfigureTimer(2,400);
		PIT_StartTimer(1);
		PIT_StartTimer(2);

		/*U1 = 1;
		U2 = 0;
		U3 = 1;
		//P = 0;
		Tx=1;
		Rx = 0;*/
  /* Application main loop that runs forever*/
for ( ; ; ) {


   /* Operating System Delay*/
   osalThreadDelayMilliseconds(250UL);
 }
}



/********************************************************************
 *                   Can Reception Function                         *
 ********************************************************************/
void can_receive() {
	if(CAN_0.IFRL.B.BUF5I == 1)
	{
		switch(CAN_0.BUF[0].ID.B.STD_ID)
		{
			case 0x81:
				updateInputs();
			break;
			case 0x301:
				fuelLevel = CAN_0.BUF[0].DATA.B[1] << 8 | CAN_0.BUF[0].DATA.B[0];
				break;
			case 0x401:
				engineCool = CAN_0.BUF[0].DATA.B[1] << 8 | CAN_0.BUF[0].DATA.B[0];
				break;
			case 0x203:
			lightControl = 1;
				break;
			default :
				CAN_0.BUF[0].ID.B.STD_ID;
				break;
		}
	}
}

/********************************************************************
 *                   Interrupt Handlers for PIT Channel 1-3         *
 ********************************************************************/

/********************************************************************
 *                   Interrupt Handlers for PIT Channel 1-3         *
 ********************************************************************/
void Pit_Channel_1()
{
		U1=~U1;
		//gearValue
	  	if(BT5 == 0 && i < 7){
			i++;
			gearValue = gearValueShift[i];
		}
		if(BT6 == 0 && i > 0){
			i--;
			gearValue = gearValueShift[i];
		}
		CAN_0.BUF[8].DATA.B[0] = gearValue;
		CAN_0.BUF[8].CS.B.CODE = 12;     //Send data


		if(SW1 == 0 && SW2 == 0)
		            {
			doorStatus = 0;
		            }
		            else if(SW1 == 0 && SW2 == 1)
		            {
		            	doorStatus = 1;
		            }
		            else if(SW1 == 1 && SW2 == 0)
		            {
		            	doorStatus = 2;
		            }
		            else
		            {
		            	doorStatus = 3;
		            }
/*		if(SW1 == 0 && SW2 == 0){
			doorStatus = 0;
		}
		else if(SW1 == 1 && SW2 == 1){
					doorStatus = 1;
				}*/
		CAN_0.BUF[9].DATA.B[0] = doorStatus;
		CAN_0.BUF[9].CS.B.CODE = 12;


		if(lightControl == 1){
			P = 1;
		}
		else if(lightControl == 0){
			P = ~P;
			errorMessage = 'E';
			CAN_0.BUF[12].DATA.B[0] = errorMessage;
			CAN_0.BUF[12].CS.B.CODE = 12;
		}


		ecuAlive = 1;
		CAN_0.BUF[13].DATA.B[0] = ecuAlive;
	    CAN_0.BUF[13].CS.B.CODE = 12;

		PIT.CHANNEL[1].TFLG.B.TIF = 1;       //Clear Interrupt Flag
}

void Pit_Channel_2()
{
	if(fuelLevel == 102){
			fuelLevelWarning = 0;
		}
		else{
			fuelLevelWarning = 1;
		}

		CAN_0.BUF[10].DATA.B[0] = fuelLevelWarning;
		CAN_0.BUF[10].CS.B.CODE = 12;

		if(engineCool >= 922){
			engWarning = 1;
		}
		else{
			engWarning = 0;
		}

		CAN_0.BUF[11].DATA.B[0] = engWarning;
		CAN_0.BUF[11].CS.B.CODE = 12;

		PIT.CHANNEL[2].TFLG.B.TIF = 1;       //Clear Interrupt Flag
		PIT.CHANNEL[2].TFLG.R = 1;
}

void Pit_Channel_3()
{
PIT.CHANNEL[3].TFLG.R = 1;
}




/*************************************************************************
*                    	DONT CHANGE ANYTHING BELOW 	!!!                  *
*                                                                        *
**************************************************************************/

/*************************************************************************
*                   Interrupt Handlers for CAN Message Buffer            *
*                 Receive Interrupt for buffer 08-15 flag cleared        *
**************************************************************************/
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_08_11_HANDLER) {
	CAN_0.IFRL.B.BUF8I  = 1;
	CAN_0.IFRL.B.BUF9I  = 1;
	CAN_0.IFRL.B.BUF10I = 1;
	CAN_0.IFRL.B.BUF11I = 1;
}
IRQ_HANDLER(SPC5_FLEXCAN0_BUF_12_15_HANDLER) {
	CAN_0.IFRL.B.BUF12I = 1;
	CAN_0.IFRL.B.BUF13I = 1;
	CAN_0.IFRL.B.BUF14I = 1;
	CAN_0.IFRL.B.BUF15I = 1;
}

/*************************************************************************
*                  Function to receive the data of the display           *
**************************************************************************/
void updateInputs()
{
	SW1 = CAN_0.BUF[0].DATA.B[0];
	SW2 = CAN_0.BUF[0].DATA.B[1];
	SW3 = CAN_0.BUF[0].DATA.B[2];
	SW4 = CAN_0.BUF[0].DATA.B[3];
	BT5 = CAN_0.BUF[0].DATA.B[4];
	BT6 = CAN_0.BUF[0].DATA.B[5];

}






