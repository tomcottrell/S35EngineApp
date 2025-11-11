#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "PDM.h"
#include "CAN.h"
#include "TickTimers.h"
#include "MiscFunctions.h"
#include "cmsis_os.h"
#include "main.h"

// PDM Output Data
pdm_output_t pdm_outputs[12] = {0};


// PDM System Data
uint16_t pdm_battery_voltage = 0;
uint8_t pdm_sensor_supply_voltage = 0;
uint16_t pdm_software_version = 0;

pdm_state_t pdm_state = PDM_STATE_CONFIGURING;

//private variables
static uint32_t pdm_last_command_time = 0;
static uint8_t command_flag  = 0;
bool pdm_fault_flag = 0;

//PDM Input Data
uint16_t pdm_analog_inputs[8] = {0};
uint16_t pdm_digital_inputs[12] = {0};
// ============================================================================
// Defines for First 12 x messages to configure output function
// ============================================================================
// ============================================================================
// BYTE 0: Configuration Identifier (Always 0x00 for configure output)
// ============================================================================
#define CONFIG_ID_OUTPUT    0x00
// ============================================================================
// BYTE 1: Output Channel Selection
// ============================================================================
#define CHANNEL_1           1
#define CHANNEL_2           2
#define CHANNEL_3           3
#define CHANNEL_4           4
#define CHANNEL_5           5
#define CHANNEL_6           6
#define CHANNEL_7           7
#define CHANNEL_8           8
#define CHANNEL_9           9
#define CHANNEL_10          10
#define CHANNEL_11          11
#define CHANNEL_12          12
// ============================================================================
// BYTE 2: Soft-Start Step Size (0-255)
// ============================================================================
// Controls how quickly output ramps up
// Lower = smoother/slower, Higher = faster/more aggressive
#define SOFTSTART_INSTANT   255  // No soft-start, immediate full power
#define SOFTSTART_VERY_FAST 100  // Very quick ramp
#define SOFTSTART_FAST      50   // Fast ramp
#define SOFTSTART_MEDIUM    25   // Moderate ramp (default)
#define SOFTSTART_SLOW      10   // Slow, gentle ramp
#define SOFTSTART_VERY_SLOW 5    // Very slow, smooth ramp
#define SOFTSTART_OFF       1    // Minimal/no soft-start
// ============================================================================
// BYTE 3: Motor/Lamp Mode
// ============================================================================
#define MODE_LAMP           0    // Resistive load (lights, heaters)
#define MODE_MOTOR          1    // Inductive load (motors, solenoids)
// ============================================================================
// BYTE 4: Loss of Communication Action
// ============================================================================
// What happens if CAN bus communication is lost
#define LOC_HOLD_POSITION   0    // 00 = CH Unchanged (Last Commanded)
#define LOC_NEGATIVE_100    1    // 01 = CH -100% (H-Bridge Only)
#define LOC_POSITIVE_100    2    // 10 = CH +100%
#define LOC_TURN_OFF        3    // 11 = CH 0% (off)
// ============================================================================
// BYTE 5: Reserved (Always 0xFF unless manufacturer specifies otherwise)
// ============================================================================
#define RESERVED_BYTE       0xFF
// ============================================================================
// BYTE 6: POR, Command Response, Braking Configuration
// ============================================================================
// --- Bits 8-4: POR Command (5 bits) - Power level at startup ---
#define POR_CMD_0_PERCENT     (0b00000 << 3)   // 0% power on reset
#define POR_CMD_25_PERCENT    (0b00100 << 3)   // 25% power on reset
#define POR_CMD_50_PERCENT    (0b01000 << 3)   // 50% power on reset
#define POR_CMD_100_PERCENT   (0b01111 << 3)   // 100% power on reset

// --- Bit 3: POR Enable ---
#define POR_ENABLE_YES        (0 << 2)         // 0 = POR Enabled
#define POR_ENABLE_NO         (1 << 2)         // 1 = POR Disabled

