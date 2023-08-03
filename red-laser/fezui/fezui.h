/*
 * fez_ui.h
 *
 *  Created on: 2023年6月18日
 *      Author: xq123
 */

#ifndef FEZUI_H_
#define FEZUI_H_

#include "u8g2.h"
#include "lefl.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "fezui_config.h"
#include "fezui_interface.h"

typedef u8g2_uint_t fezui_uint_t;
typedef u8g2_int_t fezui_int_t;

extern char fezui_buffer[BUFFER_LENGTH];

typedef struct __fezui_t
{
	u8g2_t u8g2;
    uint16_t width;
    uint16_t height;
    bool invert;
} fezui_t;

//#define FEZUI_CREATE(name) fezui_t name;
//#define FEZUI_DECLARE(name) fezui_t name;

void fezui_timer_handler();
void fezui_init();


typedef struct __fezui_point_t
{
    fezui_uint_t x;
    fezui_uint_t y;
} fezui_point_t;

typedef struct __fezui_size_t
{
    fezui_uint_t w;
    fezui_uint_t h;
} fezui_size_t;


typedef struct __fezui_base_t
{
    fezui_point_t location;
    fezui_size_t size;
} fezui_base_t;


typedef enum
{
    FEZUI_ALIGN_LEFT,
    FEZUI_ALIGN_CENTER,
    FEZUI_ALIGN_RIGHT
} fezui_align_t;
/*
 * fezui_interface.c
 */
/*
 uint8_t ui_init();
 void fezui_clear_buf();
 void fezui_send_buf();
 void fezui_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,bool fill);
 void fezui_draw_pixel(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
 void fezui_draw_line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
 */

/*
 * fezui_controls.c
 */

typedef enum
{
	ORIENTATION_HORIZAIONTAL,
	ORIENTATION_VERTICAL
} fezui_orientation_t;

void fezui_draw_cursor(fezui_t *fezui_ptr, lefl_cursor_t*c);
void fezui_draw_flowingwater(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y,
        u8g2_uint_t w, u8g2_uint_t h, lefl_bit_array_t *bits);
void fezui_draw_wave(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y,
        u8g2_uint_t w, u8g2_uint_t h, lefl_loop_array_t *arr,
        lefl_bit_array_t *l);
void fezui_draw_chart(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y,
        u8g2_uint_t w, u8g2_uint_t h, lefl_loop_array_t *arr, uint8_t max);
void fezui_veil(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, uint8_t level, uint8_t color);
void fezui_draw_scrollbar(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, float size, float value, fezui_orientation_t o);

typedef struct __fezui_slider_t
{
    //fezui_base_t base;
    //fezui_base_t base;
    float max;
    float min;
    float interval;
    float *f_ptr;
    fezui_orientation_t orientation;
} fezui_slider_t;

void fezui_slider_increase(fezui_slider_t *slider, int8_t n);
void fezui_draw_slider(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,u8g2_uint_t h, fezui_slider_t *slider);

typedef struct __fezui_tile_t
{
    char *header;
    char *icon;
    void (*tile_cb)(struct __fezui_tile_t *tile);
} fezui_tile_t;

#define ROLLING_NUMBER_OFFSET_LENGTH 8

typedef struct __fezui_rolling_number_t
{
    u8g2_long_t number;
    uint8_t digit;
    float offsets[8];
    const uint8_t *font;
} fezui_rolling_number_t;

void fezui_draw_rolling_number(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, fezui_rolling_number_t* rolling_number);
//void fezui_rolling_number_set(fezui_rolling_number_t* rolling_number, u8g2_long_t num);
#define fezui_rolling_number_set(rolling_number, num) (rolling_number)->number=(num);
void fezui_rolling_number_update(fezui_t *fezui_ptr, fezui_rolling_number_t* rolling_number);
/*
 * fezui_dialog.c
 */

typedef enum
{
	DIALOG_NORMAL,
	DIALOG_WARNING,
	DIALOG_ERROR,
} fezui_dialog_type_t;

typedef struct __fezui_dialog_t
{
    char *title;
    float *text;
    fezui_dialog_type_t type;
} fezui_dialog_t;

void fezui_draw_dialog(fezui_t *fezui_ptr, uint8_t x, uint8_t y, uint8_t w, uint8_t h, fezui_dialog_t *dialog);

/*
 * fezui_scrollviewer.c
 */

typedef struct __fezui_scrollview_t
{
    u8g2_int_t content_height;
    u8g2_int_t content_width;
    float abscissa;
    float ordinate;
} fezui_scrollview_t;

void fezui_draw_scrollview(fezui_t *fezui_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, fezui_scrollview_t* scrollview);

#endif /* FEZUI_H_ */
