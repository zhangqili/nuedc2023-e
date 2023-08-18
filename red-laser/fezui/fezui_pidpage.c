/*
 * fezui_pidpage.c
 *
 *  Created on: Aug 3, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "tim.h"
#include "steer.h"
#include "stdlib.h"
#include "rng.h"
#include "pid-control.h"

#define INTERVAL 0.01
lefl_page_t pidpage={pidpage_logic,pidpage_draw,pidpage_load};

extern bool number_editing;

#define ROW_HEIGHT 13

static fezui_scrollview_t scrollview ={.content_height=12*6,.content_width=40};

lefl_menu_t pidmenu = {
    .items = {"theta_P","theta_I", "theta_D", "phi_P","phi_I", "phi_D"},
    .selected_index = 0,
    .len = 6
};

static float target_ordinate=0;

void pidpage_logic(lefl_page_t *page)
{
    lefl_easing_pid(&(scrollview.ordinate), target_ordinate);

    if(number_editing)
    {
        lefl_cursor_set(
                &target_cursor ,
                0,
                pidmenu.selected_index*ROW_HEIGHT+3 - (u8g2_int_t)scrollview.ordinate,
                128-6,
                ROW_HEIGHT);
    }
    else
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                pidmenu.selected_index*ROW_HEIGHT+3  - (u8g2_int_t)scrollview.ordinate,
                strlen(pidmenu.items[pidmenu.selected_index])*6+3,
                ROW_HEIGHT);
    }
    if((pidmenu.selected_index+1)*ROW_HEIGHT-target_ordinate>64)
    {
        target_ordinate = (pidmenu.selected_index+1)*ROW_HEIGHT-64;
    }
    if((pidmenu.selected_index)*ROW_HEIGHT<target_ordinate)
    {
        target_ordinate = (pidmenu.selected_index)*ROW_HEIGHT;
    }
}
void pidpage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);

    sprintf(fezui_buffer,"%.2lf",theta_pid.pGain);
    u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*1 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.2lf",theta_pid.iGain);
    u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.2lf",theta_pid.dGain);
    u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*3 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.2lf",phi_pid.pGain);
    u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*4 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.2lf",phi_pid.iGain);
    u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*5 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.2lf",phi_pid.dGain);
    u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*6 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    fezui_draw_scrollview(&fezui, 0, 0, 128, 64, &scrollview);

    for(uint8_t i=0;i<pidmenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ROW_HEIGHT*(i+1) - (u8g2_int_t)scrollview.ordinate,pidmenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);
}
void pidpage_load(lefl_page_t *page)
{
    key_back.key_cb =lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            number_editing = false;
        }
        else
        {
            lefl_link_frame_go_back(&mainframe);
            fezui_save();
        }
    });
    key_go.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
        }
        else
        {
            number_editing = true;
        }
    });
    key_down.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(pidmenu.selected_index)
            {
                case 0:
                    theta_pid.pGain -= INTERVAL;
                    break;
                case 1:
                    theta_pid.iGain -= INTERVAL;
                    break;
                case 2:
                    theta_pid.dGain -= INTERVAL;
                    break;
                case 3:
                    phi_pid.pGain -= INTERVAL;
                    break;
                case 4:
                    phi_pid.iGain -= INTERVAL;
                    break;
                case 5:
                    phi_pid.dGain -= INTERVAL;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&pidmenu, 1);
        }
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(pidmenu.selected_index)
            {
                case 0:
                    theta_pid.pGain += INTERVAL;
                    break;
                case 1:
                    theta_pid.iGain += INTERVAL;
                    break;
                case 2:
                    theta_pid.dGain += INTERVAL;
                    break;
                case 3:
                    phi_pid.pGain += INTERVAL;
                    break;
                case 4:
                    phi_pid.iGain += INTERVAL;
                    break;
                case 5:
                    phi_pid.dGain += INTERVAL;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&pidmenu, -1);
        }
    });
}