// --- Bit 2: Command Type (Loss of Communication Enable/Disable) ---
#define CMD_TYPE_LOC_ENABLED  (0 << 1)         // 0 = LOC feature enabled
#define CMD_TYPE_LOC_DISABLED (1 << 1)         // 1 = LOC feature disabled

// --- Bit 1: Motor Braking ---
#define BRAKE_OFF             (0 << 0)         // 0 = No braking
#define BRAKE_ON              (1 << 0)         // 1 = Braking enabled
// ============================================================================
// BYTE 7: Load Sense Configuration (LSC) and Response
// ============================================================================
// --- Bits 7-4 (position 8.5): LSC Digital Input # (4 bits) ---
#define LSC_INPUT_1         (0 << 4)   // Link to Digital Input 1
#define LSC_INPUT_2         (1 << 4)   // Link to Digital Input 2
#define LSC_INPUT_3         (2 << 4)   // Link to Digital Input 3
#define LSC_INPUT_4         (3 << 4)   // Link to Digital Input 4
#define LSC_INPUT_5         (4 << 4)   // Link to Digital Input 5
#define LSC_INPUT_6         (5 << 4)   // Link to Digital Input 6
#define LSC_INPUT_7         (6 << 4)   // Link to Digital Input 7
#define LSC_INPUT_8         (7 << 4)   // Link to Digital Input 8
#define LSC_INPUT_9         (8 << 4)   // Link to Digital Input 9
#define LSC_INPUT_10        (9 << 4)   // Link to Digital Input 10
#define LSC_INPUT_11        (10 << 4)  // Link to Digital Input 11
#define LSC_INPUT_12        (11 << 4)  // Link to Digital Input 12
#define LSC_INPUT_DISABLED  (15 << 4)  // 0xF - LSC feature disabled

// --- Bits 3-2 (position 8.3): Calibration Time (2 bits) - Must be 0b11 (unsupported) ---
#define CAL_TIME_UNSUPPORTED (0b11 << 2)

// --- Bits 2-1: Response (2 bits) ---
#define RESPONSE_RESERVED     0b00     // 00 = Reserved
#define RESPONSE_ACTIVE_LOW   0b01     // 01 = Active Low
#define RESPONSE_ACTIVE_HIGH  0b10     // 10 = Active High
#define RESPONSE_ACTIVE_BOTH  0b11     // 11 = Active Low or High

