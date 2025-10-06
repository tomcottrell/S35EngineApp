#ifndef PDM_H
#define PDM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// PDM States
typedef enum {
    PDM_STATE_CONFIGURING,
    PDM_STATE_RUNNING,
    PDM_STATE_ERROR
} pdm_state_t;

// PDM Output structure
typedef struct {
    uint8_t diagnostics;
    uint8_t feedback;
    bool configured;
} pdm_output_t;

// External variables for UI access
extern pdm_state_t pdm_state;
extern uint16_t pdm_analog_inputs[8];
extern uint16_t pdm_digital_inputs[12];
extern pdm_output_t pdm_outputs[12];
extern uint16_t pdm_battery_voltage;
extern uint8_t pdm_sensor_supply_voltage;
extern uint16_t pdm_software_version;
extern uint8_t pdm_output_commands[12];
extern uint8_t pdm_output_enable[12];



// Function declarations
void PDM_comms(void);
void PDM_ProcessCANMessage(uint32_t arbid, uint8_t *data, uint8_t length);
void configure_pdm(void);
void send_configure_output_function(uint8_t channel);
void send_configure_outputs_1_6(void);
void send_configure_outputs_7_12(void);
void send_command_outputs_1_6(void);
void send_command_outputs_7_12(void);
void Fault_Handler(uint8_t diag,uint8_t output);
void sendoutput(uint8_t *data);
uint8_t output_enable_1_6(void);
uint8_t output_enable_7_12(void);

#ifdef __cplusplus
}
#endif

#endif // PDM_H
