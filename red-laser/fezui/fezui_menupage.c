/*
 * fezui_menupage.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"
#include "fezui_var.h"
#include "steer.h"

lefl_page_t menupage={menupage_logic,menupage_draw,menupage_load};

lefl_menu_t mainmenu = {
    .items = {"Adjust","Navigate","Frame","A4 Frame","PID","Debug"},
    .selected_index = 0,
    .len = 6,
    .menu_cb = main_menu_cb
};

#define RECT_HALF_LEHGTH 260
extern cartesian_coordinate_system_t central_point;
static cartesian_coordinate_system_t frame_point1 = {X_CENTRAL-270,Y_CENTRAL,Z_CENTRAL+270};
static cartesian_coordinate_system_t frame_point2 = {X_CENTRAL+260,Y_CENTRAL,Z_CENTRAL+270};
static cartesian_coordinate_system_t frame_point3 = {X_CENTRAL+260,Y_CENTRAL,Z_CENTRAL-260};
static cartesian_coordinate_system_t frame_point4 = {X_CENTRAL-270,Y_CENTRAL,Z_CENTRAL-260};

static fezui_scrollview_t scrollview ={.content_height=ITEM_HEIGHT*6,.content_width=40};
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
}

void main_menu_cb(lefl_menu_t *menu)
{

    switch (menu->selected_index)
    {
    case 0:
        lefl_link_frame_navigate(&mainframe, &adjustpage);
        break;
    case 1:
        lefl_link_frame_navigate(&mainframe, &navigatepage);
        break;
    case 2:
#if DISCRETE_CONTROL == 1

#else
        fezui_waiting();
        steer_set_cartesian(&central_point);
        HAL_Delay(50);
        steer_linear_follow(&central_point, &frame_point1, 7500);
        HAL_Delay(50);
        steer_linear_follow(&frame_point1, &frame_point2, 7500);
        HAL_Delay(50);
        steer_linear_follow(&frame_point2, &frame_point3, 7500);
        HAL_Delay(50);
        steer_linear_follow(&frame_point3, &frame_point4, 7500);
        HAL_Delay(50);
        steer_linear_follow(&frame_point4, &frame_point1, 7500);
#endif
        break;
    case 3:
        lefl_link_frame_navigate(&mainframe, &paperpage);
        break;
    case 4:
        lefl_link_frame_navigate(&mainframe, &pidpage);
        break;
    case 5:
        fezui_waiting();
        steer_set_cartesian(&central_point);
        HAL_Delay(1000);
        steer_linear_follow(&central_point, &frame_point1, 10000);
        HAL_Delay(100);
        steer_set_cartesian(&central_point);
        HAL_Delay(1000);
        steer_linear_follow(&central_point, &frame_point2, 10000);
        HAL_Delay(100);
        steer_set_cartesian(&central_point);
        HAL_Delay(1000);
        steer_linear_follow(&central_point, &frame_point3, 10000);
        HAL_Delay(100);
        steer_set_cartesian(&central_point);
        HAL_Delay(1000);
        steer_linear_follow(&central_point, &frame_point4, 10000);
        HAL_Delay(100);
        steer_set_cartesian(&central_point);
        HAL_Delay(1000);
        steer_linear_follow(&central_point, &frame_point1, 10000);
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

