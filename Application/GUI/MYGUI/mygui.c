#include "mygui.h"
#include "myfont.h"
#include "ugui.h"
#include "st7735.h"
#include <stdlib.h>
#include "stm32g4xx_hal_flash.h"
#include "tim.h"
#include "string.h"

#define STM32_FLASH_SIZE 	128 	 	//所选STM32的FLASH容量大小(单位为K)
    #if     STM32_FLASH_SIZE < 256      //设置扇区大小
    #define STM_SECTOR_SIZE     1024    //1K字节
    #else 
    #define STM_SECTOR_SIZE	    2048    //2K字节
    #endif	
#define STM32_FLASH_BASE    0x08000000 		//STM32 FLASH的起始地址
#define FLASH_SAVE_ADDR     STM32_FLASH_BASE+STM_SECTOR_SIZE*2*63	//写Flash的地址，这里从最后一页开始
#define FLASH_WAITETIME  	50000          	//FLASH等待超时时间

UG_GUI mygui;
Measure_t measure;
usr_config_t usr_config;
static int8_t menu_point_now = 0;       //当前光标位置
static int8_t menu_screen_now = 0;      //当前屏幕位置
MenuItem_t *head;                               // 菜单头指针
MenuItem_t Window1,Window2,Menu;
MenuItem_t Sub_Menu;

void Erase_Flash(uint32_t start_page,uint16_t num)
{
    uint32_t prim = __get_PRIMASK();
    __disable_irq();
    __HAL_FLASH_DATA_CACHE_DISABLE();
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;    //删除方式
    EraseInitStruct.Page        = start_page;                     //超始页号
    EraseInitStruct.NbPages     = num;                      //页的数量
    EraseInitStruct.Banks       = FLASH_BANK_1;             //bank号

    HAL_FLASH_Unlock();         //解锁，以准备进行FLASH操作

    MODIFY_REG(FLASH->CR, FLASH_CR_PNB, ((start_page & 0xFFU) << FLASH_CR_PNB_Pos));
    SET_BIT(FLASH->CR, FLASH_CR_PER);
    SET_BIT(FLASH->CR, FLASH_CR_STRT);

    FLASH->SR |= FLASH_SR_EOP;                 //清除标志位
    FLASH->CR = 0x80;                          //重新上锁，擦除完成
       

    HAL_FLASH_Lock();           //上锁，以结束FLASH操作
    __HAL_FLASH_DATA_CACHE_ENABLE();
    __set_PRIMASK(prim);//恢复中断现场
}

void Write_Flash(uint32_t addr,uint64_t *data, uint16_t num)
{
    Erase_Flash(63,1);

    HAL_FLASH_Unlock();
    for(uint16_t i = 0; i < num; i++)
    {
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
        while(FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE) != HAL_OK);
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, *data) != HAL_OK)
        {
            while(1);
        };
        addr += 8;
        data++;
    }
    HAL_FLASH_Lock();
}

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
    if(measure.flag == 2)
    {
        TIM8->CCR1 = 0;
    }
    else
    {
        head->button(button_act);
    }
    measure.flag = 1;
    TIM8->CCR1 = usr_config.volume/10;
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
    case Button_SET_l:
        measure.output = !measure.output;
        HAL_GPIO_TogglePin(SW_IO_GPIO_Port, SW_IO_Pin);
        /* code */
        break;
    case Button_SET_s:
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
}

