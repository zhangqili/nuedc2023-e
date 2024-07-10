#include "pid-control.h"
#include "tim.h"
#include "steer.h"
PID phi_pid;
PID theta_pid;
float phi_error;
float theta_error;


#define PI         3.14159265358979323846
#define FLOAT_EPS  1e-6
#define iszero(x)  (fabs(x)<10.0*FLOAT_EPS)

#define PHI_LIMIT_MAX angle_to_pulse_phi(110)
#define PHI_LIMIT_MIN angle_to_pulse_phi(70)


#define THETA_LIMIT_MAX angle_to_pulse_theta(110)
#define THETA_LIMIT_MIN angle_to_pulse_theta(70)

void phi_control(void)
{
    //phi_pid.errdat = 0.7 * phi_error + 0.3 * phi_pid.perr;
    PidIncCtrl(&phi_pid);
    if (phi_pid.pidout > PHI_LIMIT_MAX)
        phi_pid.pidout = PHI_LIMIT_MAX;
    else if (phi_pid.pidout < PHI_LIMIT_MIN)
        phi_pid.pidout = PHI_LIMIT_MIN;
    steer_set_phi_raw(phi_pid.pidout);
}
void theta_control(void)
{
    //theta_pid.errdat = 0.7 * theta_error + 0.3 * theta_pid.perr;
    PidIncCtrl(&theta_pid);
    if(theta_pid.pidout>THETA_LIMIT_MAX)
        theta_pid.pidout = THETA_LIMIT_MAX;
    else if(theta_pid.pidout<THETA_LIMIT_MIN)
        theta_pid.pidout=THETA_LIMIT_MIN;
    steer_set_theta_raw(theta_pid.pidout);
}

void pid_set_output(cartesian_coordinate_system_t *c)
{
    spherical_coordinate_system_t s;
#if GLOBAL_FIXED_DISTANCE == 1
    if(iszero(c->z))
    {
        s.theta = PI/2;
    }
    else
    {
        s.theta = atan(sqrt((c->x)*(c->x)+(distance)*(distance))/c->z)+asin(RADIUS_Y/sqrt((c->x)*(c->x)+(distance)*(distance)+(c->z)*(c->z)));
        if (s.theta<0)
            s.theta += PI;
    }
    if(iszero(c->x))

    {
        if(distance>0)
        {
            s.phi = PI/2;
        }
        else if(distance<0)
        {
            s.phi = PI*3/2;
        }
        else
        {
            s.phi = 0.0;
        }
    }
    else if(c->x>0)
    {
        if(distance>0)
        {
            s.phi = atan(distance/c->x);
        }
        else
        {
            s.phi = PI*2 + atan(distance/c->x);
        }
    }
    else
    {
        if(distance>0)
        {
            s.phi = PI + atan(distance/c->x);
        }
        else
        {
            s.phi = PI + atan(distance/c->x);
        }
    }
#else
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
#endif
    theta_pid.last_pidout = angle_to_pulse_theta(s.theta*180/PI);
    theta_pid.pidout = angle_to_pulse_theta(s.theta*180/PI);
    phi_pid.last_pidout = angle_to_pulse_phi(s.phi*180/PI);
    phi_pid.pidout = angle_to_pulse_phi(s.phi*180/PI);
}

void pid_init(PID *pid)
{
    pid->instate = 0;
    pid->perr = 0;
    pid->errdat = 0;
    pid->pidout = 0;
    pid->last_pidout = 0;
    pid->last_derivative = 0;

}
//增量式pid
void PidIncCtrl(PID *pid)
{
    pid->last_pidout = pid->pidout;
    pid->pidout += pid->pGain * (pid->errdat - pid->perr)
            + pid->iGain * pid->errdat
            + pid->dGain * (pid->errdat - 2 * pid->perr + pid->lastperr_errdat);
    pid->lastperr_errdat = pid->perr;
    pid->perr = pid->errdat;
}
//位置式pid, 积分分离
void PidLocCtrl(PID *pid, float limit)
{
    pid->instate += pid->errdat;
//  if (pid->errdat >= limit)
//    pid->iGain = 0;
    // if(pid->instate>100)pid->instate=100;
    // else if(pid->instate<-100)pid->instate=-100;
    pid->pidout = pid->pGain * pid->errdat + pid->iGain * pid->instate
            + pid->dGain * (pid->errdat - pid->perr);
    pid->perr = pid->errdat;
}
