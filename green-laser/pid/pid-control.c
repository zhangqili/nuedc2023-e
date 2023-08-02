#include "pid-control.h"
#include "tim.h"
PID steerx;
PID steery;
float x_error;
float y_error;


void steerx_control(void)
{
	steerx.errdat=0.7*x_error+0.3*steerx.perr;
	PidIncCtrl(&steerx);
	if(steerx.pidout>DEGREE180)steerx.pidout=DEGREE180;
	else if(steerx.pidout<DEGREE0)steerx.pidout=DEGREE0;
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,steerx.pidout);
}
void steery_control(void)
{
	steery.errdat=0.7*y_error+0.3*steery.perr;
	PidIncCtrl(&steery);
	if(steery.pidout>DEGREE180)steery.pidout=DEGREE180;
	else if(steery.pidout<DEGREE0)steery.pidout=DEGREE0;
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,steery.pidout);
}

void pid_inti(PID *pid)
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
      + pid->dGain  * (pid->errdat - pid->perr);
  pid->perr = pid->errdat;
}
