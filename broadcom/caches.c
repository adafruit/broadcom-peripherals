
#include <stdint.h>

#include "broadcom/caches.h"


#include "py/runtime.h"

uint64_t clidr;
uint64_t cache_type;
uint64_t pmu_id0 = 0;
uint64_t pmu_id1 = 0;

static uint64_t bytes_per_data_line;

void init_caches(void) {
    uint64_t temp_clidr;
    uint64_t temp_ctr;

    // // Invalidate 

    // // invalidate L1 data cache
    // for (unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
    // {
    //     for (unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
    //     {
    //         u64 nSetWayLevel =   nWay << L1_SETWAY_WAY_SHIFT
    //                    | nSet << L1_SETWAY_SET_SHIFT
    //                    | 0 << SETWAY_LEVEL_SHIFT;

    //         asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
    //     }
    // }

    // // invalidate L2 unified cache
    // for (unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
    // {
    //     for (unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
    //     {
    //         u64 nSetWayLevel =   nWay << L2_SETWAY_WAY_SHIFT
    //                    | nSet << L2_SETWAY_SET_SHIFT
    //                    | 1 << SETWAY_LEVEL_SHIFT;

    //         asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
    //     }
    // }

    // asm

    // Cache miss ratio of largest cache (L2)
    // 0x0033 LL_CACHE_MISS 0x0032 LL_CACHE
    uint64_t p0 = 0x001b;
    uint64_t p1 = 0x0003;
    uint64_t three = 0x3f | (1 << 31);
    uint64_t sctlr = 0;
    uint64_t pmcr = 1;
    uint64_t pmcc = 0;
    uint64_t id0 = 0;
    uint64_t id1 = 0;
    __asm__ volatile (
        // The ISB forces these changes to be seen before the MMU is enabled.
        "ISB\n\t"
        "MRS %[clidr], CLIDR_EL1\n\t"
        "MRS %[ctr], CTR_EL0\n\t"
        // Initialize performance counters
        "MRS %[id0], PMCEID0_EL0\n\t"
        "MRS %[id1], PMCEID1_EL0\n\t"
        "MSR PMEVTYPER4_EL0, %[p0]\n\t"
        "MSR PMEVTYPER5_EL0, %[p1]\n\t"
        // Enable the last two counters
        "MSR PMCNTENSET_EL0, %[three]\n\t"
        "MSR PMCCFILTR_EL0, %[pmcc]\n\t"
        "MSR PMCR_EL0, %[pmcr]\n\t" // Clear counters and enable
        "dsb ish\n\t"
        "isb \n\t"
        // Read System Control Register configuration data
        "MRS %[sctlr], SCTLR_EL1\n\t"
        // Turn on the instruction cache
        "ORR %[sctlr], %[sctlr], #0x1000\n\t"
        // Turn on the data cache
        "ORR %[sctlr], %[sctlr], #0x4\n\t"
        "MSR SCTLR_EL1, %[sctlr]\n\t"
        // The ISB forces these changes to be seen by the next instruction
        "ISB\n\t"
        : [clidr] "=r" (temp_clidr), [ctr] "=r" (temp_ctr)
        : [sctlr] "r" (sctlr),
          [p0] "r" (p0),
          [p1] "r" (p1),
          [three] "r" (three),
          [pmcr] "r" (pmcr),
          [pmcc] "r" (pmcc),
          [id0] "r" (id0),
          [id1] "r" (id1)
        : "memory"
    );
    clidr = temp_clidr;
    cache_type = temp_ctr;
    bytes_per_data_line = (1 << ((cache_type >> 16) & 0xf)) * sizeof(uint32_t);
    pmu_id0 = id0;
    pmu_id1 = id1;
}

void cache_miss_info(uint32_t* miss, uint32_t* total, uint64_t* count) {
    uint64_t p0 = 0;
    uint64_t p1 = 0;
    uint64_t p2 = 0;
    __asm__ volatile (
        "MRS %[p0], PMEVCNTR4_EL0\n\t"
        "MRS %[p1], PMEVCNTR5_EL0\n\t"
        "MRS %[p2], PMCCNTR_EL0\n\t"
        : [p0] "=r" (p0), [p1] "=r" (p1), [p2] "=r" (p2)
        :
    );
    mp_printf(&mp_plat_print, "count %d\n", p2);
    *miss = p0;
    *total = p1;
    *count = p2;
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
