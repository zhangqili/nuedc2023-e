#ifndef PID_CONTROL_PID_CONTROL_H_
#define PID_CONTROL_PID_CONTROL_H_
#include "main.h"
typedef struct
{
        float pGain;
        float iGain;
        float dGain;
        float ilimit;
        float instate;
        float perr;
        float errdat;
        float last_derivative;
        float lastperr_errdat;
        float last_pidout;
        int32_t pidout;
}PID;
extern PID phi_pid;
extern PID theta_pid;
void pid_init(PID *pid);
void PidIncCtrl(PID *pid);
void PidLocCtrl(PID *pid, float limit);
void phi_control(void);
void theta_control(void);
#endif
