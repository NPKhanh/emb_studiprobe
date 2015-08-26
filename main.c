#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <LPC23xx.h> 
//#include "CAN.h"
//#include "LPC2xxx_embedded.h"
#include "emb1_can_labs.h"

extern void UART_init (unsigned int, unsigned int, unsigned int, unsigned int);

//Senden von Text an UART
unsigned int PortNum, UDL, UART_mode, FIFO_mode, IntEnableValue;
extern void UART_PutChar (unsigned int PortNum, unsigned char Word);



//Empfangen von Text am Microcontroller
extern unsigned char UART_GetChar (int PortNum);
extern void init_VIC(void);

extern void CAN_Set_Acceoptance_Filter_Mode(uint32_t ACF_mode);

//CAN Einstellungen
CAN_MSG   message1 = {0x456, {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}, 8, 0,0};
CAN_MSG   My_CAN1_RX_message;
CAN_MSG   My_CAN2_RX_message;
CAN_MSG   received;
extern uint32_t CAN1_Counter_RX, CAN2_Counter_RX;
//ISRs stored receved messages temporarily in this vector before being read by the application
//Software
extern CAN_MSG  CAN1_RX_messages[CAN1_MAX_RX_MSG], CAN2_RX_messages[CAN2_MAX_RX_MSG];

int test = 0;


//char* SendingText = "01234567ABCDEFGHJ\n"; 


//char* send_whole_text (size_t UART_Num, int length, char* SendText)	//SendText zeigt auf SendingText (gewünschter Text)
//{
//	int i = 0;
//	for (i = 0; i < length; i++)
//	{
//		UART_PutChar ( (int) UART_Num, SendText[i]);
//	}
//	
//	return SendText + sizeof(char)*length;
//	
//}



char* send_char(char* val, uint8_t length, uint8_t id, uint8_t CAN_port)
{
	uint32_t test;
	
//	CAN_MSG* sending_msg;
//	
//	send_msg->data = val; 
//	send_msg->length = length; 
//	send_msg->id = id;
//	send_msg-> frame_format = 0x0; 
//	send_msg->frame_type = 0x0;
//	
//	test = CAN_Send_Message (CAN_port,send_msg);
	
	if (test == True)
	{
		return val+ sizeof(char)*length;
	}
	else
	{
		return NULL;
	}
}



void VIC_init (void)
{
	VICVectAddr7 = (unsigned long) send_char;
	VICVectPriority7 = 14;
	VICIntEnable |= 1 << 7;
}



char* read_whole_text( size_t UART_Num, int length, char* ReceivedText)
{
	int i = 0; 
	for (i = 0; i < length; i++)
	{
		ReceivedText[i] = UART_GetChar((int) UART_Num);
	}
	
	return ReceivedText + sizeof(char)*length;
	//Hier den interrupt starten für den Sendevorgang
}






void right_direct(size_t UART_NUM, int amount)
{
	int i = 0;
	if(UART_NUM == (size_t) 1)
	{
		for(; i < amount;i++)
		{
			U1RBR = 0x61 + i;
		}
	}
	else
	{
		for(; i < amount;i++)
		{
			U0RBR = 0x61 + i;
		}
	}
		
	
}



int main (void)
{
	char* S_TextPosition; 
	char* R_TextPosition;
	char TextReceived[8]; 
	unsigned char test;
	unsigned char bef = 0x808080;
	
	PortNum = 0;
	UDL = 0x8;
	UART_mode = 0x03;
	FIFO_mode = 0x87;
	IntEnableValue = 0x01;
	
	CAN_Init(0,0x3DC002); // Initialize CAN1 module e.g. Data rate value = 0x3DC002 @ 14,4 MHz 
	CAN_Init(1,0x3DC002); // Initialize CAN2 module e.g. Data rate value = 0x3DC002 @ 14,4 MHz 
	CAN_Set_Acceptance_Filter_Mode(ACCF_ON);
	
	UART_init (0, UDL, UART_mode, (IntEnableValue << 8)| FIFO_mode);
	UART_init (1, UDL, UART_mode, (IntEnableValue << 8)| FIFO_mode);
	VIC_init(); 

	
//	send_whole_text(0,1," ");
//	
//	S_TextPosition = send_whole_text(0,8,SendingText);
//	
//	send_whole_text(0,1," ");

//	S_TextPosition = send_whole_text(0,8,S_TextPosition);


	right_direct(1,8);

	R_TextPosition = read_whole_text (1,8,TextReceived);	// Just the position until where in String it got + receive the String send from UART0
	
	
	//read_whole_text(1,8,R_TextPosition);
	//send_whole_text(1,2,"YO");
  //send_whole_text(1,1,"\n");
	//send_whole_text(1,1,TextReceived);

	
	while (1)
	{
	}
	
}