#pragma once

// To ensure we loaded the correct data, we must have the CPU wait for pending
// reads before moving onto reads from other peripherals. This is done with a
// memory barrier instruction.
#define COMPLETE_MEMORY_READS __asm__ volatile ("dsb sy")
