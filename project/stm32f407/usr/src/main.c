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
 * @file      main.c
 * @brief     main source file
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
#include "driver_as3935_register_test.h"
#include "driver_as3935_basic.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
volatile uint8_t g_flag;                   /**< interrupt flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface receive callback
 * @param[in] type irq type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    uint8_t res;
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
            g_flag = 1;
            
            /* read single lightning energy */
            res = as3935_basic_read_single_lightning_energy(&energy);
            if (res == 0)
            {
                as3935_interface_debug_print("as3935: lightning energy is %d.\n", energy);
            }
            
            /* read distance */
            res = as3935_basic_read_distance(&km);
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
 * @brief     as3935 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t as3935(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"interface", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    as3935_address_t addr = AS3935_ADDRESS_0;
    as3935_interface_t interface = AS3935_INTERFACE_IIC;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* addr */
            case 1 :
            {
                /* set the addr pin */
                if (strcmp("0", optarg) == 0)
                {
                    addr = AS3935_ADDRESS_0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = AS3935_ADDRESS_1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = AS3935_ADDRESS_2;
                }
                else if (strcmp("3", optarg) == 0)
                {
                    addr = AS3935_ADDRESS_3;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* interface */
            case 2 :
            {
                /* set the interface */
                if (strcmp("iic", optarg) == 0)
                {
                    interface = AS3935_INTERFACE_IIC;
                }
                else if (strcmp("spi", optarg) == 0)
                {
                    interface = AS3935_INTERFACE_SPI;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* running times */
            case 3 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run register test */
        res = as3935_register_test(interface, addr);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        g_gpio_irq = as3935_read_test_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* read test */
        res = as3935_read_test(interface, addr, times);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_basic", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* gpio init */
        g_gpio_irq = as3935_basic_irq_handler;
        res = gpio_interrupt_init();
        if (res != 0)
        {
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* basic init */
        res = as3935_basic_init(interface, addr, a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        g_flag = 0;
        for (i = 0; i < times; i++)
        {
            while (1)
            {
                if (g_flag != 0)
                {
                    as3935_interface_debug_print("as3935: find interrupt.\n");
                    g_flag = 0;
                    
                    break;
                }
            }
        }
        
        /* basic deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        (void)as3935_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        as3935_interface_debug_print("Usage:\n");
        as3935_interface_debug_print("  as3935 (-i | --information)\n");
        as3935_interface_debug_print("  as3935 (-h | --help)\n");
        as3935_interface_debug_print("  as3935 (-p | --port)\n");
        as3935_interface_debug_print("  as3935 (-t reg | --test=reg) [--addr=<0 | 1 | 2 | 3>] [--interface=<iic | spi>]\n");
        as3935_interface_debug_print("  as3935 (-t read | --test=read) [--addr=<0 | 1 | 2 | 3>] [--interface=<iic | spi>] [--times=<num>]\n");
        as3935_interface_debug_print("  as3935 (-e basic | --example=basic) [--addr=<0 | 1 | 2 | 3>] [--interface=<iic | spi>] [--times=<num>]\n");
        as3935_interface_debug_print("\n");
        as3935_interface_debug_print("Options:\n");
        as3935_interface_debug_print("      --addr=<0 | 1 | 2 | 3>         Set the chip address.([default: 0])\n");
        as3935_interface_debug_print("  -e <basic>, --example=<basic>      Run the driver example.\n");
        as3935_interface_debug_print("  -h, --help                         Show the help.\n");
        as3935_interface_debug_print("  -i, --information                  Show the chip information.\n");
        as3935_interface_debug_print("      --interface=<iic | spi>        Set the chip interface.([default: iic])\n");
        as3935_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        as3935_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        as3935_interface_debug_print("                                     Run the driver test.\n");
        as3935_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        as3935_info_t info;
        
        /* print as3935 info */
        as3935_info(&info);
        as3935_interface_debug_print("as3935: chip is %s.\n", info.chip_name);
        as3935_interface_debug_print("as3935: manufacturer is %s.\n", info.manufacturer_name);
        as3935_interface_debug_print("as3935: interface is %s.\n", info.interface);
        as3935_interface_debug_print("as3935: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        as3935_interface_debug_print("as3935: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        as3935_interface_debug_print("as3935: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        as3935_interface_debug_print("as3935: max current is %0.2fmA.\n", info.max_current_ma);
        as3935_interface_debug_print("as3935: max temperature is %0.1fC.\n", info.temperature_max);
        as3935_interface_debug_print("as3935: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        as3935_interface_debug_print("as3935: SPI interface SCK connected to GPIOA PIN5.\n");
        as3935_interface_debug_print("as3935: SPI interface MISO connected to GPIOA PIN6.\n");
        as3935_interface_debug_print("as3935: SPI interface MOSI connected to GPIOA PIN7.\n");
        as3935_interface_debug_print("as3935: SPI interface CS connected to GPIOA PIN4.\n");
        as3935_interface_debug_print("as3935: IIC interface SCL connected to GPIOB PIN8.\n");
        as3935_interface_debug_print("as3935: IIC interface SDA connected to GPIOB PIN9.\n");
        as3935_interface_debug_print("as3935: INT connected to GPIOB PIN0.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register as3935 function */
    shell_init();
    shell_register("as3935", as3935);
    uart_print("as3935: welcome to libdriver as3935.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("as3935: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("as3935: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("as3935: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("as3935: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("as3935: param is invalid.\n");
            }
            else
            {
                uart_print("as3935: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
