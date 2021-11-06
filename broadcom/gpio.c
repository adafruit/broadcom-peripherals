#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "broadcom/defines.h"


void gpio_set_pull(uint8_t pin_number, BP_PULL_Enum pull) {
    volatile uint32_t* pupd = &GPIO->GPIO_PUP_PDN_CNTRL_REG0;
    uint32_t shift = (pin_number % 16) * 2;
    uint32_t mask = 0x3 << shift;
    uint32_t current = pupd[pin_number / 16];
    pupd[pin_number / 16] = (current & ~mask) | (pull & 0x3) << shift;
}

BP_PULL_Enum gpio_get_pull(uint8_t pin_number) {
    volatile uint32_t* pupd = &GPIO->GPIO_PUP_PDN_CNTRL_REG0;
    return (pupd[pin_number / 16] >> ((pin_number % 16) * 2)) & 0x3;
}

void gpio_set_function(uint8_t pin_number, uint8_t function) {
    volatile uint32_t* fsel = &GPIO->GPFSEL0;
    uint32_t start = fsel[pin_number / 10];
    size_t shift = (pin_number % 10) * 3;
    uint32_t mask = 0x7 << shift;
    uint32_t new = (start & ~mask) | ((function & 0x7) << shift);
    fsel[pin_number / 10] = new;
}

bool gpio_get_value(uint8_t pin_number) {
    volatile const uint32_t* lev = &GPIO->GPLEV0;
    return (lev[pin_number / 32] & (1 << (pin_number % 32))) != 0;
}

void gpio_set_value(uint8_t pin_number, bool value) {
    volatile uint32_t* lev;
    if (value) {
        lev = &GPIO->GPSET0;
    } else {
        lev = &GPIO->GPCLR0;
    }
    lev[pin_number / 32] = 1 << (pin_number % 32);  
}
