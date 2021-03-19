 char interrupt_200 = 0;
char interrupt_400 = 0;

int gear[8] = {1,2,4,8,16,32,64,128};
int i = 1;

int door = 0b00000011;

void main(void)
{

    /* turn off leds */
    U0 = 1;
    U1 = 1;
    U2 = 1;
    U3 = 1;
    U4 = 1;
    U5 = 1;
    U6 = 1;
    U7 = 1; 

    PIT_ConfigureTimer(1, 200);
	PIT_ConfigureTimer(2, 400);
	
	/* start PIT timer for channel 1 and 2 */
	PIT_StartTimer(1);
	PIT_StartTimer(2);

    /* board initialization */
    for(){
        //GEAR
        if(interrupt_200) {
            if(BT5 == 0 && i <= 6){
              i++;
            }
            if(BT6 == 0 && i >= 0){
               i--;
            }
            CAN_0.BUF[8].DATA.B[0] = gear[i];		
        	CAN_0.BUF[8].CS.B.CODE = 12;

            //DOOR
            if(SW1 == 0 && SW2 == 0)
            {
                door = 0;
            }
            else if(SW1 == 0 && SW2 == 1)
            {
                door = 1;
            }
            else if(SW1 == 1 && SW2 == 0)
            {
                door = 2;
            }
            else
            {
                door = 3;
            }
            CAN_0.BUF[9].DATA.B[0] = door;		
            CAN_0.BUF[9].CS.B.CODE = 12;

            // send alive signal
            CAN_0.BUF[12].CS.B.CODE = 12;
			
			// send error signal
			if(light_control_is_alive == 0) {
                P = ~P;
				CAN_0.BUF[13].CS.B.CODE = 12;
			} else {
				light_control_is_alive = 0;
				P = 1;
			}

			interrupt_200 = 0;
		} 
		
       
        if(interrupt_400) {
            if(fuel_level <= 102)
				CAN_0.BUF[10].DATA.B[0] = 0;
			else
				CAN_0.BUF[10].DATA.B[0] = 1;	
			CAN_0.BUF[10].CS.B.CODE = 12;
			
			/* send coolant temp warning message */	
			if(coolant_temp >= 920)
				CAN_0.BUF[11].DATA.B[0] = 0x00;
			else
				CAN_0.BUF[11].DATA.B[0] = 0x01;
			CAN_0.BUF[11].CS.B.CODE = 12;
			
			interrupt_400 = 0;
            
        }
    }

void PIT_CHANNEL_1(void)
{
	// send gear data
   interrupt_200 = 1;	
    PIT.CH[1].TFLG.B.TIF = 1;
	
}

void PIT_CHANNEL_2(void)
{
	// send gear data
    interrupt_400 = 1;	
    PIT.CH[2].TFLG.B.TIF = 1;
}


	if(CAN_0.IFRL.B.BUF5I) {
		switch(CAN_0.RXFIFO.ID.B.STD_ID) {
			case 0x301:
				fuel_level = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				U0 = ~U0;
				break;
			case 0x401:
				coolant_temp = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				U1 = ~U1;
				break;
			case 0x203:
				light_control_is_alive = 1;
				U2 = ~U2;
				break;
		}
