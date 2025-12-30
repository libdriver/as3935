[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS3935

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as3935/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AS3935는 프로그래밍 가능한 완전 통합형 낙뢰 센서 IC로, 주변의 위험한 낙뢰 활동의 존재 및 접근을 감지하고 폭풍의 중심부까지의 거리를 추정합니다. 내장된 낙뢰 감지 알고리즘은 수신 신호 패턴을 분석하여 인위적인 교란 요소를 제거합니다. 또한 AS3935는 노이즈 레벨 정보를 제공하고 노이즈 플로어 생성기 및 노이즈 플로어 평가 블록을 통해 높은 노이즈 환경 발생 시 외부 장치(예: 마이크로컨트롤러)에 알릴 수 있습니다. AS3935는 4선식 표준 SPI 또는 I2C를 통해 프로그래밍할 수 있으며, I2C를 선택하는 경우 세 가지 주소 중에서 선택할 수 있습니다. 두 개의 RC 발진기(TRCO 및 SRCO)를 통해 내부적으로 두 개의 클록을 생성하며, 자동 보정 절차를 통해 발진기의 정밀도를 향상시킬 수 있습니다. AS3935는 내부 전압 레귤레이터 또는 VDD에서 직접 전원을 공급받을 수 있습니다.

LibDriver AS3935는 LibDriver에서 출시한 AS3935용 완벽한 기능을 갖춘 드라이버입니다. 낙뢰 에너지 측정, 낙뢰 거리 측정 및 기타 여러 기능을 제공합니다. LibDriver는 MISRA 규정을 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver AS3935의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver AS3935용 플랫폼 독립적인 IIC, SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver AS3935드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver AS3935프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver AS3935오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 AS3935데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 IIC, SPI버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC, SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/as3935/index.html](https://www.libdriver.com/docs/as3935/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.