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
 * @file      driver_as3935.c
 * @brief     driver as3935 source file
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

#include "driver_as3935.h"

/**
 * @brief chip register definition
 */
#define CHIP_NAME                 "AMS AS3935"        /**< chip name */
#define MANUFACTURER_NAME         "AMS"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.4f                /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                /**< chip max supply voltage */
#define MAX_CURRENT               0.35f               /**< chip max current */
#define TEMPERATURE_MIN           -40.0f              /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f               /**< chip max operating temperature */
#define DRIVER_VERSION            1000                /**< driver version */

/**
 * @brief chip register definition
 */
#define AS3935_REG_NUMBER_0          0x00        /**< number 0 register */
#define AS3935_REG_NUMBER_1          0x01        /**< number 1 register */
#define AS3935_REG_NUMBER_2          0x02        /**< number 2 register */
#define AS3935_REG_NUMBER_3          0x03        /**< number 3 register */
#define AS3935_REG_NUMBER_4          0x04        /**< number 4 register */
#define AS3935_REG_NUMBER_5          0x05        /**< number 5 register */
#define AS3935_REG_NUMBER_6          0x06        /**< number 6 register */
#define AS3935_REG_NUMBER_7          0x07        /**< number 7 register */
#define AS3935_REG_NUMBER_8          0x08        /**< number 8 register */
#define AS3935_REG_NUMBER_A          0x3A        /**< number a register */
#define AS3935_REG_NUMBER_B          0x3B        /**< number b register */
#define AS3935_REG_PRESET_DEFAULT    0x3C        /**< preset default register */
#define AS3935_REG_CALIB_RCO         0x3D        /**< calib rco register */

/**
 * @brief      iic or spi interface read bytes
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_as3935_iic_spi_read(as3935_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == AS3935_INTERFACE_IIC)                         /* iic interface */
    {
        if (handle->iic_read(handle->iic_addr, reg, buf, len) != 0)      /* read data */
        {
            return 1;                                                    /* return error */
        }
        
        return 0;                                                        /* success return 0 */
    }
    else                                                                 /* spi interface */
    {
        uint8_t command;
        
        command = (1 << 6) | (reg & 0x3F);                               /* set command */
        if (handle->spi_read(command, buf, len) != 0)                    /* read data */
        {
            return 1;                                                    /* return error */
        }
        
        return 0;                                                        /* success return 0 */
    }
}

/**
 * @brief     iic or spi interface write bytes
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_as3935_iic_spi_write(as3935_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->iic_spi == AS3935_INTERFACE_IIC)                          /* iic interface */
    {
        if (handle->iic_write(handle->iic_addr, reg, buf, len) != 0)      /* write data */
        {
            return 1;                                                     /* return error */
        }
        
        return 0;                                                         /* success return 0 */
    }
    else                                                                  /* spi interface */
    {
        uint8_t command;
        
        command = reg & 0x3F;                                             /* set command */
        if (handle->spi_write(command, buf, len) != 0)                    /* write data */
        {
            return 1;                                                     /* return error */
        }
        
        return 0;                                                         /* success return 0 */
    }
}

/**
 * @brief     set the chip interface
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] interface chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t as3935_set_interface(as3935_handle_t *handle, as3935_interface_t interface)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }
    
    handle->iic_spi = (uint8_t)interface;        /* set interface */
    
    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *interface pointer to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as3935_get_interface(as3935_handle_t *handle, as3935_interface_t *interface)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    
    *interface = (as3935_interface_t)(handle->iic_spi);        /* get interface */
    
    return 0;                                                   /* success return 0 */
}

/**
 * @brief     set the iic address pin
 * @param[in] *handle pointer to an as3935 handle structure
 * @param[in] addr_pin address pin
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t as3935_set_addr_pin(as3935_handle_t *handle, as3935_address_t addr_pin)
{
    if (handle == NULL)                        /* check handle */
    {
        return 2;                              /* return error */
    }
    
    handle->iic_addr = (uint8_t)addr_pin;      /* set pin */
    
    return 0;                                  /* success return 0 */
}

