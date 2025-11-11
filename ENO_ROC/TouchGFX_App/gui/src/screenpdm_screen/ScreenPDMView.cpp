#include <gui/screenpdm_screen/ScreenPDMView.hpp>
#include <touchgfx/Unicode.hpp>
#include "PDM.h"

//extern uint8_t pdm_output_commands[12];

static uint8_t lastcall = 0;
static uint8_t lastcall1 = 0;
static uint8_t lastcall2 = 0;

ScreenPDMView::ScreenPDMView()
    : updateCounter(0)
{
}

void ScreenPDMView::setupScreen()
{
    ScreenPDMViewBase::setupScreen();
    updateCounter = 0;
}

void ScreenPDMView::tearDownScreen()
{
    ScreenPDMViewBase::tearDownScreen();
}

void ScreenPDMView::handleTickEvent()
{
    // Update display every 10 ticks (~100ms with 10ms tick rate)
    if(++updateCounter >= 10)
    {
        updateCounter = 0;

        updatePDMStatus();
        updateDigitalInputs();
        updateAnalogInputs();
        updateDigitalOutputs();
        // Refresh the screen
        invalidate();
    }
}

void ScreenPDMView::updatePDMStatus()
{
    // Update PDM state text
    const char* statusText;
    switch(pdm_state) {
        case PDM_STATE_CONFIGURING:
            statusText = "Configuring";
            break;
        case PDM_STATE_RUNNING:
            statusText = "Running";
            break;
        case PDM_STATE_ERROR:
            statusText = "Error";
            break;
        default:
            statusText = "Unknown";
            break;
    }

    Unicode::strncpy(textAreaPDMStatusBuffer, statusText, TEXTAREAPDMSTATUS_SIZE);
}

