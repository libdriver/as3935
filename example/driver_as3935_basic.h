/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_as3935_basic.h
 * @brief     driver as3935 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-12-23
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/12/23  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_AS3935_BASIC_H
#define DRIVER_AS3935_BASIC_H

#include "driver_as3935_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup as3935_example_driver as3935 example driver function
 * @brief    as3935 example driver modules
 * @ingroup  as3935_driver
 * @{
 */

/**
 * @brief as3935 basic example default definition
 */
#define AS3935_BASIC_DEFAULT_AFE_GAIN                         AS3935_AFE_GAIN_INDOOR                                /**< indoor */
#define AS3935_BASIC_DEFAULT_NOISE_FLOOR_LEVEL                AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_860_INDOOR_62        /**< outdoor 860 μVrms, indoor 62 μVrms */
#define AS3935_BASIC_DEFAULT_WATCHDOG_THRESHOLD               2                                                     /**< 2 */
#define AS3935_BASIC_DEFAULT_LIGHTNING_MIN_NUMBER             AS3935_LIGHTNING_MIN_NUMBER_1                         /**< 1 minimum number of lightning */
#define AS3935_BASIC_DEFAULT_SPIKE_REJECTION                  2                                                     /**< 2 */
#define AS3935_BASIC_DEFAULT_FREQUENCY_DIVISION_RATION        AS3935_FREQUENCY_DIVISION_RATION_16                   /**< division ratio 16 */
#define AS3935_BASIC_DEFAULT_MASK_DISTURBER                   AS3935_BOOL_FALSE                                     /**< disable */
#define AS3935_BASIC_DEFAULT_CAP                              0                                                     /**< 0pf */

/**
 * @brief  basic irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t as3935_basic_irq_handler(void);

/**
 * @brief     basic example init
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @param[in] *callback pointer to a callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t as3935_basic_init(as3935_interface_t interface, as3935_address_t addr_pin,
                          void (*callback)(uint8_t type));

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t as3935_basic_deinit(void);

/**
 * @brief      basic example read single lightning energy
 * @param[out] *energy pointer to an energy buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t as3935_basic_read_single_lightning_energy(uint32_t *energy);

/**
 * @brief      basic example read distance
 * @param[out] *km pointer to a km data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t as3935_basic_read_distance(int8_t *km);

/**
 * @brief  basic example clear statistics
 * @return status code
 *         - 0 success
 *         - 1 clear statistics failed
 * @note   none
 */
uint8_t as3935_basic_clear_statistics(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
