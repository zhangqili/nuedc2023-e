/*
 * fezui_menupage.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "steer.h"
#include "tim.h"
#include "pid-control.h"

lefl_page_t menupage={menupage_logic,menupage_draw,menupage_load};

lefl_menu_t mainmenu = {
    .items = {"PID", "Start", "Stop"},
    .selected_index = 0,
    .len = 3,
    .menu_cb = main_menu_cb
};

#define RECT_HALF_LEHGTH 260
#define ROW_HEIGHT 8
extern cartesian_coordinate_system_t central_point;

static fezui_scrollview_t scrollview ={.content_height=ITEM_HEIGHT*3,.content_width=40};
static float target_ordinate=0;

void menupage_logic(lefl_page_t *page)
{
    lefl_cursor_set(
            &target_cursor ,
            3,
            mainmenu.selected_index*ITEM_HEIGHT+3  - (u8g2_int_t)scrollview.ordinate,
            strlen(mainmenu.items[mainmenu.selected_index])*6+6,
            ITEM_HEIGHT);
    lefl_easing_pid(&(scrollview.ordinate), target_ordinate);

    if((mainmenu.selected_index+1)*ITEM_HEIGHT-target_ordinate>64)
    {
        target_ordinate = (mainmenu.selected_index+1)*ITEM_HEIGHT-64;
    }
    if((mainmenu.selected_index)*ITEM_HEIGHT<target_ordinate)
    {
        target_ordinate = (mainmenu.selected_index)*ITEM_HEIGHT;
    }

}
void menupage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_tf);
    for(uint8_t i=0;i<mainmenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ITEM_HEIGHT*(i+1) - (u8g2_int_t)scrollview.ordinate,mainmenu.items[i]);
    }
    fezui_draw_scrollview(&fezui, 0, 0, 128, 64, &scrollview);
    fezui_draw_cursor(&fezui, &cursor);


    if(1)
    {
        u8g2_SetFont(&(fezui.u8g2), u8g2_font_5x7_mf);
        //u8g2_ClearBuffer(&(fezui.u8g2));

        sprintf(fezui_buffer,"theta:%lf",theta_pid.errdat);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*1, fezui_buffer);

        sprintf(fezui_buffer,"phi:%lf",phi_pid.errdat);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*2, fezui_buffer);

        sprintf(fezui_buffer,"outz:%ld",theta_pid.pidout);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*3, fezui_buffer);

        sprintf(fezui_buffer,"putx:%ld",phi_pid.pidout);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*4, fezui_buffer);

        sprintf(fezui_buffer,"ms:%ld",move_step);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*5, fezui_buffer);

        sprintf(fezui_buffer,"qc:%d",Queue_Count(&point_queue));
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*6, fezui_buffer);

        sprintf(fezui_buffer,"%.0lf",target_actual_point.x);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*7, fezui_buffer);

        sprintf(fezui_buffer,"%.0lf",target_actual_point.z);
        u8g2_DrawStr(&(fezui.u8g2), 64, ROW_HEIGHT*8, fezui_buffer);
        //u8g2_SendBuffer(&(fezui.u8g2));
    }
}

void main_menu_cb(lefl_menu_t *menu)
{

    switch (menu->selected_index)
    {
    case 0:
        lefl_link_frame_navigate(&mainframe, &pidpage);
        break;
    case 1:
        STEER_TIMER_START();
        moving=true;
        break;
    case 2:
        STEER_TIMER_STOP();
        moving=false;
        break;
    default:
        break;
    }
}

void menupage_load(lefl_page_t *page)
{
    key_back.key_cb =lambda(void, (lefl_key_t*k)
    {
        ;
    });
    key_go.key_cb = lambda(void, (lefl_key_t*k)
    {
        lefl_menu_click(&mainmenu);
    });
    key_down.key_cb = lambda(void, (lefl_key_t*k)
    {
        lefl_menu_index_increase(&mainmenu, 1);
    });
    key_up.key_cb = lambda(void, (lefl_key_t*k)
    {
        lefl_menu_index_increase(&mainmenu, -1);
    });

}