// Default: LSC disabled (all bits set to 1)
#define LSC_DEFAULT         0xFF
// ============================================================================
// defines for configuration CH X
// These messages configure: Current Limit, Feedback Type, Automatic Reset, HS
// ============================================================================
// ============================================================================
// CONFIGURE OUTPUTS 1-6 and 7-12 Messages (Message IDs 0x06 and 0x07)
// ============================================================================
// ============================================================================
// BYTE 0: Message Identifier
// ============================================================================
#define CONFIG_MSG_OUTPUTS_1_6    0x06  // Configure outputs 1-6
#define CONFIG_MSG_OUTPUTS_7_12   0x07  // Configure outputs 7-12
// ============================================================================
// BYTES 1-6: Output Configuration (one byte per output)
// ============================================================================
// Each output byte is split into bit fields:
// Bits 7-4: Current Limit
// Bit 3: Feedback Type
// Bit 2: Automatic Reset Enable
// Bit 1: High Side (HS) Enable
// Bit 0: Reserved
// --- Bits 7-4: Current Limit Settings ---
// --- Bits 7-4: Current Limit Settings (2.5A increments) ---
#define CURRENT_LIMIT_0A      (0x0 << 4)   // 0.0 Amps (OFF)
#define CURRENT_LIMIT_2_5A    (0x1 << 4)   // 2.5 Amps
#define CURRENT_LIMIT_5A      (0x2 << 4)   // 5.0 Amps
#define CURRENT_LIMIT_7_5A    (0x3 << 4)   // 7.5 Amps
#define CURRENT_LIMIT_10A     (0x4 << 4)   // 10.0 Amps
#define CURRENT_LIMIT_12_5A   (0x5 << 4)   // 12.5 Amps
#define CURRENT_LIMIT_15A     (0x6 << 4)   // 15.0 Amps
// --- Bit 3: Feedback Type ---
#define FEEDBACK_CURRENT      (0b11 << 2)  // 11 = Current (ONLY supported mode)
// --- Bit 2: Automatic Reset (position 2.2) ---
#define AUTO_RESET_ENABLED    (0 << 1)     // 0 = Auto reset (5 attempts)
#define AUTO_RESET_DISABLED   (1 << 1)     // 1 = No auto reset
// --- Bit 1: High Side (HS) Enable ---
#define HS_DISABLED           (0 << 0)     // 0 = Low-side switching
#define HS_ENABLED            (1 << 0)     // 1 = High-side switching
// --- Bit 0: Reserved ---
#define OUTPUT_RESERVED       0            // Reserved bit, set to 0
// ============================================================================
// PDM Transmit rate and Enable/disable outputs
// ============================================================================
// ============================================================================
// BYTE 7: Response Configuration
// ============================================================================
// Bits 7-6: Response Rate
#define RESPONSE_RATE_500MS   (0b00 << 6)  // 500ms response rate
#define RESPONSE_RATE_250MS   (0b01 << 6)  // 250ms response rate
#define RESPONSE_RATE_50MS    (0b10 << 6)  // 50ms response rate
#define RESPONSE_RATE_10MS    (0b11 << 6)  // 10ms response rate
// Bits 5-0: Output Enable Mask (bit per output)
// For outputs 1-6: bit 0=output 1, bit 1=output 2, etc.
// For outputs 7-12: bit 0=output 7, bit 1=output 8, etc.
#define OUTPUT_1_ENABLE       (1 << 0)
#define OUTPUT_2_ENABLE       (1 << 1)
#define OUTPUT_3_ENABLE       (1 << 2)
#define OUTPUT_4_ENABLE       (1 << 3)
#define OUTPUT_5_ENABLE       (1 << 4)
#define OUTPUT_6_ENABLE       (1 << 5)
#define OUTPUT_7_ENABLE       (1 << 0)  // Same bit positions for 7-12
#define OUTPUT_8_ENABLE       (1 << 1)
#define OUTPUT_9_ENABLE       (1 << 2)
#define OUTPUT_10_ENABLE      (1 << 3)
#define OUTPUT_11_ENABLE      (1 << 4)
#define OUTPUT_12_ENABLE      (1 << 5)
#define ALL_OUTPUTS_ENABLED   0x3F         // Enable all 6 outputs (bits 0-5)
#define ALL_OUTPUTS_DISABLED  0x00         // Disable all outputs

