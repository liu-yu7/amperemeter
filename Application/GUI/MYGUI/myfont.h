#include "ugui.h"
#include "ugui_config.h"



__UG_FONT_DATA unsigned char My_font[256][32] = 
{
/* GB2312 16x16点阵字库*/
/* 0电[0xB5E7]   16x16 */
0x40,0x00,0x40,0x00,0x40,0x08,0xFE,0x1F,0x42,0x08,0x42,0x08,0xFE,0x0F,0x42,0x08,
0x42,0x08,0xFE,0x0F,0x42,0x08,0x40,0x00,0x40,0x20,0x40,0x20,0x80,0x3F,0x00,0x00,
/* GB2312 16x16点阵字库*/
/* 1流[0xC1F7]   16x16 */
0x00,0x01,0x02,0x12,0xEC,0x3F,0x08,0x02,0x01,0x09,0x86,0x10,0xE4,0x3F,0x10,0x20,
0x88,0x0A,0x84,0x0A,0x87,0x0A,0x84,0x0A,0x84,0x4A,0x84,0x4A,0x44,0x72,0x24,0x02,
/* GB2312 16x16点阵字库*/
/* 2压[0xD1B9]   16x16 */
0x00,0x10,0xFC,0x3F,0x04,0x01,0x04,0x01,0x04,0x01,0x04,0x01,0x04,0x11,0xF4,0x3F,
0x04,0x01,0x04,0x05,0x04,0x19,0x04,0x11,0x02,0x01,0x02,0x21,0xF9,0x7F,0x00,0x00,
/* GB2312 16x16点阵字库*/
/* 3校[0xD0A3]   16x16 */
0x08,0x02,0x08,0x04,0x08,0x20,0xC8,0x7F,0x3F,0x00,0x08,0x11,0x8C,0x60,0x5C,0x51,
0x2A,0x11,0x0A,0x0A,0x09,0x0A,0x08,0x04,0x08,0x0A,0x08,0x11,0xC8,0x70,0x38,0x20,
/* GB2312 16x16点阵字库*/
/* 4准[0xD7BC]   16x16 */
0x80,0x02,0x82,0x04,0x8C,0x24,0xC8,0x7F,0x40,0x04,0x68,0x14,0xD8,0x3F,0x48,0x04,
0x44,0x14,0xC7,0x3F,0x44,0x04,0x44,0x04,0x44,0x24,0xC4,0x7F,0x44,0x00,0x40,0x00,
/* GB2312 16x16点阵字库*/
/* 5过[0xB9FD]   16x16 */
0x00,0x08,0x02,0x08,0x04,0x08,0xE8,0x3F,0x00,0x08,0x00,0x08,0x8F,0x08,0x08,0x09,
0x08,0x09,0x08,0x08,0x08,0x08,0x08,0x0A,0x08,0x04,0x14,0x60,0xE2,0x3F,0x00,0x00,
/* GB2312 16x16点阵字库*/
/* 6保[0xB1A3]   16x16 */
0x10,0x10,0xD0,0x3F,0x50,0x10,0x48,0x10,0x48,0x10,0xCC,0x1F,0x4C,0x02,0x0A,0x22,
0xF9,0x7F,0x08,0x02,0x08,0x07,0x88,0x0A,0x48,0x12,0x28,0x72,0x18,0x22,0x08,0x02,
/* GB2312 16x16点阵字库*/
/* 7护[0xBBA4]   16x16 */
0x08,0x01,0x08,0x06,0x08,0x24,0xC8,0x7F,0x7F,0x20,0x48,0x20,0x68,0x20,0xD8,0x3F,
0x4C,0x20,0x4B,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x28,0x00,0x2A,0x00,0x14,0x00,
/* GB2312 16x16点阵字库*/
/* 8警[0xBEAF]   16x16 */
0x28,0x02,0xFE,0x3E,0x08,0x13,0xFC,0x14,0xAA,0x08,0xB8,0x76,0xC0,0x20,0xFF,0x7F,
0x00,0x00,0xF8,0x0F,0x00,0x00,0xF8,0x0F,0x00,0x00,0xF8,0x0F,0x08,0x08,0xF8,0x0F,
/* GB2312 16x16点阵字库*/
/* 9告[0xB8E6]   16x16 */
0x80,0x00,0x88,0x00,0x88,0x08,0xF8,0x1F,0x88,0x00,0x84,0x00,0x80,0x20,0xFF,0x7F,
0x00,0x08,0xF8,0x1F,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08,
/* GB2312 16x16点阵字库*/
/* A屏[0xC6C1]   16x16 */
0x00,0x10,0xFC,0x3F,0x04,0x10,0x04,0x10,0xFC,0x1F,0x44,0x04,0x84,0x12,0xF4,0x3F,
0x44,0x04,0x44,0x24,0xFC,0x7F,0x44,0x04,0x44,0x04,0x42,0x04,0x21,0x04,0x10,0x04,
/* GB2312 16x16点阵字库*/
/* B幕[0xC4BB]   16x16 */
0x20,0x12,0xFE,0x3F,0x20,0x0A,0xF8,0x1F,0x08,0x08,0xF8,0x0F,0x08,0x08,0xF8,0x0F,
0x20,0x20,0xFF,0x7F,0x88,0x08,0xFC,0x7F,0x8B,0x28,0x88,0x0A,0x88,0x04,0x80,0x00,
/* GB2312 16x16点阵字库*/
/* C亮[0xC1C1]   16x16 */
0x80,0x10,0xFE,0x3F,0x00,0x08,0xF8,0x1F,0x08,0x08,0x08,0x08,0xF8,0x0F,0x00,0x00,
0xFE,0x7F,0x02,0x44,0xF1,0x2F,0x10,0x04,0x10,0x04,0x10,0x44,0x08,0x44,0x06,0x78,
/* GB2312 16x16点阵字库*/
/* D度[0xB6C8]   16x16 */
0x80,0x00,0x00,0x21,0xFC,0x7F,0x44,0x04,0x44,0x14,0xFC,0x3F,0x44,0x04,0xC4,0x07,
0x04,0x00,0xF4,0x0F,0x44,0x04,0x84,0x02,0x04,0x01,0xC2,0x06,0x31,0x78,0x0C,0x20,
/* GB2312 16x16点阵字库*/
/* E蜂[0xB7E4]   16x16 */
0x08,0x01,0x08,0x01,0x08,0x3F,0xA8,0x11,0x7E,0x0A,0x2A,0x04,0x2A,0x7B,0xEA,0x24,
0x2A,0x1F,0x3E,0x04,0x8A,0x3F,0x28,0x04,0xB8,0x3F,0x27,0x04,0x02,0x04,0x00,0x04,
/* GB2312 16x16点阵字库*/
/* F鸣[0xC3F9]   16x16 */
0x00,0x02,0x10,0x11,0xBE,0x3F,0x92,0x10,0x92,0x12,0x92,0x10,0x92,0x14,0x92,0x08,
0x92,0x20,0x92,0x7F,0x1E,0x20,0x12,0x24,0xE0,0x2F,0x00,0x20,0x00,0x28,0x00,0x10,
/* GB2312 16x16点阵字库*/
/* 10器[0xC6F7]   16x16 */
0x7C,0x1F,0x44,0x11,0x44,0x11,0x44,0x11,0x7C,0x1F,0x80,0x04,0x80,0x28,0xFF,0x7F,
0x40,0x01,0x30,0x06,0x0C,0x18,0x7F,0x7F,0x44,0x11,0x44,0x11,0x44,0x11,0x7C,0x1F,
/* GB2312 16x16点阵字库*/
/* 11音[0xD2F4]   16x16 */
0x40,0x00,0x80,0x08,0xFC,0x1F,0x10,0x04,0x20,0x04,0x20,0x22,0xFF,0x7F,0x00,0x08,
0xF8,0x1F,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08,0x08,0x08,0xF8,0x0F,0x08,0x08,
/* GB2312 16x16点阵字库*/
/* 12量[0xC1BF]   16x16 */
0x00,0x08,0xF8,0x1F,0x08,0x08,0xF8,0x0F,0x08,0x28,0xFF,0x7F,0x00,0x00,0xF8,0x0F,
0x88,0x08,0xF8,0x0F,0x88,0x08,0xF8,0x0F,0x80,0x00,0xF8,0x0F,0x80,0x00,0xFE,0x3F,
/* GB2312 16x16点阵字库*/
/* 13存[0xB4E6]   16x16 */
0x40,0x00,0x40,0x20,0xFF,0x7F,0x20,0x00,0x10,0x00,0xD0,0x1F,0x08,0x08,0x08,0x04,
0x0C,0x22,0xFA,0x7F,0x09,0x02,0x08,0x02,0x08,0x02,0x08,0x02,0x88,0x02,0x08,0x01,
/* GB2312 16x16点阵字库*/
/* 14设[0xC9E8]   16x16 */
0x80,0x0F,0x84,0x08,0x88,0x08,0x88,0x08,0x80,0x08,0x40,0x70,0x2F,0x00,0xC8,0x1F,
0x88,0x10,0x88,0x08,0x08,0x09,0x08,0x05,0x28,0x02,0x18,0x0D,0xC8,0x70,0x30,0x20,
/* GB2312 16x16点阵字库*/
/* 15置[0xD6C3]   16x16 */
0xFC,0x1F,0x24,0x12,0xFC,0x1F,0x80,0x00,0xFE,0x3F,0x80,0x00,0xF8,0x0F,0x08,0x08,
0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x08,0x08,0xF8,0x0F,0x08,0x28,0xFF,0x7F,0x00,0x00,
/* GB2312 16x16点阵字库*/
/* 16欠[0xC7B7]   16x16 */
0x10,0x00,0x10,0x00,0x10,0x00,0xF8,0x3F,0x08,0x20,0x84,0x10,0x82,0x08,0x81,0x00,
0x40,0x01,0x40,0x01,0x20,0x02,0x20,0x02,0x10,0x04,0x08,0x08,0x04,0x70,0x03,0x20,
/* GB2312 16x16点阵字库*/
/* 17点[0xB5E3]   16x16 */
0x40,0x00,0x40,0x00,0x40,0x10,0xC0,0x3F,0x40,0x00,0x40,0x08,0xFC,0x1F,0x04,0x08,
0x04,0x08,0x04,0x08,0xFC,0x0F,0x00,0x00,0x94,0x08,0x24,0x13,0x22,0x22,0x01,0x20,
/* GB2312 16x16点阵字库*/
/* 18成[0xB3C9]   16x16 */
0x00,0x01,0x00,0x05,0x00,0x09,0xFC,0x3F,0x04,0x01,0x04,0x01,0x04,0x21,0x7C,0x22,
0x44,0x12,0x44,0x12,0x44,0x0C,0x54,0x04,0x24,0x46,0x02,0x49,0x81,0x50,0x00,0x60,
/* GB2312 16x16点阵字库*/
/* 19功[0xB9A6]   16x16 */
0x00,0x01,0x00,0x01,0x10,0x01,0x3F,0x01,0x08,0x21,0xE8,0x7F,0x08,0x21,0x08,0x21,
0x08,0x21,0x08,0x21,0xB8,0x20,0x8F,0x20,0x82,0x20,0x40,0x22,0x20,0x14,0x10,0x08,
    
};
const UG_FONT MY_FONT = {(unsigned char*)My_font,FONT_TYPE_1BPP,16,16,0,255,NULL};