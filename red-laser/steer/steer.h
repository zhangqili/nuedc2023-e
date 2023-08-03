/*
 * steer.h
 *
 *  Created on: Aug 2, 2023
 *      Author: xq123
 */

#ifndef STEER_H_
#define STEER_H_

#include "main.h"

typedef struct __cartesian_coordinate_system_t
{
	double x;
	double y;
	double z;
} cartesian_coordinate_system_t;


typedef struct __spherical_coordinate_system_t
{
    double r;
	double theta;
	double phi;
} spherical_coordinate_system_t;


typedef struct __schmitt_t
{
    double parameter;
    double peak;
} schmitt_t;

#define SEMICIRCLE (182720 - 69020)


#define X_CENTRAL 0
#define Y_CENTRAL 1020
#define Z_CENTRAL 200

#define THETA_CENTRAL 117370

#define PHI_CENTRAL 123370

#define FRAME_PIXEL_X 15
#define FRAME_PIXEL_Y 15
#define FRAME_PIXEL_W 106-15
#define FRAME_PIXEL_H 106-15


#define THETA_DEGREE_0 ((THETA_CENTRAL*2-SEMICIRCLE)/2)
#define THETA_DEGREE_180 ((THETA_CENTRAL*2-SEMICIRCLE)/2+SEMICIRCLE)

#define PHI_DEGREE_0 ((PHI_CENTRAL*2-SEMICIRCLE)/2)
#define PHI_DEGREE_180 ((PHI_CENTRAL*2-SEMICIRCLE)/2+SEMICIRCLE)

//#define THETA_DEGREE_0 (69020-2000)
//#define THETA_DEGREE_180 (182720-2000)

//#define PHI_DEGREE_0 66520
//#define PHI_DEGREE_180 (PHI_DEGREE_0+THETA_DEGREE_180-THETA_DEGREE_0)

#define RADIUS_Y 60

//uint32_t angle_to_pulse_theta(double angle);
#define angle_to_pulse_theta(angle) (uint32_t)(THETA_DEGREE_0+((double)angle/(double)180.0)*SEMICIRCLE)
#define angle_to_pulse_phi(angle) (uint32_t)(PHI_DEGREE_0+((double)angle/(double)180.0)*SEMICIRCLE)
#define steer_set_theta(angle) __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,angle_to_pulse_theta(angle))
#define steer_set_phi(angle) __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,angle_to_pulse_phi(angle))

double pulse_to_angle(uint32_t pulse);
void cartesian_to_spherical(spherical_coordinate_system_t *s, cartesian_coordinate_system_t *c);void cartesian_to_spherical(spherical_coordinate_system_t *s, cartesian_coordinate_system_t *c);
void steer_set_spherical(spherical_coordinate_system_t *s);
void steer_set_cartesian(cartesian_coordinate_system_t *c);
void steer_set_cartesian_vertical(cartesian_coordinate_system_t *c);
void steer_linear_follow(cartesian_coordinate_system_t *from, cartesian_coordinate_system_t *to, uint32_t period);
void steer_linear_follow_continuously(cartesian_coordinate_system_t *points, uint32_t count, uint32_t period, uint32_t pause);
void steer_linear_follow_loop_once(cartesian_coordinate_system_t *points, uint32_t count, uint32_t period, uint32_t pause);



#endif /* STEER_H_ */