void ScreenPDMView::updateDigitalOutputs()
{
	//Diagnostics
	//00 No faults
	//01 Short-circuit
	//10 Over-current
	//11 Open-circuit
    // Digital Output 3 is at index 2 (outputs are 0-indexed in the array)
    // Get the commanded value (0-127 represents 0-100%)
    int do3ValueCmd = pdm_output_commands[2];
    // Convert to percentage: 127 = 100%, so percentage = (value * 100) / 127
    // Using 100.0f * value / 127.0f gives us the actual percentage
    int do3Percentage = (do3ValueCmd * 100) / 127;
    // Get feedback current (resolution is 0.125A per LSB according to manual)
    float do3Current = pdm_outputs[2].feedback * 0.125f;
    int do3Enable = pdm_output_enable[2];
    // Get diagnostic status
    int do3Diagnostic = pdm_outputs[2].diagnostics;

    // Update the text areas
    Unicode::snprintf(textAreaD03ValueBuffer, TEXTAREAD03VALUE_SIZE, "Value: %d%%", do3Percentage);

    Unicode::snprintfFloat(textAreaD03CurrBuffer, TEXTAREAD03CURR_SIZE, "%.2fA", do3Current);

    Unicode::snprintf(textAreaD03DiagBuffer, TEXTAREAD03DIAG_SIZE, "Diag %d", do3Diagnostic);
    Unicode::snprintf(textAreaD03EnableBuffer, TEXTAREAD03ENABLE_SIZE, "ON/OFF %d", do3Enable);

    //OUTPUT DIAGNOSTICS
    //VALUES
    Unicode::snprintf(textAreaDO1Buffer, TEXTAREADO1_SIZE, "DO01=%d%%,", (pdm_output_commands[0] * 100) / 127);
    Unicode::snprintf(textAreaDO2Buffer, TEXTAREADO2_SIZE, "DO02=%d%%,", (pdm_output_commands[1] * 100) / 127);
    Unicode::snprintf(textAreaDO3Buffer, TEXTAREADO3_SIZE, "DO03=%d%%,", (pdm_output_commands[2] * 100) / 127);
    Unicode::snprintf(textAreaDO4Buffer, TEXTAREADO4_SIZE, "DO04=%d%%,", (pdm_output_commands[3] * 100) / 127);
    Unicode::snprintf(textAreaDO5Buffer, TEXTAREADO5_SIZE, "DO05=%d%%,", (pdm_output_commands[4] * 100) / 127);
    Unicode::snprintf(textAreaDO6Buffer, TEXTAREADO6_SIZE, "DO06=%d%%,", (pdm_output_commands[5] * 100) / 127);
    Unicode::snprintf(textAreaDO7Buffer, TEXTAREADO7_SIZE, "DO07=%d%%,", (pdm_output_commands[6] * 100) / 127);
    Unicode::snprintf(textAreaDO8Buffer, TEXTAREADO8_SIZE, "DO08=%d%%,", (pdm_output_commands[7] * 100) / 127);
    Unicode::snprintf(textAreaDO9Buffer, TEXTAREADO9_SIZE, "DO09=%d%%,", (pdm_output_commands[8] * 100) / 127);
    Unicode::snprintf(textAreaDO10Buffer, TEXTAREADO10_SIZE, "DO10=%d%%,", (pdm_output_commands[9] * 100) / 127);
    Unicode::snprintf(textAreaDO11Buffer, TEXTAREADO11_SIZE, "DO11=%d%%,", (pdm_output_commands[10] * 100) / 127);
    Unicode::snprintf(textAreaDO12Buffer, TEXTAREADO12_SIZE, "DO12=%d%%,", (pdm_output_commands[11] * 100) / 127);

    //FEEDBACK AND ENABLED/DISABLED
    Unicode::snprintfFloat(diag1Buffer, DIAG1_SIZE, "%.2fA", pdm_outputs[0].feedback * 0.125f);
    Unicode::snprintf(Enable1Buffer, ENABLE1_SIZE, "%d", pdm_output_enable[0]);

    Unicode::snprintfFloat(diag2Buffer, DIAG2_SIZE, "%.2fA", pdm_outputs[1].feedback * 0.125f);
    Unicode::snprintf(Enable2Buffer, ENABLE2_SIZE, "%d", pdm_output_enable[1]);

    Unicode::snprintfFloat(diag3Buffer, DIAG3_SIZE, "%.2fA", pdm_outputs[2].feedback * 0.125f);
    Unicode::snprintf(Enable3Buffer, ENABLE3_SIZE, "%d", pdm_output_enable[2]);

    Unicode::snprintfFloat(diag4Buffer, DIAG4_SIZE, "%.2fA", pdm_outputs[3].feedback * 0.125f);
    Unicode::snprintf(Enable4Buffer, ENABLE4_SIZE, "%d", pdm_output_enable[3]);

    Unicode::snprintfFloat(diag5Buffer, DIAG5_SIZE, "%.2fA", pdm_outputs[4].feedback * 0.125f);
    Unicode::snprintf(Enable5Buffer, ENABLE5_SIZE, "%d", pdm_output_enable[4]);

    Unicode::snprintfFloat(diag6Buffer, DIAG6_SIZE, "%.2fA", pdm_outputs[5].feedback * 0.125f);
    Unicode::snprintf(Enable6Buffer, ENABLE6_SIZE, "%d", pdm_output_enable[5]);

    Unicode::snprintfFloat(diag7Buffer, DIAG7_SIZE, "%.2fA", pdm_outputs[6].feedback * 0.125f);
    Unicode::snprintf(Enable7Buffer, ENABLE7_SIZE, "%d", pdm_output_enable[6]);

    Unicode::snprintfFloat(diag8Buffer, DIAG8_SIZE, "%.2fA", pdm_outputs[7].feedback * 0.125f);
    Unicode::snprintf(Enable8Buffer, ENABLE8_SIZE, "%d", pdm_output_enable[7]);

    Unicode::snprintfFloat(diag9Buffer, DIAG9_SIZE, "%.2fA", pdm_outputs[8].feedback * 0.125f);
    Unicode::snprintf(Enable9Buffer, ENABLE9_SIZE, "%d", pdm_output_enable[8]);

    Unicode::snprintfFloat(diag10Buffer, DIAG10_SIZE, "%.2fA", pdm_outputs[9].feedback * 0.125f);
    Unicode::snprintf(Enable10Buffer, ENABLE10_SIZE, "%d", pdm_output_enable[9]);

    Unicode::snprintfFloat(diag11Buffer, DIAG11_SIZE, "%.2fA", pdm_outputs[10].feedback * 0.125f);
    Unicode::snprintf(Enable11Buffer, ENABLE11_SIZE, "%d", pdm_output_enable[10]);

    Unicode::snprintfFloat(diag12Buffer, DIAG12_SIZE, "%.2fA", pdm_outputs[11].feedback * 0.125f);
    Unicode::snprintf(Enable12Buffer, ENABLE12_SIZE, "%d", pdm_output_enable[11]);
}

