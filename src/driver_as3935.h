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
 * @file      driver_as3935.h
 * @brief     driver as3935 header file
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

#ifndef DRIVER_AS3935_H
#define DRIVER_AS3935_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup as3935_driver as3935 driver function
 * @brief    as3935 driver modules
 * @{
 */

/**
 * @addtogroup as3935_basic_driver
 * @{
 */

/**
 * @brief as3935 interface enumeration definition
 */
typedef enum
{
    AS3935_INTERFACE_IIC = 0x00,        /**< iic interface */
    AS3935_INTERFACE_SPI = 0x01,        /**< spi interface */
} as3935_interface_t;

/**
 * @brief as3935 address enumeration definition
 */
typedef enum
{
    AS3935_ADDRESS_1 = 0x02,        /**< addr1 - add0 -> low - high */
    AS3935_ADDRESS_2 = 0x04,        /**< addr1 - add0 -> high - low */
    AS3935_ADDRESS_3 = 0x06,        /**< addr1 - add0 -> high - high */
} as3935_address_t;

/**
 * @brief as3935 bool enumeration definition
 */
typedef enum
{
    AS3935_BOOL_FALSE = 0x00,        /**< false */
    AS3935_BOOL_TRUE  = 0x01,        /**< true */
} as3935_bool_t;

/**
 * @brief as3935 afe gain enumeration definition
 */
typedef enum
{
    AS3935_AFE_GAIN_INDOOR  = 0x12,        /**< indoor */
    AS3935_AFE_GAIN_OUTDOOR = 0x0E,        /**< outdoor */
} as3935_afe_gain_t;

/**
 * @brief as3935 noise floor level enumeration definition
 */
typedef enum
{
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_390_INDOOR_28   = 0x0,        /**< outdoor 390 μVrms, indoor 28 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_630_INDOOR_45   = 0x1,        /**< outdoor 630 μVrms, indoor 45 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_860_INDOOR_62   = 0x2,        /**< outdoor 860 μVrms, indoor 62 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1100_INDOOR_78  = 0x3,        /**< outdoor 1100 μVrms, indoor 78 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1140_INDOOR_95  = 0x4,        /**< outdoor 1140 μVrms, indoor 95 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1570_INDOOR_112 = 0x5,        /**< outdoor 1570 μVrms, indoor 112 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1800_INDOOR_130 = 0x6,        /**< outdoor 1800 μVrms, indoor 130 μVrms */
    AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_2000_INDOOR_146 = 0x7,        /**< outdoor 2000 μVrms, indoor 146 μVrms */
} as3935_noise_floor_level_t;

/**
 * @brief as3935 lightning min number enumeration definition
 */
typedef enum
{
    AS3935_LIGHTNING_MIN_NUMBER_1  = 0x0,        /**< 1 minimum number of lightning */
    AS3935_LIGHTNING_MIN_NUMBER_5  = 0x1,        /**< 5 minimum number of lightning */
    AS3935_LIGHTNING_MIN_NUMBER_9  = 0x2,        /**< 9 minimum number of lightning */
    AS3935_LIGHTNING_MIN_NUMBER_16 = 0x3,        /**< 16 minimum number of lightning */
} as3935_lightning_min_number_t;

/**
 * @brief as3935 frequency division ration enumeration definition
 */
typedef enum
{
    AS3935_FREQUENCY_DIVISION_RATION_16  = 0x0,        /**< division ratio 16 */
    AS3935_FREQUENCY_DIVISION_RATION_32  = 0x1,        /**< division ratio 32 */
    AS3935_FREQUENCY_DIVISION_RATION_64  = 0x2,        /**< division ratio 64 */
    AS3935_FREQUENCY_DIVISION_RATION_128 = 0x3,        /**< division ratio 128 */
} as3935_frequency_division_ration_t;

/**
 * @brief as3935 interrupt status enumeration definition
 */
typedef enum
{
    AS3935_INTERRUPT_STATUS_NOISE_LEVEL_TOO_HIGH = 0x01,        /**< noise level too high */
    AS3935_INTERRUPT_STATUS_DISTURBER_DETECTED   = 0x04,        /**< disturber detected */
    AS3935_INTERRUPT_STATUS_LIGHTNING_INTERRUPT  = 0x08,        /**< lightning interrupt */
} as3935_interrupt_status_t;

/**
 * @brief as3935 handle structure definition
 */
typedef struct as3935_handle_s
{
    uint8_t iic_addr;                                                                   /**< iic address */
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*spi_init)(void);                                                          /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                                        /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);                       /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);                      /**< point to a spi_write function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    uint8_t inited;                                                                     /**< inited flag */
    uint8_t iic_spi;                                                                    /**< iic spi interface type */
} as3935_handle_t;

