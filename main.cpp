/*
 * HM-Sensor.cpp
 *
 * Created: 27.12.2015 14:47:56
 * Author : Martin
 */ 

//- load library's --------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include <newasksin.h>																		// ask sin framework
#include <avr/wdt.h>
#include "00_debug-flag.h"
#include "register.h"																		// configuration sheet
#include "dht.h"
#include "OneWire.h"

//#define SER_DBG

#define DHT_PWR		9																		// power for DHT22
#define DHT_PIN		6																		// this pin DHT22 is connected to
#define OW_PIN		5																		// this pin DS18B20 is connected to


dht DHT;
OneWire OW(OW_PIN);
waitTimer thTimer;
int16_t celsius;
uint8_t transmitDevTryMax;
uint8_t ledFreqTest = 0;
uint8_t factOscCal;

void serialEvent();
void dumpEEprom();


//- arduino functions -----------------------------------------------------------------------------------------------------
void setup() {

	// - Hardware setup ---------------------------------------
	// - everything off ---------------------------------------
	
	wdt_disable();																			// clear WDRF to avoid endless resets after WDT reset
	MCUSR &= ~(1<<WDRF);																	// stop all WDT activities
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0x00;

	EIMSK = 0;																				// disable external interrupts
	ADCSRA = 0;																				// ADC off
	power_all_disable();																	// and everything else

	factOscCal = getDefaultOSCCAL();

	// enable only what is really needed
	#ifdef SER_DBG																			// some debug
		DBG_START(SER, F("SER.\n") );														// ...some debug
		DBG(SER, F("\nHB_UW_Sen_TH_Pn\n"));
		DBG(SER, F(LIB_VERSION_STRING));													// ...and some information
	#endif

	// - AskSin related ---------------------------------------
	init_millis_timer2();																	// init timer2
	hm->init();																				// init the asksin framework
	sei();																					// enable interrupts
 
	// - user related -----------------------------------------
	#ifdef SER_DBG
		DBG(SER, F("HMID: "), _HEX(dev_ident.HMID,3), F(", MAID: "), _HEX(dev_operate.MAID,3), F("\n\n"));	// some debug
	#endif
}


//- user functions --------------------------------------------------------------------------------------------------------
void initTH(uint8_t channel) {																// init the sensor
	
	pinMode(DHT_PWR, OUTPUT);
	
	#ifdef SER_DBG
		DBG(SER, F("init th: cnl: "), channel, F("\n"));
	#endif
}

// this is called when HM wants to send measured values to peers or master
// due to asynchronous measurement we simply can take the values very quick from variables
void measureTH(uint8_t channel, cm_thsensor::s_sensVal *sensVal) {
	int16_t t;
	
	//DBG(SER, "msTH DS-t: ", celsius, F(" "), _TIME, F("\n"));
	// take temp value from DS18B20
	t = celsius / 10;
	((uint8_t *)&(sensVal->temp))[0] = ((t >> 8) & 0x7F);									// battery status is added later
	((uint8_t *)&(sensVal->temp))[1] = t & 0xFF;
	
	//DBG(SER, "msTH t: ", DHT.temperature, ", h: ", DHT.humidity, ' ', _TIME, F("\n"));
	// take humidity value from DHT22
	sensVal->hum = DHT.humidity / 10;
	// fetch battery voltage
	t = bat->get_volts();
	((uint8_t *)&(sensVal->bat))[0] = t >> 8;
	((uint8_t *)&(sensVal->bat))[1] = t & 0xFF;
}

