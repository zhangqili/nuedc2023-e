/*
 * fezui_var.h
 *
 *  Created on: Jul 21, 2023
 *      Author: xq123
 */

#ifndef FEZUI_VAR_H_
#define FEZUI_VAR_H_
#include "fezui.h"
extern char fpsstr[8];
extern char comstr[8];
extern fezui_t fezui;
extern uint32_t fezui_fps;
extern uint8_t fezui_rest_countdown;
extern const uint8_t fez_font_6x10_m[204] U8G2_FONT_SECTION("fez_font_6x10_m");

extern lefl_key_t key_up;
extern lefl_key_t key_down;
extern lefl_key_t key_go;
extern lefl_key_t key_back;

extern bool number_editing;

#endif /* FEZUI_VAR_H_ */
