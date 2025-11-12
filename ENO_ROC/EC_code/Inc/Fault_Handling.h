#ifndef Fault_Handling_H
#define Fault_Handling_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define MAX_FAULTS 20

//void IO_Mapping_Update(void);
typedef enum {
    NO_FAULT,		//0
    ALERT,			//1
    WARNING,		//2
	SHUTDOWN		//3
} Lamp_t;

typedef enum {
    EMPTY,			//0
    OVERSPEED,		//1
    UNDERSPEED,		//2
	OVERCURRENT,	//3
	LOW_OIL_PRESS,	//4
	HIGH_TEMP,		//5
	EXT_SHDN,		//6
} error_t;


// Fault structure
typedef struct {
    uint16_t error_code;
    uint8_t lamp;
    //uint8_t source;    // Which output/component
} fault_t;

extern fault_t fault_array[MAX_FAULTS];
extern uint8_t fault_count;

extern void Fault_Handling(void);
void Raise_DM(uint8_t lamp, uint8_t error_code);
extern void ack_DM(void);

#ifdef __cplusplus
}
#endif

#endif