void ScreenPDMView::updateDigitalInputs()
{
    // Digital input state meanings:
    // 0 = Open Circuit
    // 1 = Short-to-ground
    // 2 = Short-to-battery
    // 3 = Not Available

    //const char* stateText[4] = {"Open", "Gnd", "Bat", "N/A"};

    // Update all 12 digital inputs
    Unicode::snprintf(textAreaDI1Buffer, TEXTAREADI1_SIZE, "DI1 %d", pdm_digital_inputs[0] & 0x03);
    Unicode::snprintf(textAreaDI2Buffer, TEXTAREADI2_SIZE, "DI2 %d", pdm_digital_inputs[1] & 0x03);
    Unicode::snprintf(textAreaDI3Buffer, TEXTAREADI3_SIZE, "DI3 %d", pdm_digital_inputs[2] & 0x03);
    Unicode::snprintf(textAreaDI4Buffer, TEXTAREADI4_SIZE, "DI4 %d", pdm_digital_inputs[3] & 0x03);
    Unicode::snprintf(textAreaDI5Buffer, TEXTAREADI5_SIZE, "DI5 %d", pdm_digital_inputs[4] & 0x03);
    Unicode::snprintf(textAreaDI6Buffer, TEXTAREADI6_SIZE, "DI6 %d", pdm_digital_inputs[5] & 0x03);
    Unicode::snprintf(textAreaDI7Buffer, TEXTAREADI7_SIZE, "DI7 %d", pdm_digital_inputs[6] & 0x03);
    Unicode::snprintf(textAreaDI8Buffer, TEXTAREADI8_SIZE, "DI8 %d", pdm_digital_inputs[7] & 0x03);
    Unicode::snprintf(textAreaDI9Buffer, TEXTAREADI9_SIZE, "DI9 %d", pdm_digital_inputs[8] & 0x03);
    Unicode::snprintf(textAreaDI10Buffer, TEXTAREADI10_SIZE, "DI10 %d", pdm_digital_inputs[9] & 0x03);
    Unicode::snprintf(textAreaDI11Buffer, TEXTAREADI11_SIZE, "DI11 %d", pdm_digital_inputs[10] & 0x03);
    Unicode::snprintf(textAreaDI12Buffer, TEXTAREADI12_SIZE, "DI12 %d", pdm_digital_inputs[11] & 0x03);
}

void ScreenPDMView::updateAnalogInputs()
{
    // Analog inputs are 10-bit values (0-1023) representing 0-5V
    // Convert to voltage: voltage = (value / 1023.0) * 5.0

    float voltage;

    // AI1
    voltage = (pdm_analog_inputs[0] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI1Buffer, TEXTAREAAI1_SIZE, "AI1 %.2fV", voltage);

    // AI2
    voltage = (pdm_analog_inputs[1] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI2Buffer, TEXTAREAAI2_SIZE, "AI2 %.2fV", voltage);

    // AI3
    voltage = (pdm_analog_inputs[2] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI3Buffer, TEXTAREAAI3_SIZE, "AI3 %.2fV", voltage);

    // AI4
    voltage = (pdm_analog_inputs[3] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI4Buffer, TEXTAREAAI4_SIZE, "AI4 %.2fV", voltage);

    // AI5
    voltage = (pdm_analog_inputs[4] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI5Buffer, TEXTAREAAI5_SIZE, "AI5 %.2fV", voltage);

    // AI6
    voltage = (pdm_analog_inputs[5] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI6Buffer, TEXTAREAAI6_SIZE, "AI6 %.2fV", voltage);

    // AI7
    voltage = (pdm_analog_inputs[6] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI7Buffer, TEXTAREAAI7_SIZE, "AI7 %.2fV", voltage);

    // AI8
    voltage = (pdm_analog_inputs[7] / 1023.0f) * 5.0f;
    Unicode::snprintfFloat(textAreaAI8Buffer, TEXTAREAAI8_SIZE, "AI8 %.2fV", voltage);

    // Note: AI7 and AI8 exist but aren't displayed on this screen based on the UI layout
}

void ScreenPDMView::updateD03()
{
	if(lastcall == 0)
	{
		 pdm_output_commands[2] = 0;
		lastcall = 1;
	}
	else
	{
		 pdm_output_commands[2] = 127;
		lastcall = 0;
	}
}

void ScreenPDMView::updateD07()
{
	if(lastcall1 == 0)
	{
		pdm_output_enable[2] = 0; //OUTPUT 3
		lastcall1 = 1;
	}
	else
	{
		pdm_output_enable[2] = 1;
		lastcall1 = 0;
	}
}

void ScreenPDMView::updateD011()
{
	if(lastcall2 == 0)
	{
		 pdm_output_commands[10] = 0;
		lastcall2 = 1;
	}
	else
	{
		 pdm_output_commands[10] = 127;
		lastcall2 = 0;
	}
}
