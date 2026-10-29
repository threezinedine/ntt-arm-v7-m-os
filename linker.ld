MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x1FFFFFFF
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 0x1FFFFFFF
}

SECTIONS
{
    .text : 
    {
        *(.exception_handlers)
        *(.text)
    } > FLASH

    .rodata :
    {
        *(.rodata)
    } > FLASH

    .data : 
    {
        *(.data)
    } > SRAM AT> FLASH

    .bss :
    {
        *(.bss)
    } > SRAM
}