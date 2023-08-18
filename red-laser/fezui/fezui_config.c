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
#include "w25qxx.h"
#include "steer.h"
#include "pid-control.h"

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
    W25qxx_Init();
    fezui_read_pid();
    fezui_read_central();
    //theta_central = 117370;
    //phi_central = 123370;
    //theta_pid.pGain=8;
    //theta_pid.iGain=3;
    //theta_pid.dGain=0;
    //phi_pid.pGain=25;
    //phi_pid.iGain=4;
    //phi_pid.dGain=0;
    //fezui_save();
    lefl_link_frame_navigate(&mainframe, &menupage);
}
#define ROW_HEIGHT 8
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
    if(moving)
    {
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);
        //u8g2_ClearBuffer(&(fezui.u8g2));

        sprintf(fezui_buffer,"theta:%lf",theta_pid.errdat);
        u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT*1, fezui_buffer);

        sprintf(fezui_buffer,"phi:%lf",phi_pid.errdat);
        u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT*2, fezui_buffer);

        sprintf(fezui_buffer,"outz:%ld",theta_pid.pidout);
        u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT*3, fezui_buffer);

        sprintf(fezui_buffer,"putx:%ld",phi_pid.pidout);
        u8g2_DrawStr(&(fezui.u8g2), 0, ROW_HEIGHT*4, fezui_buffer);

        sprintf(fezui_buffer,"ms:%ld",move_step);
        u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*4, fezui_buffer);

        sprintf(fezui_buffer,"qc:%ld",Queue_Count(&point_queue));
        u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*5, fezui_buffer);

        sprintf(fezui_buffer,"%.0lf",target_actual_point.x);
        u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*6, fezui_buffer);

        sprintf(fezui_buffer,"%.0lf",target_actual_point.z);
        u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*7, fezui_buffer);
        //u8g2_SendBuffer(&(fezui.u8g2));
    }
    else
    {
        lefl_link_frame_draw(&mainframe);
    }
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);
    sprintf(fezui_buffer,"x:%.0f",current_actual_point.x);
    u8g2_DrawStr(&(fezui.u8g2), 96, ROW_HEIGHT*1, fezui_buffer);

    sprintf(fezui_buffer,"z:%.0f",current_actual_point.z);
    u8g2_DrawStr(&(fezui.u8g2), 96, ROW_HEIGHT*2, fezui_buffer);
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

void fezui_read_pid()
{
    W25qxx_ReadBytes((uint8_t*)(&(theta_pid.pGain)),0+4*0,4);
    W25qxx_ReadBytes((uint8_t*)(&(theta_pid.iGain)),0+4*1,4);
    W25qxx_ReadBytes((uint8_t*)(&(theta_pid.dGain)),0+4*2,4);
    W25qxx_ReadBytes((uint8_t*)(&(phi_pid.pGain)),0+4*3,4);
    W25qxx_ReadBytes((uint8_t*)(&(phi_pid.iGain)),0+4*4,4);
    W25qxx_ReadBytes((uint8_t*)(&(phi_pid.dGain)),0+4*5,4);
}

void fezui_save()
{
    uint8_t write_buffer[128];
    memcpy(write_buffer+0*4,(uint8_t*)(&(theta_pid.pGain)),4);
    memcpy(write_buffer+1*4,(uint8_t*)(&(theta_pid.iGain)),4);
    memcpy(write_buffer+2*4,(uint8_t*)(&(theta_pid.dGain)),4);
    memcpy(write_buffer+3*4,(uint8_t*)(&(phi_pid.pGain)),4);
    memcpy(write_buffer+4*4,(uint8_t*)(&(phi_pid.iGain)),4);
    memcpy(write_buffer+5*4,(uint8_t*)(&(phi_pid.dGain)),4);
    W25qxx_EraseBlock(0);
    W25qxx_WriteBlock(write_buffer, 0, 0, 4*8);
}

void fezui_read_central()
{
    W25qxx_ReadBytes((uint8_t*)(&theta_central),0+4*6,4);
    W25qxx_ReadBytes((uint8_t*)(&phi_central),0+4*7,4);
}

void fezui_save()
{
    uint8_t write_buffer[128];
    memcpy(write_buffer+4*0,(uint8_t*)(&(theta_pid.pGain)),4);
    memcpy(write_buffer+4*1,(uint8_t*)(&(theta_pid.iGain)),4);
    memcpy(write_buffer+4*2,(uint8_t*)(&(theta_pid.dGain)),4);
    memcpy(write_buffer+4*3,(uint8_t*)(&(phi_pid.pGain)),4);
    memcpy(write_buffer+4*4,(uint8_t*)(&(phi_pid.iGain)),4);
    memcpy(write_buffer+4*5,(uint8_t*)(&(phi_pid.dGain)),4);
    memcpy(write_buffer+4*6,(uint8_t*)(&theta_central),4);
    memcpy(write_buffer+4*7,(uint8_t*)(&phi_central),4);
    W25qxx_EraseBlock(0);
    W25qxx_WriteBlock(write_buffer, 0, 0, 4*8);
}

