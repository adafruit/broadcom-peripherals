#pragma once

#include "broadcom/gen/bcm2711_lpa.h"

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

void gpio_set_function(uint8_t pin_number, uint8_t function);
void gpio_set_pull(uint8_t pin_number, BP_PULL_Enum pull);
BP_PULL_Enum gpio_get_pull(uint8_t pin_number);

bool gpio_get_value(uint8_t pin_number);
void gpio_set_value(uint8_t pin_number, bool value);
