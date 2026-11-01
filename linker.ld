MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
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
        _data_start = .;
        *(.data)
        _data_end = .;
    } > SRAM AT> FLASH

    .bss :
    {
        _bss_start = .;
        *(.bss)
        _bss_end = .;
    } > SRAM
}