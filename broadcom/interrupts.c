#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#include "broadcom/gen/bcm2711_lpa.h"
#include "broadcom/gen/interrupt_handlers.h"

extern void print(const char* str);

void Default_Handler(void) {
    while (true) {}
}

uint8_t CPU_Index(void) {
    uint32_t mpidr = 0;
    __asm__("mrs     %[mpidr], mpidr_el1"
            : /* No outputs. */
            : [mpidr] "r" (mpidr));
    return mpidr & 0xff;
}

// These are CPU local.
void BP_EnableIRQs(void) {
    // We only turn on the distributor. Turning it off later may break other CPUs.
    // GIC_DIST->GICD_CTLR_b.ENABLE_GROUP0 = true;
    GIC_CPU->GICC_CTLR_b.ENABLE_GROUP_0 = true;

    uint64_t hcr_el2 = 0;
    __asm__ volatile (
        // Read Hypervisor Control Register configuration data
        "MRS %[hcr_el2], HCR_EL2\n\t"
        // Write Hypervisor Control Register configuration data
        "ORR %[hcr_el2], %[hcr_el2], #0x8000000\n\t"
        // Set [M] bit and enable the MMU.
        "MSR HCR_EL2, %[hcr_el2]\n\t"
        // The ISB forces these changes to be seen by the next instruction
        "ISB\n\t"
        : /* No outputs. */
        : [hcr_el2] "r" (hcr_el2));
    __asm__("msr    daifclr, #2");
}

void BP_DisableIRQs(void) {
    __asm__("msr    daifset, #2");
}

__attribute__((weak)) void handle_irq(void) {
    while (GIC_CPU->GICC_HPPIR_b.INTERRUPT_ID < INTERRUPT_COUNT) {
        // This register changes state after being read so make sure to read it
        // all at once. We need the full value to pass back to EOIR later.
        uint32_t current_interrupt = GIC_CPU->GICC_IAR;

        // Turn on interrupts to allow for preemption.
        BP_EnableIRQs();

        uint32_t interrupt_id = current_interrupt & GIC_CPU_GICC_IAR_INTERRUPT_ID_Msk;
        if (interrupt_id >= INTERRUPT_COUNT) {
            break;
        }

        void(* handler)(void) = interrupt_handlers[interrupt_id];
        if (handler == NULL) {
            // Unhandled interrupt. Read interrupt_id from GDB to find out the mistake.
            while(true) {}
        }
        handler();

        // Turn off interrupts while we do housekeeping.
        BP_DisableIRQs();

        GIC_CPU->GICC_EOIR = current_interrupt;
    }
}

// Minimum priority is the highest priority value. (0 is highest priority)
void BP_SetMinPriority(uint8_t priority) {
    GIC_CPU->GICC_PMR_b.PRIORITY = priority;
}

// We mimic the NVIC used in Cortex M SoCs.
void BP_EnableIRQ(IRQn_Type IRQn) {
    volatile uint8_t* targets = (volatile uint8_t*) &GIC_DIST->GICD_ITARGETSR;
    targets[IRQn] |= 1 << CPU_Index();
    volatile uint32_t* enabled = (volatile uint32_t*) &GIC_DIST->GICD_ISENABLER;
    enabled[IRQn / 32] = 1 << (IRQn % 32);
}

bool BP_GetEnableIRQ(IRQn_Type IRQn) {
    volatile uint8_t* targets = (volatile uint8_t*) &GIC_DIST->GICD_ITARGETSR;
    return (targets[IRQn] & (1 << CPU_Index())) != 0;
}

void BP_DisableIRQ(IRQn_Type IRQn) {
    volatile uint8_t* targets = (volatile uint8_t*) &GIC_DIST->GICD_ITARGETSR;
    targets[IRQn] &= ~(1 << CPU_Index());
}

bool BP_GetPendingIRQ(IRQn_Type IRQn) {
    volatile uint32_t* pending = (volatile uint32_t*) &GIC_DIST->GICD_ISPENDR;
    return (pending[IRQn / 32] & (1 << (IRQn % 32))) != 0;
}

void BP_SetPendingIRQ(IRQn_Type IRQn) {
    volatile uint32_t* set_pending = (volatile uint32_t*) &GIC_DIST->GICD_ISPENDR;
    set_pending[IRQn / 32] = 1 << (IRQn % 32);   
}

void BP_ClearPendingIRQ(IRQn_Type IRQn)  {
    volatile uint32_t* clear_pending = (volatile uint32_t*) &GIC_DIST->GICD_ICPENDR;
    clear_pending[IRQn / 32] = 1 << (IRQn % 32);
}

bool BP_GetActive(IRQn_Type IRQn) {
    volatile uint32_t* active = (volatile uint32_t*) &GIC_DIST->GICD_ISACTIVER;
    return (active[IRQn / 32] & (1 << (IRQn % 32))) != 0;
}

void BP_SetPriority(IRQn_Type IRQn, uint8_t priority) {
    volatile uint8_t* irq_priority = (volatile uint8_t*) &GIC_DIST->GICD_IPRIORITYR;
    irq_priority[IRQn] = priority;
}

uint8_t BP_GetPriority(IRQn_Type IRQn) {
    volatile uint8_t* irq_priority = (volatile uint8_t*) &GIC_DIST->GICD_IPRIORITYR;
    return irq_priority[IRQn];
}
