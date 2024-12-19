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

#include "main.h"

#include <stdint.h>
#include <stdio.h>

#include "stm32f446xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {
  /* Loop forever */
  read_temperature_setup();
  adc_gpio_setup();
  adc_test_single_setup();

  uint8_t channel_count = (ADC1->SQR1 >> ADC_SQR1_L_Pos) & 0b1111 + 1;
  for (;;) {
    for (int i = 0; i < channel_count; i++) {
      uint16_t placeholder = adc_sample();
      WAIT(FAST);
    }

    // float temperature_val = read_temperature();
    WAIT(SLOW);
  }
}

void setup_gpio() {
  // Green LED for PA5 (on nucleo board)
  GPIO_peri_clock_control(LED_GREEN_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t led_green_handler = {.p_GPIO_addr = LED_GREEN_PORT,
                                    .cfg = {.mode = GPIO_MODE_OUT,
                                            .pin_number = LED_GREEN_PIN,
                                            .speed = GPIO_SPEED_LOW,
                                            .output_type = GPIO_OP_TYPE_PUSHPULL,
                                            .float_resistor = GPIO_PUPDR_NONE}};
  GPIO_init(&led_green_handler);

  // PWM Output externally wired to PB3, attached later to timer 2 channel 2
  GPIO_peri_clock_control(PWM_GPIO_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t pwm_handler = {.p_GPIO_addr = PWM_GPIO_PORT,
                              .cfg = {.mode = GPIO_MODE_ALTFN,
                                      .pin_number = PWM_GPIO_PIN,
                                      .speed = GPIO_SPEED_MEDIUM,
                                      .output_type = GPIO_OP_TYPE_PUSHPULL,
                                      .float_resistor = GPIO_PUPDR_NONE,
                                      .alt_func_num = PWM_GPIO_ALT_FN}};
  GPIO_init(&pwm_handler);

  // User button on PC13, attached to a falling edge interrupt IRQ
  GPIO_peri_clock_control(USER_PBUTTON_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t user_btn_handler = {.p_GPIO_addr = USER_PBUTTON_PORT,
                                   .cfg = {.mode = GPIO_MODE_IT_FT,
                                           .pin_number = USER_PBUTTON_PIN,
                                           .speed = GPIO_SPEED_LOW,
                                           .output_type = GPIO_OP_TYPE_PUSHPULL,
                                           .float_resistor = GPIO_PUPDR_PULLDOWN}};
  GPIO_init(&user_btn_handler);
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  // Input capture on PB6, tied to a timer interrupt which captures the pulse width on timer 4 channel 1
  GPIO_peri_clock_control(INPUT_CAPTURE_GPIO_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t capture_handler = {.p_GPIO_addr = INPUT_CAPTURE_GPIO_PORT,
                                  .cfg = {.mode = GPIO_MODE_ALTFN,
                                          .pin_number = INPUT_CAPTURE_GPIO_PIN,
                                          .speed = GPIO_SPEED_HIGH,
                                          .output_type = GPIO_OP_TYPE_PUSHPULL,
                                          .float_resistor = GPIO_PUPDR_PULLDOWN,
                                          .alt_func_num = INPUT_CAPTURE_GPIO_ALT_FN}};
  GPIO_init(&capture_handler);
}

void adc_gpio_setup() {
  // PA 0 and 1 will be the ADC channels. That relates to ADC channels 0 and 1
  GPIOConfig_t cfg = {.mode = GPIO_MODE_ANALOG, .speed = GPIO_SPEED_MEDIUM, .float_resistor = GPIO_PUPDR_NONE};

  GPIO_peri_clock_control(ADC_CHAN0_GPIO_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t adc0_handler = {.p_GPIO_addr = ADC_CHAN0_GPIO_PORT, .cfg = cfg};
  adc0_handler.cfg.pin_number = ADC_CHAN0_GPIO_PIN;
  GPIO_init(&adc0_handler);

  GPIO_peri_clock_control(ADC_CHAN1_GPIO_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t adc1_handler = {.p_GPIO_addr = ADC_CHAN1_GPIO_PORT, .cfg = cfg};
  adc1_handler.cfg.pin_number = ADC_CHAN1_GPIO_PIN;
  GPIO_init(&adc1_handler);
}

void adc_test_single_setup() {
  RCC->APB2ENR |= (1 << RCC_APB2ENR_ADC1EN_Pos);

  // Set up ADC in single conversion mode and turn the ADC on
  ADC1->CR2 |= (0 << ADC_CR2_CONT_Pos);
  ADC1->CR2 |= (1 << ADC_CR2_ADON_Pos);
  WAIT(FAST);

  ADC1->SQR1 &= ~(0xF << ADC_SQR1_L_Pos);
  ADC1->SQR1 |= (0b10 << ADC_SQR1_L_Pos);

  // 3.Select ADC1_IN18 input channel.
  ADC1->SQR3 |= (0 << ADC_SQR3_SQ1_Pos);
  ADC1->SQR3 |= (1 << ADC_SQR3_SQ2_Pos);
  ADC1->SQR3 |= (18 << ADC_SQR3_SQ3_Pos);

  // 4.Select a sampling time greater than the minimum sampling time specified in the datasheet.
  ADC1->SMPR2 |= (0b111 << ADC_SMPR2_SMP0_Pos);
  ADC1->SMPR2 |= (0b111 << ADC_SMPR2_SMP1_Pos);
  ADC1->SMPR1 |= (0b111 << ADC_SMPR1_SMP18_Pos);

  // 5.Set the TSVREFE bit in the ADC_CCR register to wake up the temperature sensor from power down mode
  ADC123_COMMON->CCR |= (1 << ADC_CCR_TSVREFE_Pos);
  WAIT(FAST);
}

uint16_t adc_sample() {
  // Start the conversion
  ADC1->CR2 |= (1 << ADC_CR2_SWSTART_Pos);

  // Wait until the end of the conversion
  while (ADC1->SR & ADC_SR_EOC_Pos);

  // Read and return the value
  return ADC1->DR;
}

void adc_test_cont_setup() {
  // Single conversion mode
  // In Single conversion mode the ADC does one conversion. This mode is started with the
  // CONT bit at 0 by either:
  // setting the SWSTART bit in the ADC_CR2 register (for a regular channel only)

  // Once the conversion of the selected channel is complete:
  // If a regular channel was converted:
  // –The converted data are stored into the 16-bit ADC_DR register
  // –The EOC (end of conversion) flag is set
  // –An interrupt is generated if the EOCIE bit is set
  // If an injected channel was converted:
  // –The converted data are stored into the 16-bit ADC_JDR1 register
  // –The JEOC (end of conversion injected) flag is set
  // –An interrupt is generated if the JEOCIE bit is set
  // Then the ADC stops.
}
void read_temperature_setup() {
  RCC->APB2ENR |= (1 << RCC_APB2ENR_ADC1EN_Pos);

  ADC1->CR2 |= (0 << ADC_CR2_CONT_Pos);
  ADC1->CR2 |= (1 << ADC_CR2_ADON_Pos);
  WAIT(FAST);

  ADC1->SQR1 &= ~(0xF << ADC_SQR1_L_Pos);
  ADC1->SQR1 |= (0 << ADC_SQR1_L_Pos);

  // 3.Select ADC1_IN18 input channel.
  ADC1->SQR3 |= (18 << ADC_SQR3_SQ1_Pos);

  // 4.Select a sampling time greater than the minimum sampling time specified in the datasheet.
  ADC1->SMPR1 |= (0b111 << ADC_SMPR1_SMP18_Pos);

  // 5.Set the TSVREFE bit in the ADC_CCR register to wake up the temperature sensor from power down mode
  ADC123_COMMON->CCR |= (1 << ADC_CCR_TSVREFE_Pos);
  WAIT(FAST);
}

float read_temperature() {
  // 6.Start the ADC conversion by setting the SWSTART bit (or by external trigger)
  ADC1->CR2 |= (1 << ADC_CR2_SWSTART_Pos);

  while (ADC1->SR & ADC_SR_EOC_Pos);

  // 7.Read the resulting VSENSE data in the ADC data register
  uint32_t adc_val = ADC1->DR;

  // 8.Calculate the temperature using the following formula:
  return convert_adc_to_temperature(adc_val, 12);
}

float convert_adc_to_temperature(uint16_t adc_val, uint8_t adc_res) {
  // Temperature (in °C) = {(VSENSE - V25) / Avg_Slope} + 25
  // Avg_Slope = 2.5mV/C
  // THEREFORE:
  // V_25 = 0.76 -> 0.76/3.3 * 4095 = 943
  // Temp = (ADC_VAL / 4095 * 3.3 - 0.76)/0.0025
  // Temp = (ADC_VAL * 0.0008058608 - 0.76) * 400 + 25
  // Temp = (0.32234432234*ADC_VAL - 304) + 25
  // Temp = 0.32234432234*(ADC_VAL - 943) + 25

  return 0.32234432234 * (adc_val - 943) + 25;
}
