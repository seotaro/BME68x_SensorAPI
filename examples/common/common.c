/**
 * Copyright (C) 2023 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 /**
 * Modified by Taro Seo
 * 
 * Original source: https://github.com/BoschSensortec/BME68x-Sensor-API
 * Changes:
 * - Modified for Raspberry Pi compatibility
 * - Removed COINES implementations
 * - Removed SPI implementations
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include "bme68x.h"

/******************************************************************************/
/*!                Static variable definition                                 */
/*! I2C device file handle */
static int i2c_fd = -1;

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * I2C read function
 */
BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    
    if (i2c_fd < 0) {
        return -1;
    }
    
    if (ioctl(i2c_fd, I2C_SLAVE, dev_addr) < 0) {
        return -1;
    }
    
    if (write(i2c_fd, &reg_addr, 1) != 1) {
        return -1;
    }
    
    if (read(i2c_fd, reg_data, len) != len) {
        return -1;
    }
    
    return 0;
}

BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    uint8_t *buf = malloc(len + 1);
    
    if (buf == NULL) {
        return -1;
    }
    
    if (i2c_fd < 0) {
        free(buf);
        return -1;
    }
    
    if (ioctl(i2c_fd, I2C_SLAVE, dev_addr) < 0) {
        free(buf);
        return -1;
    }
    
    buf[0] = reg_addr;
    memcpy(buf + 1, reg_data, len);
    
    if (write(i2c_fd, buf, len + 1) != len + 1) {
        free(buf);
        return -1;
    }
    
    free(buf);
    return 0;
}

/*!
 * Delay function
 */
void bme68x_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

void bme68x_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BME68X_OK:

            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            printf("API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            printf("API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            printf("API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}

int8_t bme68x_interface_init(struct bme68x_dev *bme)
{
    int8_t rslt = BME68X_OK;
    static uint8_t dev_addr;
    
    if (bme != NULL) {
        // dev_addr = BME68X_I2C_ADDR_LOW;     // 0x76
        dev_addr = BME68X_I2C_ADDR_HIGH;    // 0x77
        i2c_fd = open("/dev/i2c-1", O_RDWR);
        if (i2c_fd < 0) {
            perror("Failed to open I2C device");
            return -1;
        }
        
        bme->read = bme68x_i2c_read;
        bme->write = bme68x_i2c_write;
        bme->intf = BME68X_I2C_INTF;
        
        bme->delay_us = bme68x_delay_us;
        bme->intf_ptr = &dev_addr;
        bme->amb_temp = 25; /* The ambient temperature in deg C is used for defining the heater temperature */
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}


// Function to get current time in milliseconds
uint32_t get_millis(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

