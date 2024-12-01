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
#include <stdio.h>

#include "stm32f446xx.h"
#include "stm32f446xx_gpio.h"
#include "stm32f446xx_tim.h"

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
#define LED_GREEN_ALT_FN 1

#define USER_PBUTTON_PORT GPIOC
#define USER_PBUTTON_PIN 13

#define INPUT_CAPTURE_GPIO_PORT GPIOB
#define INPUT_CAPTURE_GPIO_PIN 6
#define INPUT_CAPTURE_GPIO_ALT 2

/******* TIMERS ********/
#define TIM_TIMER_ADDR TIM5
#define TIM_CHANNEL 1

#define PWM_TIMER_ADDR TIM2
#define PWM_CHANNEL 4

#define INPUT_CAPTURE_ADDR TIM4
#define INPUT_CAPTURE_CHAN 1
#define OUTPUT_CAPTURE_CHAN_LO 2
#define OUTPUT_CAPTURE_CHAN_HI 3

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void timer_setup_test(void);

/****** CUSTOM TIMER.h CODE STARTS HERE *******
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **/
#define TIMERS {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14}
#define TIMERS_RCC_REGS                                                                                     \
  {&RCC->APB2ENR, &RCC->APB1ENR, &RCC->APB1ENR, &RCC->APB1ENR, &RCC->APB1ENR, &RCC->APB1ENR, &RCC->APB1ENR, \
   &RCC->APB2ENR, &RCC->APB2ENR, &RCC->APB2ENR, &RCC->APB2ENR, &RCC->APB1ENR, &RCC->APB1ENR, &RCC->APB1ENR}
#define TIMERS_RCC_POS                                                                                    \
  {                                                                                                       \
      RCC_APB2ENR_TIM1EN_Pos,  RCC_APB1ENR_TIM2EN_Pos,  RCC_APB1ENR_TIM3EN_Pos,  RCC_APB1ENR_TIM4EN_Pos,  \
      RCC_APB1ENR_TIM5EN_Pos,  RCC_APB1ENR_TIM6EN_Pos,  RCC_APB1ENR_TIM7EN_Pos,  RCC_APB2ENR_TIM8EN_Pos,  \
      RCC_APB2ENR_TIM9EN_Pos,  RCC_APB2ENR_TIM10EN_Pos, RCC_APB2ENR_TIM11EN_Pos, RCC_APB1ENR_TIM12EN_Pos, \
      RCC_APB1ENR_TIM13EN_Pos, RCC_APB1ENR_TIM14EN_Pos,                                                   \
  }

#define TIMER_CCR_REGS(timer) {&timer->CCR1, &timer->CCR2, &timer->CCR3, &timer->CCR4}
#define TIMER_CCMR_REGS(timer) {&timer->CCMR1, &timer->CCMR1, &timer->CCMR2, &timer->CCMR2}

#define SIZEOF(arr) ((int)sizeof(arr) / sizeof(arr[0]))
#define SIZEOFP(arr) ((int)sizeof(arr) / sizeof(uint32_t))  // Memory size of stm32f4

#define CHANNEL_CONFIGS(cfg) {&cfg->channel_1, &cfg->channel_2, &cfg->channel_3, &cfg->channel_4}

/****** END OF CUSTOM TIMER.h CODE STARTS HERE *******
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **/

