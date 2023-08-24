
MEMORY
{
    /* Kernel load address set in ports/broadcom/config.txt */
    READONLY (rx): ORIGIN = 0x100000, LENGTH = 2016K
    RAM (rw): ORIGIN = 0x300000, LENGTH = 200M
}

SECTIONS
{
    .text : {
        KEEP(*(.text.boot))
        *(.text .text.* .gnu.linkonce.t*)
    } > READONLY
    .rodata : {
        . = ALIGN(4096);
        *(.rodata .rodata.* .gnu.linkonce.r*)
    } > READONLY

    .data : {
        . = ALIGN(4096);
        . += 4K;
        _fiq_stack = .;
        . += 4K;
        _irq_stack = .;
        . += 4K;
        _abort_stack = .;
        . += 4K;
        _undef_stack = .;
        _data = .;
        *(.data .data.* .gnu.linkonce.d*)
    } > RAM
    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
        _end = .;
        end = .;
    } > RAM

    _ld_ram_end = ORIGIN(RAM) + LENGTH(RAM);

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;
