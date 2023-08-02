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
	float x;
	float y;
	float z;
} cartesian_coordinate_system_t;


typedef struct __spherical_coordinate_system_t
{
	float r;
	float theta;
	float phi;
} spherical_coordinate_system_t;

uint32_t angle_to_pulse(float angle);
void cartesian_to_spherical(spherical_coordinate_system_t *s, cartesian_coordinate_system_t *c);void cartesian_to_spherical(spherical_coordinate_system_t *s, cartesian_coordinate_system_t *c);
void steer_set_spherical(spherical_coordinate_system_t *s);
void steer_set_cartesian(cartesian_coordinate_system_t *c);
void steer_linear_follow(cartesian_coordinate_system_t *from, cartesian_coordinate_system_t *to, uint32_t period);



#endif /* STEER_H_ */
