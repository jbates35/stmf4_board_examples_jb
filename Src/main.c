/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>

#include "stm32f446xx.h"
#include "stm32f446xx_gpio.h"

#define FAST 100000
#define MEDIUM 300000
#define SLOW 1000000
#define WAIT(CNT)                              \
  do {                                         \
    for (int def_i = 0; def_i < CNT; def_i++); \
  } while (0)

/******* PINS *********/
#define LED_GREEN_PORT GPIOA
#define LED_GREEN_PIN 5

#define USER_PBUTTON_PORT GPIOC
#define USER_PBUTTON_PIN 13

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {
  GPIO_peri_clock_control(LED_GREEN_PORT, GPIO_CLOCK_ENABLE);
  GPIO_peri_clock_control(USER_PBUTTON_PORT, GPIO_CLOCK_ENABLE);

  GPIO_Handle_t gpio_handle;
  GPIO_TypeDef **addr = &gpio_handle.p_GPIO_x;
  GPIO_PinConfig_t *cfg = &gpio_handle.GPIO_pin_config;

  *addr = LED_GREEN_PORT;
  cfg->GPIO_pin_number = LED_GREEN_PIN;
  cfg->GPIO_pin_mode = GPIO_MODE_OUT;
  cfg->GPIO_pin_speed = GPIO_SPEED_LOW;
  cfg->GPIO_pin_pupd_control = GPIO_PUPDR_NONE;
  cfg->GPIO_pin_out_type = GPIO_OP_TYPE_PUSHPULL;
  cfg->GPIO_pin_alt_func_mode = 0;
  GPIO_init(&gpio_handle);

  *addr = USER_PBUTTON_PORT;
  cfg->GPIO_pin_number = USER_PBUTTON_PIN;
  cfg->GPIO_pin_mode = GPIO_MODE_IT_FT;
  cfg->GPIO_pin_speed = GPIO_SPEED_LOW;
  cfg->GPIO_pin_pupd_control = GPIO_PUPDR_PULLDOWN;
  cfg->GPIO_pin_out_type = GPIO_OP_TYPE_PUSHPULL;
  cfg->GPIO_pin_alt_func_mode = 0;
  GPIO_init(&gpio_handle);
  GPIO_irq_interrupt_config(EXTI15_10_IRQn, GPIO_INT_ENABLE);
  // GPIO_irq_priority_config(EXTI15_10_IRQn, USER_PBUTTON_PIN);

  /* Loop forever */
  for (;;) {
  }
}

void EXTI15_10_IRQHandler(void) {
  if (GPIO_irq_handling(USER_PBUTTON_PIN)) {
    GPIO_toggle_output_pin(LED_GREEN_PORT, LED_GREEN_PIN);
  }
}
