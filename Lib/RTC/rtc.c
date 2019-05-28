/*
 * rtc.c
 *
 *  Created on: 26 апр. 2019 г.
 *      Author: Admin
 */

#include "rtc.h"

// Private functions and variables ----------------------------------------------------------------
void RTC_EnterConfigMode(void);
void RTC_ExitConfigMode(void);
void RTC_WaitForSynchro(void);
void RTC_WaitForLastTask(void);
// ------------------------------------------------------------------------------------------------

void RTC_Init(void) {

	RCC->APB1ENR |= RCC_APB1ENR_BKPEN;			// 1: Backup interface clock enabled
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;			// 1: Power interface clock enable
	PWR->CR	|= PWR_CR_DBP;						// 1: Access to RTC and Backup registers enabled

	// Backup domain software reset
	RCC->BDCR |= RCC_BDCR_BDRST;				// 1: Resets the entire Backup domain
	RCC->BDCR &= ~ RCC_BDCR_BDRST;				// 0: Reset not activated

	RCC->BDCR |= RCC_BDCR_LSEON;				// 1: External 32 kHz oscillator ON
	while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0);	// 1: External 32 kHz oscillator ready

	RCC->BDCR &= ~RCC_BDCR_RTCSEL;				// Reset
	RCC->BDCR |= RCC_BDCR_RTCSEL_0;				// 01: LSE oscillator clock used as RTC clock
	RCC->BDCR |= RCC_BDCR_RTCEN;				// 1: RTC clock enabled

	RTC_WaitForSynchro();
	RTC_WaitForLastTask();

	// Interrupt
//	RTC_WaitForLastTask();

	RTC_EnterConfigMode();
	// fTR_CLK = fRTCCLK/(PRL[19:0]+1)
	RTC->PRLH = 0x0000;							// PRL[19:16]: RTC prescaler reload value high
	RTC->PRLL = 0x7FFF;							// PRL[15:0]: RTC prescaler reload value low
	RTC_ExitConfigMode();

	RTC_WaitForLastTask();
}

void RTC_SetCounter(uint32_t count) {

	RTC_EnterConfigMode();
	RTC->CNTH = count >> 16U;
	RTC->CNTL = count & RTC_CNTL_RTC_CNT;
	RTC_ExitConfigMode();
}

uint32_t RTC_GetCounter() {

	if ((RTC->CRL & RTC_CRL_RSF) != 0) {		// 1: Registers synchronized.

		uint16_t high = 0, low = 0;

		high = RTC->CNTH & RTC_CNTH_RTC_CNT;
		low = RTC->CNTL & RTC_CNTL_RTC_CNT;

		return ((uint32_t) (((uint32_t) high << 16U) | low));
	}
	return 0;
}

void RTC_SetTime(RTC_TimeTypeDef *time) {
	RTC_SetCounter((time->Hours * 3600) + (time->Minutes * 60) + time->Seconds);
}

void RTC_GetTime(RTC_TimeTypeDef *time){

	uint32_t count = RTC_GetCounter();
	time->Hours = count / 3600;
	time->Minutes = (count % 3600) / 60;
	time->Seconds = (count % 3600) % 60;
}


void RTC_EnterConfigMode() {
	RTC->CRL |= RTC_CRL_CNF;					// 1: Enter configuration mode.
}

void RTC_ExitConfigMode() {
	RTC->CRL &= ~RTC_CRL_CNF;					// 0: Exit configuration mode (start update of RTC registers).
}

void RTC_WaitForSynchro() {
	RTC->CRL &= ~RTC_CRL_RSF;					// 0: Registers not yet synchronized.
	while ((RTC->CRL & RTC_CRL_RSF) == 0);		// 1: Registers synchronized.
}

void RTC_WaitForLastTask() {
	while ((RTC->CRL & RTC_CRL_RTOFF) == 0);	// 1: Last write operation on RTC registers terminated.
}
