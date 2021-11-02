
#include <stdint.h>

#include "broadcom/caches.h"

uint64_t clidr;
uint64_t cache_type;
uint64_t pmu_id0 = 0;
uint64_t pmu_id1 = 0;

static uint64_t bytes_per_data_line;

void init_caches(void) {
    uint64_t ctr;
    uint64_t sctlr = 0;
    __asm__ volatile (
        // The ISB forces these changes to be seen before the MMU is enabled.
        "ISB\n\t"
        // Load the smallest cache line size from the cache type register.
        "MRS %[ctr], CTR_EL0\n\t"
        // Read System Control Register configuration data
        "MRS %[sctlr], SCTLR_EL1\n\t"
        // Turn on the instruction cache
        "ORR %[sctlr], %[sctlr], #0x1000\n\t"
        // Turn on the data cache
        "ORR %[sctlr], %[sctlr], #0x4\n\t"
        "MSR SCTLR_EL1, %[sctlr]\n\t"
        // The ISB forces these changes to be seen by the next instruction
        "ISB\n\t"
        : [ctr] "=r" (ctr)
        : [sctlr] "r" (sctlr)
        : "memory"
    );
    // Convert the power of two cache line size in words to bytes.
    bytes_per_data_line = (1 << ((ctr >> 16) & 0xf)) * sizeof(uint32_t);
}

// Writes values from the cache back into memory but keep a copy in the cache.
void data_clean(volatile void* starting_address, size_t size) {
    for (size_t offset = 0; offset < size + bytes_per_data_line; offset += bytes_per_data_line) {
        volatile void* ptr = starting_address + offset;
        __asm__ volatile("dc cvac, %[ptr]" : : [ptr] "r" (ptr));
    }
    __asm__ volatile("isb");
    __asm__ volatile("dsb sy");
}

// Writes values from the cache back into memory and remove it from the cache.
void data_clean_and_invalidate(volatile void* starting_address, size_t size) {
    for (size_t offset = 0; offset < size; offset += bytes_per_data_line) {
        __asm__ volatile("dc civac, %[ptr]" : : [ptr] "r" (starting_address + offset));
    }
    __asm__ volatile("isb");
    __asm__ volatile("dsb sy");
}

// Remove values from the cache because the value in memory may have changed.
void data_invalidate(volatile void* starting_address, size_t size) {
    for (size_t offset = 0; offset < size + bytes_per_data_line; offset += bytes_per_data_line) {
        __asm__ volatile("dc ivac, %[ptr]" : : [ptr] "r" (starting_address + offset): "memory");
    }
    __asm__ volatile("isb");
    __asm__ volatile("dsb sy" : : : "memory");
}
