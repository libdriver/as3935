### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(as3935 REQUIRED)
```

### 3. AS3935

#### 3.1 Command Instruction

1. Show as3935 chip and driver information.

   ```shell
   as3935 (-i | --information)
   ```

2. Show as3935 help.

   ```shell
   as3935 (-h | --help)
   ```

3. Show as3935 pin connections of the current board.

   ```shell
   as3935 (-p | --port)
   ```

4. Run as3935 register test.

   ```shell
   as3935 (-t reg | --test=reg) [--addr=<1 | 2 | 3>] [--interface=<iic | spi>]
   ```

5. Run as3935 read test, num means the test times.

   ```shell
   as3935 (-t read | --test=read) [--addr=<1 | 2 | 3>] [--interface=<iic | spi>] [--times=<num>]
   ```

6. Run as3935 basic function, num is the read times.

   ```shell
   as3935 (-e basic | --example=basic) [--addr=<1 | 2 | 3>] [--interface=<iic | spi>] [--times=<num>]
   ```

#### 3.2 Command Example

```shell
./as3935 -i

as3935: chip is AMS AS3935.
as3935: manufacturer is AMS.
as3935: interface is IIC SPI.
as3935: driver version is 1.0.
as3935: min supply voltage is 2.4V.
as3935: max supply voltage is 5.5V.
as3935: max current is 0.35mA.
as3935: max temperature is 85.0C.
as3935: min temperature is -40.0C.
```

```shell
./as3935 -p

as3935: SPI interface SCK connected to GPIO11(BCM).
as3935: SPI interface MISO connected to GPIO9(BCM).
as3935: SPI interface MOSI connected to GPIO10(BCM).
as3935: SPI interface CS connected to GPIO8(BCM).
as3935: IIC interface SCL connected to GPIO3(BCM).
as3935: IIC interface SDA connected to GPIO2(BCM).
as3935: INT connected to GPIO17(BCM).
```

```shell
./as3935 -t reg --addr=1 --interface=iic

