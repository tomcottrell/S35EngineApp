////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: xxxxxxxxx.h
//
// Description: Configuration memeory map
//
// Author: Enovation controls
//
//         http://www.enovationcontrols.com
//
// copyright Copyright 2024 by Enovation Controls.
//           This file and/or parts of it are subject to
//            software license terms.
//
//            Without written approval of Enovation Controls this
//            software may not be copied, redistributed or used
//            in any other way.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <ConfigMemory.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
// S35 CONFIGURATION MEMORY MAP
////////////////////////////////////////////////////////////////////////////////////////////////////
//Add new entries here and in ConfigMemory.h and update CONFIG_LENGTH constant
uint16_t S35_config[CONFIG_LENGTH];

const uint16_t LCD_BRIGHTNESS = 0;
const uint16_t FOUR_UP_SCREEN1_GAUGE1_SPN = 1;
const uint16_t FOUR_UP_SCREEN1_GAUGE2_SPN = 2;
const uint16_t FOUR_UP_SCREEN1_GAUGE3_SPN = 3;
const uint16_t FOUR_UP_SCREEN1_GAUGE4_SPN = 4;
const uint16_t FOUR_UP_SCREEN2_GAUGE1_SPN = 5;
const uint16_t FOUR_UP_SCREEN2_GAUGE2_SPN = 6;
const uint16_t FOUR_UP_SCREEN2_GAUGE3_SPN = 7;
const uint16_t FOUR_UP_SCREEN2_GAUGE4_SPN = 8;
const uint16_t TSC1_SCREEN_IDLE_SPEED = 9;
const uint16_t TSC1_SCREEN_PRESET1_SPEED = 10;
const uint16_t TSC1_SCREEN_PRESET2_SPEED = 11;
const uint16_t REGEN_SCREEN_GAUGE1_SPN = 12;
const uint16_t REGEN_SCREEN_GAUGE2_SPN = 13;
const uint16_t REGEN_SCREEN_GAUGE3_SPN = 14;
const uint16_t REGEN_SCREEN_GAUGE4_SPN = 15;

////////////////////////////////////////////////////////////////////////
//
// SetDefaultConfig()
//
// Updates all config variables with their default values
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void SetDefaultConfig()
{
	//DISPLAY MENU
	S35_config[LCD_BRIGHTNESS] = 80;				//LCD backlight brightness (%)

	//CUSTOMIZE INTERFACE MENU
	//four up screen 1 gauge setup
	S35_config[FOUR_UP_SCREEN1_GAUGE1_SPN] = 100;	//SPN 100 engine Oil pressure
	S35_config[FOUR_UP_SCREEN1_GAUGE2_SPN] = 110;	//SPN 110 Coolant temperature
	S35_config[FOUR_UP_SCREEN1_GAUGE3_SPN] = 92;	//SPN 92 engine load
	S35_config[FOUR_UP_SCREEN1_GAUGE4_SPN] = 3719;	//SPN 3917 DPF soot level
	//four up screen 2 gauge setup
	S35_config[FOUR_UP_SCREEN2_GAUGE1_SPN] = 98;	//SPN 98 engine Oil level
	S35_config[FOUR_UP_SCREEN2_GAUGE2_SPN] = 167;	//SPN 167 charging system voltage
	S35_config[FOUR_UP_SCREEN2_GAUGE3_SPN] = 173;	//SPN 173 engine exhaust temperature
	S35_config[FOUR_UP_SCREEN2_GAUGE4_SPN] = 247;	//SPN 247 engine hours

	//TO BE ADDED TO MENUS
	//Speed control screen
	S35_config[TSC1_SCREEN_IDLE_SPEED] = 800;
	S35_config[TSC1_SCREEN_PRESET1_SPEED] = 1200;
	S35_config[TSC1_SCREEN_PRESET2_SPEED] = 2000;
	//Regen screen gauge setup
	S35_config[REGEN_SCREEN_GAUGE1_SPN] = 3719;		//SPN 3917 DPF soot level
	S35_config[REGEN_SCREEN_GAUGE2_SPN] = 110;		//SPN 110 Coolant temperature
	S35_config[REGEN_SCREEN_GAUGE3_SPN] = 92;		//SPN 92 engine load
	S35_config[REGEN_SCREEN_GAUGE4_SPN] = 190;		//SPN 190 engine speed
}

////////////////////////////////////////////////////////////////////////
//
// UpdateAllConfigFeatures()
//
// Update all features affected by updated configuration data
//
// Updates all config variables with their default values
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void UpdateAllConfigFeatures()
{


}
