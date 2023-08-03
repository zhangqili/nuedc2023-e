/*
 * fezui_config.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "lefl.h"
#include "display.h"


uint8_t tempuint;


void fezui_init()
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(&(fezui.u8g2), U8G2_R0, u8x8_byte_hw_i2c,
        u8x8_gpio_and_delay);
    u8g2_InitDisplay(&(fezui.u8g2));
    u8g2_SetPowerSave(&(fezui.u8g2), 0);
    u8g2_SetBitmapMode(&(fezui.u8g2), 1);
    u8g2_SetFontMode(&(fezui.u8g2), 1);
    u8g2_ClearBuffer(&(fezui.u8g2));
    u8g2_SendBuffer(&(fezui.u8g2));

    lefl_link_frame_navigate(&mainframe, &menupage);
}

void fezui_timer_handler()
{
    fezui_fps++;
    lefl_key_update(&key_up, !HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin));
    lefl_key_update(&key_down, !HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin));
    lefl_key_update(&key_go, !HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin));
    lefl_key_update(&key_back, !HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin));

    lefl_link_frame_logic(&mainframe);
    lefl_cursor_move(&cursor, &target_cursor);
    u8g2_ClearBuffer(&(fezui.u8g2));
    if(fezui.invert)
    {
        u8g2_SetDrawColor(&(fezui.u8g2), 1);
        u8g2_DrawBox(&(fezui.u8g2), 0, 0, 128, 64);
        u8g2_SetDrawColor(&(fezui.u8g2), !fezui.invert);
    }
    else
    {
        u8g2_SetDrawColor(&(fezui.u8g2), !fezui.invert);
    }
    lefl_link_frame_draw(&mainframe);
#ifdef _SCREEN_REST_ON
    u8g2_SetPowerSave(&(fezui.u8g2),!fezui_rest_countdown);
    fezui_veil(&(fezui),0,0,128,64,7-fezui_rest_countdown,0);
#endif
#ifdef _FPS_ON
    u8g2_SetDrawColor(&(fezui.u8g2), 2);
    u8g2_SetFont(&(fezui.u8g2), fez_font_6x10_m);
    u8g2_DrawStr(&(fezui.u8g2),95+15,10,fpsstr);
#endif
    u8g2_SendBuffer(&(fezui.u8g2));
    fezui_fps++;
}

void fezui_waiting()
{
    u8g2_ClearBuffer(&(fezui.u8g2));
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x12_tf);
    u8g2_DrawStr(&(fezui.u8g2),0,32,"executing...");
    u8g2_SendBuffer(&(fezui.u8g2));
}

extern fezui_t fezui;


