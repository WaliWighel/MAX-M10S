# STM32H7S3 MAX-M10S GNSS Driver

STM32 HAL driver and example integration for the **u-blox MAX-M10S GNSS receiver** on an **STM32H7S3xx** MCU.

The driver communicates with the receiver over **I²C using the UBX binary protocol**, uses the MAX-M10S TX-ready signal as an external interrupt, and receives navigation packets with interrupt-driven I²C and DMA. `UBX-NAV-PVT` data is validated and converted into an application-friendly structure containing position, altitude, velocity, ground speed, and heading.

## Features

- u-blox UBX protocol over I²C
- MAX-M10S reset and SafeBoot control
- Receiver hardware-version check using `UBX-MON-VER`
- Runtime receiver configuration with `UBX-CFG-VALSET`
- `UBX-NAV-PVT` output and decoding
- UBX sync, payload-length, and checksum validation
- Interrupt-driven available-byte check
- DMA-based packet reception
- Dedicated non-cacheable DMA buffer for STM32H7 cache coherency
- 3D-fix and `gnssFixOK` validation before publishing data
- Configurable GNSS constellation setup
- Status/error reporting through `MAX_M10S_Status_t`

## Target configuration

The current implementation is configured for:

| Setting | Current value |
| --- | --- |
| MCU | STM32H7S3xx / STM32H7RS HAL |
| GNSS receiver | u-blox MAX-M10S |
| Interface | I²C2 |
| MAX-M10S I²C address | `0x42` (7-bit), passed to STM32 HAL as `0x42 << 1` |
| Protocol | UBX enabled, NMEA disabled on I²C |
| Navigation message | `UBX-NAV-PVT` |
| Navigation rate | Nominally 16 Hz (`CFG_RATE_MEAS = 1000 / 16`, i.e. 62 ms in the current integer calculation) |
| Dynamic model | Airborne `<4g` |
| Power mode | Continuous operation |
| DMA buffer | Dedicated non-cacheable RAM section |

### Enabled GNSS signals

The initialization code enables:

- GPS L1 C/A
- Galileo E1
- BeiDou B1C
- QZSS L1 C/A
- SBAS L1 C/A with differential corrections

GLONASS is disabled in the current configuration, and BeiDou B1I is explicitly disabled.

> Configuration is currently applied to **RAM only**, so the receiver configuration must be applied again after reset or power loss.

## Hardware signals

The application currently defines the following GNSS control/interrupt GPIOs:

| Signal | STM32 pin definition |
| --- | --- |
| GNSS TX-ready / EXTI input | `GPIOM`, pin 12 (`GPS_EXTI_Pin`) |
| GNSS reset | `GPIOM`, pin 11 (`GPS_RESET_Pin`) |
| GNSS SafeBoot | `GPIOM`, pin 14 (`GPS_SAFEBOOT_Pin`) |

The example application uses `I2C2`. The exact SDA/SCL GPIO mapping is generated elsewhere by STM32CubeMX/CubeIDE and is not part of the files shown here.

## How it works

The receive path is implemented as a small event-driven state machine:

1. MAX-M10S asserts its TX-ready signal when data is available.
2. `HAL_GPIO_EXTI_Callback()` calls:

   ```c
   MAX_M10S_HandleEvent(EVENT_START_CYCLE);
   ```

3. The driver reads the MAX-M10S available-byte counter at register `0xFD` using interrupt-driven I²C.
4. `HAL_I2C_MemRxCpltCallback()` reports `EVENT_I2C_RX_DONE` to the driver.
5. If data is available, the driver starts a DMA read from the I²C stream register `0xFF`.
6. The next I²C completion event validates the UBX packet and decodes the message.
7. A valid `UBX-NAV-PVT` packet updates the latest `MAX_M10S_Data_t` snapshot.

### Receive flow

```text
MAX-M10S TX-ready
        |
        v
      EXTI
        |
        v
EVENT_START_CYCLE
        |
        v
Read available bytes (0xFD, interrupt mode)
        |
        v
EVENT_I2C_RX_DONE
        |
        v
Read UBX stream (0xFF, DMA)
        |
        v
EVENT_I2C_RX_DONE
        |
        v
Validate UBX frame
        |
        v
Decode NAV-PVT
        |
        v
Update latest GNSS data
```

## Public API

### Initialize the receiver

```c
MAX_M10S_Status_t MAX_M10S_Init(I2C_HandleTypeDef *hi2c);
```

Example:

```c
if (MAX_M10S_Init(&hi2c2) != MAX_M10S_OK) {
    Error_Handler();
}
```

Initialization resets the receiver, checks its reported hardware version, builds a `CFG-VALSET` configuration, enables `NAV-PVT`, waits for the configuration acknowledgement, and then enables the GNSS interrupt path.

### Pass asynchronous events to the driver

```c
MAX_M10S_Status_t MAX_M10S_HandleEvent(Event_t event);
```

The current application uses it from the GPIO EXTI and I²C receive-complete callbacks:

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPS_EXTI_Pin) {
        MAX_M10S_HandleEvent(EVENT_START_CYCLE);
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2) {
        MAX_M10S_HandleEvent(EVENT_I2C_RX_DONE);
    }
}
```

### Read the latest navigation data

```c
MAX_M10S_Status_t MAX_M10S_CopyLatestData(MAX_M10S_Data_t *data);
```

Example:

```c
MAX_M10S_Data_t gnss;

