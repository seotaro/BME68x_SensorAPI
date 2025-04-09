/**
 * Copyright (C) 2023 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

#include <stdio.h>
#include <stdlib.h>

#include "bme68x.h"
#include "common.h"

/***********************************************************************/
/*                         Test code                                   */
/***********************************************************************/

int main(void)
{
    struct bme68x_dev bme;
    int8_t rslt;

    /* Interface preference - using I2C for Raspberry Pi */
    rslt = bme68x_interface_init(&bme);
    bme68x_check_rslt("bme68x_interface_init", rslt);

    rslt = bme68x_init(&bme);
    bme68x_check_rslt("bme68x_init", rslt);

    rslt = bme68x_selftest_check(&bme);
    bme68x_check_rslt("bme68x_selftest_check", rslt);

    if (rslt == BME68X_OK)
    {
        printf("Self-test passed\n");
    }

    if (rslt == BME68X_E_SELF_TEST)
    {
        printf("Self-test failed\n");
    }


    return rslt;
}