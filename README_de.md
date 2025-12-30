[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AS3935

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/as3935/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der AS3935 ist ein programmierbarer, vollintegrierter Blitzsensor-IC, der das Vorhandensein und die Annäherung potenziell gefährlicher Blitze in der Umgebung erkennt und die Entfernung zum Gewitterkopf schätzt. Der integrierte Blitzalgorithmus prüft das eingehende Signalmuster, um potenzielle künstliche Störquellen auszublenden. Der AS3935 kann außerdem Informationen zum Rauschpegel liefern und bei hohem Rauschpegel eine externe Einheit (z. B. einen Mikrocontroller) informieren. Hierfür stehen ein Rauschpegelgenerator und eine Rauschpegelauswertungseinheit zur Verfügung. Der AS3935 lässt sich über eine 4-Draht-Standard-SPI-Schnittstelle oder I²C programmieren. Bei Verwendung von I²C können drei verschiedene Adressen ausgewählt werden. Zwei Taktgeber, TRCO und SRCO, erzeugen intern zwei Takte. Eine automatische Kalibrierung erhöht die Präzision dieser Oszillatoren. Die Stromversorgung des AS3935 erfolgt entweder über einen internen Spannungsregler oder direkt über VDD.

LibDriver AS3935 ist ein umfassender Treiber für AS3935, entwickelt von LibDriver. Er bietet Funktionen zur Blitzenergiemessung, Blitzentfernungsmessung und weitere Funktionen. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver AS3935-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver AS3935 IIC, SPI.

/test enthält den Testcode des LibDriver AS3935-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver AS3935-Beispielcode.

/doc enthält das LibDriver AS3935-Offlinedokument.

/Datenblatt enthält AS3935-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige IIC, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-IIC, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/as3935/index.html](https://www.libdriver.com/docs/as3935/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.