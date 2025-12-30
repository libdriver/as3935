[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS3935

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as3935/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS3935是一種可程式設計的完全集成的閃電感測器IC，可檢測附近潜在危險閃電活動的存在和接近，並提供風暴頭距離的估計。 嵌入式閃電算灋檢查輸入信號模式，以拒絕潜在的人為干擾。 AS3935還可以提供關於雜訊水准的資訊，並在高雜訊條件下通過雜訊基底生成器和雜訊基底評估塊通知外部單元（例如微控制器）。 AS3935可以通過4線標準SPI或I2C進行程式設計。 此外，如果選擇後者，則可以在三個不同的地址中進行選擇。 兩個時鐘由兩個不同的RC振盪器內部生成：TRCO和SRCO。 自動校準程式可以提高這些振盪器的精度。 AS3935可以由內部電壓調節器供電，也可以直接由VDD供電。

LibDriver AS3935是LibDriver推出的AS3935全功能驅動，該驅動提供閃電能量讀取，閃電距離讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver AS3935的源文件。

/interface目錄包含了LibDriver AS3935與平台無關的IIC, SPI總線模板。

/test目錄包含了LibDriver AS3935驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver AS3935編程範例。

/doc目錄包含了LibDriver AS3935離線文檔。

/datasheet目錄包含了AS3935數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC, SPI總線模板，完成指定平台的IIC, SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/as3935/index.html](https://www.libdriver.com/docs/as3935/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
