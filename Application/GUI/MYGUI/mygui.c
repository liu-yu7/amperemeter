#include "mygui.h"
#include "ugui.h"
#include "st7735.h"
#include <stdlib.h>


UG_GUI mygui;
Measure_t measure;
usr_config_t usr_config;

void Window1_init(void)
{
    UG_FontSelect(&FONT_16X26);    
    UG_PutChar('V', 85, 0, C_RED, C_BLACK);
    UG_PutChar('A', 85, 25, C_GREEN, C_BLACK);
    UG_PutChar('W', 85, 50, C_BLUE, C_BLACK);

    UG_DrawLine(102, 0, 102, 80, C_WHITE);
    UG_DrawLine(102, 20, 160, 20, C_WHITE);
    UG_DrawLine(102, 50, 160, 50, C_WHITE);

    UG_FontSelect(&FONT_8X8);
    UG_PutChar('m', 130, 72, C_BLUE, C_BLACK);
    UG_PutChar('W', 140, 72, C_BLUE, C_BLACK);
    UG_PutChar('h', 150, 72, C_BLUE, C_BLACK);
}

void Mygui_init(void)
{
    ST7735_Init();
    UG_Init(&mygui, ST7735_DrawPoint, 160, 80);
    UG_FillScreen(C_BLACK);
    ST7735_Draw();
    Window1_init();
}

void Mygui_task()
{
    static uint8_t temp = 0;
    char buffer[6];
    //刷新开关状态
    UG_FontSelect(&FONT_16X26);
    UG_SetForecolor(C_GREEN);
    UG_PutString(115,22,"ON");

    //刷新能量
    if(measure.energy > 1.0f)
    {
        gcvt((double)measure.energy, 4, buffer);
        UG_FontSelect(&FONT_8X8);
        UG_PutChar(' ', 130, 72, C_BLUE, C_BLACK);

    }
    else if(measure.energy > 0.001f)
    {
        gcvt((double)measure.energy * 1000, 4, buffer);
    }
    else
    {
        buffer[0] = '0';
        buffer[1] = '0';
        buffer[2] = '0';
        buffer[3] = '0';
        buffer[4] = '0';
        buffer[5] = '\0';
    }
    UG_FontSelect(&FONT_10X16);
    UG_PutString(105,56,buffer);

    //刷新电压电流功率
    UG_FontSelect(&FONT_16X26);
    UG_SetForecolor(C_RED);
    gcvt((double)measure.voltage, 4, buffer);
    UG_PutString(0,2,buffer);
    UG_SetForecolor(C_GREEN);
    gcvt((double)measure.current, 4, buffer);
    UG_PutString(0,27,buffer);
    UG_SetForecolor(C_BLUE);
    gcvt((double)measure.power, 4, buffer);
    UG_PutString(0,52,buffer);

    //刷新时间
    buffer[0] = measure.time/60/10 + '0';
    buffer[1] = measure.time/60%10 + '0';
    if(temp++ < 20)
    {
        buffer[2] = ':';
    }
    else if(temp < 40)
    {
        buffer[2] = ' ';
    }
    else
    {
        temp = 0;
        buffer[2] = ':';
    }
    buffer[3] = measure.time%60/10 + '0';
    buffer[4] = measure.time%60%10 + '0';
    UG_FontSelect(&FONT_10X16);
    UG_SetForecolor(C_YELLOW_GREEN);
    UG_PutString(105,2,buffer);
    // UG_FillScreen(C_RED);
    // ST7735_Draw();
    // osDelay(1000);
    // UG_FillScreen(C_GREEN);
    ST7735_Draw();
    
}