void cnl0Change(void) {
	
	// set lowBat threshold
	bat->set(900000, *cmm[0]->list[0]->ptr_to_val(REG_CHN0_LOW_BAT_LIMIT_TH)*10);			// check voltage every 1/4 hour (900secs * 1000ms)

	// handle r/o factory osccal register
	if (*cmm[0]->list[0]->ptr_to_val(REG_CHN0_FACT_OSCCAL) != factOscCal)
	{
		uint8_t a[2];
		a[0] = REG_CHN0_FACT_OSCCAL;
		a[1] = factOscCal;
		DBG(SER, F("main: setting factOscCal register back to default: "), _HEX(a, 2), F("\n"));
		cmm[0]->list[0]->write_array(a, 2);
	}
	
	// set OSCCAL frequency
	if (uint8_t oscCal = *cmm[0]->list[0]->ptr_to_val(REG_CHN0_OSCCAL)) {
		DBG(SER, F("will set OSCCAL: old="), OSCCAL, F(", new="), oscCal, F("\n"));
		// Attention: your controller my have other factory calibration !!
		// If you are unsure about the internal RC-frequency of your chip then use factory default (oscCal-Reg = 0)
		// my chip: 1kHz - 8A=994Hz, 8B=998,4Hz, 8C=1001,6Hz, 8E=1010Hz
		// frequency measured with help of millis-ISR (toggling LED port and measuring frequency on it)

		OSCCAL = oscCal;
	} else {
		DBG(SER, F("will set default OSCCAL: "), factOscCal, F("\n"));
		OSCCAL = factOscCal;
	}
	#ifndef TIMER2_LOW_FREQ_OSC
		calibrateWatchdog();
	#endif

	// if burstRx is set ...
	if (*cmm[0]->list[0]->ptr_to_val(REG_CHN0_BURST_RX)) {
		DBG(SER, F("PM=onradio\n"));
		pom->setMode(POWER_MODE_WAKEUP_ONRADIO);											// set mode to wakeup on burst
	} else {																				// no burstRx wanted
			DBG(SER, F("peers: "), cmm[1]->peerDB.used_slots(), F("\n"));
			if (cmm[1]->peerDB.used_slots())
			{
				DBG(SER, F("PM=250ms\n"));
				pom->setMode(POWER_MODE_WAKEUP_250MS);										// set mode to awake every 250 msecs
			}
			else
			{
				DBG(SER, F("PM=8000ms\n"));
				pom->setMode(POWER_MODE_WAKEUP_8000MS);										// set mode to awake every 250 msecs
			}
	}

	// check if frequency test output is wanted
	ledFreqTest = ((*cmm[0]->list[0]->ptr_to_val(REG_CHN0_LED_MODE) & 0xC0) == 0xC0);

	// fetch transmitDevTryMax
	if ((transmitDevTryMax = *cmm[0]->list[0]->ptr_to_val(REG_CHN0_TRANS_DEV_TRY_MAX)) > 10)
		transmitDevTryMax = 10;
	else if (transmitDevTryMax < 1)
		transmitDevTryMax = 1;
}

// this is called regularly - real measurement is done here
void measure() {
	enum mState {mInit, mWait, mPwrOn, mStartDS};
	static mState state = mWait;

	if (!thTimer.done())
		return;
		
	if (state == mInit) {																	// wait some time till next measurement
		thTimer.set(88000);																	// measurement every 90 secs
		state = mWait;
	}
	else if (state == mWait) {																// power on sensor and wait 1 sec
		thTimer.set(1000);
		pinMode(OW_PIN, INPUT_PULLUP);
		digitalWrite(DHT_PWR, 1);															// power on here

		state = mPwrOn;
		#ifdef SER_DBG
			//dbg << "power on Sensor" << ' ' << _TIME << '\n';
		#endif
	}
	else if (state == mPwrOn) {																// now start measurement on DS18B20 and wait another second
		thTimer.set(1000);
		uint8_t rc = OW.reset();															// attention - OW device get ready to communicate!
		OW.skip();																			// skip rom selection - we have only one device attached
		OW.write(0x44);																		// start conversion
		#ifdef SER_DBG
			//dbg << "rc: " << rc << _TIME << '\n';
		#endif
		state = mStartDS;
	}
	else if (state == mStartDS)	{															// get results here and switch off sensor
		DHT.read22(DHT_PIN);																// read DHT22
		#ifdef SER_DBG
			dbg << "t: " << DHT.temperature << ", h: " << DHT.humidity << ' ' << _TIME << '\n';
		#endif
		
		OW.reset();																			// attention - get ready to read result from DS18B20
		OW.skip();																			// no rom selection
		OW.write(0xBE);																		// read temp from scratchpad
		int16_t t = ((int32_t)(OW.read() | (OW.read() << 8)) * 100) >> 4;					// we need only first two bytes from scratchpad
		if (t != 8500)
			celsius = t;
	
		#ifdef SER_DBG
			dbg << "DS-t: " << celsius << ' ' << _TIME << '\n';
		#endif

		digitalWrite(DHT_PWR, 0);															// power off sensor
		pinMode(OW_PIN, INPUT);
		pinMode(DHT_PIN, INPUT);
		
		#ifdef SER_DBG
			//dbg << "power off Sensor" << ' ' << _TIME << '\n';
		#endif
		state = mInit;
	}
}


int main(void)
{
	// Arduino init: initializes timers etc.
	// not needed here - all timers/functions initialized by own functions
	// init();
	
	// Initialize all functions and pins
	setup();
	
    /* Replace with your application code */
    while (1) 
    {
			// - AskSin related ---------------------------------------
			hm->poll();																		// poll the homematic main loop

			// - user related -----------------------------------------
#ifdef SER_DBG
			serialEvent();
#endif
			measure();
    }
}