if (MAX_M10S_CopyLatestData(&gnss) == MAX_M10S_OK && gnss.available) {
    float latitude_deg  = gnss.lat;
    float longitude_deg = gnss.lon;
    float altitude_mm   = gnss.hMSL;
    float ground_mm_s   = gnss.gSpeed;
    float down_mm_s     = gnss.velD;
    float heading_deg   = gnss.headMot;
}
```

## Navigation data

`MAX_M10S_Data_t` exposes:

| Field | Meaning | Unit |
| --- | --- | --- |
| `available` | Valid navigation solution available | boolean |
| `new_data` | A new valid NAV-PVT packet has been decoded | boolean |
| `lon` | Longitude | degrees |
| `lat` | Latitude | degrees |
| `hMSL` | Height above mean sea level | mm |
| `velD` | NED down velocity | mm/s |
| `gSpeed` | 2D ground speed | mm/s |
| `headMot` | Heading of motion | degrees |

A solution is marked available only when:

- `fixType == 3D fix`, and
- the UBX `gnssFixOK` flag is set.

## Status codes

| Status | Meaning |
| --- | --- |
| `MAX_M10S_OK` | Operation completed successfully |
| `MAX_M10S_ERROR` | Generic driver/configuration error |
| `MAX_M10S_I2C_ERROR` | STM32 HAL I²C operation failed |
| `MAX_M10S_INVALID_PACKET` | UBX packet failed validation |

## DMA and cache coherency

STM32H7 devices use data cache, so DMA buffers must be handled carefully. This project places the receive buffer in a dedicated linker section:

```c
#define NC_RAM __attribute__((section(".noncacheable_buffer")))
```

The linker script reserves a 1 KB non-cacheable region at the end of the configured AXI SRAM area, and `MPU_Config()` configures the corresponding region as non-cacheable. The GNSS DMA receive buffer is placed there:

```c
NC_RAM static uint8_t DMA_rx_buff[
    UBX_NAV_PVT_PAYLOAD_SIZE + UBX_FRAME_OVERHEAD_SIZE
];
```

If you move this driver into another STM32 project, preserve the linker/MPU setup or provide equivalent DMA cache maintenance.

## Suggested project layout

A typical layout for the supplied code is:

```text
Core/
├── Inc/
│   ├── main.h
│   └── SENSORS/
│       ├── max-m10s.h
│       └── max-m10s_defines.h
├── Src/
│   ├── main.c
│   └── SENSORS/
│       └── max-m10s.c
└── ...

stm32h7s3xx_flash.ld
```

## Integration checklist

1. Configure the target STM32 project with STM32 HAL.
2. Enable I²C and DMA for the GNSS interface.
3. Configure the GNSS TX-ready line as an EXTI input.
4. Configure GPIO outputs for GNSS reset and SafeBoot.
5. Add `max-m10s.c`, `max-m10s.h`, and `max-m10s_defines.h` to the project.
6. Keep the `.noncacheable_buffer` linker section and matching MPU configuration, or implement explicit D-cache maintenance around DMA transfers.
7. Initialize GPIO, DMA, and I²C before calling `MAX_M10S_Init()`.
8. Forward the EXTI and I²C receive-complete callbacks to `MAX_M10S_HandleEvent()`.
9. Use `MAX_M10S_CopyLatestData()` to obtain the latest decoded navigation solution.

## Current limitations / TODOs

The supplied implementation currently has several limitations:

- Only `UBX-NAV-PVT` is fully decoded.
- `NAV-SAT`, `NAV-CLOCK`, and `NAV-AOPSTATUS` paths are marked TODO/incomplete.
- There is no complete recovery path after DMA/I²C errors in the receive state machine.
- The driver contains a TODO for checking whether the reported available-byte count exceeds the DMA receive buffer before starting the transfer.
- `MAX_M10S_CopyLatestData()` performs a plain `memcpy`; it does not currently provide synchronization against an asynchronous update.
- `new_data` is set when a valid packet arrives but is not cleared by `MAX_M10S_CopyLatestData()`.
- Some initialization polling loops do not currently have a timeout.
- Configuration building uses dynamic allocation (`realloc`) during initialization.
- The expected MAX-M10S hardware-version string is hard-coded as `"000A0000"`.
- Two functions are declared in the public header (`UBX_nav_sat()` and `UBX_ReadAvailableData()`) but are not implemented in the supplied driver source.
- A source comment still says "5 Hz navigation" while the actual configured `output_rate_Hz` value is `16`.

These are good candidates to address before treating the driver as production-ready.

## Files of interest

- `max-m10s.c` — MAX-M10S/UBX driver, configuration, packet validation, DMA receive state machine, NAV-PVT decoder
- `max-m10s.h` — public driver API and navigation data structure
- `max-m10s_defines.h` — UBX packet types, configuration keys, message IDs, and MAX-M10S constants
- `main.c` — example initialization and HAL callback integration
- `main.h` — application events, GNSS GPIO definitions, and non-cacheable RAM attribute
- `stm32h7s3xx_flash.ld` — memory layout and `.noncacheable_buffer` section
