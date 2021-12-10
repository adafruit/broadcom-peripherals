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
    ;@ Switch to IRQ mode
    cps #18
    mov sp,#0x8000

    ;@ Switch to supervisor mode and set the stack pointer to 240MB. This
    ;@ assumes 256MB total ram and 16MB for the GPU.
    cps #19
    mov sp,#0xf000000

    ;@ Clear the BSS section
    ldr     r1, =__bss_start     // Start address
    ldr     r2, =__bss_size      // Size of the section
continue_loop:  cbz     r2, loop_done               // Quit loop if zero
    str     xzr, [r1], #8
    sub     r2, r2, #1
    cbnz    r2, continue_loop               // Loop if non-zero

done_loop:  bl main

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
