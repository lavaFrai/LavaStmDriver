#include "stm32f1xx.h"


enum ADC_SPEEDS
{
  ADC_SPEED_1_5t = 0b000, // 0.4kOm
  ADC_SPEED_7_5t = 0b001, // 5.9kOm
  ADC_SPEED_13_5t = 0b010, // 11.4kOm
  ADC_SPEED_28_5t = 0b011, // 25.2kOm
  ADC_SPEED_41_5t = 0b100, // 37.2kOm
  ADC_SPEED_55_5t = 0b101, // 50kOm
  ADC_SPEED_71_5t = 0b110,
  ADC_SPEED_239_5t = 0b111
};

int ADC_enable()
{
  ADC1->CR2 |= ADC_CR2_ADON;
  return 0;
}

int ADC_disable()
{
  ADC1->CR2 &= ~ADC_CR2_ADON;
  return 0;
}

int ADC_setChannel(int channel)
{
  ADC1->SQR1 = 0;
  ADC1->SQR3 = channel;
  return 0;
}

bool ADC_resultReady()
{
  return !((ADC1->SR >> 4) & 1);
}

bool ADC_available()
{
  return ((ADC1->SR >> 4) & 1);
}

int ADC_start()
{
  ADC1->CR2 |= ADC_CR2_SWSTART;
  return 0;
}

int ADC_get()
{
  while (!ADC_available());
  return ADC1->DR >> 4;
}

uint8_t ADC_getByte()
{
  while (!ADC_available());
  return ADC1->DR >> 8;
}

int ADC_calibrate()
{
  ADC_disable();
  ADC_enable();
  for (int i = 0; i < 255; i++);
  ADC1->CR2 |= ADC_CR2_CAL;
  while ((ADC1->CR2 & ADC_CR2_CAL) != 0);
  return 0;
}

int ADC_setSpeed(int channel, int speed)
{
    if (channel < 10)
    {
      ADC1->SMPR2 &= ~(0b111 << (channel * 3));
      ADC1->SMPR2 |= (speed << (channel * 3));
      return 0;
    }
    channel -= 10;
    ADC1->SMPR1 &= ~(111 << (channel * 3));
    ADC1->SMPR1 |= (speed << (channel * 3));
    return 0;
}

int ADC_init()
{
  // enable clock
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  // CLOCK / 6
  RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
  RCC->CFGR |=  RCC_CFGR_ADCPRE_1;
  
  ADC1->CR1 = 0; 
  ADC1->CR2 = 0;
  
  ADC1->CR2 |= ADC_CR2_EXTSEL;
  ADC1->CR2 |= ADC_CR2_EXTTRIG;
  ADC1->CR2 |= ADC_CR2_ALIGN;
  ADC_setSpeed(2, ADC_SPEED_28_5t);
  return 0;
}
 