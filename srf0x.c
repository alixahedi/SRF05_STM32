/* srf0x.c */

#include "srf0x.h"

/**
 * @brief  Initialize SRF0X handle
 * @note   Only stores handle; GPIO and timer must be ready
 * @param  hsrf: pointer to SRF0X handle
 * @retval None
 */
void srf0x_init(SRF0X_HandleTypeDef *hsrf)
{
    /* Nothing to do here if peripherals are set up externally */
}

/**
 * @brief  Send a 10 µs trigger pulse to SRF0X
 * @param  hsrf: pointer to SRF0X handle
 * @retval None
 */
void srf0x_trigger(SRF0X_HandleTypeDef *hsrf)
{
    HAL_GPIO_WritePin(hsrf->TrigPort, hsrf->TrigPin, GPIO_PIN_SET);

    /* Delay ~10 µs via DWT cycle counter (if enabled) */
    DWT->CYCCNT = 0;
    while (DWT->CYCCNT < (HAL_RCC_GetHCLKFreq() / 1000000 * 10));

    HAL_GPIO_WritePin(hsrf->TrigPort, hsrf->TrigPin, GPIO_PIN_RESET);
}

/**
 * @brief  Measure echo pulse width in microseconds
 * @param  hsrf: pointer to SRF0X handle
 * @return Pulse width in µs
 */
uint32_t srf0x_read_us(SRF0X_HandleTypeDef *hsrf)
{
    TIM_TypeDef *timer = hsrf->htim->Instance;
    uint32_t pulse_width;

    /* Send trigger */
    srf0x_trigger(hsrf);

    /* Wait for echo rising edge */
    while (HAL_GPIO_ReadPin(hsrf->EchoPort, hsrf->EchoPin) == GPIO_PIN_RESET);

    /* Reset and start timer */
    timer->CNT = 0;
    HAL_TIM_Base_Start(hsrf->htim);

    /* Wait for echo falling edge */
    while (HAL_GPIO_ReadPin(hsrf->EchoPort, hsrf->EchoPin) == GPIO_PIN_SET);

    /* Stop timer and read count */
    HAL_TIM_Base_Stop(hsrf->htim);
    pulse_width = timer->CNT;

    return pulse_width;
}

/**
 * @brief  Read distance in centimeters
 * @param  hsrf: pointer to SRF0X handle
 * @return Distance in cm
 */
float srf0x_read_cm(SRF0X_HandleTypeDef *hsrf)
{
    uint32_t time_us = srf0x_read_us(hsrf);
    return (float)time_us / 58.0f;
}