int main(void) {
  // RCC->CR |= RCC_CR_

  GPIO_peri_clock_control(LED_GREEN_PORT, GPIO_CLOCK_ENABLE);
  GPIO_peri_clock_control(USER_PBUTTON_PORT, GPIO_CLOCK_ENABLE);
  GPIO_peri_clock_control(INPUT_CAPTURE_GPIO_PORT, GPIO_CLOCK_ENABLE);

  GPIO_Handle_t gpio_handle_1 = {.p_GPIO_x = LED_GREEN_PORT,
                                 .GPIO_pin_config = {.GPIO_pin_mode = GPIO_MODE_OUT,
                                                     .GPIO_pin_number = LED_GREEN_PIN,
                                                     .GPIO_pin_speed = GPIO_SPEED_LOW,
                                                     .GPIO_pin_out_type = GPIO_OP_TYPE_PUSHPULL,
                                                     .GPIO_pin_pupd_control = GPIO_PUPDR_NONE,
                                                     .GPIO_pin_alt_func_mode = 0}};
  GPIO_init(&gpio_handle_1);

  GPIO_Handle_t gpio_handle;
  GPIO_TypeDef **gpio_addr = &gpio_handle.p_GPIO_x;
  GPIO_PinConfig_t *gpio_cfg = &gpio_handle.GPIO_pin_config;

  *gpio_addr = GPIOB;
  gpio_cfg->GPIO_pin_number = 2;
  gpio_cfg->GPIO_pin_mode = GPIO_MODE_ALTFN;
  gpio_cfg->GPIO_pin_speed = GPIO_SPEED_LOW;
  gpio_cfg->GPIO_pin_pupd_control = GPIO_PUPDR_NONE;
  gpio_cfg->GPIO_pin_out_type = GPIO_OP_TYPE_PUSHPULL;
  gpio_cfg->GPIO_pin_alt_func_mode = 1;
  GPIO_init(&gpio_handle);

  *gpio_addr = USER_PBUTTON_PORT;
  gpio_cfg->GPIO_pin_number = USER_PBUTTON_PIN;
  gpio_cfg->GPIO_pin_mode = GPIO_MODE_IT_FT;
  gpio_cfg->GPIO_pin_speed = GPIO_SPEED_LOW;
  gpio_cfg->GPIO_pin_pupd_control = GPIO_PUPDR_PULLDOWN;
  gpio_cfg->GPIO_pin_out_type = GPIO_OP_TYPE_PUSHPULL;
  gpio_cfg->GPIO_pin_alt_func_mode = 0;
  GPIO_init(&gpio_handle);
  GPIO_irq_interrupt_config(EXTI15_10_IRQn, GPIO_INT_ENABLE);

  *gpio_addr = INPUT_CAPTURE_GPIO_PORT;
  gpio_cfg->GPIO_pin_number = INPUT_CAPTURE_GPIO_PIN;
  // gpio_cfg->GPIO_pin_mode = GPIO_MODE_ALTFN;
  gpio_cfg->GPIO_pin_mode = GPIO_MODE_IN;
  gpio_cfg->GPIO_pin_speed = GPIO_SPEED_HIGH;
  gpio_cfg->GPIO_pin_pupd_control = GPIO_PUPDR_PULLDOWN;  // Will need a PD when using switch
  gpio_cfg->GPIO_pin_out_type = GPIO_OP_TYPE_PUSHPULL;
  // gpio_cfg->GPIO_pin_alt_func_mode = INPUT_CAPTURE_GPIO_ALT;
  GPIO_init(&gpio_handle);

  TimerHandle_t timer_handle;
  TIM_TypeDef **timer_addr = &timer_handle.p_base_addr;
  TimerConfig_t *timer_cfg = &timer_handle.cfg;

  timer_setup_test();

  // Timer 5 configuration
  timer_peri_clock_control(INPUT_CAPTURE_ADDR, TIMER_PERI_CLOCK_ENABLE);
  *timer_addr = INPUT_CAPTURE_ADDR;

  // overall timer specific
  timer_cfg->prescaler = 507;  // 16E6 / (PSC + 1) = ~65536
  timer_cfg->arr = 0xFFFF;
  timer_cfg->channel_count = 3;

  // channel specific
  timer_cfg->channel_2.interrupt_en = TIMER_INTERRUPT_ENABLE;
  timer_cfg->channel_2.ccr = 0;
  timer_cfg->channel_2.channel_mode = TIMER_CHANNEL_MODE_COMPARE;

  timer_cfg->channel_3.interrupt_en = TIMER_INTERRUPT_ENABLE;
  timer_cfg->channel_3.ccr = 0xFFFF / 4;
  timer_cfg->channel_3.channel_mode = TIMER_CHANNEL_MODE_COMPARE;

  // Initialize timer, enable the associated ISR
  timer_init(&timer_handle);
  timer_irq_interrupt_config(TIM4_IRQn, TIMER_IRQ_ENABLE);

  // Timer 2 configuration
  timer_peri_clock_control(PWM_TIMER_ADDR, TIMER_PERI_CLOCK_ENABLE);
  *timer_addr = PWM_TIMER_ADDR;

  // overall timer specific
  timer_cfg->prescaler = 253;  // 16M / (PSC + 1) = ~65536
  timer_cfg->arr = 1000;
  timer_cfg->channel_count = 4;

  // channel specific
  timer_cfg->channel_4.interrupt_en = TIMER_INTERRUPT_DISABLE;
  timer_cfg->channel_4.ccr = 0;
  timer_cfg->channel_4.channel_mode = TIMER_CHANNEL_MODE_PWM_HI;
  timer_cfg->channel_4.gpio_en = TIMER_GPIO_ENABLE;

  timer_init(&timer_handle);

  /* Loop forever */
  for (;;) {
    uint8_t val = GPIO_get_input(INPUT_CAPTURE_GPIO_PORT, INPUT_CAPTURE_GPIO_PIN);
    GPIO_set_output(LED_GREEN_PORT, LED_GREEN_PIN, val);
    WAIT(FAST);
  }
}

