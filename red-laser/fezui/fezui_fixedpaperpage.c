/*
 * fezui_fixedpaperpage.c
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

lefl_page_t fixedpaperpage={fixedpaperpage_logic,fixedpaperpage_draw,fixedpaperpage_load};

extern bool number_editing;

#define ROW_HEIGHT 13

cartesian_coordinate_system_t fixed_a4_points[4]={{30,Y_CENTRAL,29},{137,Y_CENTRAL,29},{137,Y_CENTRAL,110},{30,Y_CENTRAL,110}};
cartesian_coordinate_system_t fixed_a4_actual_points[4]={{0,Y_CENTRAL,0},{0,Y_CENTRAL,0},{0,Y_CENTRAL,0},{0,Y_CENTRAL,0}};

static fezui_scrollview_t scrollview ={.content_height=12*9,.content_width=40};

lefl_menu_t fixedpapermenu = {
    .items = {"X1","Z1", "X2", "Z2", "X3", "Y3", "X4", "Y4", "Go"},
    .selected_index = 0,
    .len = 9
};

static float target_ordinate=0;

void fixedpaperpage_logic(lefl_page_t *page)
{
    lefl_easing_pid(&(scrollview.ordinate), target_ordinate);

    if(number_editing)
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                fixedpapermenu.selected_index*ROW_HEIGHT+3 - (u8g2_int_t)scrollview.ordinate,
                64-10,
                ROW_HEIGHT);
    }
    else
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                fixedpapermenu.selected_index*ROW_HEIGHT+3  - (u8g2_int_t)scrollview.ordinate,
                strlen(fixedpapermenu.items[fixedpapermenu.selected_index])*6+3,
                ROW_HEIGHT);
    }
    if((fixedpapermenu.selected_index+1)*ROW_HEIGHT-target_ordinate>64)
    {
        target_ordinate = (fixedpapermenu.selected_index+1)*ROW_HEIGHT-64;
    }
    if((fixedpapermenu.selected_index)*ROW_HEIGHT<target_ordinate)
    {
        target_ordinate = (fixedpapermenu.selected_index)*ROW_HEIGHT;
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        pixel_to_reality(fixed_a4_actual_points+i, fixed_a4_points+i);
    }
}
void fixedpaperpage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[0].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*1 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[0].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[1].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*3 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[1].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*4 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[2].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*5 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[2].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*6 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[3].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*7 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_points[3].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*8 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[0].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*1 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[0].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[1].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*3 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[1].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*4 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[2].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*5 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[2].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*6 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[3].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*7 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",fixed_a4_actual_points[3].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*8 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    u8g2_DrawFrame(&(fezui.u8g2), 64, 0, 64, 64);

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(fixed_a4_actual_points[0].x/10), 56-(int16_t)(fixed_a4_actual_points[0].z/10), 96+(int16_t)(fixed_a4_actual_points[1].x/10), 56-(int16_t)(fixed_a4_actual_points[1].z/10));

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(fixed_a4_actual_points[1].x/10), 56-(int16_t)(fixed_a4_actual_points[1].z/10), 96+(int16_t)(fixed_a4_actual_points[2].x/10), 56-(int16_t)(fixed_a4_actual_points[2].z/10));

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(fixed_a4_actual_points[2].x/10), 56-(int16_t)(fixed_a4_actual_points[2].z/10), 96+(int16_t)(fixed_a4_actual_points[3].x/10), 56-(int16_t)(fixed_a4_actual_points[3].z/10));

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(fixed_a4_actual_points[3].x/10), 56-(int16_t)(fixed_a4_actual_points[3].z/10), 96+(int16_t)(fixed_a4_actual_points[0].x/10), 56-(int16_t)(fixed_a4_actual_points[0].z/10));


    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(fixed_a4_actual_points[0].x/10), 56-(int16_t)(fixed_a4_actual_points[0].z/10), "1");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(fixed_a4_actual_points[1].x/10), 56-(int16_t)(fixed_a4_actual_points[1].z/10), "2");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(fixed_a4_actual_points[2].x/10), 56-(int16_t)(fixed_a4_actual_points[2].z/10), "3");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(fixed_a4_actual_points[3].x/10), 56-(int16_t)(fixed_a4_actual_points[3].z/10), "4");

    fezui_draw_scrollview(&fezui, 0, 0, 63, 64, &scrollview);

    for(uint8_t i=0;i<fixedpapermenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ROW_HEIGHT*(i+1) - (u8g2_int_t)scrollview.ordinate,fixedpapermenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);
}
void fixedpaperpage_load(lefl_page_t *page)
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
        if(fixedpapermenu.selected_index == 8)
        {
            fezui_waiting();
            steer_set_cartesian(fixed_a4_actual_points);
            HAL_Delay(1000);
            steer_linear_follow_loop_once(fixed_a4_actual_points
                    ,4,5000,100);
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
            switch(fixedpapermenu.selected_index)
            {
                case 0:
                    fixed_a4_points[0].x -= 1;
                    break;
                case 1:
                    fixed_a4_points[0].z -= 1;
                    break;
                case 2:
                    fixed_a4_points[1].x -= 1;
                    break;
                case 3:
                    fixed_a4_points[1].z -= 1;
                    break;
                case 4:
                    fixed_a4_points[2].x -= 1;
                    break;
                case 5:
                    fixed_a4_points[2].z -= 1;
                    break;
                case 6:
                    fixed_a4_points[3].x -= 1;
                    break;
                case 7:
                    fixed_a4_points[3].z -= 1;
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&fixedpapermenu, -1);
        }
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(fixedpapermenu.selected_index)
            {
                case 0:
                    fixed_a4_points[0].x += 1;
                    break;
                case 1:
                    fixed_a4_points[0].z += 1;
                    break;
                case 2:
                    fixed_a4_points[1].x += 1;
                    break;
                case 3:
                    fixed_a4_points[1].z += 1;
                    break;
                case 4:
                    fixed_a4_points[2].x += 1;
                    break;
                case 5:
                    fixed_a4_points[2].z += 1;
                    break;
                case 6:
                    fixed_a4_points[3].x += 1;
                    break;
                case 7:
                    fixed_a4_points[3].z += 1;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&fixedpapermenu, 1);
        }
    });
}