as3935: chip is AMS AS3935.
as3935: manufacturer is AMS.
as3935: interface is IIC SPI.
as3935: driver version is 1.0.
as3935: min supply voltage is 2.4V.
as3935: max supply voltage is 5.5V.
as3935: max current is 0.35mA.
as3935: max temperature is 85.0C.
as3935: min temperature is -40.0C.
as3935: start register test.
as3935: as3935_set_interface/as3935_get_interface test.
as3935: set interface iic.
as3935: check interface ok.
as3935: set interface spi.
as3935: check interface ok.
as3935: as3935_set_addr_pin/as3935_get_addr_pin test.
as3935: set addr pin address1.
as3935: check addr pin ok.
as3935: set addr pin address2.
as3935: check addr pin ok.
as3935: set addr pin address3.
as3935: check addr pin ok.
as3935: as3935_set_power_down/as3935_get_power_down test.
as3935: enable power down.
as3935: check power down ok.
as3935: disable power down.
as3935: check power down ok.
as3935: as3935_set_afe_gain_boost/as3935_get_afe_gain_boost test.
as3935: set afe gain boost indoor.
as3935: check afe gain boost ok.
as3935: set afe gain boost outdoor.
as3935: check afe gain boost ok.
as3935: as3935_set_noise_floor_level/as3935_get_noise_floor_level test.
as3935: set noise floor level outdoor 390 μVrms, indoor 28 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 630 μVrms, indoor 45 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 860 μVrms, indoor 62 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 1100 μVrms, indoor 78 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 1140 μVrms, indoor 95 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 1570 μVrms, indoor 112 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 1800 μVrms, indoor 130 μVrms.
as3935: check noise floor level ok.
as3935: set noise floor level outdoor 2000 μVrms, indoor 146 μVrms.
as3935: check noise floor level ok.
as3935: as3935_set_watchdog_threshold/as3935_get_watchdog_threshold test.
as3935: set watchdog threshold 13.
as3935: as3935_set_clear_statistics/as3935_get_clear_statistics test.
as3935: enable clear statistics.
as3935: check clear statistics ok.
as3935: disable clear statistics.
as3935: check clear statistics ok.
as3935: as3935_set_lightning_min_number/as3935_get_lightning_min_number test.
as3935: set lightning min number 1.
as3935: check lightning min number ok.
as3935: set lightning min number 5.
as3935: check lightning min number ok.
as3935: set lightning min number 9.
as3935: check lightning min number ok.
as3935: set lightning min number 16.
as3935: check lightning min number ok.
as3935: as3935_set_spike_rejection/as3935_get_spike_rejection test.
as3935: set spike rejection 1.
as3935: check spike rejection ok.
as3935: as3935_set_frequency_division_ration/as3935_get_frequency_division_ration test.
as3935: set frequency division ration 16.
as3935: check frequency division ration ok.
as3935: set frequency division ration 32.
as3935: check frequency division ration ok.
as3935: set frequency division ration 64.
as3935: check frequency division ration ok.
as3935: set frequency division ration 128.
as3935: check frequency division ration ok.
as3935: as3935_set_mask_disturber/as3935_get_mask_disturber test.
as3935: enable mask disturber.
as3935: check mask disturber ok.
as3935: disable mask disturber.
as3935: check mask disturber ok.
as3935: as3935_get_interrupt_status test.
as3935: interrupt status is 0x00.
as3935: as3935_set_lc_osc_display_on_irq/as3935_get_display_lc_osc_on_irq test.
as3935: enable lc osc display on irq.
as3935: check lc osc display on irq ok.
as3935: disable lc osc display on irq.
as3935: check lc osc display on irq ok.
as3935: as3935_set_system_rc_osc_display_on_irq/as3935_get_system_rc_osc_display_on_irq test.
as3935: enable system rc osc display on irq.
as3935: check system rc osc display on irq ok.
as3935: disable system rc osc display on irq.
as3935: check system rc osc display on irq ok.
as3935: as3935_set_timer_rc_osc_display_on_irq/as3935_get_timer_rc_osc_display_on_irq test.
as3935: enable timer rc osc display on irq.
as3935: check timer rc osc display on irq ok.
as3935: disable timer rc osc display on irq.
as3935: check timer rc osc display on irq ok.
as3935: as3935_set_internal_tuning_capacitor/as3935_get_internal_tuning_capacitor test.
as3935: set internal tuning capacitor 12.
as3935: check internal tuning capacitor ok.
as3935: as3935_get_timer_rc_osc_calibration_done test.
as3935: timer rc osc calibration done is false.
as3935: as3935_get_timer_rc_osc_calibration_not_ok test.
as3935: timer rc osc calibration not ok is false.
as3935: as3935_get_system_rc_osc_calibration_done test.
as3935: system rc osc calibration done is true.
as3935: as3935_get_system_rc_osc_calibration_not_ok test.
as3935: system rc osc calibration not ok is false.
as3935: as3935_reset_default test.
as3935: check reset default ok.
as3935: as3935_set_calibrate_rc_osc test.
as3935: check calibrate rc osc ok.
as3935: as3935_internal_tuning_capacitor_convert_to_register/as3935_internal_tuning_capacitor_convert_to_data test.
as3935: set internal tuning capacitor 11.5pf.
as3935: check internal tuning capacitor 8.0pf.
as3935: finish register test.
```

```shell
./as3935 -t read --addr=1 --interface=iic --times=1

as3935: chip is AMS AS3935.
as3935: manufacturer is AMS.
as3935: interface is IIC SPI.
as3935: driver version is 1.0.
as3935: min supply voltage is 2.4V.
as3935: max supply voltage is 5.5V.
as3935: max current is 0.35mA.
as3935: max temperature is 85.0C.
as3935: min temperature is -40.0C.
as3935: start read test.
as3935: irq lightning interrupt.
as3935: lightning energy is 200.
as3935: distance is 5km.
as3935: find interrupt.
as3935: finish read test.
```

```shell
./as3935 -e basic --addr=1 --interface=iic --times=1

as3935: irq lightning interrupt.
as3935: lightning energy is 200.
as3935: distance is 5km.
as3935: find interrupt.
```

```shell
./as3935 -h

Usage:
  as3935 (-i | --information)
  as3935 (-h | --help)
  as3935 (-p | --port)
  as3935 (-t reg | --test=reg) [--addr=<1 | 2 | 3>] [--interface=<iic | spi>]
  as3935 (-t read | --test=read) [--addr=<1 | 2 | 3>] [--interface=<iic | spi>] [--times=<num>]
  as3935 (-e basic | --example=basic) [--addr=<1 | 2 | 3>] [--interface=<iic | spi>] [--times=<num>]

Options:
      --addr=<1 | 2 | 3>             Set the chip address.([default: 1])
  -e <basic>, --example=<basic>      Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
      --interface=<iic | spi>        Set the chip interface.([default: iic])
  -p, --port                         Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>
                                     Run the driver test.
      --times=<num>                  Set the running times.([default: 3])
```

