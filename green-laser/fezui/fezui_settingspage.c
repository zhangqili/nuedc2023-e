/*
 * fezui_settingspage.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */
#include "fezui.h"
#include "fezui_var.h"
lefl_menu_t settingsmenu = {
    .items = {"Keys Configuration","Calibration","Debug","About"},
    .selected_index = 0,
    .len = 4,
    .menu_cb = settings_menu_cb
};
lefl_page_t settingspage={settingspage_logic,settingspage_draw,settingspage_load};
void settingspage_logic(lefl_page_t *page)
{

    //fezui_menu_update_selection(&settingsmenu);
    /*
    if(lefl_key_is_triggered(keys+2))
    {
        lefl_link_frame_go_back(&mainframe);
    }
    if(lefl_key_is_triggered(keys+3))
    {
        lefl_menu_click(&settingsmenu);
    }
    */
    lefl_cursor_set(&target_cursor, 3,
            settingsmenu.selected_index * ITEM_HEIGHT + 3,
            strlen(settingsmenu.items[settingsmenu.selected_index]) * 6 + 6,
            ITEM_HEIGHT);
}
void settingspage_draw(lefl_page_t *page)
{
    u8g2_SetFont(&(fezui.u8g2), u8g2_font_6x13_tf);
    for(uint8_t i=0;i<settingsmenu.len;i++)
    {
        u8g2_DrawStr(&(fezui.u8g2),5,ITEM_HEIGHT*(i+1),settingsmenu.items[i]);
    }
    fezui_draw_cursor(&fezui, &cursor);
}
void settings_menu_cb(lefl_menu_t *menu)
{

    switch (menu->selected_index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        break;
    }
}
void settingspage_load(lefl_page_t *page)
{
}
