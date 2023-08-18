/*
 * steer_def.h
 *
 *  Created on: Aug 4, 2023
 *      Author: xq123
 */

#ifndef STEER_DEF_H_
#define STEER_DEF_H_


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



//#define THETA_CENTRAL 117370
#define THETA_CENTRAL theta_central

//#define PHI_CENTRAL 123370
#define PHI_CENTRAL phi_central

#define FRAME_PIXEL_X (14*2)
#define FRAME_PIXEL_Y (14*2)
#define FRAME_PIXEL_W ((106-14)*2)
#define FRAME_PIXEL_H ((106-14)*2)


#define THETA_DEGREE_0 ((THETA_CENTRAL*2-SEMICIRCLE)/2)
#define THETA_DEGREE_180 ((THETA_CENTRAL*2-SEMICIRCLE)/2+SEMICIRCLE)

#define PHI_DEGREE_0 ((PHI_CENTRAL*2-SEMICIRCLE)/2)
#define PHI_DEGREE_180 ((PHI_CENTRAL*2-SEMICIRCLE)/2+SEMICIRCLE)

#define STEER_TIMER_START() HAL_TIM_Base_Start_IT(&htim6)
#define STEER_TIMER_STOP() HAL_TIM_Base_Stop_IT(&htim6)

#endif /* STEER_DEF_H_ */
