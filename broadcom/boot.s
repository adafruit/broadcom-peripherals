.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core), else hang
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    // We're not on the main core, so hang in an infinite wait loop
1:  wfe
    b       1b
2:  // We're on the main core!

    // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1

    adr    x0, vectors        // load VBAR_EL1 with virtual
    // msr    vbar_el3, x0        // vector table address
    msr    vbar_el1, x0        // vector table address
    msr    vbar_el2, x0        // vector table address
    isb

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
3:  cbz     w2, 4f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b               // Loop if non-zero

    // Jump to our main() routine in C (make sure it doesn't return)
4:  bl      main
    // In case it does return, halt the master core too
    b       1b

.macro  ventry  label
.align  7
b   \label
.endm

.macro  irq_entry
// Make a new stack frame
stp x29, x30, [sp, #-16]!       /* save x29, x30 onto stack */

mrs x29, elr_el2            /* save elr_el1, spsr_el1 onto stack */
mrs x30, spsr_el2
stp x29, x30, [sp, #-16]!

// Save all registers
stp x1, x0, [sp, #-16]!
stp x3, x2, [sp, #-16]!
stp x5, x4, [sp, #-16]!
stp x7, x6, [sp, #-16]!
stp x9, x8, [sp, #-16]!
stp x11, x10, [sp, #-16]!
stp x13, x12, [sp, #-16]!
stp x15, x14, [sp, #-16]!
stp x17, x16, [sp, #-16]!
stp x19, x18, [sp, #-16]!
stp x21, x20, [sp, #-16]!
stp x23, x22, [sp, #-16]!
stp x25, x24, [sp, #-16]!
stp x27, x26, [sp, #-16]!
str x28, [sp, #-16]!
.endm

.macro  irq_exit
ldr x28, [sp], #16
ldp x27, x26, [sp], #16
ldp x25, x24, [sp], #16
ldp x23, x22, [sp], #16
ldp x21, x20, [sp], #16
ldp x19, x18, [sp], #16
ldp x17, x16, [sp], #16
ldp x15, x14, [sp], #16
ldp x13, x12, [sp], #16
ldp x11, x10, [sp], #16
ldp x9, x8, [sp], #16
ldp x7, x6, [sp], #16
ldp x5, x4, [sp], #16
ldp x3, x2, [sp], #16
ldp x1, x0, [sp], #16

ldp x29, x30, [sp], #16     /* restore elr_el1, spsr_el1 from stack */
msr elr_el2, x29
msr spsr_el2, x30

ldp x29, x30, [sp], #16     /* restore x29, x30 from stack */

eret
.endm


/*
 * Exception vectors.
 */
.align  11
.globl vectors 
vectors:
    ventry  hard_fault           // Synchronous EL1t
    ventry  hard_fault            // IRQ EL1t
    ventry  hard_fault            // FIQ EL1t
    ventry  hard_fault          // Error EL1t

    ventry  hard_fault           // Synchronous EL1h
    ventry  irq                 // IRQ EL1h
    ventry  hard_fault            // FIQ EL1h
    ventry  hard_fault          // Error EL1h

    ventry  hard_fault         // Synchronous 64-bit EL0
    ventry  hard_fault          // IRQ 64-bit EL0
    ventry  hard_fault          // FIQ 64-bit EL0
    ventry  hard_fault            // Error 64-bit EL0

    ventry  hard_fault         // Synchronous 32-bit EL0
    ventry  hard_fault          // IRQ 32-bit EL0
    ventry  hard_fault          // FIQ 32-bit EL0
    ventry  hard_fault            // Error 32-bit EL0

hard_fault:
    irq_entry
    bl   HardFault_IRQHandler
    // Should never return
    b err_hang

irq:
    irq_entry 
    bl  handle_irq
    irq_exit 

.globl err_hang
err_hang: b err_hang