/**
 * @brief as3935 information structure definition
 */
typedef struct as3935_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} as3935_info_t;

/**
 * @}
 */

/**
 * @defgroup as3935_link_driver as3935 link driver function
 * @brief    as3935 link driver modules
 * @ingroup  as3935_driver
 * @{
 */

/**
 * @brief     initialize as3935_handle_t structure
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] STRUCTURE as3935_handle_t
 * @note      none
 */
#define DRIVER_AS3935_LINK_INIT(HANDLE, STRUCTURE)              memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_IIC_INIT(HANDLE, FUC)                (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_IIC_DEINIT(HANDLE, FUC)              (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_IIC_READ(HANDLE, FUC)                (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_IIC_WRITE(HANDLE, FUC)               (HANDLE)->iic_write = FUC

/**
 * @brief     link spi_init function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a spi_init function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_SPI_INIT(HANDLE, FUC)                (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a spi_deinit function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_SPI_DEINIT(HANDLE, FUC)              (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a spi_read function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_SPI_READ(HANDLE, FUC)                (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a spi_write function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_SPI_WRITE(HANDLE, FUC)               (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_DELAY_MS(HANDLE, FUC)                (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_DEBUG_PRINT(HANDLE, FUC)             (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to an as3935 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_AS3935_LINK_RECEIVE_CALLBACK(HANDLE, FUC)        (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup as3935_basic_driver as3935 basic driver function
 * @brief    as3935 basic driver modules
 * @ingroup  as3935_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an as3935 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as3935_info(as3935_info_t *info);

/**
 * @brief     set the chip interface
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] interface chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t as3935_set_interface(as3935_handle_t *handle, as3935_interface_t interface);

/**
 * @brief      get the chip interface
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *interface pointer to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as3935_get_interface(as3935_handle_t *handle, as3935_interface_t *interface);

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t as3935_set_addr_pin(as3935_handle_t *handle, as3935_address_t addr_pin);

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as3935_get_addr_pin(as3935_handle_t *handle, as3935_address_t *addr_pin);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to an as3935 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_irq_handler(as3935_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an as3935 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi or iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t as3935_init(as3935_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an as3935 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t as3935_deinit(as3935_handle_t *handle);

/**
 * @brief      read single lightning energy
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *energy pointer to an energy buffer
 * @return     status code
 *             - 0 success
 *             - 1 read single lightning energy failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_read_single_lightning_energy(as3935_handle_t *handle, uint32_t *energy);

/**
 * @brief      read distance
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *raw pointer to a raw data buffer
 * @param[out] *km pointer to a km data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read distance failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_read_distance(as3935_handle_t *handle, uint8_t *raw, int8_t *km);

/**
 * @brief     set power down
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_power_down(as3935_handle_t *handle, as3935_bool_t enable);

/**
 * @brief      get power down
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power down failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_power_down(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief     set afe gain boost
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] gain afe gain
 * @return    status code
 *            - 0 success
 *            - 1 set afe gain boost failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_afe_gain_boost(as3935_handle_t *handle, as3935_afe_gain_t gain);

/**
 * @brief      get afe gain boost
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *gain pointer to an afe gain buffer
 * @return     status code
 *             - 0 success
 *             - 1 get afe gain boost failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_afe_gain_boost(as3935_handle_t *handle, as3935_afe_gain_t *gain);

/**
 * @brief     set noise floor level
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] level noise floor level
 * @return    status code
 *            - 0 success
 *            - 1 set noise floor level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_noise_floor_level(as3935_handle_t *handle, as3935_noise_floor_level_t level);

/**
 * @brief      get noise floor level
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *level pointer to a noise floor level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get noise floor level failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_noise_floor_level(as3935_handle_t *handle, as3935_noise_floor_level_t *level);

/**
 * @brief     set watchdog threshold
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] threshold watchdog threshold
 * @return    status code
 *            - 0 success
 *            - 1 set watchdog threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 threshold > 0x0F
 * @note      none
 */
uint8_t as3935_set_watchdog_threshold(as3935_handle_t *handle, uint8_t threshold);

