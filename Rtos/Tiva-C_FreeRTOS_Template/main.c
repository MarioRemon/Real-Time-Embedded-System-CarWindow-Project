#include <stdint.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "TM4C123GH6PM.h"
#include "DIO.h"
#include "semphr.h"

xQueueHandle xQueueUpAuto;
xQueueHandle xQueueUpManual;

xQueueHandle xQueueDownAuto;
xQueueHandle xQueueDownManual;

xQueueHandle xQueueDriverUpAuto;
xQueueHandle xQueueDriverUpManual;

xQueueHandle xQueueDriverDownAuto;
xQueueHandle xQueueDriverDownManual;


portBASE_TYPE xStatusReceiveUpAuto;
portBASE_TYPE xStatusReceiveUpManual;
	
portBASE_TYPE xStatusSendDown;

portBASE_TYPE xStatusReceiveDown;

xSemaphoreHandle xBinarySemaphore;

portBASE_TYPE xStatusSendUp;

uint8_t isJam = 0;
int super = 0;

uint8_t isAuto;

void passUpAuto()
{
	
	while(1)
	{
		// Works when the queue has value from the car handler as the automatic push up button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveUpAuto = xQueueReceive(xQueueUpAuto,&isAuto ,portMAX_DELAY);  
		//portMAX_DELAY is used to ensure that the queue is unlocked only when a value is sent to it only
			super = 1;
			//Motor turn up
			GPIO_PORTB_DATA_R = 0x01;
			// while the limit switch is not pressed (the window lmit) or no other function is needed to be done then keep window going up
			while(((GPIO_PORTB_DATA_R & 0x08) != 0x08) && super == 1)
			{
				vTaskDelay(400/portTICK_PERIOD_MS);
			}
			//stop the window from going up
			GPIO_PORTB_DATA_R &= 0xfe;
		taskYIELD();
	}
	
}

void passUpManual()
{
	while(1)
	{
		// Works when the queue has value from the car handler as the Manual push up button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveUpManual = xQueueReceive(xQueueUpManual,&isAuto ,portMAX_DELAY);	
		//Motor turn up
		GPIO_PORTB_DATA_R = 0x01;
		// while the limit switch is not pressed (the window lmit) and the push up manual is still pressed then keep window going up
		while(((GPIO_PORTB_DATA_R & 0x08) != 0x08 && (GPIO_PORTA_DATA_R & 0x04) == 0x04))
		{
				vTaskDelay(500/portTICK_PERIOD_MS);
		}
		//stop the window from going up
		GPIO_PORTB_DATA_R &= 0xfe;
	}
		taskYIELD();
}

void passDown()
{
	while(1)
	{
		portBASE_TYPE xStatusReceiveDown;
		// Works when the queue has value from the car handler as the Automatic push down button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveDown = xQueueReceive(xQueueDownAuto,&isAuto ,portMAX_DELAY);
			super = 1;
		//Motor turn down
			GPIO_PORTB_DATA_R = 0x02;
		// while the limit switch is not pressed (the window lmit) or no other function is needed to be done then keep window going down	
		while(((GPIO_PORTB_DATA_R & 0x10) != 0x10) && super == 1)
			{
				vTaskDelay(500/portTICK_PERIOD_MS);
			}
			//stop the window from going down
			GPIO_PORTB_DATA_R &= 0xfd;
		taskYIELD();
	}
}

void passDownManual()
{
	while(1)
	{
		portBASE_TYPE xStatusReceiveDown;
		// Works when the queue has value from the car handler as the Manual push down button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveDown = xQueueReceive(xQueueDownManual,&isAuto ,portMAX_DELAY);
		//Motor turn down
		GPIO_PORTB_DATA_R = 0x02;
		// while the limit switch is not pressed (the window lmit) and the push up manual is still pressed then keep window going down
		while(((GPIO_PORTB_DATA_R & 0x10) != 0x10) && (ReadPin(&GPIO_PORTA_DATA_R, 3)  == 1))
		{
				vTaskDelay(500/portTICK_PERIOD_MS);
		}
		//stop the window from going down
		GPIO_PORTB_DATA_R &= 0xfd;
		taskYIELD();
	}
}

