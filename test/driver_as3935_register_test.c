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
 * @file      driver_as3935_register_test.c
 * @brief     driver as3935 register test source file
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

#include "driver_as3935_register_test.h"
#include <stdlib.h>

static as3935_handle_t gs_handle;        /**< as3935 handle */

/**
 * @brief     register test
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t as3935_register_test(as3935_interface_t interface, as3935_address_t addr_pin)
{
    uint8_t res;
    uint8_t threshold;
    uint8_t threshold_check;
    uint8_t rejection;
    uint8_t rejection_check;
    uint8_t cap;
    uint8_t cap_check;
    uint8_t reg;
    float pf;
    float pf_check;
    as3935_address_t addr_check;
    as3935_interface_t interface_check;
    as3935_info_t info;
    as3935_bool_t enable;
    as3935_afe_gain_t gain;
    as3935_noise_floor_level_t level;
    as3935_lightning_min_number_t number;
    as3935_frequency_division_ration_t division;
    as3935_interrupt_status_t status;
    
    /* link interface function */
    DRIVER_AS3935_LINK_INIT(&gs_handle, as3935_handle_t);
    DRIVER_AS3935_LINK_IIC_INIT(&gs_handle, as3935_interface_iic_init);
    DRIVER_AS3935_LINK_IIC_DEINIT(&gs_handle, as3935_interface_iic_deinit);
    DRIVER_AS3935_LINK_IIC_READ(&gs_handle, as3935_interface_iic_read);
    DRIVER_AS3935_LINK_IIC_WRITE(&gs_handle, as3935_interface_iic_write);
    DRIVER_AS3935_LINK_SPI_INIT(&gs_handle, as3935_interface_spi_init);
    DRIVER_AS3935_LINK_SPI_DEINIT(&gs_handle, as3935_interface_spi_deinit);
    DRIVER_AS3935_LINK_SPI_READ(&gs_handle, as3935_interface_spi_read);
    DRIVER_AS3935_LINK_SPI_WRITE(&gs_handle, as3935_interface_spi_write);
    DRIVER_AS3935_LINK_DELAY_MS(&gs_handle, as3935_interface_delay_ms);
    DRIVER_AS3935_LINK_DEBUG_PRINT(&gs_handle, as3935_interface_debug_print);
    DRIVER_AS3935_LINK_RECEIVE_CALLBACK(&gs_handle, as3935_interface_receive_callback);
    
    /* get information */
    res = as3935_info(&info);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        as3935_interface_debug_print("as3935: chip is %s.\n", info.chip_name);
        as3935_interface_debug_print("as3935: manufacturer is %s.\n", info.manufacturer_name);
        as3935_interface_debug_print("as3935: interface is %s.\n", info.interface);
        as3935_interface_debug_print("as3935: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        as3935_interface_debug_print("as3935: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        as3935_interface_debug_print("as3935: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        as3935_interface_debug_print("as3935: max current is %0.2fmA.\n", info.max_current_ma);
        as3935_interface_debug_print("as3935: max temperature is %0.1fC.\n", info.temperature_max);
        as3935_interface_debug_print("as3935: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    as3935_interface_debug_print("as3935: start register test.\n");
    
    /* as3935_set_interface/as3935_get_interface test */
    as3935_interface_debug_print("as3935: as3935_set_interface/as3935_get_interface test.\n");
    
    /* set iic interface */
    res = as3935_set_interface(&gs_handle, AS3935_INTERFACE_IIC);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set interface failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: set interface iic.\n");
    res = as3935_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get interface failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: check interface %s.\n", (interface_check == AS3935_INTERFACE_IIC) ? "ok" : "error");
    
    /* set spi interface */
    res = as3935_set_interface(&gs_handle, AS3935_INTERFACE_SPI);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set interface failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: set interface spi.\n");
    res = as3935_get_interface(&gs_handle, &interface_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get interface failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: check interface %s.\n", (interface_check == AS3935_INTERFACE_SPI) ? "ok" : "error");
    
    /* as3935_set_addr_pin/as3935_get_addr_pin test */
    as3935_interface_debug_print("as3935: as3935_set_addr_pin/as3935_get_addr_pin test.\n");

    /* address1 */
    res = as3935_set_addr_pin(&gs_handle, AS3935_ADDRESS_1);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set addr pin failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: set addr pin address1.\n");
    res = as3935_get_addr_pin(&gs_handle, &addr_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get addr pin failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: check addr pin %s.\n", (AS3935_ADDRESS_1 == addr_check) ? "ok" : "error");
    
    /* address2 */
    res = as3935_set_addr_pin(&gs_handle, AS3935_ADDRESS_2);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set addr pin failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: set addr pin address2.\n");
    res = as3935_get_addr_pin(&gs_handle, &addr_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get addr pin failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: check addr pin %s.\n", (AS3935_ADDRESS_2 == addr_check) ? "ok" : "error");
    
    /* address3 */
    res = as3935_set_addr_pin(&gs_handle, AS3935_ADDRESS_3);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set addr pin failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: set addr pin address3.\n");
    res = as3935_get_addr_pin(&gs_handle, &addr_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get addr pin failed.\n");
       
        return 1;
    }
    as3935_interface_debug_print("as3935: check addr pin %s.\n", (AS3935_ADDRESS_3 == addr_check) ? "ok" : "error");
    
    /* set the interface */
    res = as3935_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set interface failed.\n");
       
        return 1;
    }
    
    /* set the address pin */
    res = as3935_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set addr pin failed.\n");
       
        return 1;
    }
    
    /* init the as3935 */
    res = as3935_init(&gs_handle);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: init failed.\n");
       
        return 1;
    }
    
    /* as3935_set_power_down/as3935_get_power_down test */
    as3935_interface_debug_print("as3935: as3935_set_power_down/as3935_get_power_down test.\n");
    
    /* enable power down */
    res = as3935_set_power_down(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set power down failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: enable power down.\n");
    res = as3935_get_power_down(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get power down failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check power down %s.\n", (enable == AS3935_BOOL_TRUE) ? "ok" : "error");
    
    /* disable power down */
    res = as3935_set_power_down(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set power down failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: disable power down.\n");
    res = as3935_get_power_down(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get power down failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check power down %s.\n", (enable == AS3935_BOOL_FALSE) ? "ok" : "error");
    
    /* as3935_set_afe_gain_boost/as3935_get_afe_gain_boost test */
    as3935_interface_debug_print("as3935: as3935_set_afe_gain_boost/as3935_get_afe_gain_boost test.\n");
    
    /* set afe gain boost indoor */
    res = as3935_set_afe_gain_boost(&gs_handle, AS3935_AFE_GAIN_INDOOR);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set afe gain boost failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set afe gain boost indoor.\n");
    res = as3935_get_afe_gain_boost(&gs_handle, &gain);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get afe gain boost failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check afe gain boost %s.\n", (gain == AS3935_AFE_GAIN_INDOOR) ? "ok" : "error");
    
    /* set afe gain boost outdoor */
    res = as3935_set_afe_gain_boost(&gs_handle, AS3935_AFE_GAIN_OUTDOOR);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set afe gain boost failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set afe gain boost outdoor.\n");
    res = as3935_get_afe_gain_boost(&gs_handle, &gain);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get afe gain boost failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check afe gain boost %s.\n", (gain == AS3935_AFE_GAIN_OUTDOOR) ? "ok" : "error");
    
    /* as3935_set_noise_floor_level/as3935_get_noise_floor_level test */
    as3935_interface_debug_print("as3935: as3935_set_noise_floor_level/as3935_get_noise_floor_level test.\n");
    
    /* set noise floor level outdoor 390 μVrms, indoor 28 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_390_INDOOR_28);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 390 μVrms, indoor 28 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_390_INDOOR_28) ? "ok" : "error");
    
    /* set noise floor level outdoor 630 μVrms, indoor 45 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_630_INDOOR_45);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 630 μVrms, indoor 45 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_630_INDOOR_45) ? "ok" : "error");
    
    /* set noise floor level outdoor 860 μVrms, indoor 62 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_860_INDOOR_62);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 860 μVrms, indoor 62 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_860_INDOOR_62) ? "ok" : "error");
    
    /* set noise floor level outdoor 1100 μVrms, indoor 78 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1100_INDOOR_78);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 1100 μVrms, indoor 78 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1100_INDOOR_78) ? "ok" : "error");
    
    /* set noise floor level outdoor 1140 μVrms, indoor 95 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1140_INDOOR_95);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 1140 μVrms, indoor 95 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1140_INDOOR_95) ? "ok" : "error");
    
    /* set noise floor level outdoor 1570 μVrms, indoor 112 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1570_INDOOR_112);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 1570 μVrms, indoor 112 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1570_INDOOR_112) ? "ok" : "error");
    
    /* set noise floor level outdoor 1800 μVrms, indoor 130 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1800_INDOOR_130);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 1800 μVrms, indoor 130 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_1800_INDOOR_130) ? "ok" : "error");
    
    /* set noise floor level outdoor 2000 μVrms, indoor 146 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_2000_INDOOR_146);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set noise floor level outdoor 2000 μVrms, indoor 146 μVrms.\n");
    res = as3935_get_noise_floor_level(&gs_handle, &level);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check noise floor level %s.\n", (level == AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_2000_INDOOR_146) ? "ok" : "error");
    
    /* as3935_set_watchdog_threshold/as3935_get_watchdog_threshold test */
    as3935_interface_debug_print("as3935: as3935_set_watchdog_threshold/as3935_get_watchdog_threshold test.\n");
    
    threshold = rand() % 0xFU;
    res = as3935_set_watchdog_threshold(&gs_handle, threshold);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set watchdog threshold failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set watchdog threshold %d.\n", threshold);
    res = as3935_get_watchdog_threshold(&gs_handle, &threshold_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get watchdog threshold failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* as3935_set_clear_statistics/as3935_get_clear_statistics test */
    as3935_interface_debug_print("as3935: as3935_set_clear_statistics/as3935_get_clear_statistics test.\n");
    
    /* enable clear statistics */
    res = as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: enable clear statistics.\n");
    res = as3935_get_clear_statistics(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check clear statistics %s.\n", (enable == AS3935_BOOL_TRUE) ? "ok" : "error");
    
    /* disable clear statistics */
    res = as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: disable clear statistics.\n");
    res = as3935_get_clear_statistics(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check clear statistics %s.\n", (enable == AS3935_BOOL_FALSE) ? "ok" : "error");
    
    /* as3935_set_lightning_min_number/as3935_get_lightning_min_number test */
    as3935_interface_debug_print("as3935: as3935_set_lightning_min_number/as3935_get_lightning_min_number test.\n");
    
    /* set lightning min number 1 */
    res = as3935_set_lightning_min_number(&gs_handle, AS3935_LIGHTNING_MIN_NUMBER_1);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set lightning min number 1.\n");
    res = as3935_get_lightning_min_number(&gs_handle, &number);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check lightning min number %s.\n", (number == AS3935_LIGHTNING_MIN_NUMBER_1) ? "ok" : "error");
    
    /* set lightning min number 5 */
    res = as3935_set_lightning_min_number(&gs_handle, AS3935_LIGHTNING_MIN_NUMBER_5);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set lightning min number 5.\n");
    res = as3935_get_lightning_min_number(&gs_handle, &number);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check lightning min number %s.\n", (number == AS3935_LIGHTNING_MIN_NUMBER_5) ? "ok" : "error");
    
    /* set lightning min number 9 */
    res = as3935_set_lightning_min_number(&gs_handle, AS3935_LIGHTNING_MIN_NUMBER_9);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set lightning min number 9.\n");
    res = as3935_get_lightning_min_number(&gs_handle, &number);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check lightning min number %s.\n", (number == AS3935_LIGHTNING_MIN_NUMBER_9) ? "ok" : "error");
    
    /* set lightning min number 16 */
    res = as3935_set_lightning_min_number(&gs_handle, AS3935_LIGHTNING_MIN_NUMBER_16);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set lightning min number 16.\n");
    res = as3935_get_lightning_min_number(&gs_handle, &number);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check lightning min number %s.\n", (number == AS3935_LIGHTNING_MIN_NUMBER_16) ? "ok" : "error");
    
    /* as3935_set_spike_rejection/as3935_get_spike_rejection test */
    as3935_interface_debug_print("as3935: as3935_set_spike_rejection/as3935_get_spike_rejection test.\n");
    
    rejection = rand() % 0xFU;
    res = as3935_set_spike_rejection(&gs_handle, rejection);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set spike rejection failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set spike rejection %d.\n", rejection);
    res = as3935_get_spike_rejection(&gs_handle, &rejection_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get spike rejection failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check spike rejection %s.\n", (rejection == rejection_check) ? "ok" : "error");
    
    /* as3935_set_frequency_division_ration/as3935_get_frequency_division_ration test */
    as3935_interface_debug_print("as3935: as3935_set_frequency_division_ration/as3935_get_frequency_division_ration test.\n");
    
    /* set frequency division ration 16 */
    res = as3935_set_frequency_division_ration(&gs_handle, AS3935_FREQUENCY_DIVISION_RATION_16);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set frequency division ration 16.\n");
    res = as3935_get_frequency_division_ration(&gs_handle, &division);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check frequency division ration %s.\n", (division == AS3935_FREQUENCY_DIVISION_RATION_16) ? "ok" : "error");
    
    /* set frequency division ration 32 */
    res = as3935_set_frequency_division_ration(&gs_handle, AS3935_FREQUENCY_DIVISION_RATION_32);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set frequency division ration 32.\n");
    res = as3935_get_frequency_division_ration(&gs_handle, &division);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check frequency division ration %s.\n", (division == AS3935_FREQUENCY_DIVISION_RATION_32) ? "ok" : "error");
    
    /* set frequency division ration 64 */
    res = as3935_set_frequency_division_ration(&gs_handle, AS3935_FREQUENCY_DIVISION_RATION_64);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set frequency division ration 64.\n");
    res = as3935_get_frequency_division_ration(&gs_handle, &division);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check frequency division ration %s.\n", (division == AS3935_FREQUENCY_DIVISION_RATION_64) ? "ok" : "error");
    
    /* set frequency division ration 128 */
    res = as3935_set_frequency_division_ration(&gs_handle, AS3935_FREQUENCY_DIVISION_RATION_128);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set frequency division ration 128.\n");
    res = as3935_get_frequency_division_ration(&gs_handle, &division);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check frequency division ration %s.\n", (division == AS3935_FREQUENCY_DIVISION_RATION_128) ? "ok" : "error");
    
    /* as3935_set_mask_disturber/as3935_get_mask_disturber test */
    as3935_interface_debug_print("as3935: as3935_set_mask_disturber/as3935_get_mask_disturber test.\n");
    
    /* enable mask disturber */
    res = as3935_set_mask_disturber(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set mask disturber failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: enable mask disturber.\n");
    res = as3935_get_mask_disturber(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get mask disturber failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check mask disturber %s.\n", (enable == AS3935_BOOL_TRUE) ? "ok" : "error");
    
    /* disable mask disturber */
    res = as3935_set_mask_disturber(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set mask disturber failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: disable mask disturber.\n");
    res = as3935_get_mask_disturber(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get mask disturber failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check mask disturber %s.\n", (enable == AS3935_BOOL_FALSE) ? "ok" : "error");
    
    /* as3935_get_interrupt_status test */
    as3935_interface_debug_print("as3935: as3935_get_interrupt_status test.\n");
    
    /* get interrupt status */
    res = as3935_get_interrupt_status(&gs_handle, &status);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get interrupt status failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: interrupt status is 0x%02X.\n", status);
    
    /* as3935_set_lc_osc_display_on_irq/as3935_get_display_lc_osc_on_irq test */
    as3935_interface_debug_print("as3935: as3935_set_lc_osc_display_on_irq/as3935_get_display_lc_osc_on_irq test.\n");
    
    /* enable lc osc display on irq */
    res = as3935_set_lc_osc_display_on_irq(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: enable lc osc display on irq.\n");
    res = as3935_get_display_lc_osc_on_irq(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get lc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check lc osc display on irq %s.\n", (enable == AS3935_BOOL_TRUE) ? "ok" : "error");
    
    /* disable lc osc display on irq */
    res = as3935_set_lc_osc_display_on_irq(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: disable lc osc display on irq.\n");
    res = as3935_get_display_lc_osc_on_irq(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get lc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check lc osc display on irq %s.\n", (enable == AS3935_BOOL_FALSE) ? "ok" : "error");
    
    /* as3935_set_system_rc_osc_display_on_irq/as3935_get_system_rc_osc_display_on_irq test */
    as3935_interface_debug_print("as3935: as3935_set_system_rc_osc_display_on_irq/as3935_get_system_rc_osc_display_on_irq test.\n");
    
    /* enable system rc osc display on irq */
    res = as3935_set_system_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set system rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: enable system rc osc display on irq.\n");
    res = as3935_get_system_rc_osc_display_on_irq(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get system rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check system rc osc display on irq %s.\n", (enable == AS3935_BOOL_TRUE) ? "ok" : "error");
    
    /* disable system rc osc display on irq */
    res = as3935_set_system_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set system rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: disable system rc osc display on irq.\n");
    res = as3935_get_system_rc_osc_display_on_irq(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get system rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check system rc osc display on irq %s.\n", (enable == AS3935_BOOL_FALSE) ? "ok" : "error");
    
    /* as3935_set_timer_rc_osc_display_on_irq/as3935_get_timer_rc_osc_display_on_irq test */
    as3935_interface_debug_print("as3935: as3935_set_timer_rc_osc_display_on_irq/as3935_get_timer_rc_osc_display_on_irq test.\n");
    
    /* enable timer rc osc display on irq */
    res = as3935_set_timer_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set timer rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: enable timer rc osc display on irq.\n");
    res = as3935_get_timer_rc_osc_display_on_irq(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get timer rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check timer rc osc display on irq %s.\n", (enable == AS3935_BOOL_TRUE) ? "ok" : "error");
    
    /* disable timer rc osc display on irq */
    res = as3935_set_timer_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set timer rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: disable timer rc osc display on irq.\n");
    res = as3935_get_timer_rc_osc_display_on_irq(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get timer rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check timer rc osc display on irq %s.\n", (enable == AS3935_BOOL_FALSE) ? "ok" : "error");
    
    /* as3935_set_internal_tuning_capacitor/as3935_get_internal_tuning_capacitor test */
    as3935_interface_debug_print("as3935: as3935_set_internal_tuning_capacitor/as3935_get_internal_tuning_capacitor test.\n");
    
    cap = rand() % 0xFU;
    res = as3935_set_internal_tuning_capacitor(&gs_handle, cap);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set internal tuning capacitor failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set internal tuning capacitor %d.\n", cap);
    res = as3935_get_internal_tuning_capacitor(&gs_handle, &cap_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get internal tuning capacitor failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check internal tuning capacitor %s.\n", (cap_check == cap) ? "ok" : "error");
    
    /* as3935_get_timer_rc_osc_calibration_done test */
    as3935_interface_debug_print("as3935: as3935_get_timer_rc_osc_calibration_done test.\n");
    
    /* get timer rc osc calibration done */
    res = as3935_get_timer_rc_osc_calibration_done(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get timer rc osc calibration done failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: timer rc osc calibration done is %s.\n", (enable == AS3935_BOOL_TRUE) ? "true" : "false");
    
    /* as3935_get_timer_rc_osc_calibration_not_ok test */
    as3935_interface_debug_print("as3935: as3935_get_timer_rc_osc_calibration_not_ok test.\n");
    
    /* get timer rc osc calibration not ok */
    res = as3935_get_timer_rc_osc_calibration_not_ok(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get timer rc osc calibration not ok failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: timer rc osc calibration not ok is %s.\n", (enable == AS3935_BOOL_TRUE) ? "true" : "false");
    
    /* as3935_get_system_rc_osc_calibration_done test */
    as3935_interface_debug_print("as3935: as3935_get_system_rc_osc_calibration_done test.\n");
    
    /* get system rc osc calibration done */
    res = as3935_get_system_rc_osc_calibration_done(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get system rc osc calibration done failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: system rc osc calibration done is %s.\n", (enable == AS3935_BOOL_TRUE) ? "true" : "false");
    
    /* as3935_get_system_rc_osc_calibration_not_ok test */
    as3935_interface_debug_print("as3935: as3935_get_system_rc_osc_calibration_not_ok test.\n");
    
    /* get system rc osc calibration not ok */
    res = as3935_get_system_rc_osc_calibration_not_ok(&gs_handle, &enable);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: get system rc osc calibration not ok failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: system rc osc calibration not ok is %s.\n", (enable == AS3935_BOOL_TRUE) ? "true" : "false");
    
    /* as3935_reset_default test */
    as3935_interface_debug_print("as3935: as3935_reset_default test.\n");
    
    /* reset default */
    res = as3935_reset_default(&gs_handle);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: reset default failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check reset default %s.\n", (res == 0) ? "ok" : "error");
    
    /* as3935_set_calibrate_rc_osc test */
    as3935_interface_debug_print("as3935: as3935_set_calibrate_rc_osc test.\n");
    
    /* set calibrate rc osc */
    res = as3935_set_calibrate_rc_osc(&gs_handle);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set calibrate rc osc failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check calibrate rc osc %s.\n", (res == 0) ? "ok" : "error");
    as3935_interface_delay_ms(10);
    
    /* as3935_internal_tuning_capacitor_convert_to_register/as3935_internal_tuning_capacitor_convert_to_data test */
    as3935_interface_debug_print("as3935: as3935_internal_tuning_capacitor_convert_to_register/as3935_internal_tuning_capacitor_convert_to_data test.\n");
    
    /* internal tuning capacitor convert to register */
    pf = (float)(rand() % 1200) / 10.0f;
    res = as3935_internal_tuning_capacitor_convert_to_register(&gs_handle, pf, &reg);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: internal tuning capacitor convert to register failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: set internal tuning capacitor %0.1fpf.\n", pf);
    res = as3935_internal_tuning_capacitor_convert_to_data(&gs_handle, reg, &pf_check);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: internal tuning capacitor convert to data failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    as3935_interface_debug_print("as3935: check internal tuning capacitor %0.1fpf.\n", pf_check);
    
    /* finish register test */
    as3935_interface_debug_print("as3935: finish register test.\n");
    (void)as3935_deinit(&gs_handle);
    
    return 0;
}
