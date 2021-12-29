;@-------------------------------------------------------------------------
;@
;@ Copyright (c) 2012 David Welch dwelch@dwelch.com
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@
;@-------------------------------------------------------------------------

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler
reset_handler:      .word reset
undefined_handler:  .word hard_fault
swi_handler:        .word hard_fault
prefetch_handler:   .word hard_fault
data_handler:       .word hard_fault
unused_handler:     .word hard_fault
irq_handler:        .word irq
fiq_handler:        .word hard_fault

reset:
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    ;@ Disable interrupts
    cpsid aif 

    ;@ Set the stack pointer for the different modes
    ;@ Switch to FIQ mode
    cps #0x12
    mov sp,#0x7000

    ;@ Switch to IRQ mode
    cps #0x12
    mov sp,#0x8000

    ;@ Switch to abort mode
    cps #0x17
    mov sp,#0x6000

    ;@ Switch to undefined mode
    cps #0x18
    mov sp,#0x5000

    ;@ Switch to system mode and set the stack pointer to 240MB. This
    ;@ assumes 256MB total ram and 16MB for the GPU.
    cps #0x1f
    mov sp,#0xf000000

    ;@ Clear the BSS section
    ldr     r1, =__bss_start     // Start address
    ldr     r2, =__bss_end       // End of bss
    movs    r0, #0
    subs    r2, r2, r1              // r2 is now size
    ble     loop_done               // Skip loop if zero
continue_loop:
    subs    r2, r2, #4
    str     r0, [r1, r2]
    bgt continue_loop               // Loop if non-zero

loop_done:
    bl main

err_hang: b err_hang

irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl  handle_irq
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

hard_fault:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl   HardFault_IRQHandler
    // Should never return
    b err_hang

;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------
