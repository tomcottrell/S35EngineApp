#include "PDM.h"
#include "IO_Mapping.h"
//#include "Engine_App

//This is not running anywhere, either create a funciton or run on a tick

// Link parameter name to PDM Digital/Analog Inputs
// e.g Oil_Pressure_Switch = pdm_digital_inputs[0] // DI1
// e.g pdm_digital_inputs[1] // DI2
// pdm_digital_inputs[2] // DI3
// pdm_digital_inputs[3] // DI4
// pdm_digital_inputs[4] // DI5
// pdm_digital_inputs[5] // DI6
// pdm_digital_inputs[6] // DI7
// pdm_digital_inputs[7] // DI8
// pdm_digital_inputs[8] // DI9
// pdm_digital_inputs[9] // DI10
// pdm_digital_inputs[10]// DI11
// pdm_digital_inputs[11]// DI12


//e.g Oil Pressure = pdm_analog_inputs[0] //AI1
// pdm_analog_inputs[1] //AI2
// pdm_analog_inputs[2] //AI3
// pdm_analog_inputs[3] //AI4
// pdm_analog_inputs[4] //AI5
// pdm_analog_inputs[5] //AI6
// pdm_analog_inputs[6] //AI7
// pdm_analog_inputs[7] //AI8

// Link parameter name to PDM Digital Inputs
// For example link Engine App Parameters to the PDM
//e.g pdm_output_commands[0] = Crank; //D01
//e.g pdm_output_commands[1] = Fuel; //D02
//pdm_output_commands[2]; //D03
//pdm_output_commands[3]; //D04
//pdm_output_commands[4]; //D05
//pdm_output_commands[5]; //D06
//pdm_output_commands[6]; //D07
//pdm_output_commands[7]; //D08
//pdm_output_commands[8]; //D09
//pdm_output_commands[9]; //D010
//pdm_output_commands[10]; //D011
//pdm_output_commands[11]; //D012