/*Task for the Driver to get the window up*/
void driverUp()
{
	while(1)
	{
		portBASE_TYPE xStatusReceiveUp;
		// Works when the queue has value from the car handler as the automatic push up button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveUp = xQueueReceive(xQueueDriverUpAuto,&isAuto ,portMAX_DELAY);
		
			super = 1;
			//Motor turn up
			GPIO_PORTB_DATA_R = 0x01;
		// while the limit switch is not pressed (the window lmit) or no other function is needed to be done then keep window going up
			while(((GPIO_PORTB_DATA_R & 0x08) != 0x08) && super == 1)
			{
				vTaskDelay(400/portTICK_PERIOD_MS);
			}
			//stop the window from going up
			GPIO_PORTB_DATA_R &= 0xfe;
			taskYIELD();
	}
	
}

void driverUpManual()
{
	while(1)
	{
		portBASE_TYPE xStatusReceiveUp;
		// Works when the queue has value from the car handler as the Manual push up button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveUp = xQueueReceive(xQueueDriverUpManual,&isAuto ,portMAX_DELAY);
		//Motor turn up
		GPIO_PORTB_DATA_R = 0x01;
		// while the limit switch is not pressed (the window lmit) and the push up manual is still pressed then keep window going down
		while(((GPIO_PORTB_DATA_R & 0x08) != 0x08)&& ReadPin(&GPIO_PORTA_DATA_R, 4)  == 1)
		{
				vTaskDelay(500/portTICK_PERIOD_MS);
		}
		//stop the window from going up
		GPIO_PORTB_DATA_R &= 0xfe;
		taskYIELD();
	}
	
}

void driverDown()
{
	while(1)
	{
		portBASE_TYPE xStatusReceiveDown;
		// Works when the queue has value from the car handler as the Automatic push down button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveDown = xQueueReceive(xQueueDriverDownAuto, &isAuto ,portMAX_DELAY);
			super = 1;
			//Motor turn down
			GPIO_PORTB_DATA_R = 0x02;
			// while the limit switch is not pressed (the window lmit) or no other function is needed to be done then keep window going down
			while(((GPIO_PORTB_DATA_R & 0x10) != 0x10) && super == 1)
			{
				vTaskDelay(500/portTICK_PERIOD_MS);
			}
			//stop the window from going down
			GPIO_PORTB_DATA_R &= 0xfd;
		}
		taskYIELD();
}

void driverDownManual()
{
	while(1)
	{
		portBASE_TYPE xStatusReceiveDown;
		// Works when the queue has value from the car handler as the Manual push down button is pressed 
		//it sends value to the queue, and it is no more blocked
		xStatusReceiveDown = xQueueReceive(xQueueDriverDownManual, &isAuto ,portMAX_DELAY);
		//Motor turn down
		GPIO_PORTB_DATA_R = 0x02;
		// while the limit switch is not pressed (the window lmit) and the push up manual is still pressed then keep window going down
		while(((GPIO_PORTB_DATA_R & 0x10) != 0x10) && (ReadPin(&GPIO_PORTA_DATA_R, 5)  == 1))
			{
					vTaskDelay(500/portTICK_PERIOD_MS);
			}
			//stop the window from going down
			GPIO_PORTB_DATA_R &= 0xfd;
		taskYIELD();
	}
}

void jamProtection()
{
	xSemaphoreTake(xBinarySemaphore,0);
	for(;;)
	{
		//semaphore binary
		xSemaphoreTake(xBinarySemaphore,portMAX_DELAY);
		//stop the motor from going up
		GPIO_PORTB_DATA_R &= 0xfe;
		//turn the motor to go down for 0.5 seconds
		GPIO_PORTB_DATA_R = 0x02;
		vTaskDelay(500/portTICK_PERIOD_MS);
		//stop the motor from going down
		GPIO_PORTB_DATA_R &= 0xfd;
		taskYIELD();
	}
}

