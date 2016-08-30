#ifndef _REGISTER_h
	#define _REGISTER_h


    /**
     * @brief Libraries needed to run AskSin library
     */
    #include <AS.h>
    #include "hardware.h"
    #include "hmkey.h"
	#include <THSensor.h>

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
	THSensor thsens;                                                        // create instance of channel module

	// some forward declarations
	extern void initTH1();
	extern void measureTH1(THSensor::s_meas *);

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
    const uint8_t devIdnt[] PROGMEM = {               // testID 
        /* firmwareVersion 1 byte */  0x10,           // or GE 
		/* modelID         2 byte */  0xF2,0x01,
		/* subTypeID       1 byte */  0x70,           // replace __ by a valid type id 
		/* deviceInfo      3 byte */  0x01,0x01,0x00, // device info not found, replace by valid values 
    }; 


    /**
     * @brief Register definitions
	 * The values are addresses in relation to the start address defines in cnlTbl
	 * Register values can found in related Device-XML-File.
	 *
	 * Special register list 0: 0x0A, 0x0B, 0x0C
	 * Special register list 1: 0x08
	 *
	 * @See Defines.h
	 *
	 * @See: cnlTbl
	 */
	const uint8_t cnlAddr[] PROGMEM = {
		// channel: 0, list: 0
		0x01,0x05,0x0a,0x0b,0x0c,0x12,0x14,0x23,
		// channel: 1, list: 1
		//0x08,
		// channel: 1, list: 4
		0x01,0x02,
	};  // 10 byte
	// List 0: 0x01, 0x05, 0x0A, 0x0B, 0x0C, 0x12, 0x14, 0x23
	//uint8_t burstRx;         // 0x01,             startBit:0, bits:8
	//uint8_t             :6;  // 0x05              startBit:0, bits:6
	//uint8_t ledMode     :2;  // 0x05,             startBit:6, bits:2
	//uint8_t pairCentral[3];  // 0x0A, 0x0B, 0x0C, startBit:0, bits:8 (3 mal)
	//uint8_t lowBatLimit;     // 0x12,             startBit:0, bits:8
	//uint8_t transmDevTryMax; // 0x14,             startBit:0, bits:8
	//uint8_t osccal;          // 0x23              startBit:0, bits:8

	// List 4: 0x01,0x02,
	//uint8_t  peerNeedsBurst:1; // 0x01, s:0, e:1
	//uint8_t  useDHTTemp    :1; // 0x01, s:1, e:1
	//uint8_t                :6; //
	//uint8_t  tempCorr          // 0x02, max +/- 12,7°C (1/10°C)


    /**
     * @brief Channel, List defaults
     * Source of the default values is the respective xml file.
	 * This values are the defined default values and should be set
	 * in the first start function.
	 */
    const uint8_t cnlDefs[] = {
        // channel: 0, list: 0
        0x00,0x40,0x00,0x00,0x00,0x15,0x03,0x00,
		// channel: 1, list: 1
		//0x10,0x00,0x00,
		// channel: 1, list: 4
		0x00,0x00,
		// channel: 2, list: 1, link to 01 01
		// channel: 2, list: 4, link to 01 04
		// channel: 3, list: 1, link to 01 01
		// channel: 3, list: 4, link to 01 04
		// channel: 4, list: 1, link to 01 01
		// channel: 4, list: 4, link to 01 04
		// channel: 5, list: 1, link to 01 01
		// channel: 5, list: 4, link to 01 04
		// channel: 6, list: 1, link to 01 01
		// channel: 6, list: 4, link to 01 04
    }; // 10 byte

    /**
     * @brief Channel - List translation table
     * channel, list, startIndex, start address in EEprom, hidden
     * do not edit the table, if you need more peers edit the defines accordingly.
     */
	#define PHY_ADDR_START 0x20
	#define CNL_01_PEERS   6 

	const EE::s_cnlTbl cnlTbl[] = {
		// cnl, lst, sIdx, sLen, hide, pAddr 
		{ 0,   0,    0,    8,    0, PHY_ADDR_START },
		{ 1,   4,    8,    2,    0, cnlTbl[0].pAddr + cnlTbl[0].sLen },
	}; // 20 byte 

	/**
	 * @brief Peer-Device-List-Table
	 * maximum allowed peers, link to row in cnlTbl, start address in EEprom
	 */
	const EE::s_peerTbl peerTbl[] = {
		// pMax, pLink, pAddr; 
		{            0, 0, cnlTbl[1].pAddr + (cnlTbl[1].sLen * CNL_01_PEERS) },
		{ CNL_01_PEERS, 1, peerTbl[0].pAddr + (peerTbl[0].pMax * 4) },
	}; // 24 byte 	

    /**
     * @brief Struct with basic information for the AskSin library.
     * amount of user channels, amount of lines in the channel table,
     * link to devIdent byte array, link to cnlAddr byte array
     */
	//EE::s_devDef devDef = {
	//	1, 2, devIdnt, cnlAddr,
	//};	// 6 Byte

	/**
	 * @brief Struct with basic information for the AskSin library.
	 * amount of user channels, amount of lines in the channel table,
	 * link to devIdent byte array, link to cnlAddr byte array
	 */
	const uint8_t cnl_max = 1;
	const uint8_t cnl_tbl_max = 3;

    /**
     * @brief Sizing of the user module register table.
     * Within this register table all user modules are registered to make
     * them accessible for the AskSin library
     */
	RG::s_modTable modTbl[cnl_max + 1];


    /**
     * @brief Regular start function
     * This function is called by the main function every time when the device starts,
     * here we can setup everything which is needed for a proper device operation
     */
	void everyTimeStart(void) {
		
        // channel 0 section 
		hm.ld.set(welcome);
		hm.confButton.config(1);
		cnl0Change();														// initialize with values from eeprom

		// channel 1 section 
		thsens.regInHM(1, 4);													// register sensor module on channel 1, with a list4 and introduce asksin instance
		thsens.config(&initTH1, &measureTH1);								// configure the user class and handover addresses to respective functions and variables
	}

    /**
     * @brief First time start function
     * This function is called by the main function on the first boot of a device.
     * First boot is indicated by a magic byte in the eeprom.
     * Here we can setup everything which is needed for a proper device operation, like cleaning
     * of eeprom variables, or setting a default link in the peer table for 2 channels
     */
	void firstTimeStart(void) {

    #ifdef SER_DBG
		// some debug
		dbg << F("First time start active:\n");
		dbg << F("cnl\tlst\tsIdx\tsLen\thide\tpAddr\n");
		//for (uint8_t i = 0; i < devDef.lstNbr; i++) {
			// cnl, lst, sIdx, sLen, hide, pAddr 
			//dbg << cnlTbl[i].cnl << "\t" << cnlTbl[i].lst << "\t" << cnlTbl[i].sIdx << "\t" << cnlTbl[i].sLen << "\t" << cnlTbl[i].vis << "\t" << cnlTbl[i].pAddr << "\n";
		//}
    #endif

		// fill register with default values, peer registers are not filled while done in usermodules
		hm.ee.setList(0, 0, 0, (uint8_t*)&cnlDefs[0]);
		//hm.ee.setList(1, 1, 0, (uint8_t*)&cnlDefs[6]);

		// format peer db
		hm.ee.clearPeers();
	}
#endif
