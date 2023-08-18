/*
 * fezui_debugpage.c
 *
 *  Created on: 2023年6月24日
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
#include "steer.h"

lefl_page_t debugpage={debugpage_logic,debugpage_draw,debugpage_load};

#define INTERVAL 1

extern bool number_editing;
extern double frame_offsets[4];

#define ROW_HEIGHT 13

static fezui_scrollview_t scrollview ={.content_height=ROW_HEIGHT*6,.content_width=40};

lefl_menu_t debugmenu = {
    .items = {"Distance", "FrameOffset0", "FrameOffset1", "FrameOffset2", "FrameOffset3", "Z_CENTRAL"},
    .selected_index = 0,
    .len = 6
};

extern cartesian_coordinate_system_t central_point;
static float target_ordinate=0;

void debugpage_logic(lefl_page_t *page)
{
    lefl_easing_pid(&(scrollview.ordinate), target_ordinate);

    if(number_editing)
    {
        lefl_cursor_set(
                &target_cursor ,
                0,
                debugmenu.selected_index*ROW_HEIGHT+3 - (u8g2_int_t)scrollview.ordinate,
                128-6,
                ROW_HEIGHT);
    }
    else
    {
        lefl_cursor_set(
                &target_cursor ,
                3,
                debugmenu.selected_index*ROW_HEIGHT+3  - (u8g2_int_t)scrollview.ordinate,
                strlen(debugmenu.items[debugmenu.selected_index])*6+3,
                ROW_HEIGHT);
    }
    if((debugmenu.selected_index+1)*ROW_HEIGHT-target_ordinate>64)
    {
        target_ordinate = (debugmenu.selected_index+1)*ROW_HEIGHT-64;
    }
    if((debugmenu.selected_index)*ROW_HEIGHT<target_ordinate)
    {
        target_ordinate = (debugmenu.selected_index)*ROW_HEIGHT;
    }
    steer_set_cartesian(&central_point);

}
void debugpage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);

    sprintf(fezui_buffer,"%.0lf",distance);
    u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*1 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",frame_offsets[0]);
    u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*2 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);
    sprintf(fezui_buffer,"%.0lf",frame_offsets[1]);
    u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*3 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);
    sprintf(fezui_buffer,"%.0lf",frame_offsets[2]);
    u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*4 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);
    sprintf(fezui_buffer,"%.0lf",frame_offsets[3]);
    u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*5 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    sprintf(fezui_buffer,"%.0lf",central_point.z);
    u8g2_DrawStr(&(fezui.u8g2), 80, ROW_HEIGHT*6 - (u8g2_int_t)scrollview.ordinate, fezui_buffer);

    fezui_draw_scrollview(&fezui, 0, 0, 128, 64, &scrollview);

    for(uint8_t i=0;i<debugmenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ROW_HEIGHT*(i+1) - (u8g2_int_t)scrollview.ordinate,debugmenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);

}

void debugpage_load(lefl_page_t *page)
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
            number_editing = false;
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
            switch(debugmenu.selected_index)
            {
                case 0:
                    distance -= INTERVAL;
                    break;
                case 1:
                    frame_offsets[0] -= 5;
                    break;
                case 2:
                    frame_offsets[1] -= 5;
                    break;
                case 3:
                    frame_offsets[2] -= 5;
                    break;
                case 4:
                    frame_offsets[3] -= 5;
                    break;
                case 5:
                    central_point.z -= 2;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&debugmenu, 1);
        }
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        if(number_editing)
        {
            switch(debugmenu.selected_index)
            {
                case 0:
                    distance += INTERVAL;
                    break;
                case 1:
                    frame_offsets[0] += 5;
                    break;
                case 2:
                    frame_offsets[1] += 5;
                    break;
                case 3:
                    frame_offsets[2] += 5;
                    break;
                case 4:
                    frame_offsets[3] += 5;
                    break;
                case 5:
                    central_point.z += 2;
                    break;
                default:
                    break;
            }
        }
        else
        {
            lefl_menu_index_increase(&debugmenu, -1);
        }
    });

}

