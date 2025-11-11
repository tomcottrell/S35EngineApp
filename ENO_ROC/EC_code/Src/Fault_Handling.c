#include "PDM.h"
#include "Fault_Handling.h"
#include "Engine_App.h"
#include "frequency.h"
#include "main.h"

#define NO_FAULT 0
#define ALERT 1
#define WARNING 2
#define SHUTDOWN 3

#define EMPTY 0
#define OVERSPEED 1
#define UNDERSPEED 2

#define MAX_FAULTS 20

fault_t fault_array[MAX_FAULTS];
uint8_t fault_count = 0;
static uint32_t Underspeed_Bubble = 0;
static bool Underspeed_Triggered = false;
//static uint32_t Overspeed_Bubble = 0;
//static bool Overspeed_Triggered = false;

void Fault_Handling()
{
	// OVERSPEED
	if(frequency > 2500 && (engine_state == RUNNING))
	{
		//if(!Overspeed_Triggered)
		//{
		//	Overspeed_Bubble = HAL_GetTick();
		//	Overspeed_Triggered = true;
		//}
		//else if(HAL_GetTick() - Overspeed_Bubble > 1000){ //1 second
			engine_action = STOP;
			Raise_DM(SHUTDOWN,OVERSPEED);
		//	Overspeed_Bubble = 0;
		//}
	}
	//else
	//{
	//	Overspeed_Triggered = false;
	//}

	// UNDERSPEED
	if(frequency < 500 && (engine_state == RUNNING))
	{
		if(!Underspeed_Triggered)
		{
			Underspeed_Bubble = HAL_GetTick();
			Underspeed_Triggered = true;
		}
		else if(HAL_GetTick() - Underspeed_Bubble > 1000)	//1 second
		{
			engine_action = STOP;
			Raise_DM(SHUTDOWN,UNDERSPEED);
			Underspeed_Bubble = 0;
		}
	}
	else
	{
		Underspeed_Triggered = false;
	}
}

void Raise_DM(uint8_t lamp, uint8_t error_code)
{
	// Push - Add a fault to the array
	if(lamp == 0 && error_code == 0)
	{
		if (fault_count < MAX_FAULTS) {
		        fault_array[fault_count].error_code = error_code;
		        fault_array[fault_count].lamp = lamp;
		}
	}
	else
	{
		if (fault_count < MAX_FAULTS) {
			fault_array[fault_count].error_code = error_code;
			fault_array[fault_count].lamp = lamp;
        //fault_array[fault_count].source = source;
			fault_count++;
		}
	}
}

//void ack_DM(uint8_t error_code) can do this way when we want to remove a specific dm
void ack_DM()
{
	uint8_t error_code = 0;
	error_code =  fault_array[0].error_code;
	// Pop - Remove a specific fault when acknowledged
	for (uint8_t i = 0; i < fault_count; i++) {
	        if (fault_array[i].error_code == error_code) {
	            // Shift all faults down to fill the gap
	            for (uint8_t j = i; j < fault_count - 1; j++) {
	                fault_array[j] = fault_array[j + 1];
	            }
	            fault_count--;
	            break;
	        }
	    }
	if (fault_count == 0)
		{
			Raise_DM(NO_FAULT,EMPTY);
			return;  // Guard against empty array
		}
}