void timer_setup_test(void) {
  int i = 0;  // To emulate what would happen in a loop
  int input_filter_cnt = 10;

  volatile uint32_t *ccmr_reg[] = TIMER_CCMR_REGS(INPUT_CAPTURE_ADDR);

  // NOTE: Switch timer channel to 4 after

  // Timer 3 //
  //Input capture mode//
  RCC->APB1ENR |= (1 << RCC_APB1ENR_TIM4EN_Pos);

  INPUT_CAPTURE_ADDR->CR1 |= (1 << TIM_CR1_DIR_Pos);

  ccmr_reg[0] = 0;
  ccmr_reg[1] = 0;

  // 1.
  // Select the active input: TIMx_CCR1 must be linked to the TI1 input, so write the CC1S
  // bits to 01 in the TIMx_CCMR1 register. As soon as CC1S becomes different from 00,
  // the channel is configured in input and the TIMx_CCR1 register becomes read-only.
  // NOTE: WHen implementing API, likely do this before putting in any ccr value
  *ccmr_reg[i] &= ~(0b11 << ((TIM_CCMR1_CC1S_Pos + i * 8) % 16));
  *ccmr_reg[i] |= (0b01 << ((TIM_CCMR1_CC1S_Pos + i * 8) % 16));

  // 2.
  // Program the appropriate input filter duration in relation with the signal connected to the
  // timer (by programming the ICxF bits in the TIMx_CCMRx register if the input is one of
  // the TIx inputs). Let’s imagine that, when toggling, the input signal is not stable during at
  // must 5 internal clock cycles. We must program a filter duration longer than these 5
  // clock cycles. We can validate a transition on TI1 when 8 consecutive samples with the
  // new level have been detected (sampled at fDTS frequency). Then write IC1F bits to
  // 0011 in the TIMx_CCMR1 register.
  *ccmr_reg[i] &= ~(0b1111 << ((TIM_CCMR1_CC1S_Pos + i * 8) % 16));
  *ccmr_reg[i] |= (0b1111 << ((TIM_CCMR1_CC1S_Pos + i * 8) % 16));

  // 3.
  // Select the edge of the active transition on the TI1 channel by writing the CC1P and
  // CC1NP bits to 00 in the TIMx_CCER register (rising edge in this case).
  INPUT_CAPTURE_ADDR->CCER &= ~(0b111 << (TIM_CCER_CC1P_Pos + i * 4));
  INPUT_CAPTURE_ADDR->CCER |= (0b000 << (TIM_CCER_CC1P_Pos + i * 4));
  // NOTE: When making API, 101 is both edges, 001 is falling edge, 000 is rising edge

  // 4.
  // Program the input prescaler. In our example, we wish the capture to be performed at
  // each valid transition, so the prescaler is disabled (write IC1PS bits to 00 in the
  // TIMx_CCMR1 register).
  *ccmr_reg[i] &= ~(0b11 << ((TIM_CCMR1_IC1PSC_Pos + i * 8) % 16));
  *ccmr_reg[i] |= (0b00 << ((TIM_CCMR1_IC1PSC_Pos + i * 8) % 16));

  // 5.
  // Enable capture from the counter into the capture register by setting the CC1E bit in the
  // TIMx_CCER register.
  INPUT_CAPTURE_ADDR->CCER |= (1 << (TIM_CCER_CC1E_Pos + i * 4));

  // 6.
  // If needed, enable the related interrupt request by setting the CC1IE bit in the
  // TIMx_DIER register, and/or the DMA request by setting the CC1DE bit in the
  // TIMx_DIER register.
  INPUT_CAPTURE_ADDR->DIER |= (1 << (i + 1));

  TIM4->CR1 |= (1 << TIM_CR1_CEN_Pos);
}

void TIM4_IRQHandler(void) {
  if (timer_irq_handling(INPUT_CAPTURE_ADDR, INPUT_CAPTURE_CHAN)) {
    uint16_t capture_val = INPUT_CAPTURE_ADDR->CCR1;
    float pwm_alpha = (float)capture_val / INPUT_CAPTURE_ADDR->ARR;
    timer_set_pwm_percent(PWM_TIMER_ADDR, PWM_CHANNEL, pwm_alpha);
  }
  if (timer_irq_handling(INPUT_CAPTURE_ADDR, OUTPUT_CAPTURE_CHAN_HI)) {
    GPIO_set_output(LED_GREEN_PORT, LED_GREEN_PIN, 1);
  }

  if (timer_irq_handling(INPUT_CAPTURE_ADDR, OUTPUT_CAPTURE_CHAN_LO)) {
    GPIO_set_output(LED_GREEN_PORT, LED_GREEN_PIN, 0);
  }
}

void EXTI15_10_IRQHandler(void) {
  if (GPIO_irq_handling(USER_PBUTTON_PIN)) {
    GPIO_toggle_output(LED_GREEN_PORT, LED_GREEN_PIN);
  }
}
