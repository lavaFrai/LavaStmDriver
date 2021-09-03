#include <stm32f1xx.h>
#pragma once


enum SPI1_SPEEDS 
{
  SPI1_DIV2 = 0b000,
  SPI1_DIV4 = 0b001,
  SPI1_DIV8 = 0b010,
  SPI1_DIV16 = 0b011,
  SPI1_DIV32 = 0b100,
  SPI1_DIV64 = 0b101,
  SPI1_DIV128 = 0b110,
  SPI1_DIV256 = 0b111
};

int SPI1_init()
{
  // enable SPI1 and PORTA
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk 
                | GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk
                | GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk);
  GPIOA->CRL |= (0x02<<GPIO_CRL_CNF5_Pos) | (0x03<<GPIO_CRL_MODE5_Pos);
  GPIOA->CRL |= (0x01<<GPIO_CRL_CNF6_Pos) | (0x00<<GPIO_CRL_MODE6_Pos);
  GPIOA->CRL |= (0x02<<GPIO_CRL_CNF7_Pos) | (0x03<<GPIO_CRL_MODE7_Pos);
  SPI1->CR1 = 0<<SPI_CR1_DFF_Pos  // size of cadr - 8bit
    | 0<<SPI_CR1_LSBFIRST_Pos     // MBS first
    | 1<<SPI_CR1_SSM_Pos          // software ss
    | 1<<SPI_CR1_SSI_Pos          
    | 0b100<<SPI_CR1_BR_Pos        // clock = F_CPU / 32
    | 1<<SPI_CR1_MSTR_Pos         // master mode
    | 0<<SPI_CR1_CPOL_Pos | 0<<SPI_CR1_CPHA_Pos; // SPI mode = 0
  
  SPI1->CR1 |= 1<<SPI_CR1_SPE_Pos; // enable SPI
  return 0;
}

int SPI1_deinit()
{
  SPI1->CR1 &= ~(1<<SPI_CR1_SPE_Pos); // disable SPI
  return 0;
}

int SPI1_enable()
{
  SPI1->CR1 |= 1<<SPI_CR1_SPE_Pos; // enable SPI
  return 0;
}

int SPI1_disable()
{
  SPI1->CR1 &= ~(1<<SPI_CR1_SPE_Pos); // disable SPI
  return 0;
}

int SPI1_setFrequency(int FREQUENCY)
{
  SPI1_disable();
  SPI1->CR1 &= ~(0b111<<SPI_CR1_BR_Pos);
  SPI1->CR1 |= (FREQUENCY<<SPI_CR1_BR_Pos);
  SPI1_enable();
  return 0;
}

void SPI1_write(uint16_t data)
{
  while(!(SPI1->SR & SPI_SR_TXE));
  SPI1->DR = data;
}

uint16_t SPI1_read()
{
  SPI1->DR = 0xFF;
  while(!(SPI1->SR & SPI_SR_RXNE));
  return SPI1->DR;
}

uint16_t SPI1_transaction(uint16_t data)
{
  SPI1->DR = data;
  while(!(SPI1->SR & SPI_SR_RXNE));
  return SPI1->DR;
}
