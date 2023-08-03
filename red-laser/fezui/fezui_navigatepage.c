/*
 * fezui_navigatepage.c
 *
 *  Created on: Aug 2, 2023
 *      Author: xq123
 */
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
#include "stdlib.h"
#include "rng.h"

lefl_page_t navigatepage={navigatepage_logic,navigatepage_draw,navigatepage_load};

extern bool number_editing;

#define ROW_HEIGHT 13

static cartesian_coordinate_system_t point1={0,Y_CENTRAL,200};
static cartesian_coordinate_system_t point2={0,Y_CENTRAL,400};

lefl_menu_t navigatemenu = {
    .items = {"X1","Z1", "X2", "Z2", "Go"},
    .selected_index = 0,
    .len = 5
};


void navigatepage_logic(lefl_page_t *page)
{
    if(number_editing)
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                navigatemenu.selected_index*ROW_HEIGHT+3 ,
                64-3,
                ROW_HEIGHT);
    }
    else
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                navigatemenu.selected_index*ROW_HEIGHT+3 ,
                strlen(navigatemenu.items[navigatemenu.selected_index])*6+3,
                ROW_HEIGHT);
    }
}
void navigatepage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x12_tf);

    sprintf(fezui_buffer,"%.0lf",point1.x);
    u8g2_DrawStr(&(fezui.u8g2), 20, ROW_HEIGHT*1, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",point1.z);
    u8g2_DrawStr(&(fezui.u8g2), 20, ROW_HEIGHT*2, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",point2.x);
    u8g2_DrawStr(&(fezui.u8g2), 20, ROW_HEIGHT*3, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",point2.z);
    u8g2_DrawStr(&(fezui.u8g2), 20, ROW_HEIGHT*4, fezui_buffer);

    u8g2_DrawFrame(&(fezui.u8g2), 64, 0, 64, 64);

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(point1.x/30), 60-(int16_t)(point1.z/30), 96+(int16_t)(point2.x/30), 60-(int16_t)(point2.z/30));

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(point1.x/30), 60-(int16_t)(point1.z/30), "1");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(point2.x/30), 60-(int16_t)(point2.z/30), "2");
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_tf);
    for(uint8_t i=0;i<navigatemenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ROW_HEIGHT*(i+1),navigatemenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);
}
void navigatepage_load(lefl_page_t *page)
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
        }
    });
    key_go.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(navigatemenu.selected_index == 4)
        {

#if DISCRETE_CONTROL == 1
            move_count = cartesian_length(&point1, &point2)/10;
            moving = true;
            from_actual_point = &point1;
            from_actual_point = &point2;
            STEER_TIMER_START();
            while(moving);
#else
            fezui_waiting();
            steer_set_cartesian(&point1);
            HAL_Delay(1000);
            steer_linear_follow(&point1, &point2, 5000);
#endif
            return;
        }
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
            switch(navigatemenu.selected_index)
            {
                case 0:
                    point1.x -= 5;
                    break;
                case 1:
                    point1.z -= 5;
                    break;
                case 2:
                    point2.x -= 5;
                    break;
                case 3:
                    point2.z -= 5;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&navigatemenu, -1);
        }
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(navigatemenu.selected_index)
            {
                case 0:
                    point1.x += 5;
                    break;
                case 1:
                    point1.z += 5;
                    break;
                case 2:
                    point2.x += 5;
                    break;
                case 3:
                    point2.z += 5;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&navigatemenu, 1);
        }
    });
}