/**
 * @brief      get the iic address pin
 * @param[in]  *handle pointer to an as3935 handle structure
 * @param[out] *addr_pin pointer to an address pin buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as3935_get_addr_pin(as3935_handle_t *handle, as3935_address_t *addr_pin)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    
    *addr_pin = (as3935_address_t)(handle->iic_addr);        /* get pin */
    
    return 0;                                                 /* success return 0 */
}

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
uint8_t as3935_set_power_down(as3935_handle_t *handle, as3935_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_0, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 0 failed.\n");                   /* read reg 0 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 0);                                                         /* clear settings */
    prev |= enable << 0;                                                       /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_0, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 0 failed.\n");                  /* write reg 0 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_power_down(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_0, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 0 failed.\n");                   /* read reg 0 failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 0) & 0x01);                             /* get bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_afe_gain_boost(as3935_handle_t *handle, as3935_afe_gain_t gain)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_0, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 0 failed.\n");                   /* read reg 0 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(0x1F << 1);                                                      /* clear settings */
    prev |= gain << 1;                                                         /* set gain */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_0, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 0 failed.\n");                  /* write reg 0 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_afe_gain_boost(as3935_handle_t *handle, as3935_afe_gain_t *gain)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_0, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 0 failed.\n");                   /* read reg 0 failed */
        
        return 1;                                                              /* return error */
    }
    *gain = (as3935_afe_gain_t)((prev >> 1) & 0x1F);                           /* set gain */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_noise_floor_level(as3935_handle_t *handle, as3935_noise_floor_level_t level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_1, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 1 failed.\n");                   /* read reg 1 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(0x7 << 4);                                                       /* clear settings */
    prev |= level << 4;                                                        /* set level */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_1, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 1 failed.\n");                  /* write reg 1 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_noise_floor_level(as3935_handle_t *handle, as3935_noise_floor_level_t *level)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_1, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 1 failed.\n");                   /* read reg 1 failed */
        
        return 1;                                                              /* return error */
    }
    *level = (as3935_noise_floor_level_t)((prev >> 4) & 0x07);                 /* set level */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_watchdog_threshold(as3935_handle_t *handle, uint8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (threshold > 0x0F)                                                      /* check threshold */
    {
        handle->debug_print("as3935: threshold > 0x0F.\n");                    /* threshold > 0x0F */
        
        return 4;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_1, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 1 failed.\n");                   /* read reg 1 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(0xF << 0);                                                       /* clear settings */
    prev |= threshold << 0;                                                    /* set threshold */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_1, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 1 failed.\n");                  /* write reg 1 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_watchdog_threshold(as3935_handle_t *handle, uint8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_1, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 1 failed.\n");                   /* read reg 1 failed */
        
        return 1;                                                              /* return error */
    }
    *threshold =  (prev >> 0) & 0x0F;                                          /* set threshold */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_clear_statistics(as3935_handle_t *handle, as3935_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_2, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 2 failed.\n");                   /* read reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 6);                                                         /* clear settings */
    prev |= enable << 6;                                                       /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_2, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 2 failed.\n");                  /* write reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_clear_statistics(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_2, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 2 failed.\n");                   /* read reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 6) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_lightning_min_number(as3935_handle_t *handle, as3935_lightning_min_number_t number)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_2, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 2 failed.\n");                   /* read reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(3 << 4);                                                         /* clear settings */
    prev |= number << 4;                                                       /* set number */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_2, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 2 failed.\n");                  /* write reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_lightning_min_number(as3935_handle_t *handle, as3935_lightning_min_number_t *number)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_2, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 2 failed.\n");                   /* read reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    *number = (as3935_lightning_min_number_t)((prev >> 4) & 0x03);             /* set number */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_spike_rejection(as3935_handle_t *handle, uint8_t rejection)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (rejection > 0x0F)                                                      /* check rejection */
    {
        handle->debug_print("as3935: rejection > 0x0F.\n");                    /* rejection > 0x0F */
        
        return 4;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_2, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 2 failed.\n");                   /* read reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(0xF << 0);                                                       /* clear settings */
    prev |= rejection << 0;                                                    /* set rejection */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_2, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 2 failed.\n");                  /* write reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_spike_rejection(as3935_handle_t *handle, uint8_t *rejection)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_2, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 2 failed.\n");                   /* read reg 2 failed */
        
        return 1;                                                              /* return error */
    }
    *rejection = (prev >> 0) & 0x0F;                                           /* set rejection */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_frequency_division_ration(as3935_handle_t *handle, as3935_frequency_division_ration_t division)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_3, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 3 failed.\n");                   /* read reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(3 << 6);                                                         /* clear settings */
    prev |= division << 6;                                                     /* set division */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_3, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 3 failed.\n");                  /* write reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_frequency_division_ration(as3935_handle_t *handle, as3935_frequency_division_ration_t *division)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_3, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 3 failed.\n");                   /* read reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    *division = (as3935_frequency_division_ration_t)((prev >> 6) & 0x03);      /* set division */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_mask_disturber(as3935_handle_t *handle, as3935_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_3, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 3 failed.\n");                   /* read reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 5);                                                         /* clear settings */
    prev |= enable << 5;                                                       /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_3, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 3 failed.\n");                  /* write reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_mask_disturber(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_3, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 3 failed.\n");                   /* read reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 5) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_interrupt_status(as3935_handle_t *handle, as3935_interrupt_status_t *status)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_3, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 3 failed.\n");                   /* read reg 3 failed */
        
        return 1;                                                              /* return error */
    }
    *status = (as3935_interrupt_status_t)((prev >> 0) & 0x0F);                 /* set status */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_read_single_lightning_energy(as3935_handle_t *handle, uint32_t *energy)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_4, buf, 3);        /* read config */
    if (res != 0)                                                            /* check the result */
    {
        handle->debug_print("as3935: read reg 4 failed.\n");                 /* read reg 4 failed */
        
        return 1;                                                            /* return error */
    }
    *energy = (uint32_t)((uint32_t)(buf[2] & 0x1F) << 16) |
              (uint32_t)((uint32_t)buf[1] << 8) | buf[0];                    /* set energy */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t as3935_read_distance(as3935_handle_t *handle, uint8_t *raw, int8_t *km)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_7, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 7 failed.\n");                   /* read reg 7 failed */
        
        return 1;                                                              /* return error */
    }
    *raw = prev & 0x3F;                                                        /* set distance */
    if ((*raw) == 0x3F)                                                        /* check output of range */
    {
        *km = -1;                                                              /* set -1 */
    }
    else
    {
        *km = (int8_t)(*raw);                                                  /* convert to km */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_lc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 7);                                                         /* clear settings */
    prev |= enable << 7;                                                       /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_8, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 8 failed.\n");                  /* write reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_display_lc_osc_on_irq(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 7) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_system_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 6);                                                         /* clear settings */
    prev |= enable << 6;                                                       /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_8, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 8 failed.\n");                  /* write reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_system_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 6) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_timer_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(1 << 5);                                                         /* clear settings */
    prev |= enable << 5;                                                       /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_8, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 8 failed.\n");                  /* write reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_timer_rc_osc_display_on_irq(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 5) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_set_internal_tuning_capacitor(as3935_handle_t *handle, uint8_t cap)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (cap > 0xF)                                                             /* check the result */
    {
        handle->debug_print("as3935: cap > 0xF.\n");                           /* cap > 0xF */
        
        return 4;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    prev &= ~(0xF << 0);                                                       /* clear settings */
    prev |= cap << 0;                                                          /* set cap */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_8, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 8 failed.\n");                  /* write reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_internal_tuning_capacitor(as3935_handle_t *handle, uint8_t *cap)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_8, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 8 failed.\n");                   /* read reg 8 failed */
        
        return 1;                                                              /* return error */
    }
    *cap = prev & 0x0F;                                                        /* set cap */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_timer_rc_osc_calibration_done(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_A, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg a failed.\n");                   /* read reg a failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 7) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_timer_rc_osc_calibration_not_ok(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_A, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg a failed.\n");                   /* read reg a failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 6) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_system_rc_osc_calibration_done(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_B, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg b failed.\n");                   /* read reg b failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 7) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_get_system_rc_osc_calibration_not_ok(as3935_handle_t *handle, as3935_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_B, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg b failed.\n");                   /* read reg b failed */
        
        return 1;                                                              /* return error */
    }
    *enable = (as3935_bool_t)((prev >> 6) & 0x01);                             /* set bool */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_reset_default(as3935_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    prev = 0x96U;                                                                     /* set all registers in default mode */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_PRESET_DEFAULT, &prev, 1);        /* write config */
    if (res != 0)                                                                     /* check the result */
    {
        handle->debug_print("as3935: write command failed.\n");                       /* write command failed */
        
        return 1;                                                                     /* return error */
    }
    
    return 0;                                                                         /* success return 0 */
}

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
uint8_t as3935_set_calibrate_rc_osc(as3935_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    prev = 0x96U;                                                              /* calibrates automatically the internal rc oscillators */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_CALIB_RCO, &prev, 1);      /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write command failed.\n");                /* write command failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_internal_tuning_capacitor_convert_to_register(as3935_handle_t *handle, float pf, uint8_t *reg)
{
    if (handle == NULL)                 /* check handle */
    {
        return 2;                       /* return error */
    }
    if (handle->inited != 1)            /* check handle initialization */
    {
        return 3;                       /* return error */
    }
    
    *reg = (uint8_t)(pf / 8.0f);        /* convert real data to register data */
    
    return 0;                           /* success return 0 */
}

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
uint8_t as3935_internal_tuning_capacitor_convert_to_data(as3935_handle_t *handle, uint8_t reg, float *pf)
{
    if (handle == NULL)               /* check handle */
    {
        return 2;                     /* return error */
    }
    if (handle->inited != 1)          /* check handle initialization */
    {
        return 3;                     /* return error */
    }
    
    *pf = (float)(reg) * 8.0f;        /* convert raw data to real data */
    
    return 0;                         /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an as3935 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 close failed
 * @note      none
 */
static uint8_t a_as3935_close(as3935_handle_t *handle)
{
    if (handle->iic_spi == AS3935_INTERFACE_IIC)                        /* iic interface */
    {
        if (handle->iic_deinit() != 0)                                  /* iic deinit */
        {
            handle->debug_print("as3935: iic deinit failed.\n");        /* iic deinit failed */
       
            return 1;                                                   /* return error */
        }
        
        return 0;                                                       /* success return 0 */
    }
    else                                                                /* spi interface */
    {
        if (handle->spi_deinit() != 0)                                  /* spi deinit */
        {
            handle->debug_print("as3935: spi deinit failed.\n");        /* spi deinit failed */
       
            return 1;                                                   /* return error */
        }
        
        return 0;                                                       /* success return 0 */
    }
}

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
uint8_t as3935_init(as3935_handle_t *handle)
{
    if (handle == NULL)                                                    /* check handle */
    {
        return 2;                                                          /* return error */
    }
    if (handle->debug_print == NULL)                                       /* check debug_print */
    {
        return 3;                                                          /* return error */
    }
    if (handle->iic_init == NULL)                                          /* check iic_init */
    {
        handle->debug_print("as3935: iic_init is null.\n");                /* iic_init is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->iic_deinit == NULL)                                        /* check iic_deinit */
    {
        handle->debug_print("as3935: iic_deinit is null.\n");              /* iic_deinit is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->iic_read == NULL)                                          /* check iic_read */
    {
        handle->debug_print("as3935: iic_read is null.\n");                /* iic_read is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->iic_write == NULL)                                         /* check iic_write */
    {
        handle->debug_print("as3935: iic_write is null.\n");               /* iic_write is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->spi_init == NULL)                                          /* check spi_init */
    {
        handle->debug_print("as3935: spi_init is null.\n");                /* spi_init is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->spi_deinit == NULL)                                        /* check spi_deinit */
    {
        handle->debug_print("as3935: spi_deinit is null.\n");              /* spi_deinit is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->spi_read == NULL)                                          /* check spi_read */
    {
        handle->debug_print("as3935: spi_read is null.\n");                /* spi_read is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->spi_write == NULL)                                         /* check spi_write */
    {
        handle->debug_print("as3935: spi_write is null.\n");               /* spi_write is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->delay_ms == NULL)                                          /* check delay_ms */
    {
        handle->debug_print("as3935: delay_ms is null.\n");                /* delay_ms is null */
       
        return 3;                                                          /* return error */
    }
    if (handle->receive_callback == NULL)                                  /* check receive_callback */
    {
        handle->debug_print("as3935: receive_callback is null.\n");        /* receive_callback is null */
       
        return 3;                                                          /* return error */
    }
    
    if (handle->iic_spi == AS3935_INTERFACE_IIC)                           /* iic interface */
    {
        if (handle->iic_init() != 0)                                       /* initialize iic bus */
        {
            handle->debug_print("as3935: iic init failed.\n");             /* iic init failed */
            
            return 1;                                                      /* return error */
        }
    }
    else                                                                   /* spi interface */
    {
        if (handle->spi_init() != 0)                                       /* initialize spi bus */
        {
            handle->debug_print("as3935: spi init failed.\n");             /* spi init failed */
           
            return 1;                                                      /* return error */
        }
    }
    handle->inited = 1;                                                    /* flag finish initialization */
    
    return 0;                                                              /* success return 0 */
}

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
uint8_t as3935_deinit(as3935_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_0, &prev, 1);        /* read config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: read reg 0 failed.\n");                   /* read reg 0 failed */
        
        return 4;                                                              /* return error */
    }
    prev &= ~(1 << 0);                                                         /* clear settings */
    prev |= 1 << 0;                                                            /* set bool */
    res = a_as3935_iic_spi_write(handle, AS3935_REG_NUMBER_0, &prev, 1);       /* write config */
    if (res != 0)                                                              /* check the result */
    {
        handle->debug_print("as3935: write reg 0 failed.\n");                  /* write reg 0 failed */
        
        return 4;                                                              /* return error */
    }
    res = a_as3935_close(handle);                                              /* close */
    if (res != 0)                                                              /* check result */
    {
        return 1;                                                              /* return error */
    }
    
    handle->inited = 0;                                                        /* flag close */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t as3935_irq_handler(as3935_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    uint8_t status;
    
    if (handle == NULL)                                                                             /* check handle */
    {
        return 2;                                                                                   /* return error */
    }
    if (handle->inited != 1)                                                                        /* check handle initialization */
    {
        return 3;                                                                                   /* return error */
    }
    
    handle->delay_ms(2);                                                                            /* delay 2ms */
    res = a_as3935_iic_spi_read(handle, AS3935_REG_NUMBER_3, &prev, 1);                             /* read config */
    if (res != 0)                                                                                   /* check the result */
    {
        handle->debug_print("as3935: read reg 3 failed.\n");                                        /* read reg 3 failed */
        
        return 1;                                                                                   /* return error */
    }
    status = ((prev >> 0) & 0x0F);                                                                  /* set status */
    
    if (status == (uint8_t)AS3935_INTERRUPT_STATUS_NOISE_LEVEL_TOO_HIGH)                            /* if noise level too high */
    {
        if (handle->receive_callback != NULL)                                                       /* if receive callback */
        {
            handle->receive_callback((uint8_t)AS3935_INTERRUPT_STATUS_NOISE_LEVEL_TOO_HIGH);        /* run callback */
        }
    }
    if (status == (uint8_t)AS3935_INTERRUPT_STATUS_DISTURBER_DETECTED)                              /* if disturber detected */
    {
        if (handle->receive_callback != NULL)                                                       /* if receive callback */
        {
            handle->receive_callback((uint8_t)AS3935_INTERRUPT_STATUS_DISTURBER_DETECTED);          /* run callback */
        }
    }
    if (status == (uint8_t)AS3935_INTERRUPT_STATUS_LIGHTNING_INTERRUPT)                             /* if lightning interrupt */
    {
        if (handle->receive_callback != NULL)                                                       /* if receive callback */
        {
            handle->receive_callback((uint8_t)AS3935_INTERRUPT_STATUS_LIGHTNING_INTERRUPT);         /* run callback */
        }
    }
    
    return 0;                                                                                       /* success return 0 */
}

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
uint8_t as3935_set_reg(as3935_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    return a_as3935_iic_spi_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t as3935_get_reg(as3935_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }
    if (handle->inited != 1)                                    /* check handle initialization */
    {
        return 3;                                               /* return error */
    }
    
    return a_as3935_iic_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an as3935 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t as3935_info(as3935_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(as3935_info_t));                         /* initialize as3935 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC SPI", 8);                         /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
