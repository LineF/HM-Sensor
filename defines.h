/*
 * defines.h
 *
 * Created: 25.01.2017 22:27:26
 *  Author: Martin
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

// Register in Channel 0
#define	REG_CHN0_BURST_RX					1							// Register 1.0 / 1.0: 0=burstRx off, otherwise on
#define	REG_CHN0_LED_MODE					5							// Register 5.6 / 0.1: bit 6 LED mode (on/off)
#define	REG_CHN0_PAIR_CENTRAL				10							// Register 10.0 / 3.0: 3 byte HMID
#define	REG_CHN0_LOW_BAT_LIMIT_TH			18							// Register 18.0 / 1.0: in 1/10 volts
#define	REG_CHN0_TRANS_DEV_TRY_MAX			20							// Register 20.0 / 1.0: 1-10, maxRetryCount
#define	REG_CHN0_OSCCAL						35							// Register 35.0 / 1.0: 0=default, other=ATMEL:OSCCAL
#define	REG_CHN0_FACT_OSCCAL				36							// Register 36.0 / 1.0: readonly register - do not set
#define	REG_CHN0_CYCL_MSG_OFFSET			37							// Register 37.0 / 1.0: cyclic Message Offset in 50ms steps (-100..100)

#define	TIMER2_LOW_FREQ_OSC												// use 32KHz crystal

#endif /* DEFINES_H_ */