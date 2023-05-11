#include "tm4c123gh6pm.h"
#include "TM4C123GH6PM.h"
#include "bitwise_operation.h"
void car_init(void)
{
    SYSCTL_RCGCGPIO_R |= 0X0b;              // open clock for Port A,B,D
    while((SYSCTL_PRGPIO_R & 0X0b)==0);     // checks that Port A,B,D is opened
		//D0 == (Passeneger side) Passenger window up automatic
		//D1 == (Passeneger side) Passenger window down automatic
		//D2 == (Driver     side) Passenger window up automatic
		//D3 == (Driver     side) Passenger window down automatic
		//D6 == the jam flag, if the window finds an obstacle while it is going up.
		//D7 == ON/OFF switch, if it is closed the passeneger side doesn't work while the driver is not affected by this button
		GPIO_PORTD_LOCK_R |= 0x4C4F434B;
    GPIO_PORTD_CR_R   |= 0Xcf;     // open 6 pins   0,1,2,3,6,7
    GPIO_PORTD_DIR_R  |= 0X00;     // input  6 pins buttons
		GPIO_PORTD_PDR_R  |= 0xcf;     // pull Down for external pushButtons
		GPIO_PORTD_DEN_R  |= 0Xcf;     // Enable Push Buttons
		GPIO_PORTD_DATA_R  |= 0X00;    // Initialize
		
		//B0 == Motorin the up direction
		//B1 == Motorin the down direction
		//B3 == limit switch at the top the window
		//B4 == limit switch at the bottom of the window
	  GPIO_PORTB_CR_R		|= 0X1b;      // open 4 pins  0,1,3,4
    GPIO_PORTB_DIR_R  |= 0X03;     	// 2 (3,4) inputs / 2 (0,1) output buttons
		GPIO_PORTB_PDR_R  |= 0x18;			// pull Down for external Limit switches
    GPIO_PORTB_DEN_R	|= 0X1b;			// Enable Port B
	
		//A2 == (Passeneger side) Passenger window up Manual
		//A3 == (Passeneger side) Passenger window down Manual
		//A4 == (Driver     side) Passenger window up Manual
		//A5 == (Driver     side) Passenger window down Manual
		GPIO_PORTA_CR_R		|= 0X3c;      // open 4 pins  2,3,4,5
    GPIO_PORTA_DIR_R  |= 0X00;     	// 4 inputs
		GPIO_PORTA_PDR_R  |= 0x3c;			// pull Down for external pushButtons
    GPIO_PORTA_DEN_R	|= 0X3c;			// Enable Port A
	
}
void DIO_WritePort (unsigned long volatile * Port ,unsigned char value)
{
  *Port = value;
}
void DIO_WritePin(unsigned long  volatile* Port , int Pin, unsigned char value)
{
  if(value == 0)
  {
    clearBit( *Port,  Pin);
  }
  else if(value == 1)
  {
    setBit( *Port,  Pin);
  }
}
char ReadPin(unsigned long  volatile* Port, int Pin)
{
    return getBit(*Port ,Pin);
}
unsigned long ReadPort(unsigned long  volatile* Port)
{
  return *Port;
}
void delay(int timer)
{
  int count = 0;
  while(count <= timer)
  {
    count++;
  }
}
void toggle(unsigned long  volatile* Port, int Pin)
{
  if(ReadPin(Port, Pin) == 1)
  {
    clearBit( *Port,  Pin);
  }
  else if(ReadPin(Port, Pin) == 0)
  {
    setBit( *Port,  Pin);
  } 
}