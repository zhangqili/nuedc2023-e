#include "pid-control.h"
#include "tim.h"
#include "steer.h"
PID phi_pid;
PID theta_pid;
float phi_error;
float theta_error;

#define PHI_LIMIT_MAX angle_to_pulse_phi(120)
#define PHI_LIMIT_MIN angle_to_pulse_phi(60)


#define THETA_LIMIT_MAX angle_to_pulse_theta(160)
#define THETA_LIMIT_MIN angle_to_pulse_theta(40)

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
    pid->pidout += pid->pGain/10 * (pid->errdat - pid->perr)
            + pid->iGain /10* pid->errdat
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
