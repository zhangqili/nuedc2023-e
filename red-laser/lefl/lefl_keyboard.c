/*
 * lefl_keyboard.c
 */
#include "lefl.h"
char *ansi_104_keymap[6][17] = {
    {"Esc      ", "F1", "F2", "F3", "F4  ", "F5", "F6", "F7", "F8  ", "F9", "F10", "F11", "F12  ", "Prtsc ", "Scroll Lock", "Pause Break"},
    {"`     ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "0 ", "- ", "= ", "Backspace  ", "Insert", "    Home   ", "       PgUp"},
    {"Tab    ", "Q ", "W ", "E ", "R ", "T ", "Y ", "U ", "I ", "O ", "P ", "[ ", "] ", "       \\  ", "Delete", "    End    ", "       PgDn"},
    {"CapsLk  ", "A ", "S ", "D ", "F ", "G ", "H ", "J ", "K ", "L ", "; ", "' ", "     Enter  "},
    {"Shift    ", "Z ", "X ", "C ", "V ", "B ", "N ", "M ", ", ", ". ", "/ ", "       Shift  ", "      ","    Up     ", "           "},
    {"Ctrl", "   ", "Alt", "        Space        ", "Alt", "   ", "Menu", "Ctrl  ", "Left  ", "    Down   ", "      Right"},
};

void lefl_keyboard_x_increase(lefl_keyboard_t *keyboard,int8_t delta)
{
    keyboard->x+=delta;
    if(keyboard->x>=LEFL_KEYBOARD_MAX_X)
    {
        keyboard->x=0;
        keyboard->y++;
        if(keyboard->y>=LEFL_KEYBOARD_MAX_Y)
        {
            keyboard->x=0;
            keyboard->y=0;
        }
    }
    if(keyboard->x<0)
    {
        keyboard->x=LEFL_KEYBOARD_MAX_X-1;
        keyboard->y--;
        if(keyboard->y<0)
        {
            keyboard->x=LEFL_KEYBOARD_MAX_X-1;
            keyboard->y=LEFL_KEYBOARD_MAX_Y-1;
        }
    }
}
void lefl_keyboard_y_increase(lefl_keyboard_t *keyboard,int8_t delta)
{
    keyboard->y+=delta;
    if(keyboard->y>=LEFL_KEYBOARD_MAX_Y)
    {
        keyboard->y=0;
        keyboard->x++;
        if(keyboard->x>=LEFL_KEYBOARD_MAX_X)
        {
            keyboard->y=0;
            keyboard->x=0;
        }
    }
    if(keyboard->y<0)
    {
        keyboard->y=LEFL_KEYBOARD_MAX_Y-1;
        keyboard->x--;
        if(keyboard->x<0)
        {
            keyboard->y=LEFL_KEYBOARD_MAX_Y-1;
            keyboard->x=LEFL_KEYBOARD_MAX_X-1;
        }
    }
}

void lefl_keyboard_draw(lefl_keyboard_t *keyboard)
{

}
