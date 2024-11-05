#include "mygui.h"
#include "ugui.h"
#include "st7735.h"
#include <stdlib.h>


UG_GUI mygui;
Measure_t measure;
usr_config_t usr_config;
uint8_t last_sw = 0xff;
MenuItem_t *head;                               // 菜单头指针
MenuItem_t Window1,Window2;

MenuItem_t Creat_Menu(void (*init)() ,void (*action)(), void (*button)(Button_Act_e button_act), MenuItem_t *back)
{
    MenuItem_t new;
    new.init = init;
    new.action = action;
    new.button = button;
    new.back = back;
    return new;
}

void Button_Callback(Button_Act_e button_act)
{
    head->button(button_act);
}

void Button_uni(Button_Act_e button_act)
{
    switch (button_act)
    {
    case Button_UP:
        /* code */
        head->back->init();
        head = head->back;
        break;
    case Button_DOWN:
        head->next->init();
        head = head->next;
        /* code */
        break;
    case Button_SET_s:
        measure.output = !measure.output;
        HAL_GPIO_TogglePin(SW_IO_GPIO_Port, SW_IO_Pin);
        /* code */
        break;
    case Button_SET_l:
        head->sub->init();
        head = head->sub;
        /* code */
        break;
    
    default:
        break;
    }
}

void Window1_init(void)
{
    UG_FillScreen(C_BLACK);
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

    last_sw = 0xff; 
}

void Window1_update(void)
{
    static uint8_t temp = 0;
    char buffer[6];
    //刷新开关状态
    if(last_sw != measure.output)
    {
        if(measure.output == 1)
        {   
            UG_FillFrame(105,22,157,48,C_GREEN);
            UG_FontSelect(&FONT_16X26);
            UG_SetForecolor(C_WHITE);
            UG_SetBackcolor(C_GREEN);
            UG_PutString(115,22,"ON");
        }
        else
        {
            UG_FillFrame(105,22,157,48,C_RED);
            UG_FontSelect(&FONT_16X26);
            UG_SetForecolor(C_WHITE);
            UG_SetBackcolor(C_RED);
            UG_PutString(107,22,"OFF");
        }
        UG_SetBackcolor(C_BLACK);
    }
    
    last_sw = measure.output;

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
    UG_SetForecolor(C_GREEN);
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

    if(head != &Window1)
    {
        head->init();
    }

}

void Window2_init()
{
    UG_FillScreen(C_BLACK);
    UG_DrawLine(20, 0, 20, 61, C_WHITE);
    UG_DrawLine(20, 61, 160, 61, C_WHITE);
    UG_FontSelect(&FONT_5X8);
    UG_PutChar('0' ,13, 57, C_WHITE, C_BLACK);
    //UG_DrawLine(102, 50, 160, 50, C_WHITE);

    //上下箭头
    UG_DrawLine(50, 65, 50, 79, C_GREEN);           //|
    UG_DrawLine(110, 65, 110, 79, C_RED);           //|

    UG_DrawLine(50, 79, 47, 76, C_GREEN);           //'\'
    UG_DrawLine(50, 79, 53, 76, C_GREEN);           //'/'
    UG_DrawLine(110, 65, 107, 68, C_RED);        
    UG_DrawLine(110, 65, 113, 68, C_RED);   

}

void Window2_update()
{
    UG_DrawLine(20, 0, 20, 61, C_WHITE);
    UG_FillFrame(21, 0, 160, 60, C_BLACK);          //清空画布

    //刷新y轴
    if(measure.current_reso < 1.0f)
    {   
        UG_FontSelect(&FONT_8X8);
        UG_PutString(3, 65, "mA");
        char buffer[4];
        UG_FillFrame(0, 0, 19, 8, C_BLACK); 
        UG_FontSelect(&FONT_5X8);
        itoa(measure.current_reso*1000, buffer, 10);
        UG_PutString(2,2,buffer);
    }
    else
    {
        UG_FontSelect(&FONT_8X8);
        UG_PutString(3, 65, " A");
        char buffer[4];
        UG_FillFrame(0, 0, 19, 8, C_BLACK); 
        UG_FontSelect(&FONT_5X8);
        itoa(measure.current_reso, buffer, 10);
        UG_PutString(10,0,buffer);
    }

    //刷新曲线
    uint8_t temp = measure.current_buf_index;
    for(int i = 0; i < 35; i++)
    {
        uint8_t next, x;
        x = measure.current_buf[temp++];
        if(temp == 36)
        {
            temp = 0;
        }
        next = measure.current_buf[temp];
        uint16_t x2 = (uint16_t)160 - (i * 4);
        //UG_DrawPixel(x2, 60 - next, C_RED);
        UG_DrawLine(x2 - 4, 60 - next, x2, 60 - x, C_RED);
    }

    //刷新最大最小电流值
    UG_FontSelect(&FONT_10X16);
    UG_SetForecolor(C_GREEN);
    char buffer[6];
    gcvt(measure.min_current, 2, buffer);
    buffer[3] = '\0';
    UG_PutString(58, 65, buffer);
    UG_SetForecolor(C_RED);
    gcvt(measure.max_current, 2, buffer);
    buffer[3] = '\0';
    UG_PutString(118, 65, buffer);
    UG_SetForecolor(C_WHITE);
    
    if(head != &Window2)
    {
        head->init();
    }

}

void Mygui_init(void)
{
    measure.output = 1;
    measure.current_reso = 0.125f;
    ST7735_Init();
    UG_Init(&mygui, ST7735_DrawPoint, 160, 80);
    UG_FillScreen(C_BLACK);
    ST7735_Draw();

    Window1 = Creat_Menu(Window1_init, Window1_update, Button_uni, &Window2);
    Window1.next = &Window2;
    Window2 = Creat_Menu(Window2_init, Window2_update, Button_uni, &Window1);
    Window2.next = &Window1;
    head = &Window1;

    Window1_init();
}

void Mygui_task()
{
    head->action();  
}