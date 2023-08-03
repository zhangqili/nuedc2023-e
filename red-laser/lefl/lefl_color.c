/*
 * lefl_color.c
 */
#include "lefl.h"

void lefl_rgb_to_hsv(lefl_color_hsv_t *hsv, lefl_color_rgb_t *rgb)
{
	float max;
	float min;
	max=rgb->r;
	min=rgb->r;
	max=rgb->g>max?rgb->g:max;
	min=rgb->g<min?rgb->g:min;
	max=rgb->b>max?rgb->b:max;
	min=rgb->b<min?rgb->b:min;
	if(max==min)
	{
		hsv->h=0;
	}
	if(max==rgb->r&&rgb->g>=rgb->b)
	{
		hsv->h=60*(rgb->g-rgb->b)/(max-min);
	}
	else
	{
		hsv->h=60*(rgb->g-rgb->b)/(max-min)+360;
	}
	if(max==rgb->g)
	{
		hsv->h=60*(rgb->b-rgb->r)/(max-min)+120;
	}
	if(max==rgb->b)
	{
		hsv->h=60*(rgb->r-rgb->g)/(max-min)+240;
	}
	if(max==0)
	{
		hsv->s=0;
	}
	else
	{
		hsv->s=100*(1-min/max);
	}
	hsv->v=max*100/255;
}
void lefl_hsv_to_rgb(lefl_color_rgb_t *rgb, lefl_color_hsv_t *hsv)
{
	uint8_t hi;
	float f;
	hi=hsv->h/60;
	f=(float)hsv->h/60-hi;
	switch(hi)
	{
	case 0:
		rgb->r=hsv->v*255/100;
		rgb->g=hsv->v*255*(100-(1-f)*hsv->s)/10000;
		rgb->b=hsv->v*255*(100-hsv->s)/10000;
		break;
	case 1:
		rgb->r=hsv->v*255*(100-f*hsv->s)/10000;
		rgb->g=hsv->v*255/100;
		rgb->b=hsv->v*255*(100-hsv->s)/10000;
		break;
	case 2:
		rgb->r=hsv->v*255*(100-hsv->s)/10000;
		rgb->g=hsv->v*255/100;
		rgb->b=hsv->v*255*(100-(1-f)*hsv->s)/10000;
		break;
	case 3:
		rgb->r=hsv->v*255*(100-hsv->s)/10000;
		rgb->g=hsv->v*255*(100-f*hsv->s)/10000;
		rgb->b=hsv->v*255/100;
		break;
	case 4:
		rgb->r=hsv->v*255*(100-(1-f)*hsv->s)/10000;
		rgb->g=hsv->v*255*(100-hsv->s)/10000;
		rgb->b=hsv->v*255/100;
		break;
	case 5:
		rgb->r=hsv->v*255/100;
		rgb->g=hsv->v*255*(100-hsv->s)/10000;
		rgb->b=hsv->v*255*(100-f*hsv->s)/10000;
		break;
	default:
		break;
	}
}