void Window1_update(void)
{
    Window1_init();
    static uint8_t temp = 0;
    char buffer[6];
    //刷新开关状态
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

void Clear_Screen()
{
    UG_FillScreen(C_BLACK);
}

float p = 0.1f,i = 0.01f,i_sub_x, i_sub_y;

void Menu_update()
{
    Clear_Screen();
    static uint8_t menu_sta = 0xff;

    if(menu_sta != menu_point_now + menu_screen_now)
    {
        UG_FillFrame(4*16+4, 0, 5*16+4, 80, C_BLACK);
        menu_sta = menu_screen_now + menu_point_now;
        head->sub = &Sub_Menu;
    }

    //画框，用pid过冲实现果冻感
    static uint8_t frame_x_last, frame_y_last;
    static uint8_t frame_x, frame_y;
    uint8_t frame_x_taget, frame_y_taget;
    float speed_x, speed_y;

    //计算新的框位置
    if(menu_sta == 5)
    {
        frame_x_taget = 5+16*5;
    }
    else
    {
        frame_x_taget = 5+16*4;
    }
    frame_y_taget = menu_point_now*20;


    if(frame_x_taget == frame_x_last)
    {
        i_sub_x = 0;
    }
    else
    {
        i_sub_x += (frame_x_taget - frame_x_last) * i;
    }
    if(frame_y_taget == frame_y_last)
    {
        i_sub_y = 0;
    }
    else
    {
        i_sub_y += (frame_y_taget - frame_y_last) * i;
    }

    speed_x = (frame_x_taget - frame_x_last) * p + i_sub_x;
    speed_y = (frame_y_taget - frame_y_last) * p + i_sub_y;
    if(speed_x > 3)
    {
        speed_x = 3;
    }
    else if(speed_x < -3)
    {
        speed_x = -3;
    }
    if(speed_y > 2)
    {
        speed_y = 2;
    }
    else if(speed_y < -2)
    {
        speed_y = -2;
    }

    frame_x += speed_x;
    frame_y += speed_y;
    if(frame_x > 100)
    {
        frame_x = 100;
    }
    if(frame_y > 60)
    {
        frame_y = 60;
    }

    

    if(frame_y_last != frame_y || frame_x_last != frame_x) 
    {
        //消除上次的框
        UG_DrawRoundFrame(1, frame_y_last, frame_x_last, frame_y_last + 19, 4, C_BLACK);
    }

        //Clear_Screen();
    UG_FontSelect(&MY_FONT);

    //画菜单
    if(0 - menu_screen_now >= 0 && 0 - menu_screen_now <= 3)
    {
        UG_PutChar(0x00, 4, 2 + (0 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //电
        UG_PutChar(0x01, 1*16+4, 2 + (0 - menu_screen_now)*20, C_WHITE, C_BLACK);              //流
        UG_PutChar(0x03, 2*16+4, 2 + (0 - menu_screen_now)*20, C_WHITE, C_BLACK);              //校
        UG_PutChar(0x04, 3*16+4, 2 + (0 - menu_screen_now)*20, C_WHITE, C_BLACK);              //准
    }
    if(1 - menu_screen_now >= 0 && 1 - menu_screen_now <= 3)
    {
        UG_PutChar(0x00, 4, 2 + (1 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //电
        UG_PutChar(0x02, 1*16+4, 2 + (1 - menu_screen_now)*20, C_WHITE, C_BLACK);              //压
        UG_PutChar(0x03, 2*16+4, 2 + (1 - menu_screen_now)*20, C_WHITE, C_BLACK);              //校
        UG_PutChar(0x04, 3*16+4, 2 + (1 - menu_screen_now)*20, C_WHITE, C_BLACK);              //准
    }
    if(2 - menu_screen_now >= 0 && 2 - menu_screen_now <= 3)
    {
        UG_PutChar(0x05, 4, 2 + (2 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //过
        UG_PutChar(0x01, 1*16+4, 2 + (2 - menu_screen_now)*20, C_WHITE, C_BLACK);              //流
        UG_PutChar(0x06, 2*16+4, 2 + (2 - menu_screen_now)*20, C_WHITE, C_BLACK);              //保
        UG_PutChar(0x07, 3*16+4, 2 + (2 - menu_screen_now)*20, C_WHITE, C_BLACK);              //护
    }
    if(3 - menu_screen_now >= 0 && 3 - menu_screen_now <= 3)
    {
        UG_PutChar(0x16, 4, 2 + (3 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //欠
        UG_PutChar(0x02, 1*16+4, 2 + (3 - menu_screen_now)*20, C_WHITE, C_BLACK);              //压
        UG_PutChar(0x06, 2*16+4, 2 + (3 - menu_screen_now)*20, C_WHITE, C_BLACK);              //保
        UG_PutChar(0x07, 3*16+4, 2 + (3 - menu_screen_now)*20, C_WHITE, C_BLACK);              //护
    }
    if(4 - menu_screen_now >= 0 && 4 - menu_screen_now <= 3)
    {
        UG_PutChar(0x0A, 4, 2 + (4 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //屏
        UG_PutChar(0x0B, 1*16+4, 2 + (4 - menu_screen_now)*20, C_WHITE, C_BLACK);              //幕
        UG_PutChar(0x0C, 2*16+4, 2 + (4 - menu_screen_now)*20, C_WHITE, C_BLACK);              //亮
        UG_PutChar(0x0D, 3*16+4, 2 + (4 - menu_screen_now)*20, C_WHITE, C_BLACK);              //度
    }
    if(5 - menu_screen_now >= 0 && 5 - menu_screen_now <= 3)
    {
        UG_PutChar(0x0E, 4, 2 + (5 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //蜂
        UG_PutChar(0x0F, 1*16+4, 2 + (5 - menu_screen_now)*20, C_WHITE, C_BLACK);              //鸣
        UG_PutChar(0x10, 2*16+4, 2 + (5 - menu_screen_now)*20, C_WHITE, C_BLACK);              //器
        UG_PutChar(0x11, 3*16+4, 2 + (5 - menu_screen_now)*20, C_WHITE, C_BLACK);              //音
        UG_PutChar(0x12, 4*16+4, 2 + (5 - menu_screen_now)*20, C_WHITE, C_BLACK);              //量
    }
    if(6 - menu_screen_now >= 0 && 6 - menu_screen_now <= 3)
    {
        UG_PutChar(0x06, 4, 2 + (6 - menu_screen_now)*20, C_WHITE, C_BLACK);                   //保
        UG_PutChar(0x13, 1*16+4, 2 + (6 - menu_screen_now)*20, C_WHITE, C_BLACK);              //存
        UG_PutChar(0x14, 2*16+4, 2 + (6 - menu_screen_now)*20, C_WHITE, C_BLACK);              //设
        UG_PutChar(0x15, 3*16+4, 2 + (6 - menu_screen_now)*20, C_WHITE, C_BLACK);              //置
    } 
    //画新的框
    UG_DrawRoundFrame(1, frame_y, frame_x, frame_y + 19, 4, C_WHITE);
    frame_x_last = frame_x;
    frame_y_last = frame_y;
    
}

void Button_Menu(Button_Act_e button_act)
{
    switch (button_act)
    {
    case Button_UP:
        /* code */
        if(menu_point_now > 0)
        {
            menu_point_now--;
        }
        else if(menu_screen_now > 0)
        {
            menu_screen_now--;
        }
        break;
    case Button_DOWN:
        if(menu_point_now < 3)
        {
            menu_point_now++;
        }
        else if(menu_screen_now < 3)
        {
            menu_screen_now++;
        }
        /* code */
        break;
    case Button_SET_l:
        head->back->init();
        head = head->back;
        /* code */
        break;
    case Button_SET_s:
        head->sub->init();
        head = head->sub;
        /* code */
        break;
    
    default:
        break;
    }
}

static uint8_t v_p = 0;    //校准点 (5V,10V,15V,20V,25V,30V)
static uint8_t c_p = 0;    //校准点 (1A,2A,5A,10A,20A,50A)
static uint8_t s_p = 0;    //设置位数
static uint8_t s_f = 0;    //设置标志

void SubMenu_update()
{
    Clear_Screen();
    if(menu_point_now + menu_screen_now == 0)                   //电流校准
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x03, 20, 15, C_WHITE, C_BLACK);    //校
        UG_PutChar(0x04, 36, 15, C_WHITE, C_BLACK);    //准
        UG_PutChar(0x17, 52, 15, C_WHITE, C_BLACK);    //点
        UG_PutChar(0x00, 28, 40, C_WHITE, C_BLACK);    //电
        UG_PutChar(0x01, 44, 40, C_WHITE, C_BLACK);    //流

        UG_FontSelect(&FONT_10X16);
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
        switch (c_p)
        {
            case 0:
                UG_PutString(80, 15, "< 1A >");
                break;
            case 1:
                UG_PutString(80, 15, "< 2A >");
                break;
            case 2: 
                UG_PutString(80, 15, "< 5A >");
                break;
            case 3:
                UG_PutString(80, 15, "< 10A >");
                break;
            case 4:
                UG_PutString(80, 15, "< 20A >");
                break;
            case 5:
                UG_PutString(80, 15, "< 50A >");
                break;        
            default:
                break;
        }

        
        char buffer[10];
        UG_SetForecolor(C_GREEN);
        gcvt((double)measure.current, 4, buffer);
        UG_PutString(72,40,buffer);
        UG_PutChar('A', 130, 40, C_GREEN, C_BLACK);
        UG_FontSelect(&FONT_8X8);
        UG_SetForecolor(C_RED);
        gcvt((double)usr_config.current_offset[c_p], 4, buffer);
        UG_PutString(44,70,buffer);

    }
    else if(menu_point_now + menu_screen_now == 1)              //电压校准
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x03, 20, 15, C_WHITE, C_BLACK);    //校
        UG_PutChar(0x04, 36, 15, C_WHITE, C_BLACK);    //准
        UG_PutChar(0x17, 52, 15, C_WHITE, C_BLACK);    //点
        UG_PutChar(0x00, 28, 40, C_WHITE, C_BLACK);    //电
        UG_PutChar(0x02, 44, 40, C_WHITE, C_BLACK);    //流

        UG_FontSelect(&FONT_10X16);
        UG_SetBackcolor(C_BLACK);
        UG_SetForecolor(C_WHITE);
        switch (v_p)
        {
            case 0:
                UG_PutString(80, 15, "< 5V >");
                break;
            case 1:
                UG_PutString(80, 15, "< 10V >");
                break;
            case 2: 
                UG_PutString(80, 15, "< 15V >");
                break;
            case 3:
                UG_PutString(80, 15, "< 20V >");
                break;
            case 4:
                UG_PutString(80, 15, "< 25V >");
                break;
            case 5:
                UG_PutString(80, 15, "< 30V >");
                break;        
            default:
                break;
        }

        
        char buffer[10];
        UG_SetForecolor(C_GREEN);
        gcvt((double)measure.voltage, 4, buffer);
        UG_PutString(72,40,buffer);
        UG_PutChar('V', 130, 40, C_GREEN, C_BLACK);
        UG_FontSelect(&FONT_8X8);
        UG_SetForecolor(C_RED);
        gcvt((double)usr_config.voltage_offset[v_p], 4, buffer);
        UG_PutString(44,70,buffer);
    }
    else if(menu_point_now + menu_screen_now == 2)              //过流保护
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x05, 20 + 30, 15, C_WHITE, C_BLACK);    //过
        UG_PutChar(0x01, 36 + 30, 15, C_WHITE, C_BLACK);    //流
        UG_PutChar(0x06, 52 + 30, 15, C_WHITE, C_BLACK);    //保
        UG_PutChar(0x07, 68 + 30, 15, C_WHITE, C_BLACK);    //护

        UG_FontSelect(&FONT_16X26);
        UG_PutChar('0' + (uint32_t)usr_config.limit_current/10, 25, 40, C_RED, C_BLACK);    
        UG_PutChar('0' + (uint32_t)usr_config.limit_current%10, 45, 40, C_RED, C_BLACK);
        UG_PutChar('.', 65, 40, C_WHITE, C_BLACK);
        UG_PutChar('0' + (uint32_t)(usr_config.limit_current*10)%10, 85, 40, C_RED, C_BLACK);
        UG_PutChar('0' + (uint32_t)(usr_config.limit_current*100)%10, 105, 40, C_RED, C_BLACK);
        UG_PutChar('A', 125, 40, C_WHITE, C_BLACK);
        UG_DrawLine(25, 67, 125, 67, C_BLACK);
        UG_DrawLine(27+s_p*20, 67, 38+s_p*20, 67, C_WHITE);
        if(s_f == 1)
        {
            static uint8_t i = 0;
            i++;
            if(i < 10)
            {
                UG_PutChar(' ', 25 + s_p*20, 40, C_RED, C_BLACK);    
            }
            else if(i > 20)
            {
                i = 0;
            }
            
        }

    }
    else if(menu_point_now + menu_screen_now == 3)              //欠压保护
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x16, 20 + 30, 15, C_WHITE, C_BLACK);    //欠
        UG_PutChar(0x02, 36 + 30, 15, C_WHITE, C_BLACK);    //压
        UG_PutChar(0x06, 52 + 30, 15, C_WHITE, C_BLACK);    //保
        UG_PutChar(0x07, 68 + 30, 15, C_WHITE, C_BLACK);    //护

        UG_FontSelect(&FONT_16X26);
        UG_PutChar('0' + (uint32_t)usr_config.limit_voltage/10, 25, 40, C_RED, C_BLACK);    
        UG_PutChar('0' + (uint32_t)usr_config.limit_voltage%10, 45, 40, C_RED, C_BLACK);
        UG_PutChar('.', 65, 40, C_WHITE, C_BLACK);
        UG_PutChar('0' + (uint32_t)(usr_config.limit_voltage*10)%10, 85, 40, C_RED, C_BLACK);
        UG_PutChar('0' + (uint32_t)(usr_config.limit_voltage*100)%10, 105, 40, C_RED, C_BLACK);
        UG_PutChar('V', 125, 40, C_WHITE, C_BLACK);
        UG_DrawLine(25, 67, 125, 67, C_BLACK);
        UG_DrawLine(27+s_p*20, 67, 38+s_p*20, 67, C_WHITE);
        if(s_f == 1)
        {
            static uint8_t i = 0;
            i++;
            if(i < 10)
            {
                UG_PutChar(' ', 25 + s_p*20, 40, C_RED, C_BLACK);    
            }
            else if(i > 20)
            {
                i = 0;
            }
            
        } 
    }
    else if(menu_point_now + menu_screen_now == 4)              //屏幕亮度
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x0A, 20 + 30, 15, C_WHITE, C_BLACK);    //屏
        UG_PutChar(0x0B, 36 + 30, 15, C_WHITE, C_BLACK);    //幕
        UG_PutChar(0x0C, 52 + 30, 15, C_WHITE, C_BLACK);    //亮
        UG_PutChar(0x0D, 68 + 30, 15, C_WHITE, C_BLACK);    //度

        UG_DrawRoundFrame(30, 60, 130, 63, 2, C_WHITE);
        UG_FillFrame(30, 61, 30+usr_config.light/10, 62, C_WHITE);
        UG_FillFrame(31+usr_config.light/10, 61, 129, 62, C_BLACK);

    }
    else if(menu_point_now + menu_screen_now == 5)              //蜂鸣器音量
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x0E, 20 + 22, 15, C_WHITE, C_BLACK);    //蜂
        UG_PutChar(0x0F, 36 + 22, 15, C_WHITE, C_BLACK);    //鸣
        UG_PutChar(0x10, 52 + 22, 15, C_WHITE, C_BLACK);    //器
        UG_PutChar(0x11, 68 + 22, 15, C_WHITE, C_BLACK);    //音
        UG_PutChar(0x12, 84 + 22, 15, C_WHITE, C_BLACK);    //量

        UG_DrawRoundFrame(30, 60, 130, 63, 2, C_WHITE);
        UG_FillFrame(30, 61, 30+usr_config.volume/10, 62, C_WHITE);
        UG_FillFrame(31+usr_config.volume/10, 61, 129, 62, C_BLACK);
    }
    else if(menu_point_now + menu_screen_now == 6)              //保存设置
    {
        UG_FontSelect(&MY_FONT);
        UG_PutChar(0x06, 20 + 30, 35, C_WHITE, C_BLACK);    //保
        UG_PutChar(0x13, 36 + 30, 35, C_WHITE, C_BLACK);    //存
        UG_PutChar(0x18, 52 + 30, 35, C_WHITE, C_BLACK);    //成
        UG_PutChar(0x19, 68 + 30, 35, C_WHITE, C_BLACK);    //功
    }
}

void Button_SubMenu(Button_Act_e button_act)
{
    if(menu_point_now + menu_screen_now == 0)
    {
        switch (button_act)
        {
            case Button_UP:
                /* code */
                if(c_p > 0)
                {
                    c_p--;
                }
                break;
            case Button_DOWN:
                if (c_p < 5)    
                {
                    c_p++;
                }
                
                break;
            case Button_SET_s:
                 switch (c_p)
                 {
                 case 0:
                    if(measure.current > 0.7f && measure.current < 1.3f)
                    usr_config.current_offset[c_p] = 1.0f-measure.current;
                    break;
                 case 1:
                    if(measure.current > 1.5f && measure.current < 2.5f)
                    usr_config.current_offset[c_p] = 2.0f-measure.current;
                    break;
                 case 2:
                    if(measure.current > 4.0f && measure.current < 6.0f)
                    usr_config.current_offset[c_p] = 5.0f-measure.current;
                    break;
                 case 3:
                    if(measure.current > 8.0f && measure.current < 12.0f)
                    usr_config.current_offset[c_p] = 10.0f-measure.current;
                    break;
                 case 4:
                    if(measure.current > 15.0f && measure.current < 25.0f)
                    usr_config.current_offset[c_p] = 20.0f-measure.current;
                    break;
                 case 5:
                    if(measure.current > 40.0f && measure.current < 60.0f)
                    usr_config.current_offset[c_p] = 50.0f-measure.current;
                    break;
                 default:
                    break;
                 }
                /* code */
                break;
            case Button_SET_l:
                head->back->init();
                head = head->back;
                /* code */
                break;
            
            default:
                break;
        }
    }
    else if(menu_point_now + menu_screen_now == 1)
    {
        switch (button_act)
        {
            case Button_UP:
                /* code */
                if(v_p > 0)
                {
                    v_p--;
                }
                break;
            case Button_DOWN:
                if (v_p < 5)    
                {
                    v_p++;
                }
                
                break;
            case Button_SET_s:
                 switch (v_p)
                 {
                 case 0:
                    if(measure.voltage > 3.0f && measure.voltage < 7.0f)
                    usr_config.voltage_offset[v_p] = 5.0f-measure.voltage;
                    break;
                 case 1:
                    if(measure.voltage > 8.0f && measure.voltage < 12.0f)
                    usr_config.voltage_offset[v_p] = 10.0f-measure.voltage;
                    break;
                 case 2:
                    if(measure.voltage > 13.0f && measure.voltage < 17.0f)
                    usr_config.voltage_offset[v_p] = 15.0f-measure.voltage;
                    break;
                 case 3:
                    if(measure.voltage > 18.0f && measure.voltage < 22.0f)
                    usr_config.voltage_offset[v_p] = 20.0f-measure.voltage;
                    break;
                 case 4:
                    if(measure.voltage > 23.0f && measure.voltage < 27.0f)
                    usr_config.voltage_offset[v_p] = 25.0f-measure.voltage;
                    break;
                 case 5:
                    if(measure.voltage > 28.0f && measure.voltage < 32.0f)
                    usr_config.voltage_offset[v_p] = 30.0f-measure.voltage;
                    break;
                 default:
                    break;
                 }
                /* code */
                break;
            case Button_SET_l:
                head->back->init();
                head = head->back;
                /* code */
                break;
            
            default:
                break;
        }
    }
    else if(menu_point_now + menu_screen_now == 2)
    {
        switch (button_act)
        {
        case Button_UP:
            /* code */
            if(s_f == 0)
            {
                if(s_p > 0)
                {
                    s_p--;
                }
                if(s_p == 2) s_p--;
            }
            else 
            {
                switch (s_p)
                {
                case 0:
                    usr_config.limit_current += 10;
                    break;
                case 1:
                    usr_config.limit_current += 1;
                    break;
                case 3: 
                    usr_config.limit_current += 0.1f;
                    break;
                case 4:
                    usr_config.limit_current += 0.01f;
                default:
                    break;
                }
                if(usr_config.limit_current > 30.0f) usr_config.limit_current = 30.0f;
            }
            break;
        case Button_DOWN:
            if(s_f == 0)
            {
                if(s_p < 4)
                {
                    s_p++;
                }
                if(s_p == 2) s_p++;
            }
            else 
            {
                switch (s_p)
                {
                case 0:
                    usr_config.limit_current -= 10;
                    break;
                case 1:
                    usr_config.limit_current -= 1;
                    break;
                case 3: 
                    usr_config.limit_current -= 0.1f;
                    break;
                case 4:
                    usr_config.limit_current -= 0.01f;
                default:
                    break;
                }
                if(usr_config.limit_current < 0.0f) usr_config.limit_current = 0.0f;
            }
            break;
        case Button_SET_l:
            if(s_f == 1)
            {
                s_f = 0;
            }
            else
            {
                head->back->init();
                head = head->back;
            }
            break;
        case Button_SET_s:
            if(s_f == 0)s_f = 1;
            else s_f = 0;
            break;
        default:
            break;
        }
    }
    else if(menu_point_now + menu_screen_now == 3)
    {
        switch (button_act)
        {
        case Button_UP:
            /* code */
            if(s_f == 0)
            {
                if(s_p > 0)
                {
                    s_p--;
                }
                if(s_p == 2) s_p--;
            }
            else 
            {
                switch (s_p)
                {
                case 0:
                    usr_config.limit_voltage += 10;
                    break;
                case 1:
                    usr_config.limit_voltage += 1;
                    break;
                case 3: 
                    usr_config.limit_voltage += 0.1f;
                    break;
                case 4:
                    usr_config.limit_voltage += 0.01f;
                default:
                    break;
                }
                if(usr_config.limit_voltage > 30.0f) usr_config.limit_voltage = 30.0f;
            }
            break;
        case Button_DOWN:
            if(s_f == 0)
            {
                if(s_p < 4)
                {
                    s_p++;
                }
                if(s_p == 2) s_p++;
            }
            else 
            {
                switch (s_p)
                {
                case 0:
                    usr_config.limit_voltage -= 10;
                    break;
                case 1:
                    usr_config.limit_voltage -= 1;
                    break;
                case 3: 
                    usr_config.limit_voltage -= 0.1f;
                    break;
                case 4:
                    usr_config.limit_voltage -= 0.01f;
                default:
                    break;
                }
                if(usr_config.limit_voltage < 0.0f) usr_config.limit_voltage = 0.0f;
            }
            /* code */
            break;
        case Button_SET_l:
            if(s_f == 1)
            {
                s_f = 0;
            }
            else
            {
                head->back->init();
                head = head->back;
            }
            break;
        case Button_SET_s:
            if(s_f == 0)s_f = 1;
            else s_f = 0;
            break;
        default:
            break;
        }
    }
    else if(menu_point_now + menu_screen_now == 4)
    {
        switch (button_act)
        {
            case Button_UP:
                if(usr_config.light < 900) usr_config.light += 20;
                break;
            case Button_DOWN:
                if(usr_config.light > 100) usr_config.light -= 20;
                break;
            case Button_SET_l:
                break;
            case Button_SET_s:
                head->back->init();
                head = head->back;
                break;
            default:
                break;  
        }
    }
    else if(menu_point_now + menu_screen_now == 5)
    {
        switch (button_act)
        {
            case Button_UP:
                if(usr_config.volume < 900) usr_config.volume += 20;
                break;
            case Button_DOWN:
                if(usr_config.volume > 0) usr_config.volume -= 20;
                break;
            case Button_SET_l:
                break;
            case Button_SET_s:
                head->back->init();
                head = head->back;
                break;
            default:
                break;  
        }
    }
    else if(menu_point_now + menu_screen_now == 6)
    {
        if(button_act == Button_SET_s)
        {
            usr_config.flag = 1;
            Write_Flash(FLASH_SAVE_ADDR, (uint64_t*)&usr_config, sizeof(usr_config_t)/8);
            NVIC_SystemReset(); 
        }
    }
}

void Mygui_init(void)
{
    memcpy(&usr_config, (__IO uint32_t *)(FLASH_SAVE_ADDR), sizeof(usr_config_t));

    if(usr_config.flag != 1)
    {
        usr_config.flag = 0;
        usr_config.volume = 500;
        usr_config.current_limit_sw = 0;
        for(int i = 0; i < 6; i++)
        {
            usr_config.current_offset[i] = 0;
            usr_config.voltage_offset[i] = 0;
        }
        usr_config.light = 500;
        usr_config.limit_current = 0;
        usr_config.limit_voltage = 0;
        usr_config.voltage_limit_sw = 0;  
    }


    measure.output = 1;
    measure.current_reso = 0.125f;
    ST7735_Init();
    UG_Init(&mygui, ST7735_DrawPoint, 160, 80);
    UG_FillScreen(C_BLACK);
    ST7735_Draw();

    Window1 = Creat_Menu(Window1_init, Window1_update, Button_uni, &Window2);
    Window1.next = &Window2;
    Window1.sub = &Menu;
    Window2 = Creat_Menu(Window2_init, Window2_update, Button_uni, &Window1);
    Window2.next = &Window1;
    Window2.sub = &Menu;

    Menu = Creat_Menu(Clear_Screen, Menu_update, Button_Menu, &Window1);
    Menu.sub = &Sub_Menu;

    Sub_Menu = Creat_Menu(Clear_Screen, SubMenu_update, Button_SubMenu, &Menu);

    head = &Window1;

    Window1_init();
}

void Mygui_task()
{
    head->action();  
    TIM4->CCR2 = usr_config.light;
}