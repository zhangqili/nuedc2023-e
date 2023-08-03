/*
 * fez_ui_controls.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"

void fezui_draw_cursor(fezui_t *fezui_ptr, lefl_cursor_t*c)
{
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 2);
    u8g2_DrawBox(&(fezui_ptr->u8g2), c->x, c->y, c->w, c->h);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), !fezui_ptr->invert);
}

void fezui_draw_flowingwater(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, lefl_bit_array_t *bits)
{
    for (uint8_t i=0;i<w;i++)
    {
        //if (lefl_bit_array_get(bits, i))
        //  u8g2_DrawVLine(&(fezui.u8g2),x+i,y,h);
        if (lefl_bit_array_get(bits, i))
          u8g2_DrawVLine(&(fezui_ptr->u8g2),x+i,y+1,h-2);
        if (lefl_bit_array_get(bits, i)^lefl_bit_array_get(bits, i+1))
          u8g2_DrawVLine(&(fezui_ptr->u8g2),x+i,y,h);
    }
}


void fezui_draw_wave(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, lefl_loop_array_t *arr, lefl_bit_array_t *l)
{
    uint8_t y1;
    uint8_t y2;
    for (int8_t i = 0; i < w - 1; i++)
    {
        y1=lefl_loop_array_get(arr, i)/128;
        y2=lefl_loop_array_get(arr, i+1)/128;
        if (y1 > h || y2 > h)
            if (!lefl_bit_array_get(l, i))
                u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + 1,x + w - i - 2,y+1);
            else
            {
                u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - i - 1, y + i, h - 1);
                u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + 1,x + w - i - 2,y+1);
            }
        else if (lefl_bit_array_get(l, i))
        {
            u8g2_DrawVLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1, y1);
            u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h - y1,x + w - i - 2,y+h-y2);
        }
        else
            u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i - 1, y + h-y1,x + w - i - 2,y+h-y2);
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), u8g2_font_micro_tr);
    u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, 17, 7);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), 2);
    sprintf(fezui_buffer, "%04d", lefl_loop_array_get(arr, 0));
    u8g2_DrawStr(&(fezui_ptr->u8g2), x + 1, y + 6, fezui_buffer);
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), !fezui_ptr->invert);
    //u8g2_DrawButtonUTF8(&(fezui.u8g2), x+1, y+6, U8G2_BTN_INV, 2, 1, 1, UI_TempString);
}

void fezui_draw_chart(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, lefl_loop_array_t *arr, uint8_t max)
{
    float y1;
    float y2;
    float ymax = (float)max;
    for (int8_t i = 0; i < w; i++)
    {
        y1=lefl_loop_array_get(arr, i);
        y2=lefl_loop_array_get(arr, i+1);

        u8g2_DrawLine(&(fezui_ptr->u8g2), x + w - i,
            h + y - (uint8_t) ((float) (h - 1) * (y1 / ymax)),
            x + w - i - 1,
            h + y - (uint8_t) ((float) (h - 1) * (y2 / ymax)));
    }
}

void fezui_veil(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, uint8_t level, uint8_t color)
{
	uint8_t previous_color;
	previous_color = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
    u8g2_SetDrawColor(&(fezui_ptr->u8g2), color);
    switch(level)
    {
        case 0:
            break;
        case 2:
            for (u8g2_uint_t i = y + 2; i < y+h; i+=4)
            {
                for (u8g2_uint_t j = x + 2; j < x+w; j+=4)
                {
                    u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
                }
            }
        case 1:
            for (u8g2_uint_t i = y; i < y+h; i+=4)
            {
                for (u8g2_uint_t j = x; j < x+w; j+=4)
                {
                    u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
                }
            }
            break;
        case 6:
            for (u8g2_uint_t i = y; i < y+h; i+=2)
            {
                for (u8g2_uint_t j = x+1; j < x+w; j+=2)
                {
                    u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
                }
            }
        case 5:
            for (u8g2_uint_t i = y+1; i < y+h; i+=2)
            {
                for (u8g2_uint_t j = x; j < x+w; j+=2)
                {
                    u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
                }
            }
        case 4:
            for (u8g2_uint_t i = y+1; i < y+h; i+=2)
            {
                for (u8g2_uint_t j = x+1; j < x+w; j+=2)
                {
                    u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
                }
            }
        case 3:
            for (u8g2_uint_t i = y; i < y+h; i+=2)
            {
                for (u8g2_uint_t j = x; j < x+w; j+=2)
                {
                    u8g2_DrawPixel(&(fezui_ptr->u8g2), j, i);
                }
            }
            break;
        case 7:
            u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, w, h);
            break;
        default:
            break;
    }
	u8g2_SetDrawColor(&(fezui_ptr->u8g2),previous_color);
}

void fezui_draw_scrollbar(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, float size, float value, fezui_orientation_t o)
{
	if(o == ORIENTATION_HORIZAIONTAL)
	{
		u8g2_DrawHLine(&(fezui_ptr->u8g2), x, y+2, w);
		u8g2_DrawBox(&(fezui_ptr->u8g2), x + (w-w*size)*value, y ,w*size, h);
	}
	else
	{
		u8g2_DrawVLine(&(fezui_ptr->u8g2), x+2, y, h);
		u8g2_DrawBox(&(fezui_ptr->u8g2), x , y  + (h-h*size)*value, w, h*size);
	}
}

void fezui_slider_increase(fezui_slider_t *slider, int8_t n)
{
    *(slider->f_ptr)+=slider->interval*n;
    if(*(slider->f_ptr)>slider->max)
        *(slider->f_ptr)=slider->max;
    if(*(slider->f_ptr)<slider->min)
        *(slider->f_ptr)=slider->min;
}

void fezui_draw_slider(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_slider_t *slider)
{
	if(slider->orientation == ORIENTATION_HORIZAIONTAL)
	{
		u8g2_DrawHLine(&(fezui_ptr->u8g2), x+1, y+h/2, w-2);
		u8g2_DrawBox(&(fezui_ptr->u8g2), x + ((*(slider->f_ptr)-slider->min)/slider->max-slider->min)*(w-2), y, 3, h);
	}
	else
	{
		u8g2_DrawVLine(&(fezui_ptr->u8g2), x+w/2, y+1, h-2);
		u8g2_DrawBox(&(fezui_ptr->u8g2), x , y + ((slider->max-*(slider->f_ptr))/slider->max-slider->min)*(h-2), w, 3);
	}
}

void fezui_draw_rolling_number(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, fezui_rolling_number_t* rolling_number)
{
    const uint8_t *temp_font=(fezui_ptr->u8g2).font;
    u8g2_SetFont(&(fezui_ptr->u8g2), rolling_number->font);
    uint8_t font_height = u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2));
    uint8_t font_width = u8g2_GetMaxCharWidth(&(fezui_ptr->u8g2));
    uint8_t actual_digit = 1;
    char str_buffer[2]={0};
    u8g2_long_t num=rolling_number->number;
    for (;num/=10;actual_digit++);
    num=rolling_number->number;
    u8g2_SetClipWindow(&(fezui_ptr->u8g2),x,y-font_height,x+rolling_number->digit*font_width,y+1);
    for (uint8_t i = 0; i < rolling_number->digit; i++)
    {
        for (uint8_t j = 0; j < 10; j++)
        {
            str_buffer[0] = j+48;
            u8g2_DrawStr(&(fezui_ptr->u8g2), x+(rolling_number->digit-i-1)*font_width, y+j*font_height-(u8g2_int_t)rolling_number->offsets[i], str_buffer);
        }
    }
    u8g2_SetMaxClipWindow(&(fezui_ptr->u8g2));
    u8g2_SetFont(&(fezui_ptr->u8g2), temp_font);
}

void fezui_rolling_number_update(fezui_t *fezui_ptr, fezui_rolling_number_t* rolling_number)
{
    const uint8_t *temp_font=(fezui_ptr->u8g2).font;
    u8g2_SetFont(&(fezui_ptr->u8g2), rolling_number->font);
    u8g2_long_t num=rolling_number->number;
    lefl_easing_pid(rolling_number->offsets, num%10*u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2)));
    for (uint8_t i = 1; i < rolling_number->digit; i++)
    {
        if(num/=10)
        {
            lefl_easing_pid(rolling_number->offsets+i, num%10*(u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2))));
        }
        else
        {
            lefl_easing_pid(rolling_number->offsets+i, -1-(u8g2_GetMaxCharHeight(&(fezui_ptr->u8g2))));
        }
    }
    u8g2_SetFont(&(fezui_ptr->u8g2), temp_font);
}
