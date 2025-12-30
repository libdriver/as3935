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
 * @file      driver_as3935_basic.c
 * @brief     driver as3935 basic source file
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

#include "driver_as3935_basic.h"

static as3935_handle_t gs_handle;        /**< as3935 handle */

/**
 * @brief  basic irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t as3935_basic_irq_handler(void)
{
    if (as3935_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

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
                          void (*callback)(uint8_t type))
{
    uint8_t res;
    uint8_t cap;
    
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
    DRIVER_AS3935_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
    /* set the interface */
    res = as3935_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set interface failed.\n");
       
        return 1;
    }
    
    /* set addr pin */
    res = as3935_set_addr_pin(&gs_handle, addr_pin);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set addr pin failed.\n");
       
        return 1;
    }
    
    /* as3935 initialization */
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
    
    /* set default afe gain */
    res = as3935_set_afe_gain_boost(&gs_handle, AS3935_BASIC_DEFAULT_AFE_GAIN);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set afe gain boost failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default noise floor level */
    res = as3935_set_noise_floor_level(&gs_handle, AS3935_BASIC_DEFAULT_NOISE_FLOOR_LEVEL);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set noise floor level failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default watchdog threshold */
    res = as3935_set_watchdog_threshold(&gs_handle, AS3935_BASIC_DEFAULT_WATCHDOG_THRESHOLD);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set watchdog threshold failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default lightning min number */
    res = as3935_set_lightning_min_number(&gs_handle, AS3935_BASIC_DEFAULT_LIGHTNING_MIN_NUMBER);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set lightning min number failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default spike rejection */
    res = as3935_set_spike_rejection(&gs_handle, AS3935_BASIC_DEFAULT_SPIKE_REJECTION);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set spike rejection failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default frequency division ration */
    res = as3935_set_frequency_division_ration(&gs_handle, AS3935_BASIC_DEFAULT_FREQUENCY_DIVISION_RATION);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set frequency division ration failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default mask disturber */
    res = as3935_set_mask_disturber(&gs_handle, AS3935_BASIC_DEFAULT_MASK_DISTURBER);
    if (res != 0)
    {
        as3935_interface_debug_print("as3935: set mask disturber failed.\n");
        (void)as3935_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert cap */
    res = as3935_internal_tuning_capacitor_convert_to_register(&gs_handle, AS3935_BASIC_DEFAULT_CAP, &cap);
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
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t as3935_basic_deinit(void)
{
    /* power down */
    if (as3935_set_power_down(&gs_handle, AS3935_BOOL_TRUE) != 0)
    {
        return 1;
    }
    
    /* deinit */
    if (as3935_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read single lightning energy
 * @param[out] *energy pointer to an energy buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t as3935_basic_read_single_lightning_energy(uint32_t *energy)
{
    /* read */
    if (as3935_read_single_lightning_energy(&gs_handle, energy) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      basic example read distance
 * @param[out] *km pointer to a km data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t as3935_basic_read_distance(int8_t *km)
{
    uint8_t raw;
    
    /* read */
    if (as3935_read_distance(&gs_handle, &raw, km) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example clear statistics
 * @return status code
 *         - 0 success
 *         - 1 clear statistics failed
 * @note   none
 */
uint8_t as3935_basic_clear_statistics(void)
{
    /* clear statistics */
    if (as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_TRUE) != 0)
    {
        return 1;
    }
    
    /* clear statistics */
    if (as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_FALSE) != 0)
    {
        return 1;
    }
    
    /* clear statistics */
    if (as3935_set_clear_statistics(&gs_handle, AS3935_BOOL_TRUE) != 0)
    {
        return 1;
    }
    
    return 0;
}
