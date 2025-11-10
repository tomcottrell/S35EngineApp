#include "PDM.h"
#include "Fault_Handling.h"
#include "Engine_App.h"
#include "frequency.h"

#define NO_FAULT 0
#define ALERT 1
#define WARNING 2
#define SHUTDOWN 3

#define OVERSPEED 1
#define UNDERSPEED 2

#define MAX_FAULTS 20
fault_t fault_array[MAX_FAULTS];
uint8_t fault_count = 0;

void Fault_Handling()
{
	// OVERSPEED
	if(frequency > 2000 && (engine_state == RUNNING))
	{
		engine_action = STOP;;
		Raise_DM(SHUTDOWN,OVERSPEED);
	}
	if(frequency < 1000 && (engine_state == RUNNING))
		{
			engine_action = STOP;;
			Raise_DM(SHUTDOWN,UNDERSPEED);
		}
}

void Raise_DM(uint8_t lamp, uint8_t error_code)
{
	// Push - Add a fault to the array
    if (fault_count < MAX_FAULTS) {
        fault_array[fault_count].error_code = error_code;
        fault_array[fault_count].lamp = lamp;
        //fault_array[fault_count].source = source;
        fault_count++;
    }
}

//void ack_DM(uint8_t error_code) can do this way when we want to remove a specific dm
void ack_DM()
{
	uint8_t error_code = 0;
	if (fault_count == 0) return;  // Guard against empty array

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
}
