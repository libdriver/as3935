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
 * @file      driver_as3935_read_test.c
 * @brief     driver as3935 read test source file
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

#include "driver_as3935_read_test.h"

static as3935_handle_t gs_handle;        /**< as3935 handle */
volatile static uint8_t gs_flag;         /**< interrupt flag */

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    uint8_t res;
    uint8_t raw;
    int8_t km;
    uint32_t energy;
    
    switch (type)
    {
        case AS3935_INTERRUPT_STATUS_NOISE_LEVEL_TOO_HIGH :
        {
            as3935_interface_debug_print("as3935: irq noise level too high.\n");
            
            break;
        }
        case AS3935_INTERRUPT_STATUS_DISTURBER_DETECTED :
        {
            as3935_interface_debug_print("as3935: irq disturber detected.\n");
            
            break;
        }
        case AS3935_INTERRUPT_STATUS_LIGHTNING_INTERRUPT :
        {
            as3935_interface_debug_print("as3935: irq lightning interrupt.\n");
            gs_flag = 1;
            
            /* read single lightning energy */
            res = as3935_read_single_lightning_energy(&gs_handle, &energy);
            if (res == 0)
            {
                as3935_interface_debug_print("as3935: lightning energy is %d.\n", energy);
            }
            
            /* read distance */
            res = as3935_read_distance(&gs_handle, &raw, &km);
            if (res == 0)
            {
                as3935_interface_debug_print("as3935: distance is %dkm.\n", km);
            }
            
            break;
        }
        default :
        {
            as3935_interface_debug_print("as3935: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief  read test irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t as3935_read_test_irq_handler(void)
{
    if (as3935_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     read test
 * @param[in] interface chip interface
 * @param[in] addr_pin iic device address
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t as3935_read_test(as3935_interface_t interface, as3935_address_t addr_pin, uint32_t times)
{
    uint8_t res;
    uint8_t cap;
    uint32_t i;
    as3935_info_t info;
    
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
    DRIVER_AS3935_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
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
    
    /* start read test */
    as3935_interface_debug_print("as3935: start read test.\n");
    
    /* init as3935 */
    res = as3935_init(&gs_handle);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: init failed.\n");
       
        return 1;
    }
    
    /* power on */
    res = as3935_set_power_down(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set power down failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* reset default */
    res = as3935_reset_default(&gs_handle);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: reset default failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 10ms */
    as3935_interface_delay_ms(10);
    
    /* set calibrate rc osc */
    res = as3935_set_calibrate_rc_osc(&gs_handle);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set calibrate rc osc failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable system rc osc display on irq */
    res = as3935_set_system_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set system rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 2ms */
    as3935_interface_delay_ms(2);
    
    /* disable system rc osc display on irq */
    res = as3935_set_system_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set system rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable lc osc display on irq */
    res = as3935_set_lc_osc_display_on_irq(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable timer rc osc display on irq */
    res = as3935_set_timer_rc_osc_display_on_irq(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set timer rc osc display on irq failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear statistics */
    res = as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear statistics */
    res = as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear statistics */
    res = as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_TRUE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set clear statistics failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set afe gain indoor */
    res = as3935_set_afe_gain_boost(&gs_handle, AS3935_AFE_GAIN_INDOOR);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set afe gain boost failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set outdoor 860 μVrms, indoor 62 μVrms */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_NOISE_FLOOR_LEVEL_OUTDOOR_860_INDOOR_62);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set watchdog threshold 2 */
    res = as3935_set_watchdog_threshold(&gs_handle, 2);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set watchdog threshold failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lightning min number 1 */
    res = as3935_set_lightning_min_number(&gs_handle, AS3935_LIGHTNING_MIN_NUMBER_1);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set spike rejection 2 */
    res = as3935_set_spike_rejection(&gs_handle, 2);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set spike rejection failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set frequency division ration 16 */
    res = as3935_set_frequency_division_ration(&gs_handle, AS3935_FREQUENCY_DIVISION_RATION_16);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable mask disturber */
    res = as3935_set_mask_disturber(&gs_handle, AS3935_BOOL_FALSE);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set mask disturber failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert cap */
    res = as3935_internal_tuning_capacitor_convert_to_register(&gs_handle, 0, &cap);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: internal tuning capacitor convert to register failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set internal tuning capacitor */
    res = as3935_set_internal_tuning_capacitor(&gs_handle, cap);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set internal tuning capacitor failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    gs_flag = 0;
    for (i = 0; i < times; i++)
    {
        while (1)
        {
            if (gs_flag != 0)
            {
                as3935_interface_debug_print("as3935: find interrupt.\n");
                gs_flag = 0;
                
                break;
            }
        }
    }
    
    /* finish read test */
    as3935_interface_debug_print("as3935: finish read test.\n");
    (void)as3935_deinit(&gs_handle);
    
    return 0;
}
