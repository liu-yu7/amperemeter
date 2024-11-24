#ifndef __MYGUI_H
#define __MYGUI_H

#include "ugui.h"


typedef struct
{
    uint32_t time;      //运行时间，单位：秒
    uint8_t output;     //输出状态，0：关闭，1：开启
    float current;      //电流，单位：A
    float voltage;      //电压，单位：V
    float power;        //功率，单位：W
    float energy;       //电能，单位：Wh
    float current_reso;  //电流分辨率，单位：A
    uint8_t current_buf[36];    //电流缓冲区，用于绘制曲线
    uint8_t current_buf_index;  //电流缓冲区索引
    float min_current;    //最小电流，单位：A
    float max_current;    //最大电流，单位：A
    uint8_t flag;         //标志位1：按键触发，2：保护
} Measure_t;

typedef struct
{
    uint32_t flag;                  //写flash标志位，0：未写，1：已写
    uint32_t current_limit_sw;      //电流限制开关，0：关闭，1：开启
    float limit_current;            //电流限制，单位：A
    uint32_t voltage_limit_sw;      //电压限制开关，0：关闭，1：开启
    float limit_voltage;            //电压限制，单位：V

    float current_offset[6];        //电流校准(1A,2A,5A,10A,20A,50A)，单位：A
    float voltage_offset[6];        //电压校准(5V,10V,15V,20V,25V,30V)，单位：V

    uint32_t light;                 //背光亮度，0-100
    uint32_t volume;                //蜂鸣器音量，0-100
    uint32_t unuse;                  //未使用

}usr_config_t;

typedef enum
{
    Button_UP,
    Button_DOWN,        
    Button_SET_s,       //短按
    Button_SET_l,       //长按
}Button_Act_e;


typedef struct MenuItem
{
	struct MenuItem *next;			                /* 指向下一个菜单项的指针 */
	struct MenuItem *back;			                /* 指向上一个菜单项的指针 */
	struct MenuItem *sub;			                /* 指向该级子菜单项的指针 */
    void (*init)();				                    /* 指向初始化函数的指针 */
	void (*action)();				                /* 指向执行函数的指针 */
    void (*button)(Button_Act_e button_act);		/* 指向该菜单按键回调函数 */
}MenuItem_t;

extern Measure_t measure;
extern usr_config_t usr_config;

void Button_Callback(Button_Act_e button_act);
void Mygui_init(void);
void Mygui_task();

#endif