void carHandler()
{
	uint8_t isAuto = 0;
	while(1)
	{
				//Passenger up Auto and the On/Off switch is On only
				if(((GPIO_PORTD_DATA_R & 0x01) == 0x01) && (ReadPin(&GPIO_PORTD_DATA_R, 7)  == 0))
				{		
					super = 0;					
					xStatusSendUp = xQueueSendToBack(xQueueUpAuto, &isAuto, 100);
				}
				//Passeneger up Manual and the On/Off switch is On only
				else if(((GPIO_PORTA_DATA_R & 0x04) == 0x04) && (ReadPin(&GPIO_PORTD_DATA_R, 7)  == 0))
				{	
					super = 0;
					xStatusSendUp = xQueueSendToBack(xQueueUpManual, &isAuto, 100);
				}
				//passeneger down Auto and the On/Off switch is On only
				else if((ReadPin(&GPIO_PORTD_DATA_R, 1)  == 1) && (ReadPin(&GPIO_PORTD_DATA_R, 7)  == 0))
				{
					super = 0;
					portBASE_TYPE xStatusSendDown;
					xStatusSendDown = xQueueSendToBack(xQueueDownAuto, &isAuto, 100);
				}
				//passenger down Manual and the On/Off switch is On only
				else if((ReadPin(&GPIO_PORTA_DATA_R, 3)  == 1) && (ReadPin(&GPIO_PORTD_DATA_R, 7)  == 0))
				{
					super = 0;
					portBASE_TYPE xStatusSendDown;
					xStatusSendDown = xQueueSendToBack(xQueueDownManual, &isAuto, 100);
				}
				// Driver to get the passeneger window up
				else if(ReadPin(&GPIO_PORTD_DATA_R, 2)  == 1)
				{
					super = 0;
					portBASE_TYPE xStatusSendUp;
					xStatusSendUp = xQueueSendToBack(xQueueDriverUpAuto, &isAuto, 100);
				}
				// Driver to get the passeneger window up
				else if(ReadPin(&GPIO_PORTA_DATA_R, 4)  == 1)
				{
					super = 0;
					portBASE_TYPE xStatusSendUp;
					xStatusSendUp = xQueueSendToBack(xQueueDriverUpManual, &isAuto, 100);
				}
				// Driver to get the passeneger window down
				else if(ReadPin(&GPIO_PORTD_DATA_R, 3)  == 1)
				{
					super = 0;
					portBASE_TYPE xStatusSendDown;
					xStatusSendDown = xQueueSendToBack(xQueueDriverDownAuto, &isAuto, 100);
				}
				// Driver to get the passeneger window down
				else if(ReadPin(&GPIO_PORTA_DATA_R, 5)  == 1)
				{
					super = 0;
					portBASE_TYPE xStatusSendDown;
					xStatusSendDown = xQueueSendToBack(xQueueDriverDownManual, &isAuto, 100);
				}
				// the jam is only recognized when the window is going up
				else if((ReadPin(&GPIO_PORTD_DATA_R, 6)  == 1) && (ReadPin(&GPIO_PORTB_DATA_R, 0)  == 1))
				{
					// to close all the running actions on the window
					super = 0;
					// close the motor
					GPIO_PORTB_DATA_R &= 0xfc;
					portBASE_TYPE xHigherPriorityTaskWoken = pdTRUE;
					xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken); //, &xHigherPriorityTaskWoken);		
				}
	}
}

int main()
{
	car_init();
	vSemaphoreCreateBinary(xBinarySemaphore);
	
	xQueueUpAuto 		= xQueueCreate(10, sizeof(int));
	xQueueUpManual 		= xQueueCreate(10, sizeof(int));
	
	xQueueDownAuto 	= xQueueCreate(10, sizeof(int));
	xQueueDownManual 	= xQueueCreate(10, sizeof(int)); 	
	
	xQueueDriverUpAuto 		= xQueueCreate(10, sizeof(int));
	xQueueDriverUpManual 		= xQueueCreate(10, sizeof(int));
	
	xQueueDriverDownAuto 	= xQueueCreate(10, sizeof(int));
	xQueueDriverDownManual 	= xQueueCreate(10, sizeof(int)); 	
	
	if(xBinarySemaphore != NULL)
	{
		
		xTaskCreate(carHandler,
			"Car Handler",
			128,
			NULL,
			1,
			NULL);
		
			xTaskCreate(driverUp,
				"Driver Up Button",
				32,
				NULL,
				4,
				NULL);
		
		xTaskCreate(driverUpManual,
				"Driver Up Button",
				32,
				NULL,
				4,
				NULL);
		
			xTaskCreate(driverDown,
				"Driver Down Button",
				64,
				NULL,
				4,
				NULL);
			
		xTaskCreate(driverDownManual,
				"Driver Down Button",
				64,
				NULL,
				4,
				NULL);			
		
		xTaskCreate(passUpAuto,
				"Passeneger Up Button",
				32,
				NULL,
				2,
				NULL);
		
		xTaskCreate(passUpManual,
				"Passeneger Up Button Manual",
				32,
				NULL,
				2,
				NULL);
				
		xTaskCreate(passDown,
				"Passeneger Down Button",
				32,
				NULL,
				2,
				NULL);
		xTaskCreate(passDownManual,
				"Passeneger Down Button",
				32,
				NULL,
				2,
				NULL);
				
		xTaskCreate(jamProtection,
				"Jam Protection",
				32,
				NULL,
				3,
				NULL);
	}	
	
	vTaskStartScheduler();
	
	for (;;);
	
}