#include "PDM.h"
#include "IO_Mapping.h"
#include "Engine_App.h"

extern bool Output1_control;
extern bool Output2_control;

//This is not running anywhere, either create a funciton or run on a tick
void IO_Mapping_Update()
{
// ============================================================================
// INPUTS
// ============================================================================
// PDM
LOW_OIL_PRESSURE = pdm_digital_inputs[0]; 	// DI1
HIGH_ENG_TEMP = pdm_digital_inputs[1]; 		// DI2
EXTERNAL_SHUTDOWN = pdm_digital_inputs[2]; 	// DI3
//pdm_digital_inputs[3]; // DI4
//pdm_digital_inputs[4]; // DI5
//pdm_digital_inputs[5]; // DI6
//pdm_digital_inputs[6]; // DI7
//pdm_digital_inputs[7]; // DI8
//pdm_digital_inputs[8]; // DI9
//pdm_digital_inputs[9]; // DI10
//pdm_digital_inputs[10];// DI11
//pdm_digital_inputs[11];// DI12

//pdm_analog_inputs[0]; //AI1
//pdm_analog_inputs[1]; //AI2
//pdm_analog_inputs[2]; //AI3
//pdm_analog_inputs[3]; //AI4
//pdm_analog_inputs[4]; //AI5
//pdm_analog_inputs[5]; //AI6
//pdm_analog_inputs[6]; //AI7
//pdm_analog_inputs[7]; //AI8
//
// S35


// ============================================================================
// OUTPUTS
// ============================================================================
// PDM
//pdm_output_commands[0]; //D01
//pdm_output_commands[1]; //D02
//pdm_output_commands[2]; //D03
//pdm_output_commands[3]; //D04
//pdm_output_commands[4]; //D05
//pdm_output_commands[5]; //D06
//pdm_output_commands[6] = FUEL; //D07
//pdm_output_commands[7]; //D08
//pdm_output_commands[8] = CRANK; //D09
//pdm_output_commands[9] = CRANK; //D010
//pdm_output_commands[10] = CRANK; //D011
//pdm_output_commands[11]; //D012

pdm_output_enable[0] = 0; 		//1
pdm_output_enable[1] = 0; 		//2
//pdm_output_enable[2]; 		//3
pdm_output_enable[3] = 0; 		//4
pdm_output_enable[4] = FUEL; 	//5
pdm_output_enable[5] = 0; 		//6
pdm_output_enable[6] = FUEL; 	//7
pdm_output_enable[7] = CRANK; 	//8
pdm_output_enable[8] = 0; 		//9
pdm_output_enable[9]= CRANK; 	//10
pdm_output_enable[10] = 0; 		//11
pdm_output_enable[11] = CRANK; 	//12

// S35
Output1_control = CRANK;
Output2_control = FUEL;
//
}
