#include <stdbool.h>
#include <stdint.h>


#include "mmu.h"

// Each entry is a gig.
uint64_t level_1_table[512] __attribute__((aligned(4096)));

// First gig is stack, code, rodata then data, bss, ram and then GPU.
uint64_t level_2_0x0_0000_0000_to_0x0_4000_0000[512] __attribute__((aligned(4096)));

#if BCM_VERSION == 2711
// Third gig has peripherals
uint64_t level_2_0x0_c000_0000_to_0x1_0000_0000[512] __attribute__((aligned(4096)));
#endif

void setup_mmu_flat_map(void) {
    #if BCM_VERSION == 2837
    // First two MB 0x0000_0000 to 0x0020_0000
    level_2_0x0_0000_0000_to_0x0_4000_0000[0] = 0x0000000000000000 | 
                                                MM_DESCRIPTOR_MAIR_INDEX(MT_READONLY) |
                                                MM_DESCRIPTOR_INNER_SHAREABLE |
                                                MM_DESCRIPTOR_ACCESS_FLAG |
                                                MM_DESCRIPTOR_BLOCK |
                                                MM_DESCRIPTOR_VALID; 
    for (uint64_t i = 1; i < 256 - 8; i++) {
        level_2_0x0_0000_0000_to_0x0_4000_0000[i] = (0x0000000000000000 + (i << 21)) |
                                                    MM_DESCRIPTOR_EXECUTE_NEVER |
                                                    MM_DESCRIPTOR_MAIR_INDEX(MT_READONLY) |
                                                    MM_DESCRIPTOR_INNER_SHAREABLE |
                                                    MM_DESCRIPTOR_ACCESS_FLAG |
                                                    MM_DESCRIPTOR_BLOCK |
                                                    MM_DESCRIPTOR_VALID;
    }
    // Last 16 MB of 512MB RAM are shared with the GPU.
    for (uint64_t i = 256 - 8; i < 256; i++) {
        level_2_0x0_0000_0000_to_0x0_4000_0000[i] = (0x0000000000000000 + (i << 21)) |
                                                    MM_DESCRIPTOR_EXECUTE_NEVER |
                                                    MM_DESCRIPTOR_MAIR_INDEX(MT_READONLY) |
                                                    MM_DESCRIPTOR_INNER_SHAREABLE |
                                                    MM_DESCRIPTOR_ACCESS_FLAG |
                                                    MM_DESCRIPTOR_BLOCK |
                                                    MM_DESCRIPTOR_VALID;
    }

    // Set peripherals to register access. 0x3f00_0000 to 0x4000_0000
    for (uint64_t i = 504; i < 512; i++) {
        level_2_0x0_0000_0000_to_0x0_4000_0000[i] = (0x000000000000000 + (i << 21)) |
                                                    MM_DESCRIPTOR_EXECUTE_NEVER |
                                                    MM_DESCRIPTOR_MAIR_INDEX(MT_DEVICE_nGnRnE) |
                                                    MM_DESCRIPTOR_ACCESS_FLAG |
                                                    MM_DESCRIPTOR_BLOCK |
                                                    MM_DESCRIPTOR_VALID;
    }
    level_1_table[0] = ((uint64_t) level_2_0x0_0000_0000_to_0x0_4000_0000) |
                       MM_DESCRIPTOR_TABLE |
                       MM_DESCRIPTOR_VALID;
    #endif
    #if BCM_VERSION == 2711
    // First two MB
    level_2_0x0_0000_0000_to_0x0_4000_0000[0] = 0x0000000000000000 | 
                                                MM_DESCRIPTOR_MAIR_INDEX(MT_READONLY) |
                                                MM_DESCRIPTOR_INNER_SHAREABLE |
                                                MM_DESCRIPTOR_ACCESS_FLAG |
                                                MM_DESCRIPTOR_BLOCK |
                                                MM_DESCRIPTOR_VALID; 
    for (uint64_t i = 1; i < 512 - 8; i++) {
        level_2_0x0_0000_0000_to_0x0_4000_0000[i] = (0x0000000000000000 + (i << 21)) |
                                                    MM_DESCRIPTOR_EXECUTE_NEVER |
                                                    MM_DESCRIPTOR_MAIR_INDEX(MT_READONLY) |
                                                    MM_DESCRIPTOR_INNER_SHAREABLE |
                                                    MM_DESCRIPTOR_ACCESS_FLAG |
                                                    MM_DESCRIPTOR_BLOCK |
                                                    MM_DESCRIPTOR_VALID;
    }
    // Last 16 MB are shared with the GPU.
    for (uint64_t i = 512 - 8; i < 512; i++) {
        level_2_0x0_0000_0000_to_0x0_4000_0000[i] = (0x0000000000000000 + (i << 21)) |
                                                    MM_DESCRIPTOR_EXECUTE_NEVER |
                                                    MM_DESCRIPTOR_MAIR_INDEX(MT_READONLY) |
                                                    MM_DESCRIPTOR_INNER_SHAREABLE |
                                                    MM_DESCRIPTOR_ACCESS_FLAG |
                                                    MM_DESCRIPTOR_BLOCK |
                                                    MM_DESCRIPTOR_VALID;
    }
    level_1_table[0] = ((uint64_t) level_2_0x0_0000_0000_to_0x0_4000_0000) |
                       MM_DESCRIPTOR_TABLE |
                       MM_DESCRIPTOR_VALID;

    // Set peripherals to register access.
    for (uint64_t i = 480; i < 512; i++) {
        level_2_0x0_c000_0000_to_0x1_0000_0000[i] = (0x00000000c0000000 + (i << 21)) |
                                                    MM_DESCRIPTOR_EXECUTE_NEVER |
                                                    MM_DESCRIPTOR_MAIR_INDEX(MT_DEVICE_nGnRnE) |
                                                    MM_DESCRIPTOR_ACCESS_FLAG |
                                                    MM_DESCRIPTOR_BLOCK |
                                                    MM_DESCRIPTOR_VALID;
    }
    level_1_table[3] = ((uint64_t) level_2_0x0_c000_0000_to_0x1_0000_0000) |
                       MM_DESCRIPTOR_TABLE |
                       MM_DESCRIPTOR_VALID;
    #endif
    uint64_t mair = MAIR_VALUE;
    uint64_t tcr = TCR_VALUE;
    uint64_t ttbr0 = ((uint64_t) level_1_table) | MM_TTBR_CNP;
    uint64_t sctlr = 0;
    __asm__ volatile (
        // Clear the TLB
        "TLBI VMALLE1\n\t"
        // Set MAIR
        "MSR MAIR_EL1, %[mair]\n\t"
        // Set TTBR0
        "MSR TTBR0_EL1, %[ttbr0]\n\t"
        // Set TCR
        "MSR TCR_EL1, %[tcr]\n\t"
        // The ISB forces these changes to be seen before the MMU is enabled.
        "ISB\n\t"
        // Read System Control Register configuration data
        "MRS %[sctlr], SCTLR_EL1\n\t"
        // Set [M] bit and enable the MMU.
        "ORR %[sctlr], %[sctlr], #0x1\n\t"
        "dsb ish\n\t"
        "isb\n\t"
        "MSR SCTLR_EL1, %[sctlr]\n\t"
        // The ISB forces these changes to be seen by the next instruction
        "ISB\n\t"
        : /* No outputs. */
        : [mair] "r" (mair),
          [tcr] "r" (tcr),
          [ttbr0] "r" (ttbr0),
          [sctlr] "r" (sctlr)
    );
}