// Output diagnostic to call externally
// 1 = Enabled, 0 = Disabled
// Index 0-11 corresponds to Outputs 1-12
uint8_t pdm_output_enable[12] = {
    1,  // Output 1
    1,  // Output 2
    1,  // Output 3
    1,  // Output 4
    1,  // Output 5
    1,  // Output 6
    1,  // Output 7
    1,  // Output 8
    1,  // Output 9
    1,  // Output 10
    1,  // Output 11
    1   // Output 12
};
// ============================================================================
// COMMAND OUTPUTS Messages (Message IDs 0x04 and 0x05)
// ============================================================================
// These messages command output power levels
// ============================================================================
// BYTES 1-6: Output Power Command (one byte per output)
// ============================================================================
// Each byte represents the output power level:
// 0 = OFF (0%)
// 127 = Full power (100%)
// Values 0-127 for proportional control
#define OUTPUT_OFF              0     // Output completely off
#define OUTPUT_10_PERCENT       13    // ~10% power
#define OUTPUT_25_PERCENT       32    // ~25% power
#define OUTPUT_50_PERCENT       64    // 50% power
#define OUTPUT_75_PERCENT       95    // ~75% power
#define OUTPUT_100_PERCENT      127   // Full 100% power
// ============================================================================
// Define output on start up
// ============================================================================
uint8_t pdm_output_commands[12] = {
	OUTPUT_100_PERCENT,    		// Output 1
	OUTPUT_100_PERCENT,    		// Output 2
	OUTPUT_100_PERCENT, 		// Output 3
	OUTPUT_100_PERCENT,    		// Output 4
	OUTPUT_100_PERCENT,    		// Output 5
	OUTPUT_100_PERCENT,    		// Output 6
	OUTPUT_100_PERCENT,  		// Output 7
	OUTPUT_100_PERCENT,    		// Output 8
	OUTPUT_100_PERCENT,    		// Output 9
	OUTPUT_100_PERCENT,    		// Output 10
	OUTPUT_100_PERCENT,  		// Output 11
	OUTPUT_100_PERCENT			// Output 12
};
// ============================================================================
// Helper Macros for Bit Manipulation
// ============================================================================
#define BUILD_BYTE6(por_cmd, por_enable, cmd_type, brake)((por_cmd) | (por_enable) | (cmd_type) | (brake))
#define BUILD_BYTE7(lsc_input, cal_time, response)((lsc_input) | (cal_time) | (response))
// Build output configuration byte
#define BUILD_OUTPUT_CONFIG(current, feedback, auto_reset, hs)((current) | (feedback) | (auto_reset) | (hs) | OUTPUT_RESERVED)
// Build byte 7 with response rate and output enable mask
#define BUILD_RESPONSE_BYTE(rate, enable_mask)((rate) | (enable_mask))
uint8_t output_enable_1_6(void);
uint8_t output_enable_7_12(void);
// ============================================================================
// Set the output function for each output
// ============================================================================
// Configure Output 1
// ============================================================================
void configureOutput_1(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_1;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = BUILD_BYTE7(LSC_INPUT_DISABLED, CAL_TIME_UNSUPPORTED, RESPONSE_ACTIVE_BOTH);

    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 2
// ============================================================================
void configureOutput_2(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_2;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 3
// ============================================================================
void configureOutput_3(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_3;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 4
// ============================================================================
void configureOutput_4(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_4;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 5
// ============================================================================
void configureOutput_5(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_5;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 6
// ============================================================================
void configureOutput_6(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_6;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 7
// ============================================================================
void configureOutput_7(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_7;
    data[2] = SOFTSTART_VERY_FAST;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 8
// ============================================================================
void configureOutput_8(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_8;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 9
// ============================================================================
void configureOutput_9(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_9;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 10
// ============================================================================
void configureOutput_10(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_10;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 11
// ============================================================================
void configureOutput_11(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_11;
    data[2] = SOFTSTART_SLOW;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
// ============================================================================
// Configure Output 12
// ============================================================================
void configureOutput_12(uint8_t *data) {
    data[0] = CONFIG_ID_OUTPUT;
    data[1] = CHANNEL_12;
    data[2] = SOFTSTART_MEDIUM;
    data[3] = MODE_MOTOR;
    data[4] = (0xFC | LOC_TURN_OFF);  // Upper 6 bits reserved (111111xx);
    data[5] = RESERVED_BYTE;
    // Byte 6: Start OFF, no response, no braking
    data[6] = BUILD_BYTE6(POR_CMD_0_PERCENT, POR_ENABLE_NO, CMD_TYPE_LOC_DISABLED, BRAKE_OFF);
    data[7] = LSC_DEFAULT;
    //send configure message
    sendoutput(data);
}
//===============================================================================
// ============================================================================
// USE DEFINES FROM ABOVE TO SET Current Limit, Feedback Type, Automatic Reset, HS
// ============================================================================
void send_configure_outputs_1_6 ()
{
	uint8_t data[8];

    data[0] = 0x06;                        // Configuration identifier
    data[1] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[2] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[3] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[4] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[5] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[6] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[7] = 0xFF;

    // Send configuration message
    sendoutput(data);
}

void send_configure_outputs_7_12 ()
{
	uint8_t data[8];

    data[0] = 0x07;                        // Configuration identifier
    data[1] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED); //output 7
    data[2] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[3] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[4] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[5] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[6] = BUILD_OUTPUT_CONFIG(CURRENT_LIMIT_2_5A, FEEDBACK_CURRENT, AUTO_RESET_DISABLED, HS_DISABLED);
    data[7] = 0xFF;

    // Send configuration message
    sendoutput(data);
}
// ============================================================================
// Adjust response rate in Byte 7
// ============================================================================
void send_command_outputs_1_6(void)
{
	uint8_t data[8];

    data[0] = 4;      // MUST be 4 for command outputs 1-6
    data[1] = pdm_output_commands[0];    // Output 1:
    data[2] = pdm_output_commands[1];    // Output 2:
    data[3] = pdm_output_commands[2];    // Output 3:
    data[4] = pdm_output_commands[3];    // Output 4:
    data[5] = pdm_output_commands[4];    // Output 5:
    data[6] = pdm_output_commands[5];    // Output 6:
    data[7] = BUILD_RESPONSE_BYTE(RESPONSE_RATE_50MS, output_enable_1_6());   // All enabled + 50ms rate

    // Send configuration message
    sendoutput(data);
}

void send_command_outputs_7_12(void)
{
	uint8_t data[8];
    data[0] = 5;      // MUST be 4 for command outputs 1-6
    data[1] = pdm_output_commands[6];    // Output 7
    data[2] = pdm_output_commands[7];    // Output 8
    data[3] = pdm_output_commands[8];    // Output 9
    data[4] = pdm_output_commands[9];    // Output 10
    data[5] = pdm_output_commands[10];   // Output 11
    data[6] = pdm_output_commands[11];   // Output 12
    data[7] = BUILD_RESPONSE_BYTE(RESPONSE_RATE_50MS, output_enable_7_12());;   // All enabled + 50ms rate

    // Send configuration message
    sendoutput(data);
}
// ============================================================================
// PDM operation function - should not need to touch
// ============================================================================
void PDM_comms()
{
	uint32_t current_time = HAL_GetTick(); //will this have issues with size??

	switch(pdm_state) {
		case PDM_STATE_CONFIGURING:
			configure_pdm();
			pdm_last_command_time = HAL_GetTick();
			pdm_state = PDM_STATE_RUNNING;
		break;

		case PDM_STATE_RUNNING:
			if((current_time - pdm_last_command_time) >= 100) {
				if(command_flag == 0)
				{
					send_command_outputs_1_6();
					command_flag = 1;
				}
				else
				{
					send_command_outputs_7_12();
					command_flag = 0;
				}
				pdm_last_command_time = current_time;
			}

		for(int i=0;i <= 11;i++)
			{
			Fault_Handler(pdm_outputs[i].diagnostics,i);
			}
		break;

		case PDM_STATE_ERROR:

		break;
	}

}
// ============================================================================
// reading PDM messages, called in CAN.c
// ============================================================================
void PDM_ProcessCANMessage(uint32_t arbid, uint8_t *data, uint8_t length){


    // All PDM feedback messages use data[0] as the message type identifier
    uint8_t message_id = data[0];

    switch(message_id) {
        case 128:  // Analog Inputs 1-2, Digital Inputs
            // Process digital inputs (bytes 1-3)
            pdm_digital_inputs[0] = (data[1] >> 0) & 0x03;  // DI1
            pdm_digital_inputs[1] = (data[1] >> 2) & 0x03;  // DI2
            pdm_digital_inputs[2] = (data[1] >> 4) & 0x03;  // DI3
            pdm_digital_inputs[3] = (data[1] >> 6) & 0x03;  // DI4
            pdm_digital_inputs[4] = (data[2] >> 0) & 0x03;  // DI5
            pdm_digital_inputs[5] = (data[2] >> 2) & 0x03;  // DI6
            pdm_digital_inputs[6] = (data[2] >> 4) & 0x03;  // DI7
            pdm_digital_inputs[7] = (data[2] >> 6) & 0x03;  // DI8
            pdm_digital_inputs[8] = (data[3] >> 0) & 0x03;  // DI9
            pdm_digital_inputs[9] = (data[3] >> 2) & 0x03;  // DI10
            pdm_digital_inputs[10] = (data[3] >> 4) & 0x03; // DI11
            pdm_digital_inputs[11] = (data[3] >> 6) & 0x03; // DI12

            // Process analog inputs 1-2 (bytes 4-7)
            pdm_analog_inputs[0] = (data[5] << 8) | data[4]; // AI1
            pdm_analog_inputs[1] = (data[7] << 8) | data[6]; // AI2
            break;

        case 129:  // Analog Inputs 3-4, Output Diagnostics
            // Process output diagnostics (bytes 1-3)
            // Process output diagnostics (bytes 2-4 in message = data[1]-data[3])
            // Each byte contains 4 diagnostic values (2 bits each)
            // FIXED: Bit order is MSB to LSB (bit 8 down to bit 1)
            pdm_outputs[0].diagnostics = (data[1] >> 6) & 0x03;  // Output 1 - bits 7-8 of byte 2
            pdm_outputs[1].diagnostics = (data[1] >> 4) & 0x03;  // Output 2 - bits 5-6 of byte 2
            pdm_outputs[2].diagnostics = (data[1] >> 2) & 0x03;  // Output 3 - bits 3-4 of byte 2
            pdm_outputs[3].diagnostics = (data[1] >> 0) & 0x03;  // Output 4 - bits 1-2 of byte 2

            pdm_outputs[4].diagnostics = (data[2] >> 6) & 0x03;  // Output 5 - bits 7-8 of byte 3
            pdm_outputs[5].diagnostics = (data[2] >> 4) & 0x03;  // Output 6 - bits 5-6 of byte 3
            pdm_outputs[6].diagnostics = (data[2] >> 2) & 0x03;  // Output 7 - bits 3-4 of byte 3
            pdm_outputs[7].diagnostics = (data[2] >> 0) & 0x03;  // Output 8 - bits 1-2 of byte 3

            pdm_outputs[8].diagnostics = (data[3] >> 6) & 0x03;  // Output 9 - bits 7-8 of byte 4
            pdm_outputs[9].diagnostics = (data[3] >> 4) & 0x03;  // Output 10 - bits 5-6 of byte 4
            pdm_outputs[10].diagnostics = (data[3] >> 2) & 0x03; // Output 11 - bits 3-4 of byte 4
            pdm_outputs[11].diagnostics = (data[3] >> 0) & 0x03; // Output 12 - bits 1-2 of byte 4

            // Keep your analog input parsing exactly as it is (already working correctly)
            pdm_analog_inputs[2] = (data[5] << 8) | data[4]; // AI3
            pdm_analog_inputs[3] = (data[7] << 8) | data[6]; // AI4
            break;

        case 130:  // Analog Inputs 5-6, Battery and Sensor Supply
            // Process supply voltages
            pdm_sensor_supply_voltage = data[1]; // Sensor supply status
            pdm_battery_voltage = (data[3] << 8) | data[2]; // Battery voltage

            // Process analog inputs 5-6 (bytes 4-7)
            pdm_analog_inputs[4] = (data[5] << 8) | data[4]; // AI5
            pdm_analog_inputs[5] = (data[7] << 8) | data[6]; // AI6
            break;

        case 131:  // Analog Inputs 7-8, Software Version, Power Supply
            // Process power supply status and software version
            pdm_software_version = (data[3] << 8) | data[2]; // Software version

            // Process analog inputs 7-8 (bytes 4-7)
            pdm_analog_inputs[6] = (data[5] << 8) | data[4]; // AI7
            pdm_analog_inputs[7] = (data[7] << 8) | data[6]; // AI8
            break;

        case 132:  // Outputs 1-6 Feedback
            // Process outputs 1-6 feedback
            for(int i = 0; i < 6; i++) {
                pdm_outputs[i].feedback = data[i + 1];
            }
            break;

        case 133:  // Outputs 7-12 Feedback
            // Process outputs 7-12 feedback
            for(int i = 0; i < 6; i++) {
                pdm_outputs[i + 6].feedback = data[i + 1];
            }
            break;

        case 134:  // Output Function Handshake
            // Configuration handshake received
            //if(pdm_state == PDM_STATE_CONFIGURING) {
            //    uint8_t output_channel = data[1];
            //    if(output_channel >= 1 && output_channel <= 12) {
            //       pdm_outputs[output_channel - 1].configured = true;
            //        pdm_config_retries = 0;
            //
            //        // Move to next output if not done
            //        if(output_channel < 12) {
            //            pdm_current_config_output = output_channel + 1;
            //            pdm_config_timeout = GetTickCount() + PDM_CONFIG_TIMEOUT_MS;
            //            PDM_ConfigureOutput(pdm_current_config_output);
            //        }
            //    }
            //}
            //break;

        case 135:  // Output Configuration Handshake Channels 1-6
            // Group configuration handshake for outputs 1-6
            break;

        case 136:  // Output Configuration Handshake Channels 7-12
            // Group configuration handshake for outputs 7-12
            break;
    }
}

// ============================================================================
// Configure PDM to the above settings
// ============================================================================
void configure_pdm() //using osDelay feels fine here, need to change to tick for general running
{
	uint8_t data[8];
	configureOutput_1(data);
	osDelay(100);
	configureOutput_2(data);
	osDelay(100);
	configureOutput_3(data);
	osDelay(100);
	configureOutput_4(data);
	osDelay(100);
	configureOutput_5(data);
	osDelay(100);
	configureOutput_6(data);
	osDelay(100);
	configureOutput_7(data);
	osDelay(100);
	configureOutput_8(data);
	osDelay(100);
	configureOutput_9(data);
	osDelay(100);
	configureOutput_10(data);
	osDelay(100);
	configureOutput_11(data);
	osDelay(100);
	configureOutput_12(data);
	osDelay(100);
	send_configure_outputs_1_6 ();
	osDelay(100);
	send_configure_outputs_7_12 ();
	osDelay(100);
}


// ============================================================================
// CAN TX function
// ============================================================================
void sendoutput(uint8_t *data) {
    // Send configuration message
    CAN_tx_header.ExtId = 0x18EF1E11; //SA 30
    CAN_tx_header.IDE = CAN_ID_EXT;
    CAN_tx_header.RTR = CAN_RTR_DATA;
    CAN_tx_header.DLC = 8;

    memcpy(can_tx_data, data, 8);

    HAL_CAN_AddTxMessage(&hcan1, &CAN_tx_header, can_tx_data, &CAN_tx_mailbox);
}

// ============================================================================
// Output Enable function
// ============================================================================
uint8_t output_enable_1_6(void) {
    uint8_t mask = 0;
    mask |= (pdm_output_enable[0] << 0);  // Output 1
    mask |= (pdm_output_enable[1] << 1);  // Output 2
    mask |= (pdm_output_enable[2] << 2);  // Output 3
    mask |= (pdm_output_enable[3] << 3);  // Output 4
    mask |= (pdm_output_enable[4] << 4);  // Output 5
    mask |= (pdm_output_enable[5] << 5);  // Output 6
    return mask;
}

uint8_t output_enable_7_12(void) {
    uint8_t mask = 0;
    mask |= (pdm_output_enable[6] << 0);   // Output 7
    mask |= (pdm_output_enable[7] << 1);   // Output 8
    mask |= (pdm_output_enable[8] << 2);   // Output 9
    mask |= (pdm_output_enable[9] << 3);   // Output 10
    mask |= (pdm_output_enable[10] << 4);  // Output 11
    mask |= (pdm_output_enable[11] << 5);  // Output 12
    return mask;
}

// ============================================================================
// Diagnostic Checking Function
// ============================================================================
void Fault_Handler(uint8_t diag,uint8_t output)
{
	if(diag == 2)
	{
		//pdm_output_commands[output] = 0;
		pdm_output_enable[output] = 0;
		pdm_fault_flag = 1;
	}
}
