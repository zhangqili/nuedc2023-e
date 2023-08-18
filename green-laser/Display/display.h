/*
 * display.h
 *
 *  Created on: 2023年5月23日
 *      Author: xq123
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "main.h"
#include "u8g2.h"


#define MAX_LEN    128  //
#define OLED_ADDRESS  0x78 // oled模块从机地址
#define OLED_CMD   0x00  // 写命令
#define OLED_DATA  0x40  // 写数据

extern u8g2_t u8g2;

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void draw(u8g2_t *u8g2);

#endif /* DISPLAY_H_ */
