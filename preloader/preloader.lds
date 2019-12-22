OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)

SECTIONS
{
    .code : {
        . = ALIGN(4);
        KEEP(*(.entry))
        KEEP(*(.text))
        KEEP(*(.got.plt))
        KEEP(*(.data))
        KEEP(*(.rodata*))
        KEEP(*(.loadArgs))
    }
    /DISCARD/ : {*(*)}
}