/*
 * fezui_paperpage.c
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

lefl_page_t paperpage={paperpage_logic,paperpage_draw,paperpage_load};

extern bool number_editing;

#define ROW_HEIGHT 13

cartesian_coordinate_system_t a4_points[4]={{0,Y_CENTRAL,0},{0,Y_CENTRAL,0},{0,Y_CENTRAL,0},{0,Y_CENTRAL,0}};
cartesian_coordinate_system_t a4_actual_points[4]={{0,Y_CENTRAL,0},{0,Y_CENTRAL,0},{0,Y_CENTRAL,0},{0,Y_CENTRAL,0}};

static fezui_scrollview_t scrollview ={.content_height=12*9,.content_width=40};

lefl_menu_t papermenu = {
    .items = {"X1","Z1", "X2", "Z2", "X3", "Y3", "X4", "Y4", "Go"},
    .selected_index = 0,
    .len = 9
};

static float target_ordinate=0;

void paperpage_logic(lefl_page_t *page)
{
    lefl_easing_pid(&(scrollview.ordinate), target_ordinate);

    if(number_editing)
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                papermenu.selected_index*ROW_HEIGHT+3 - (u8g2_int_t)scrollview.ordinate,
                64-10,
                ROW_HEIGHT);
    }
    else
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                papermenu.selected_index*ROW_HEIGHT+3  - (u8g2_int_t)scrollview.ordinate,
                strlen(papermenu.items[papermenu.selected_index])*6+3,
                ROW_HEIGHT);
    }
    if((papermenu.selected_index+1)*ROW_HEIGHT-target_ordinate>64)
    {
        target_ordinate = (papermenu.selected_index+1)*ROW_HEIGHT-64;
    }
    if((papermenu.selected_index)*ROW_HEIGHT<target_ordinate)
    {
        target_ordinate = (papermenu.selected_index)*ROW_HEIGHT;
    }
    for (uint8_t i = 0; i < 4; i++)
    {
        pixel_to_reality(a4_actual_points+i, a4_points+i);
    }
}
void paperpage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);

    sprintf(fezui_buffer,"%.0lf",a4_points[0].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*1 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[0].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[1].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*3 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[1].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*4 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[2].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*5 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[2].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*6 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[3].x);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*7 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_points[3].z);
    u8g2_DrawStr(&(fezui.u8g2), 18, ROW_HEIGHT*8 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[0].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*1 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[0].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[1].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*3 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[1].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*4 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[2].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*5 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[2].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*6 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[3].x);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*7 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",a4_actual_points[3].z);
    u8g2_DrawStr(&(fezui.u8g2), 38, ROW_HEIGHT*8 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    u8g2_DrawFrame(&(fezui.u8g2), 64, 0, 64, 64);

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(a4_actual_points[0].x/10), 56-(int16_t)(a4_actual_points[0].z/10), 96+(int16_t)(a4_actual_points[1].x/10), 56-(int16_t)(a4_actual_points[1].z/10));

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(a4_actual_points[1].x/10), 56-(int16_t)(a4_actual_points[1].z/10), 96+(int16_t)(a4_actual_points[2].x/10), 56-(int16_t)(a4_actual_points[2].z/10));

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(a4_actual_points[2].x/10), 56-(int16_t)(a4_actual_points[2].z/10), 96+(int16_t)(a4_actual_points[3].x/10), 56-(int16_t)(a4_actual_points[3].z/10));

    u8g2_DrawLine(&(fezui.u8g2), 96+(int16_t)(a4_actual_points[3].x/10), 56-(int16_t)(a4_actual_points[3].z/10), 96+(int16_t)(a4_actual_points[0].x/10), 56-(int16_t)(a4_actual_points[0].z/10));


    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(a4_actual_points[0].x/10), 56-(int16_t)(a4_actual_points[0].z/10), "1");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(a4_actual_points[1].x/10), 56-(int16_t)(a4_actual_points[1].z/10), "2");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(a4_actual_points[2].x/10), 56-(int16_t)(a4_actual_points[2].z/10), "3");

    u8g2_DrawStr(&(fezui.u8g2),96+(int16_t)(a4_actual_points[3].x/10), 56-(int16_t)(a4_actual_points[3].z/10), "4");

    fezui_draw_scrollview(&fezui, 0, 0, 63, 64, &scrollview);

    for(uint8_t i=0;i<papermenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ROW_HEIGHT*(i+1) - (u8g2_int_t)scrollview.ordinate,papermenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);
}
void paperpage_load(lefl_page_t *page)
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
        if(papermenu.selected_index == 8)
        {
#if DISCRETE_CONTROL == 1
            fezui_waiting();
            move_count = 1000;
            moving = true;
            from_actual_point = a4_actual_points+0;
            from_actual_point = a4_actual_points+0;
            STEER_TIMER_START();
            while(moving);
            move_count = cartesian_length(a4_actual_points+0, a4_actual_points+1)/10;
            moving = true;
            from_actual_point = a4_actual_points+0;
            from_actual_point = a4_actual_points+1;
            STEER_TIMER_START();
            while(moving);
            move_count = cartesian_length(a4_actual_points+1, a4_actual_points+2)/10;
            moving = true;
            from_actual_point = a4_actual_points+1;
            from_actual_point = a4_actual_points+2;
            STEER_TIMER_START();
            while(moving);
            move_count = cartesian_length(a4_actual_points+2, a4_actual_points+3)/10;
            moving = true;
            from_actual_point = a4_actual_points+2;
            from_actual_point = a4_actual_points+3;
            STEER_TIMER_START();
            while(moving);
            move_count = cartesian_length(a4_actual_points+3, a4_actual_points+0)/10;
            moving = true;
            from_actual_point = a4_actual_points+3;
            from_actual_point = a4_actual_points+0;
            STEER_TIMER_START();
            while(moving);
#else
            fezui_waiting();
            steer_set_cartesian(a4_points);
            HAL_Delay(1000);
            steer_linear_follow_loop_once(a4_actual_points
                    ,4,5000,100);
            return;
#endif
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
            switch(papermenu.selected_index)
            {
                case 0:
                    a4_points[0].x -= 1;
                    break;
                case 1:
                    a4_points[0].z -= 1;
                    break;
                case 2:
                    a4_points[1].x -= 1;
                    break;
                case 3:
                    a4_points[1].z -= 1;
                    break;
                case 4:
                    a4_points[2].x -= 1;
                    break;
                case 5:
                    a4_points[2].z -= 1;
                    break;
                case 6:
                    a4_points[3].x -= 1;
                    break;
                case 7:
                    a4_points[3].z -= 1;
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&papermenu, -1);
        }
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(papermenu.selected_index)
            {
                case 0:
                    a4_points[0].x += 1;
                    break;
                case 1:
                    a4_points[0].z += 1;
                    break;
                case 2:
                    a4_points[1].x += 1;
                    break;
                case 3:
                    a4_points[1].z += 1;
                    break;
                case 4:
                    a4_points[2].x += 1;
                    break;
                case 5:
                    a4_points[2].z += 1;
                    break;
                case 6:
                    a4_points[3].x += 1;
                    break;
                case 7:
                    a4_points[3].z += 1;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&papermenu, 1);
        }
    });
}



