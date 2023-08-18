/*
 * fezui_adjust.c
 *
 *  Created on: Aug 2, 2023
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "tim.h"
#include "steer.h"

lefl_page_t adjustpage={adjustpage_logic,adjustpage_draw,adjustpage_load};

extern uint32_t theta_central;
extern uint32_t phi_central;

lefl_menu_t adjustmenu = {
    .items = {"theta","phi"},
    .selected_index = 0,
    .len = 2,
};


void adjustpage_logic(lefl_page_t *page)
{
    if(number_editing)
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                adjustmenu.selected_index*ITEM_HEIGHT+3 ,
                128-3,
                ITEM_HEIGHT);
    }
    else
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                adjustmenu.selected_index*ITEM_HEIGHT+3 ,
                strlen(adjustmenu.items[adjustmenu.selected_index])*6+3,
                ITEM_HEIGHT);
    }
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,theta_central);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,phi_central);
}
void adjustpage_draw(lefl_page_t *page)
{
    sprintf(fezui_buffer,"%ld",theta_central);
    u8g2_DrawStr(&(fezui.u8g2), 64, ITEM_HEIGHT*1, fezui_buffer);

    sprintf(fezui_buffer,"%ld",phi_central);
    u8g2_DrawStr(&(fezui.u8g2), 64, ITEM_HEIGHT*2, fezui_buffer);

    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_tf);
    for(uint8_t i=0;i<adjustmenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ITEM_HEIGHT*(i+1),adjustmenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);
}
void adjustpage_load(lefl_page_t *page)
{
    key_back.key_cb =lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            number_editing = false;
        }
        else
        {
            fezui_save();
            lefl_link_frame_go_back(&mainframe);
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
            switch(adjustmenu.selected_index)
            {
                case 0:
                    theta_central -= 100;
                    break;
                case 1:
                    phi_central -= 100;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&adjustmenu, 1);
        }
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(adjustmenu.selected_index)
            {
                case 0:
                    theta_central += 100;
                    break;
                case 1:
                    phi_central += 100;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&adjustmenu, -1);
        }
    });
}

