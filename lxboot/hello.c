#include "lxboot.h"

// workaround for gcc bug
void __aeabi_unwind_cpp_pr0(){}
void __aeabi_unwind_cpp_pr1(){}

void Micom_InvertControl(int enable){
	unsigned char packet[] = {MICOM_I2C_INVERTER, enable};
	Micom_I2C_Write(packet, sizeof(packet));
}

void Micom_PanelControl(int enable){
	unsigned char packet[] = {MICOM_I2C_PANEL, enable};
	Micom_I2C_Write(packet, sizeof(packet));
}

void appendCommand(LXCommand *cmd){
	int newTableSize = sizeof(LXCommand *) * (*gNumberOfCommands + 1);
	gCmdTableDynamic = realloc(gCmdTableDynamic, newTableSize);

	LXCommand *newCommandPointer = &gCmdTableDynamic[*gNumberOfCommands];
	newCommandPointer = cmd;
	(*gNumberOfCommands)++;
}

int dispOffCommand(int argc, char *argv[]){
	printf("Turning off Inverter...\n");
	Micom_InvertControl(0);
	printf("Turning off Panel...\n");
	Micom_PanelControl(0);
	return 0;
}

int sayHiCommand(int argc, char *argv[]){
	printf("Smx says hi from lxboot!\n");
	printf("===================\n");
	printf("OpenLGTV Rocks!\n");
	printf("===================\n");
	printf("Have a nice day :)\n");
	return 0;
}

int ENTRY_FUNC main(int argc, char *argv[]){
	LXCommand *sayHi = malloc(sizeof(LXCommand));
	sayHi->cmdName = "hello";
	sayHi->cmdFunc = &sayHiCommand;
	sayHi->cmdDescr = "greets you";
	sayHi->cmdDescrArgs = NULL;
	sayHi->defaultArg = NULL;
	appendCommand(sayHi);

	LXCommand *dispOff = malloc(sizeof(LXCommand));
	dispOff->cmdName = "dispoff";
	dispOff->cmdFunc = &dispOffCommand;
	dispOff->cmdDescr = "turns off inverter and panel";
	dispOff->cmdDescrArgs = NULL;
	dispOff->defaultArg = NULL;
	appendCommand(dispOff);
	return 0;
}