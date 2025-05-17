# STM32 SRF0X Library

A lightweight library for interfacing SRF04 and SRF05 ultrasonic distance sensors with STM32 microcontrollers using HAL drivers. Supports blocking measurements in microseconds and centimeters.

## Features

* **SRF04 & SRF05 Support**: Works with both popular ultrasonic modules.
* **Microsecond Resolution**: Uses a 1 MHz timer for precise pulse-width measurement.
* **Simple API**: Easy-to-use functions to trigger, read raw pulse width, and get distance in cm.
* **Modular Handle**: Encapsulate per-sensor configuration in an `SRF0X_HandleTypeDef`.
* **HAL-Based**: Leverages STM32Cube HAL; minimal external dependencies.

## Table of Contents

* [Getting Started](#getting-started)

  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)

  * [Initialization](#initialization)
  * [Measuring Distance](#measuring-distance)
  * [Example](#example)
* [API Reference](#api-reference)
* [Contributing](#contributing)
* [License](#license)

## Getting Started

### Prerequisites

* **Hardware**:

  * STM32 microcontroller with Cortex-M core
  * SRF04 or SRF05 ultrasonic sensor
  * Connections:

    * *Trig* pin → any GPIO output
    * *Echo* pin → any GPIO input
    * Shared ground and VCC (5 V or 3.3 V as your sensor supports)
* **Software**:

  * STM32CubeIDE (or any toolchain with HAL libraries)
  * C compiler supporting STM32 HAL

### Installation

1. **Clone or download** this repository:

   ```bash
   git clone https://github.com/alixahedi/STM32-SRF0X.git
   ```
2. **Copy source files** into your project:

   * `srf0x.h` → your project's `Inc/` folder
   * `srf0x.c` → your project's `Src/` folder
3. **Include header** where needed:

   ```c
   #include "srf0x.h"
   ```
4. **Configure peripherals** as described below.

## Usage

### Initialization

1. **Configure GPIOs** in CubeMX (or code):

   * *TrigPin*: Output push-pull
   * *EchoPin*: Input (no pull or pull-down as needed)

2. **Configure a timer** (e.g., `TIM2`) for 1 MHz counter:

   * Prescaler = `(HAL_RCC_GetPCLK1Freq()/1000000 - 1)`
   * Counter mode = up

3. **Enable DWT cycle counter** (optional, for 10 µs trigger delay):

   ```c
   CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
   DWT->CTRL    |= DWT_CTRL_CYCCNTENA_Msk;
   ```

4. **Fill the handle** and call `srf0x_init()`:

   ```c
   SRF0X_HandleTypeDef hsrfx;
   hsrfx.TrigPort = GPIOA;
   hsrfx.TrigPin  = GPIO_PIN_9;
   hsrfx.EchoPort = GPIOA;
   hsrfx.EchoPin  = GPIO_PIN_10;
   hsrfx.htim     = &htim2;  // must be 1 MHz timer

   srf0x_init(&hsrfx);
   ```

### Measuring Distance

* **Get raw pulse width (µs)**:

  ```c
  uint32_t pulse_us = srf0x_read_us(&hsrfx);
  ```
* **Get distance (cm)**:

  ```c
  float dist_cm = srf0x_read_cm(&hsrfx);
  ```

### Example

```c
#include "main.h"
#include "srf0x.h"

extern TIM_HandleTypeDef htim2;

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM2_Init();
    
    // Enable DWT for microsecond delay
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL    |= DWT_CTRL_CYCCNTENA_Msk;
    
    // Sensor handle
    SRF0X_HandleTypeDef sensor = {
        .TrigPort = GPIOA,
        .TrigPin  = GPIO_PIN_9,
        .EchoPort = GPIOA,
        .EchoPin  = GPIO_PIN_10,
        .htim     = &htim2
    };
    srf0x_init(&sensor);
    
    while (1)
    {
        float distance = srf0x_read_cm(&sensor);
        // Use distance (e.g., print via UART or display)
        HAL_Delay(100);
    }
}
```

## API Reference

### `SRF0X_HandleTypeDef`

| Member   | Type                  | Description                   |
| -------- | --------------------- | ----------------------------- |
| TrigPort | `GPIO_TypeDef *`      | GPIO port for trigger         |
| TrigPin  | `uint16_t`            | GPIO pin number for trigger   |
| EchoPort | `GPIO_TypeDef *`      | GPIO port for echo            |
| EchoPin  | `uint16_t`            | GPIO pin number for echo      |
| htim     | `TIM_HandleTypeDef *` | Pointer to 1 MHz timer handle |

### Functions

```c
void srf0x_init(SRF0X_HandleTypeDef *hsrf);
```

* Initialize sensor handle (peripherals must be ready).

```c
void srf0x_trigger(SRF0X_HandleTypeDef *hsrf);
```

* Send a 10 µs pulse to trigger the sensor.

```c
uint32_t srf0x_read_us(SRF0X_HandleTypeDef *hsrf);
```

* Measure echo pulse width, returns microseconds.

```c
float srf0x_read_cm(SRF0X_HandleTypeDef *hsrf);
```

* Measure distance, returns centimeters (`pulse_us / 58.0f`).

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/NewFeature`)
3. Commit your changes (`git commit -m "Add NewFeature"`)
4. Push to branch (`git push origin feature/NewFeature`)
5. Open a pull request

Please ensure code is formatted and documented in the same style.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 📞 Contact
For questions or suggestions, feel free to reach out via email:
📧 Alixahedi@gmail.com

<br/>
<a href="https://twitter.com/alixahedi">
<img align="left" alt="Alix | Twitter" width="22px" src="https://github.com/alixahedi/alixahedi/blob/main/assests/img/social/Twitter.png" />
</a>
<a href="https://www.linkedin.com/in/ali-zahedi-b5a360158//">
<img align="left" alt="Ali's LinkedIN" width="22px" src="https://github.com/alixahedi/alixahedi/blob/main/assests/img/social/Linkedin.png" />
</a>
<a href="https://www.facebook.com/Alixahedi/">
<img align="left" alt="Ali's FaceBook" width="22px" src="https://github.com/alixahedi/alixahedi/blob/main/assests/img/social/fb.png" />
</a>
<a href="https://www.instagram.com/Alixahedi">
<img align="left" alt="Ali's Instagram" width="22px" src="https://github.com/alixahedi/alixahedi/blob/main/assests/img/social/insta.png" />
</a>
<br/>
