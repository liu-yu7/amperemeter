#include "st7735.h"
#include "stm32g431xx.h"
#include "main.h"
#include "tim.h"
#include <stdint.h>

#define ST7735_SLPOUT   0x11
#define ST7735_FRMCTR1  0xB1
#define ST7735_FRMCTR2  0xB2
#define ST7735_FRMCTR3  0xB3
#define ST7735_INVCTR   0xB4
#define ST7735_PWCTR1   0xC0
#define ST7735_PWCTR2   0xC1
#define ST7735_PWCTR3   0xC2
#define ST7735_PWCTR4   0xC3
#define ST7735_PWCTR5   0xC4
#define ST7735_VMCTR1   0xC5
#define ST7735_COLMOD   0x3A
#define ST7735_GMCTRP1  0xE0
#define ST7735_GMCTRN1  0xE1
#define ST7735_NORON    0x13
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_INVOFF   0x20
#define ST7735_INVON    0x21

#define ST7735_MADCTL     0x36
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MV  0x20

void ST7735_Reset(void)
{
  HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(100);
}

void ST7735_WriteCommand(uint8_t cmd)
{
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_INSTANCE, &cmd, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteByte(uint8_t data)
{
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_INSTANCE, &data, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteData(uint8_t *data, size_t data_size)
{
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_INSTANCE, data, data_size, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_SetRotation(uint8_t rotation)
{
  uint8_t madctl = 0;

  switch (rotation)
  {
    case 0:
      madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_MODE;
      break;
    case 1:
      madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_MODE;
      break;
    case 2:
      madctl = ST7735_MADCTL_MODE;
      break;
    case 3:
      madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_MODE;
      break;
  }

  ST7735_WriteCommand(ST7735_MADCTL);
  ST7735_WriteByte(madctl);
}

void ST7735_Init(void) {
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  TIM4->CCR2 = 500;
  // Initialize the display
  ST7735_Reset();
  ST7735_WriteCommand(ST7735_SLPOUT);
  HAL_Delay(120);
  ST7735_WriteCommand(ST7735_FRMCTR1);
  ST7735_WriteByte(0x01);
  ST7735_WriteByte(0x2C);
  ST7735_WriteByte(0x2D);
  ST7735_WriteCommand(ST7735_FRMCTR2);
  ST7735_WriteByte(0x01);
  ST7735_WriteByte(0x2C);
  ST7735_WriteByte(0x2D);
  ST7735_WriteCommand(ST7735_FRMCTR3);
  ST7735_WriteByte(0x01);
  ST7735_WriteByte(0x2C);
  ST7735_WriteByte(0x2D);
  ST7735_WriteByte(0x01);
  ST7735_WriteByte(0x2C);
  ST7735_WriteByte(0x2D);
  ST7735_WriteCommand(ST7735_INVCTR);
  ST7735_WriteByte(0x07);
  ST7735_WriteCommand(ST7735_PWCTR1);
  ST7735_WriteByte(0xA2);
  ST7735_WriteByte(0x02);
  ST7735_WriteByte(0x84);
  ST7735_WriteCommand(ST7735_PWCTR2);
  ST7735_WriteByte(0xC5);
  ST7735_WriteCommand(ST7735_PWCTR3);
  ST7735_WriteByte(0x0A);
  ST7735_WriteByte(0x00);
  ST7735_WriteCommand(ST7735_PWCTR4);
  ST7735_WriteByte(0x8A);
  ST7735_WriteByte(0x2A);
  ST7735_WriteCommand(ST7735_PWCTR5);
  ST7735_WriteByte(0x8A);
  ST7735_WriteByte(0xEE);
  ST7735_WriteCommand(ST7735_VMCTR1);
  ST7735_WriteByte(0x0E);
  ST7735_WriteCommand(ST7735_INVERSE ? ST7735_INVON : ST7735_INVOFF);
  ST7735_WriteCommand(ST7735_COLMOD);
  ST7735_WriteByte(0x05);
  ST7735_WriteCommand(ST7735_CASET);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x7F);
  ST7735_WriteCommand(ST7735_RASET);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x9F);
  ST7735_WriteCommand(ST7735_GMCTRP1);
  ST7735_WriteByte(0x02);
  ST7735_WriteByte(0x1C);
  ST7735_WriteByte(0x07);
  ST7735_WriteByte(0x12);
  ST7735_WriteByte(0x37);
  ST7735_WriteByte(0x32);
  ST7735_WriteByte(0x29);
  ST7735_WriteByte(0x2D);
  ST7735_WriteByte(0x29);
  ST7735_WriteByte(0x25);
  ST7735_WriteByte(0x2B);
  ST7735_WriteByte(0x39);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x01);
  ST7735_WriteByte(0x03);
  ST7735_WriteByte(0x10);
  ST7735_WriteCommand(ST7735_GMCTRN1);
  ST7735_WriteByte(0x03);
  ST7735_WriteByte(0x1D);
  ST7735_WriteByte(0x07);
  ST7735_WriteByte(0x06);
  ST7735_WriteByte(0x2E);
  ST7735_WriteByte(0x2C);
  ST7735_WriteByte(0x29);
  ST7735_WriteByte(0x2D);
  ST7735_WriteByte(0x2E);
  ST7735_WriteByte(0x2E);
  ST7735_WriteByte(0x37);
  ST7735_WriteByte(0x3F);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x00);
  ST7735_WriteByte(0x02);
  ST7735_WriteByte(0x10);
  ST7735_WriteCommand(ST7735_NORON);
  HAL_Delay(10);
  ST7735_WriteCommand(ST7735_DISPON);
  HAL_Delay(10);

  ST7735_SetRotation(ST7735_ROTATION);
  ST7735_FillScreen(ST7735_BLACK);
}

void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  x0 += ST7735_XSTART;
  y0 += ST7735_YSTART;

  x1 += ST7735_XSTART;
  y1 += ST7735_YSTART;

  ST7735_WriteCommand(ST7735_CASET);
  uint8_t data[] = { 0x00, x0, 0x00, x1};
  ST7735_WriteData(data, sizeof(data));

  ST7735_WriteCommand(ST7735_RASET);
  data[1] = y0;
  data[3] = y1;
  ST7735_WriteData(data, sizeof(data));
}

void ST7735_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
  static uint8_t buff[ST7735_WIDTH * 2];
  uint16_t i = 0;

  for (i = 0; i < width; i++) {
    buff[i * 2] = color >> 8;
    buff[i * 2 + 1] = color & 0xFF;
  }

  ST7735_SetAddressWindow(x, y, x + width - 1, y + height - 1);
  ST7735_WriteCommand(ST7735_RAMWR);
  // Write the color data
  for (i = 0; i < height; i++) {
    ST7735_WriteData(buff, sizeof(uint16_t) * width);
  }
}

void ST7735_FillScreen(uint16_t color)
{
  ST7735_DrawRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}