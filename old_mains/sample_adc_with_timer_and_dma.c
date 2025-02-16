
/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f446xx_adc.h"
#include "stm32f446xx_dma.h"
#include "stm32f446xx_gpio.h"
#include "stm32f446xx_tim.h"

#define SIZEOF(arr) ((unsigned int)sizeof(arr) / sizeof(arr[0]))

#define FAST 100000
#define MEDIUM 300000
#define SLOW 1000000
#define WAIT(CNT)                                          \
  do {                                                     \
    for (int sleep_cnt = 0; sleep_cnt < CNT; sleep_cnt++); \
  } while (0)

#define ADC1_CHAN0_GPIO_PORT GPIOA
#define ADC1_CHAN0_GPIO_PIN 0

#define ADC1_CHAN1_GPIO_PORT GPIOA
#define ADC1_CHAN1_GPIO_PIN 1

void adc_gpio_setup();
void adc_tim_scan_example(volatile uint16_t *out_arr, const uint8_t arr_len);

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

volatile int adc_cnt = 0;
volatile uint16_t adc_arr[3];

int main(void) {
  adc_gpio_setup();
  adc_tim_scan_example(adc_arr, (uint8_t)SIZEOF(adc_arr));

  for (;;) {
  }
}

void DMA2_Stream0_IRQHandler(void) {
  if (dma_irq_handling(DMA2, 0, DMA_INTERRUPT_TYPE_FULL_TRANSFER_COMPLETE)) {
    uint16_t temp_adc_val = adc_arr[2];
    float temp = convert_adc_to_temperature(temp_adc_val, ADC_RESOLUTION_12_BIT);
    float set_breakpoint_here = temp;
  }
}

void adc_gpio_setup() {
  // PA 0 and 1 will be the ADC channels. That relates to ADC channels 0 and 1
  GPIOConfig_t cfg = {.mode = GPIO_MODE_ANALOG, .speed = GPIO_SPEED_MEDIUM, .float_resistor = GPIO_PUPDR_NONE};

  // ADC 0
  GPIO_peri_clock_control(ADC1_CHAN0_GPIO_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t adc0_handler = {.p_GPIO_addr = ADC1_CHAN0_GPIO_PORT, .cfg = cfg};
  adc0_handler.cfg.pin_number = ADC1_CHAN0_GPIO_PIN;
  GPIO_init(&adc0_handler);

  // ADC 1
  GPIO_peri_clock_control(ADC1_CHAN1_GPIO_PORT, GPIO_CLOCK_ENABLE);
  GPIOHandle_t adc1_handler = {.p_GPIO_addr = ADC1_CHAN1_GPIO_PORT, .cfg = cfg};
  adc1_handler.cfg.pin_number = ADC1_CHAN1_GPIO_PIN;
  GPIO_init(&adc1_handler);
}

void adc_tim_scan_example(volatile uint16_t *out_arr, const uint8_t arr_len) {
  TimerHandle_t adc_tim_handle = {.cfg = {.channel_1 = {.channel_mode = TIMER_CHANNEL_MODE_COMPARE,
                                                        .capture_edge = TIMER_CAPTURE_RISING_EDGE,
                                                        .capture_input_filter = TIMER_CAPTURE_FILTER_MEDIUM,
                                                        .gpio_en = TIMER_GPIO_DISABLE,
                                                        .ccr = 0,
                                                        .interrupt_en = TIMER_INTERRUPT_ENABLE},
                                          .channel_count = 1,
                                          .arr = 0xffff,
                                          .prescaler = 507},
                                  .p_base_addr = TIM5};
  timer_peri_clock_control(TIM5, 1);
  NVIC_EnableIRQ(TIM5_IRQn);
  timer_init(&adc_tim_handle);

  DMAHandle_t adc_dma_handle = {
      .cfg = {.in = {.addr = (uintptr_t)&ADC1->DR, .type = DMA_IO_TYPE_PERIPHERAL, .inc = DMA_IO_ARR_STATIC},
              .out = {.addr = (uintptr_t)out_arr, .type = DMA_IO_TYPE_MEMORY, .inc = DMA_IO_ARR_INCREMENT},
              .mem_data_size = DMA_DATA_SIZE_16_BIT,
              .peri_data_size = DMA_DATA_SIZE_16_BIT,
              .dma_elements = arr_len,
              .channel = 0b000,
              .priority = DMA_PRIORITY_MAX,
              .circ_buffer = DMA_BUFFER_CIRCULAR,
              .flow_control = DMA_PERIPH_NO_FLOW_CONTROL,
              .interrupt_en =
                  {
                      .full_transfer = DMA_INTERRUPT_ENABLE,
                      .transfer_error = DMA_INTERRUPT_DISABLE,
                      .direct_mode_error = DMA_INTERRUPT_DISABLE,
                      .half_transfer = DMA_INTERRUPT_DISABLE,
                  }},
      .p_stream_addr = DMA2_Stream0};
  dma_peri_clock_control(DMA2, DMA_PERI_CLOCK_ENABLE);
  dma_stream_init(&adc_dma_handle);
  //
  NVIC_EnableIRQ(DMA2_Stream0_IRQn);

  ADCHandle_t adc_init_struct = {
      .addr = ADC1,
      .cfg = {.dual_cfg.en = ADC_DUAL_MODE_DISABLE,
              .inj_autostart = ADC_INJ_AUTOSTART_OFF,
              .main_seq_chan_cfg = {.en = ADC_SCAN_ENABLE,
                                    .sequence = {{.channel = 0, .speed = ADC_CHANNEL_SPEED_LOW},
                                                 {.channel = 1, .speed = ADC_CHANNEL_SPEED_LOW},
                                                 {.channel = 16, .speed = ADC_CHANNEL_SPEED_LOW}

                                    },
                                    .sequence_length = 3},
              .main_inj_chan_cfg.en = ADC_SCAN_DISABLE,
              .eoc_sel = ADC_INTERRUPT_EOC_SELECT_GROUP,
              .temp_or_bat_en = ADC_TEMPORBAT_TEMPERATURE,
              .resolution = ADC_RESOLUTION_12_BIT,
              .interrupt_en = ADC_INTERRUPT_ENABLE,
              .trigger_cfg = {.mode = ADC_TRIGGER_MODE_CONTINUOUS,
                              .timer_sel = ADC_TRIGGER_TIM5_CH1,
                              .edge_sel = ADC_TRIGGER_EDGE_RISING}}};
  adc_peri_clock_control(ADC1, ADC_PERI_CLOCK_ENABLE);
  adc_init(&adc_init_struct);
}
