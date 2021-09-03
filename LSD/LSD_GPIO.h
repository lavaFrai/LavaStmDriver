#include <stm32f1xx.h>
#pragma once


enum GPIO_MODES
{
  INPUT_ANALOG = 0b0000,
  INPUT_FLOATING = 0b0100,
  INPUT_PULL = 0b1000,
  OUTPUT_PUSH_PULL = 0b0011,
  OUTPUT_OPEN_DRAIN = 0b0111,
  ALT_PUSH_PULL = 0b1010,
  ALT_OPEN_DRAIN = 0b1110,
  
  // alternative names
  IN_ANALOG = 0b0000,
  IN_FLOATING = 0b0100,
  IN_PULL = 0b1000,
  OUT_PUSH_PULL = 0b0011,
  OUT_OPEN_DRAIN = 0b0111
};

int GPIO_mode(GPIO_TypeDef *GPIO_PORT, int GPIO_PIN, int GPIO_MODE)
{
  if (GPIO_PIN <= 7)
    {
      GPIO_PORT->CRL &= ~(0b1111 << (GPIO_PIN * 4));
      GPIO_PORT->CRL |= (GPIO_MODE << (GPIO_PIN * 4));
      return 0;
    }
    GPIO_PORT->CRH &= ~(0b1111 << ((GPIO_PIN - 8) * 4));
    GPIO_PORT->CRH |= (GPIO_MODE << ((GPIO_PIN - 8) * 4));
    return 0;
}

int GPIO_enable(GPIO_TypeDef *PORT)
{
  if (PORT == GPIOA)
  {
    RCC->APB2ENR |= (1 << (2));
    return 0;
  }
  if (PORT == GPIOB)
  {
    RCC->APB2ENR |= (1 << (3));
    return 0;
  }
  if (PORT == GPIOC)
  {
    RCC->APB2ENR |= (1 << (4));
    return 0;
  }
  if (PORT == GPIOD)
  {
    RCC->APB2ENR |= (1 << (5));
    return 0;
  }
  if (PORT == GPIOE)
  {
    RCC->APB2ENR |= (1 << (6));
    return 0;
  }
  return 0;
}

int GPIO_disable(GPIO_TypeDef *PORT)
{
  if (PORT == GPIOA)
  {
    RCC->APB2ENR &= ~(1 << (2));
    return 0;
  }
  if (PORT == GPIOB)
  {
    RCC->APB2ENR &= ~(1 << (3));
    return 0;
  }
  if (PORT == GPIOC)
  {
    RCC->APB2ENR &= ~(1 << (4));
    return 0;
  }
  if (PORT == GPIOD)
  {
    RCC->APB2ENR &= ~(1 << (5));
    return 0;
  }
  if (PORT == GPIOE)
  {
    RCC->APB2ENR &= ~(1 << (6));
    return 0;
  }
  return 0;
}

#define GPIO_set(PORT, PIN) ((PORT)->ODR |= (1<<(PIN)));
#define GPIO_clear(PORT, PIN) ((PORT)->ODR &= ~(1<<(PIN)));
#define GPIO_read(PORT, PIN) ((PORT)->IDR & (1 << PIN));

int GPIO_write(GPIO_TypeDef *PORT, int PIN, bool VALUE)
{
  if (VALUE)
  {
    GPIO_set(PORT, PIN);
    return 0;
  }
  GPIO_clear(PORT, PIN);
  return 0;
}
