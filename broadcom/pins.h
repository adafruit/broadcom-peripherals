/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Scott Shawcroft for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

typedef enum {
    PIN_FUNCTION_NONE = 0,
    PIN_FUNCTION_I2C = 1,
    PIN_FUNCTION_SPI = 2,
    PIN_FUNCTION_UART = 3
} pin_function_type_t;

typedef enum {
    I2C_FUNCTION_SDA = 0,
    I2C_FUNCTION_SCL = 1
} pin_i2c_function_t;

typedef enum {
    SPI_FUNCTION_SCLK = 0,
    SPI_FUNCTION_MOSI = 1,
    SPI_FUNCTION_MISO = 2,
    SPI_FUNCTION_CE0 = 3,
    SPI_FUNCTION_CE1 = 4,
    SPI_FUNCTION_CE2 = 5
} pin_spi_function_t;

typedef enum {
    UART_FUNCTION_TXD = 0,
    UART_FUNCTION_RXD = 1,
    UART_FUNCTION_CTS = 2,
    UART_FUNCTION_RTS = 3
} pin_uart_function_t;

typedef struct {
    uint8_t type;
    uint8_t index;
    uint8_t function;
} pin_function_t;

typedef struct {
    PIN_PREFIX_FIELDS
    uint8_t number;
    pin_function_t[6] functions;
} mcu_pin_obj_t;
