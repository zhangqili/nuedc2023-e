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

uint32_t theta_central;
uint32_t phi_central;

schmitt_t theta_adjust = {.parameter = 500};
schmitt_t phi_adjust = {.parameter = 500};

cartesian_coordinate_system_t current_point = {0,Y_CENTRAL,0};
cartesian_coordinate_system_t current_actual_point = {0,Y_CENTRAL,0};
cartesian_coordinate_system_t target_actual_point;
cartesian_coordinate_system_t *from_actual_point;
cartesian_coordinate_system_t *to_actual_point;

void (* action)(void);
void (* steer_set_target)(void);

uint32_t line_count=0;
uint32_t move_count=0;
uint32_t move_step=0;

bool moving;

cartesian_coordinate_system_t *point_collection[16];
QUEUE_HandleTypeDef point_queue;

void steer_schmitt_init()
{

}

double pulse_to_angle(uint32_t pulse)
{
    return (pulse - (double)THETA_DEGREE_0)/((double)SEMICIRCLE)*180.0;
}

int32_t powi(double base, uint32_t p)
{
    if(p)
    {
        while(p--)
            base*=base;
        return base;
    }
    else
    {
        return 1;
    }
}

double solve_equation(cartesian_coordinate_system_t *c)
{
    double ce = sqrt((c->x)*(c->x)+(c->y)*(c->y)-RADIUS_Y*RADIUS_Y);
    double a = (((c->z)*(c->z)*(c->z)*(c->z))/(RADIUS_Y*RADIUS_Y*RADIUS_Y*RADIUS_Y)-2*(c->z)*(c->z)/(RADIUS_Y*RADIUS_Y)+1);
    double b = 2*((c->z)*(c->z)-RADIUS_Y*RADIUS_Y+ce*ce-(c->z)*(c->z)*ce*ce/(RADIUS_Y*RADIUS_Y))- 4*ce*ce;
    double c1 = RADIUS_Y*RADIUS_Y*RADIUS_Y*RADIUS_Y - 2*RADIUS_Y*RADIUS_Y*ce*ce + ce*ce*ce*ce + 4*ce*ce*RADIUS_Y*RADIUS_Y;
    double delta = b*b-4*a*c1;
    double m2 = (-b+sqrt(delta))/(2*a);
    double m = sqrt(m2);
    return m;
}

void cartesian_to_spherical(spherical_coordinate_system_t *s, cartesian_coordinate_system_t *c)
{
	s->r = sqrt((c->x)*(c->x)+(c->y)*(c->y)+(c->z)*(c->z));
	if(c->z>0)
	{
	    s->theta = atan(sqrt((c->x)*(c->x)+(c->y)*(c->y))/c->z);
	}
	else
	{
        s->theta = PI + atan(sqrt((c->x)*(c->x)+(c->y)*(c->y))/c->z);
    }
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

void spherical_to_cartesian(cartesian_coordinate_system_t *c, spherical_coordinate_system_t *s)
{
    static cartesian_coordinate_system_t p1 = {300,Y_CENTRAL,300};
    static cartesian_coordinate_system_t p2 = {-300,Y_CENTRAL,-300};
    steer_linear_follow(&p1, &p2, 5000);
    theta_adjust.peak = __HAL_TIM_GET_COMPARE(&htim5,TIM_CHANNEL_3);
    theta_adjust.peak = __HAL_TIM_GET_COMPARE(&htim5,TIM_CHANNEL_3);
}

void steer_set_spherical(spherical_coordinate_system_t *s)
{
    steer_set_theta(s->theta*180/PI);
    steer_set_phi(s->phi*180/PI);
}

void steer_set_cartesian(cartesian_coordinate_system_t *c)
{
    steer_set_cartesian_vertical(c);
    //spherical_coordinate_system_t s={0};
    //cartesian_to_spherical(&s, c);

    //steer_set_theta(s.theta*180/PI);
    //steer_set_phi(s.phi*180/PI);
}

void steer_set_cartesian_vertical(cartesian_coordinate_system_t *c)
{

    spherical_coordinate_system_t s;
    if(iszero(c->z))
    {
        s.theta = PI/2;
    }
    else
    {
        s.theta = atan(sqrt((c->x)*(c->x)+(c->y)*(c->y))/c->z)+asin(RADIUS_Y/sqrt((c->x)*(c->x)+(c->y)*(c->y)+(c->z)*(c->z)));
        if (s.theta<0)
            s.theta += PI;
    }
    if(iszero(c->x))

    {
        if(c->y>0)
        {
            s.phi = PI/2;
        }
        else if(c->y<0)
        {
            s.phi = PI*3/2;
        }
        else if(iszero(c->y))
        {
            s.phi = 0.0;
        }
    }
    else if(c->x>0)
    {
        if(c->y>0)
        {
            s.phi = atan(c->y/c->x);
        }
        else if(c->y<=0)
        {
            s.phi = PI*2 + atan(c->y/c->x);
        }
    }
    else
    {
        if(c->y>0)
        {
            s.phi = PI + atan(c->y/c->x);
        }
        else if(c->y<=0)
        {
            s.phi = PI + atan(c->y/c->x);
        }
    }
    steer_set_theta(s.theta*180/PI);
    steer_set_phi(s.phi*180/PI);
}


void steer_linear_follow(cartesian_coordinate_system_t *from, cartesian_coordinate_system_t *to, uint32_t period)
{
    cartesian_coordinate_system_t c;
    for (uint32_t i = 0; i < period; i++)
    {
        get_point_on_line(&c,from,to,period,i);
        steer_set_cartesian(&c);
        //HAL_Delay(1);
    }
}

void get_point_on_line(cartesian_coordinate_system_t *output, cartesian_coordinate_system_t *from, cartesian_coordinate_system_t *to, uint32_t period, uint32_t x)
{
    output->x = from->x + (to->x-from->x)*((double)(x))/((double)(period));
    output->y = from->y + (to->y-from->y)*((double)(x))/((double)(period));
    output->z = from->z + (to->z-from->z)*((double)(x))/((double)(period));
}

void steer_linear_follow_continuously(cartesian_coordinate_system_t *points, uint32_t count, uint32_t period, uint32_t pause)
{
    for (uint32_t i = 1; i < count; i++)
    {
        steer_linear_follow(points+i-1, points+i, period);
        HAL_Delay(pause);
    }
}

void steer_linear_follow_loop_once(cartesian_coordinate_system_t *points, uint32_t count, uint32_t period, uint32_t pause)
{
    for (uint32_t i = 1; i < count; i++)
    {
        steer_linear_follow(points+i-1, points+i, period);
        HAL_Delay(pause);
    }
    steer_linear_follow(points+count-1, points, period);
}

void pixel_to_reality(cartesian_coordinate_system_t *to,cartesian_coordinate_system_t *from)
{
    to->x = -250 + 500.0*(from->x - (double)FRAME_PIXEL_X)/((double)FRAME_PIXEL_W) + X_CENTRAL;
    to->z = 250 - 500.0*(from->z - (double)FRAME_PIXEL_Y)/((double)FRAME_PIXEL_H) + Z_CENTRAL;
}


double cartesian_length(cartesian_coordinate_system_t *p1, cartesian_coordinate_system_t *p2)
{
    return sqrt(((p1->x)-(p2->x))*((p1->x)-(p2->x))+((p1->y)-(p2->y))*((p1->y)-(p2->y))+((p1->z)-(p2->z))*((p1->z)-(p2->z)));
}

void steer_linear_follow_to(cartesian_coordinate_system_t *to, uint32_t period)
{

}
