/*
 * steer.c
 *
 *  Created on: Aug 2, 2023
 *      Author: xq123
 */
#include "steer.h"
#include "math.h"
#include "tim.h"

#define PI         3.14159265358979323846
#define FLOAT_EPS  1e-6
#define iszero(x)  (fabs(x)<10.0*FLOAT_EPS)

#define ZERO

uint32_t angle_to_pulse(float angle)
{
	return 69020+(angle/180.0)*(182720-72520);
}

void cartesian_to_spherical(spherical_coordinate_system_t *s, cartesian_coordinate_system_t *c)
{
	s->r = sqrt((c->x)*(c->x)+(c->y)*(c->y)+(c->z)*(c->z));
	s->theta = atan(sqrt((c->x)*(c->x)+(c->y)*(c->y))/c->z);
	if(iszero(c->x))
	{
	    if(c->y>0)
	    {
	        s->phi = PI/2;
	    }
	    else if(c->y<0)
	    {
            s->phi = PI*3/2;
	    }
        else if(iszero(c->y))
        {
            s->phi = 0.0;
        }
	}
	else if(c->x>0)
	{
        if(c->y>0)
        {
            s->phi = atan(c->y/c->x);
        }
        else if(c->y<=0)
        {
            s->phi = PI*2 + atan(c->y/c->x);
        }
	}
	else if(c->x<0)
	{
        if(c->y>0)
        {
            s->phi = PI + atan(c->y/c->x);
        }
        else if(c->y<=0)
        {
            s->phi = PI + atan(c->y/c->x);
        }
    }
}

void steer_set_spherical(spherical_coordinate_system_t *s)
{
    //__HAL_TIM_SET_COMPARE();
}

void steer_set_cartesian(cartesian_coordinate_system_t *c)
{
    spherical_coordinate_system_t s={0};
    cartesian_to_spherical(&s, c);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,angle_to_pulse(s.theta*180/PI));
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,angle_to_pulse(s.phi*180/PI));
}


void steer_linear_follow(cartesian_coordinate_system_t *from, cartesian_coordinate_system_t *to, uint32_t period)
{
    cartesian_coordinate_system_t c;
    for (uint32_t i = 0; i < period; i++)
    {
        c.x = from->x + (to->x-from->x)*((float)(i))/((float)(period));
        c.y = from->y + (to->y-from->y)*((float)(i))/((float)(period));
        c.z = from->z + (to->z-from->z)*((float)(i))/((float)(period));
        steer_set_cartesian(&c);
        HAL_Delay(1);
    }
}
