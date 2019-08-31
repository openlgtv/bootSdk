SECTIONS
{
    . = 0x2000000;
	.entry : { *(.entry)}
    .text : { *(.text)}
    .data : { *(.data)}
    .bss : { *(.bss)}
}