/**
 * @brief      get watchdog threshold
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *threshold pointer to a watchdog threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get watchdog threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_watchdog_threshold(as3935_handle_t *handle, uint8_t *threshold);

/**
 * @brief     enable or disable clear statistics
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set clear statistics failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_clear_statistics(as3935_handle_t *handle, as3935_bool_t enable);

/**
 * @brief      get clear statistics status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get clear statistics failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_clear_statistics(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief     set lightning min number
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] number lightning min number
 * @return    status code
 *            - 0 success
 *            - 1 set lightning min number failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_lightning_min_number(as3935_handle_t *handle, as3935_lightning_min_number_t number);

/**
 * @brief      get lightning min number
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *number pointer to a lightning min number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get lightning min number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_lightning_min_number(as3935_handle_t *handle, as3935_lightning_min_number_t *number);

/**
 * @brief     set spike rejection
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] rejection spike rejection
 * @return    status code
 *            - 0 success
 *            - 1 set spike rejection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 rejection > 0x0F
 * @note      none
 */
uint8_t as3935_set_spike_rejection(as3935_handle_t *handle, uint8_t rejection);

/**
 * @brief      get spike rejection
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *rejection pointer to a spike rejection buffer
 * @return     status code
 *             - 0 success
 *             - 1 get spike rejection failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_spike_rejection(as3935_handle_t *handle, uint8_t *rejection);

/**
 * @brief     set frequency division ration
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] division frequency division 
 * @return    status code
 *            - 0 success
 *            - 1 set frequency division ration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_frequency_division_ration(as3935_handle_t *handle, as3935_frequency_division_ration_t division);

/**
 * @brief      get frequency division ration
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *division pointer to a frequency division buffer 
 * @return     status code
 *             - 0 success
 *             - 1 get frequency division ration failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_frequency_division_ration(as3935_handle_t *handle, as3935_frequency_division_ration_t *division);

/**
 * @brief     enable or disable mask disturber
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set mask disturber failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_mask_disturber(as3935_handle_t *handle, as3935_bool_t enable);

/**
 * @brief      get mask disturber status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mask disturber failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_mask_disturber(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief      get interrupt status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_interrupt_status(as3935_handle_t *handle, as3935_interrupt_status_t *status);

/**
 * @brief     enable or disable lc osc display on irq
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set lc osc display on irq failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_lc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t enable);

/**
 * @brief      get lc osc display on irq status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get lc osc display on irq failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_display_lc_osc_on_irq(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief     enable or disable system rc osc display on irq
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set system rc osc display on irq failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_system_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t enable);

/**
 * @brief      get system rc osc display on irq status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get system rc osc display on irq failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_system_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief     enable or disable timer rc osc display on irq
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set timer rc osc display on irq failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_timer_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t enable);

/**
 * @brief      get timer rc osc display on irq status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer rc osc display on irq failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_timer_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief     set internal tuning capacitor
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] cap capacitor
 * @return    status code
 *            - 0 success
 *            - 1 set internal tuning capacitor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 cap > 0xF
 * @note      none
 */
uint8_t as3935_set_internal_tuning_capacitor(as3935_handle_t *handle, uint8_t cap);

/**
 * @brief      get internal tuning capacitor
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *cap pointer to a capacitor buffer
 * @return     status code
 *             - 0 success
 *             - 1 get internal tuning capacitor failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_internal_tuning_capacitor(as3935_handle_t *handle, uint8_t *cap);

/**
 * @brief      get timer rc osc calibration done status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer rc osc calibration done failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_timer_rc_osc_calibration_done(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief      get timer rc osc calibration not ok status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timer rc osc calibration not ok failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_timer_rc_osc_calibration_not_ok(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief      get system rc osc calibration done status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get system rc osc calibration done failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_system_rc_osc_calibration_done(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief      get system rc osc calibration not ok status
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get system rc osc calibration not ok failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_system_rc_osc_calibration_not_ok(as3935_handle_t *handle, as3935_bool_t *enable);

/**
 * @brief     reset to default
 * @param[in] *handle pointer to an as3935 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 reset default failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_reset_default(as3935_handle_t *handle);

/**
 * @brief     calibrate rc osc
 * @param[in] *handle pointer to an as3935 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 calibrate rc osc failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_calibrate_rc_osc(as3935_handle_t *handle);

/**
 * @brief      convert the internal tuning capacitor to the register raw data
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[in]  pf capacitor in pf
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_internal_tuning_capacitor_convert_to_register(as3935_handle_t *handle, float pf, uint8_t *reg);

/**
 * @brief      convert the register raw data to the internal tuning capacitor
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[in]  reg register raw data
 * @param[out] *pf pointer to a pf buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_internal_tuning_capacitor_convert_to_data(as3935_handle_t *handle, uint8_t reg, float *pf);

/**
 * @}
 */

/**
 * @defgroup as3935_extern_driver as3935 extern driver function
 * @brief    as3935 extern driver modules
 * @ingroup  as3935_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t as3935_set_reg(as3935_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t as3935_get_reg(as3935_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
