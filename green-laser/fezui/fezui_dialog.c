/*
 * fezui_dialog.c
 *
 *  Created on: Jul 20, 2023
 *      Author: xq123
 */
#include "fezui.h"

void fezui_draw_dialog(fezui_t *fezui_ptr, uint8_t x, uint8_t y, uint8_t w, uint8_t h, fezui_dialog_t *dialog)
{
	uint8_t previous_color;
	previous_color = u8g2_GetDrawColor(&(fezui_ptr->u8g2));
	u8g2_SetDrawColor(&(fezui_ptr->u8g2),1);
	u8g2_DrawBox(&(fezui_ptr->u8g2), x, y, w, h);

	u8g2_SetDrawColor(&(fezui_ptr->u8g2),0);
	u8g2_DrawFrame(&(fezui_ptr->u8g2), x, y, w, h);

	u8g2_SetDrawColor(&(fezui_ptr->u8g2),previous_color);
}
