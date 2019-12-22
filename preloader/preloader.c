/**
 * Copyright(C) Stefano Moioli <smxdev4.gmail.com>
 */
#include "preloader.h"

#ifdef CONFIG_TARGET_BCM63138
#include "bcm63138.h"
#else
#define puts(x)
#define putchar(x)
#endif

struct params {
	word_t *loadAddr;
	word_t numWords;
	word_t code[];
};

// pin loadArgs at the end of code, see ldscript
__attribute__((__section__(".loadArgs")))
struct params loadArgs = {
	.loadAddr = (word_t *)0xDEADBEE0,
	.numWords = (word_t)0xDEADBEE1
};

__attribute__((noreturn))
__attribute__((naked))
void ENTRY_FUNC start(void){	
	word_t *codePtr;
	word_t *destPtr;
	word_t numWords;
	
	puts("Preloader v0.1 by Smx\r\n");
	codePtr = loadArgs.code;
	destPtr = loadArgs.loadAddr;
	numWords = loadArgs.numWords;
	while(numWords-- > 0){
		*(destPtr++) = *(codePtr++);
	}
	
	puts("Flush\r\n");
	asm volatile(
		"MOV    r0, #0\n\t"
		"MCR    p15, 0, r0, c7, c5, 0\n\t" //Flush entire ICache
		"DSB	sy\r\n"
		"ISB"
		:::
	);
	puts("Bye!\r\n");
	void (*entryPoint)(void) = (void *)loadArgs.loadAddr;
	entryPoint();

	puts("payload returned, halt\r\n");
	while(1){}
}