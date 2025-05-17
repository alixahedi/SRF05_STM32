/* srf0x.h */

#ifndef SRF0X_H
#define SRF0X_H

#include <stdint.h>

/**
 * @brief   Include appropriate HAL header
 */
#if __has_include("stm32f1xx_hal.h")
    #include "stm32f1xx_hal.h"
#elif __has_include("stm32c0xx_hal.h")
    #include "stm32c0xx_hal.h"
#elif __has_include("stm32g4xx_hal.h")
    #include "stm32g4xx_hal.h"
#endif

/**
 * @brief  SRF0X (SRF04/SRF05) handle structure
 */
typedef struct {
    GPIO_TypeDef      *TrigPort;   /**< Trigger pin port */
    uint16_t           TrigPin;    /**< Trigger pin number */
    GPIO_TypeDef      *EchoPort;   /**< Echo pin port */
    uint16_t           EchoPin;    /**< Echo pin number */
    TIM_HandleTypeDef *htim;       /**< Timer handle (must run at 1 MHz) */
} SRF0X_HandleTypeDef;

/**
 * @brief  Initialize SRF0X handle
 * @note   Timer and GPIO must be configured before calling this
 * @param  hsrf: pointer to SRF0X handle
 * @retval None
 */
void srf0x_init(SRF0X_HandleTypeDef *hsrf);

/**
 * @brief  Send a 10 µs trigger pulse
 * @param  hsrf: pointer to SRF0X handle
 * @retval None
 */
void srf0x_trigger(SRF0X_HandleTypeDef *hsrf);

/**
 * @brief  Measure echo pulse width in microseconds (blocking)
 * @param  hsrf: pointer to SRF0X handle
 * @return Pulse width in µs
 */
uint32_t srf0x_read_us(SRF0X_HandleTypeDef *hsrf);

/**
 * @brief  Read distance in centimeters (blocking)
 * @param  hsrf: pointer to SRF0X handle
 * @return Distance in cm
 * @note   Distance (cm) = pulse_duration (µs) / 58
 */
float srf0x_read_cm(SRF0X_HandleTypeDef *hsrf);

#endif /* SRF0X_H */
