/*
 * fezui_scrollviewer.c
 *
 *  Created on: Jul 21, 2023
 *      Author: xq123
 */
#include "fezui.h"

void fezui_draw_scrollview(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_scrollview_t* scrollview)
{
    if(scrollview->content_width>w&&scrollview->content_height>h)
    {
        fezui_draw_scrollbar(fezui_ptr, x, y + h - 5, w - 5, 5, (float)w/(float)scrollview->content_width, (float)scrollview->abscissa/(float)(scrollview->content_width-w), ORIENTATION_HORIZAIONTAL);
        fezui_draw_scrollbar(fezui_ptr, x + w - 5, y, 5, h-5, (float)h/(float)scrollview->content_height, (float)scrollview->ordinate/(float)(scrollview->content_height-h), ORIENTATION_VERTICAL);
    }
    else if(scrollview->content_width>w)
    {
        fezui_draw_scrollbar(fezui_ptr, x, y + h - 5, w, 5, (float)w/(float)scrollview->content_width, (float)scrollview->abscissa/(float)(scrollview->content_width-w), ORIENTATION_HORIZAIONTAL);
    }
    else if(scrollview->content_height>h)
    {
        fezui_draw_scrollbar(fezui_ptr, x + w - 5, y, 5, h, (float)h/(float)scrollview->content_height, (float)scrollview->ordinate/(float)(scrollview->content_height-h), ORIENTATION_VERTICAL);
    }
}
