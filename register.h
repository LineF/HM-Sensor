#ifndef _REGISTER_h
	#define _REGISTER_h


	/**
	 * @brief Libraries needed to run AskSin library
	 */
	#include <newasksin.h>
	#include "defines.h"
	#include "hmkey.h"

    /**
     * @brief Stage the modules and declare external functions.
     *
     * This functions are the call backs for user modules,
     * declaration is in the register.h but the functions needs
     * to be defined in the user sketch.
     */
	AES *aes = new NO_AES;														// NO_AES or HAS_AES
	COM *com = new CC1101(&pin_B4, &pin_B3, &pin_B5, &pin_B2, &pin_D2);
	CBN *cbn = new CBN(1, &pin_B0);
	LED *led = new LED(&pin_D4, &pin_D3);
	BAT *bat = new EXT_BAT(3600000, 210, &pin_D7, &pin_C1, 10, 47);
	POM *pom = new POM(POWER_MODE_WAKEUP_250MS);


	/*
	* cmSwitch requires this functions in the user sketch:
	* void cmSwitch::initSwitch(uint8_t channel);
	* void cmSwitch::switchSwitch(uint8_t channel, uint8_t status);
	*/
	//uint8_t burstRx;         // 0x01,             startBit:0, bits:8
	//uint8_t             :6;  // 0x05              startBit:0, bits:6
	//uint8_t ledMode     :2;  // 0x05,             startBit:6, bits:2
	//uint8_t pairCentral[3];  // 0x0A, 0x0B, 0x0C, startBit:0, bits:8 (3 mal)
	//uint8_t lowBatLimit;     // 0x12,             startBit:0, bits:8
	//uint8_t transmDevTryMax; // 0x14,             startBit:0, bits:8
	//uint8_t osccal;          // 0x23              startBit:0, bits:8
	//uint8_t factosccal;      // 0x24              startBit:0, bits:8
	//uint8_t cyclMsgOffset;   // 0x25              startBit:0, bits:8

	const uint8_t cm_maintenance_ChnlReg[] PROGMEM = { 0x01,0x05,0x08,0x0a,0x0b,0x0c,0x12,0x14,0x23,0x24,0x25, };
	const uint8_t cm_maintenance_ChnlDef[] PROGMEM = { 0x00,0x40,0x00,0x00,0x00,0x00,0x15,0x03,0x00,0x00,0x05, };
	const uint8_t cm_maintenance_ChnlLen = sizeof(cm_maintenance_ChnlReg);

	CM_MASTER *cmm[2] = {
		new CM_MAINTENANCE(0),
		new cm_thsensor(10),
	};


	 /*
	 * @brief HMID, Serial number, HM-Default-Key, Key-Index
	 */
	const uint8_t HMSerialData[] PROGMEM = {
		/* HMID */			  0x58, 0x25, 0xA3,
		/* Serial number */	  'M', 'P', 'T', 'H', '0', '0', '0', '0', '4', '0',		// HBremote01 
		/* Default-Key */	  HM_DEVICE_AES_KEY,
		/* Key-Index */		  HM_DEVICE_AES_KEY_INDEX,
	};
	 

	/**
	 * @brief Settings of HM device
	 * firmwareVersion: The firmware version reported by the device
	 *					Sometimes this value is important for select the related device-XML-File
	 *
	 * modelID:			Important for identification of the device.
	 *					@See Device-XML-File /device/supported_types/type/parameter/const_value
	 *
	 * subType:			Identifier if device is a switch or a blind or a remote
	 * DevInfo:			Sometimes HM-Config-Files are referring on byte 23 for the amount of channels.
	 *					Other bytes not known.
	 *					23:0 0.4, means first four bit of byte 23 reflecting the amount of channels.
	 */
    const uint8_t dev_static[] PROGMEM = {               // testID 
        /* firmwareVersion 1 byte */  0x15,           // or GE 
		/* modelID         2 byte */  0xF2,0x01,
		/* subTypeID       1 byte */  0x70,           // replace __ by a valid type id 
		/* deviceInfo      3 byte */  0x01,0x01,0x00, // device info not found, replace by valid values 
    }; 


    /**
     * @brief Regular start function
     * This function is called by the main function every time when the device starts,
     * here we can setup everything which is needed for a proper device operation
     */
	void everyTimeStart(void) {
	}

	/**
	 * @brief First time start function
	 * This function is called by the main function on the first boot of a device.
	 * First boot is indicated by a magic byte in the eeprom.
	 * Here we can setup everything which is needed for a proper device operation, like cleaning
	 * of eeprom variables, or setting a default link in the peer table for 2 channels
	 */
	void firstTimeStart(void) {
		dbg << F("new magic!\n");
	}
#endif
