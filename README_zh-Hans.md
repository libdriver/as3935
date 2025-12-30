[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS3935

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as3935/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS3935是一种可编程的完全集成的闪电传感器IC，可检测附近潜在危险闪电活动的存在和接近，并提供风暴头距离的估计。嵌入式闪电算法检查输入信号模式，以拒绝潜在的人为干扰。AS3935还可以提供关于噪声水平的信息，并在高噪声条件下通过噪声基底生成器和噪声基底评估块通知外部单元（例如微控制器）。AS3935可以通过4线标准SPI或I2C进行编程。此外，如果选择后者，则可以在三个不同的地址中进行选择。两个时钟由两个不同的RC振荡器内部生成：TRCO和SRCO。自动校准程序可以提高这些振荡器的精度。AS3935可以由内部电压调节器供电，也可以直接由VDD供电。

LibDriver AS3935是LibDriver推出的AS3935全功能驱动，该驱动提供闪电能量读取，闪电距离读取等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver AS3935的源文件。

/interface目录包含了LibDriver AS3935与平台无关的IIC, SPI总线模板。

/test目录包含了LibDriver AS3935驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver AS3935编程范例。

/doc目录包含了LibDriver AS3935离线文档。

/datasheet目录包含了AS3935数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC, SPI总线模板，完成指定平台的IIC, SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_as3935_basic.h"

uint8_t res;
uint32_t i;
volatile uint8_t g_flag;
uint8_t (*g_gpio_irq)(void) = NULL;

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

/* gpio init */
g_gpio_irq = as3935_basic_irq_handler;
res = gpio_interrupt_init();
if (res != 0)
{
    g_gpio_irq = NULL;

    return 1;
}

/* basic init */
res = as3935_basic_init(AS3935_INTERFACE_IIC, AS3935_ADDRESS_0, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...
    
g_flag = 0;
for (i = 0; i < 3; i++)
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
    
    ...
}

...
    
/* basic deinit */
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;
(void)as3935_basic_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/as3935/index.html](https://www.libdriver.com/docs/as3935/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。