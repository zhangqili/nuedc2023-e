/*
 * fezui_var.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */
#include "fezui_var.h"

char fpsstr[8] = {0};
char comstr[8] = {0};

fezui_t fezui =
{
    .invert=0
};

lefl_link_frame_t mainframe = {
        .current_page=&menupage
    };

uint8_t fezui_rest_countdown = SCREEN_REST_TIME;
uint8_t fezui_debug;
uint32_t fezui_fps = 0;
bool number_editing;

/*
  Fontname: fez_font_6x10
  Copyright: Created with Fony 1.4.6
  Glyphs: 26/26
  BBX Build Mode: 0x
*/
const uint8_t fez_font_6x10_m[204] U8G2_FONT_SECTION("fez_font_6x10_m") =
  "\32\0\3\3\3\4\4\5\4\6\12\0\0\11\0\12\12\0\0\0\0\0\257 \5\0\324\16!\5\0\324"
  "\16\42\5\0\324\16#\5\0\324\16$\5\0\324\16%\5\0\324\16&\5\0\324\16'\5\0\324\16("
  "\5\0\324\16)\5\0\324\16*\5\0\324\16+\5\0\324\16,\5\0\324\16-\6\16\204\16\3.\5"
  "\0\324\16/\5\0\324\16\60\10M\204\16\233\276\31\61\7I\206\216\203\0\62\12M\204\216b\342\61\261"
  "\0\63\12M\204\216b\242\61\321\0\64\11M\204\216\230\66c\16\65\12M\204\16cb\61\321\0\66\12"
  "M\204\16c\242M\315\0\67\10M\204\216b\376\0\70\12M\204\16\233\332M\315\0\71\12M\204\16\233"
  "\232\61\321\0\0\0\0\4\377\377\0";


lefl_key_t key_up;
lefl_key_t key_down;
lefl_key_t key_go;
lefl_key_t key_back;

lefl_cursor_t cursor={0,0,128,64};
lefl_cursor_t target_cursor={0,0,0,0};






