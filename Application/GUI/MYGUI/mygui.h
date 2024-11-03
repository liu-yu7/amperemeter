#ifndef __MYGUI_H
#define __MYGUI_H

#include "ugui.h"


typedef struct
{
    uint32_t time;     //运行时间，单位：秒
    uint8_t output;    //输出状态，0：关闭，1：开启
    float current;      //电流，单位：A
    float voltage;      //电压，单位：V
    float power;        //功率，单位：W
    float energy;       //电能，单位：Wh
    uint8_t current_buf[60];  //电流缓冲区，用于绘制曲线
} Measure_t;

typedef struct
{
    uint32_t current_limit_sw;      //电流限制开关，0：关闭，1：开启
    float limit_current;            //电流限制，单位：A
    uint32_t voltage_limit_sw;      //电压限制开关，0：关闭，1：开启
    float limit_voltage;            //电压限制，单位：V

    float current_offset[6];        //电流校准(1A,2A,5A,10A,20A,50A)，单位：A
    float voltage_offset[6];        //电压校准(5V,10V,15V,20V,25V,30V)，单位：V

    uint32_t light;                 //背光亮度，0-100
    uint32_t volume;                //蜂鸣器音量，0-100

}usr_config_t;

extern Measure_t measure;
extern usr_config_t usr_config;

void Mygui_init(void);
void Mygui_task();

#endif