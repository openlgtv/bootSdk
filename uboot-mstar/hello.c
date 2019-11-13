#include <stdint.h>
#include "exports.h"

/*
#define EXPORT_FUNC(x)	\
	asm (		\
"	.globl " #x "\n"	\
"	.section .text\n"	\
#x ":\n"				\
"	ldr	ip, ="XSTR(GD_ADDR)"\n"	\
"	ldr	pc, [ip, %1]\n"	\
);

#include "_exports.h"*/

void hexdump(void *addr, unsigned int size){
	uint8_t *pb = (uint8_t *)addr;

	for(unsigned int i=0; i<size; i++){
		if((i % 16) == 0){
			if(i > 0){
				printf("\n");
			}
			printf("%08X: ", i);
		}
		printf("%02X ", pb[i]);
	}
	printf("\n");
}

int ENTRY_FUNC main(int argc, char *argv[]){
	printf("new\n");
	
	printf("Smx says hi from lxboot!\n");
	printf("===================\n");
	printf("OpenLGTV Rocks!\n");
	printf("===================\n");
	printf("Have a nice day :)\n");

	printf("dump gd\n");

	gd_t *gd = GET_GD();
	hexdump(gd, sizeof(gd_t));

	printf("jt: %08X\n", gd->jt);
	hexdump(gd->jt, sizeof(void *) * XF_MAX);


	return 0xDEAD;
}
