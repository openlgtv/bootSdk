SECTIONS
{
	. = 0x25000000;
	.entry : { *(.entry)}
	.text : { *(.text)}
	.got.plt : { *(.got.plt)}
	.data : { *(.data)}
	.rodata : { *(.rodata*)}
	.bss : { *(.bss)}
	/DISCARD/ : {*(*)}
}
