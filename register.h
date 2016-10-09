#ifndef _REGISTER_h
	#define _REGISTER_h


    /**
     * @brief Libraries needed to run AskSin library
     */
    #include <AS.h>
    #include "hardware.h"
    #include "hmkey.h"
	#include <cmMaintenance.h>
	#include <cmTHSensWeather.h>

	// Register in Channel 0
	#define	REG_CHN0_BURST_RX					1							// Register 1.0 / 1.0: 0=burstRx off, otherwise on
	#define	REG_CHN0_LED_MODE					5							// Register 5.6 / 0.1: bit 6 LED mode (on/off)
	#define	REG_CHN0_PAIR_CENTRAL				10							// Register 10.0 / 3.0: 3 byte HMID
	#define	REG_CHN0_LOW_BAT_LIMIT_TH			18							// Register 18.0 / 1.0: in 1/10 volts
	#define	REG_CHN0_TRANS_DEV_TRY_MAX			20							// Register 20.0 / 1.0: 1-10, maxRetryCount
	#define	REG_CHN0_OSCCAL						35							// Register 35.0 / 1.0: 0=default, other=ATMEL:OSCCAL

    /**
     * @brief Stage the modules and declare external functions.
     *
     * This functions are the call backs for user modules,
     * declaration is in the register.h but the functions needs
     * to be defined in the user sketch.
     */
	AS hm;                                                                  // asksin framework


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

	const uint8_t cmMaintenance_ChnlReg[] PROGMEM = { 0x01,0x05,0x0a,0x0b,0x0c,0x12,0x14,0x23, };
	const uint8_t cmMaintenance_ChnlDef[] PROGMEM = { 0x00,0x40,0x00,0x00,0x00,0x15,0x03,0x00, };
	const uint8_t cmMaintenance_ChnlLen = 8;

	cmMaster *ptr_CM[2] = {
		new cmMaintenance(0),
		new cmTHSensWeather(10),
	};


	 /*
     * @brief HMID, Serial number, HM-Default-Key, Key-Index
     */
    const uint8_t HMSerialData[] PROGMEM = {
        /* HMID */            0x58,0x23,0xFF,
        /* Serial number */   'X','M','S','1','2','3','4','5','6','7',		// HBremote01 
        /* Default-Key */     HM_DEVICE_AES_KEY,
        /* Key-Index */       HM_DEVICE_AES_KEY_INDEX,
    };
	 

    /**
     * @brief Settings of HM device
     * firmwareVersion: The firmware version reported by the device
	 *                  Sometimes this value is important for select the related device-XML-File
	 *
	 * modelID:         Important for identification of the device.
	 *                  @See Device-XML-File /device/supported_types/type/parameter/const_value
	 *
	 * subType:         Identifier if device is a switch or a blind or a remote
	 * DevInfo:         Sometimes HM-Config-Files are referring on byte 23 for the amount of channels.
	 *                  Other bytes not known.
	 *                  23:0 0.4, means first four bit of byte 23 reflecting the amount of channels.
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
		led.set(welcome);
		btn.config(1);
		//cnl0Change();														// initialize with values from eeprom
	}

    /**
     * @brief First time start function
     * This function is called by the main function on the first boot of a device.
     * First boot is indicated by a magic byte in the eeprom.
     * Here we can setup everything which is needed for a proper device operation, like cleaning
     * of eeprom variables, or setting a default link in the peer table for 2 channels
     */
	void firstTimeStart(void) {
		dbg << F("\nnew magic!\n\n");
	}
#endif
