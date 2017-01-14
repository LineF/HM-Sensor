//- -----------------------------------------------------------------------------------------------------------------------
// AskSin driver implementation
// 2013-08-03 <trilu@gmx.de> Creative Commons - http://creativecommons.org/licenses/by-nc-sa/3.0/de/
//- -----------------------------------------------------------------------------------------------------------------------
//- AskSin hardware definition ----------------------------------------------------------------------------------------
//- with a lot of support from martin876 at FHEM forum
//- -------------------------------------------------------------------------------------------------------------------

#ifndef _HARDWARE_h
	#define _HARDWARE_h

	#define DEBOUNCE                   5
	#define TIMER2_LOW_FREQ_OSC

	#if defined(__AVR_ATmega328P__)
		//- hardware specific general setup
		#define PCINT_CALLBACK													// enables the pin change interrupt callback in user sketch

		//- configuration key  ----------------------------------------------------------------------------------------
		//#define CONFIG_KEY             PIN_B0									// define the config key pin port

	#elif defined(__AVR_ATmega32U4__)
		//- configuration key  ----------------------------------------------------------------------------------------
		//#define CONFIG_KEY             PIN_B6									// define the config key pin port

	#else
		#error "Error: cc1100 CS and GDO0 not defined for your hardware in hardware.h!"
	#endif
	//- ---------------------------------------------------------------------------------------------------------------


	//- wake up pin ---------------------------------------------------------------------------------------------------
	#if defined(__AVR_ATmega32U4__)
		#define WAKE_UP_DDR            DDRE										// define wake up pin port and remaining pin
		#define WAKE_UP_PORT           PORTE
		#define WAKE_UP_PNR            PINE
		#define WAKE_UP_PIN            PINE2
	#endif
	//- ---------------------------------------------------------------------------------------------------------------

#endif
