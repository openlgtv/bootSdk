/**
 * Copyright(C) Stefano Moioli <smxdev4.gmail.com>
 */
#ifndef __PRELOADER_BCM63138_H
#define __PRELOADER_BCM63138_H

#define _CFE_
#include "63138_map_part.h"

static inline __attribute__((always_inline)) void board_init(){
	// stop watchdog timer
	TIMER->SoftRst = 0;
	TIMER->WatchDogCtl = 0xee00;
	TIMER->WatchDogCtl = 0x00ee;
	
	volatile Uart *uart = UART;
	uart->control = TXEN | RXEN | BRGEN;
	uart->config = BITS8SYM | ONESTOP;
	uart->intMask = 0;
}


static inline __attribute__((always_inline)) void putchar(char ch){
	while(!(UART->intStatus & TXFIFOEMT));
	UART->Data = ch;
}

static inline __attribute__((always_inline)) void puts(const char *str){
	uint32_t status = 0;
	
	while(*str != '\0' && !status){
		do {
			status = UART->intStatus & TXFIFOEMT;
		} while( !status );
		UART->Data = *(str++);
		status  = UART->intStatus & (TXOVFERR|TXUNDERR);
	}

	if( status ) {
		/* Reset TX FIFO */
        UART->fifoctl |= RSTTXFIFOS;
		str--;
	}
}

#endif