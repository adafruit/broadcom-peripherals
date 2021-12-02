#pragma once

#include "broadcom/defines.h"

typedef enum {
    GPIO_FUNCTION_INPUT,
    GPIO_FUNCTION_OUTPUT,
    GPIO_FUNCTION_ALT5,
    GPIO_FUNCTION_ALT4,
    GPIO_FUNCTION_ALT0,
    GPIO_FUNCTION_ALT1,
    GPIO_FUNCTION_ALT2,
    GPIO_FUNCTION_ALT3,
} BP_Function_Enum;

// Map the alt number to the actual value.
extern BP_Function_Enum FSEL_VALUES[6];

void gpio_set_function(uint8_t pin_number, BP_Function_Enum function);
void gpio_set_pull(uint8_t pin_number, BP_PULL_Enum pull);
BP_PULL_Enum gpio_get_pull(uint8_t pin_number);

bool gpio_get_value(uint8_t pin_number);
void gpio_set_value(uint8_t pin_number, bool value);
