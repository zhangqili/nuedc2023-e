#ifndef PID_CONTROL_PID_CONTROL_H_
#define PID_CONTROL_PID_CONTROL_H_
#include "main.h"
#define DEGREE0 69020
#define DEGREE180 182720
typedef struct
{
        float pGain;//P����ϵ��
        float iGain;//I����ϵ��
        float dGain;//D΢��ϵ��
        float ilimit;//�����޷�
        float instate;//������
        float perr;//�ϴ����
        float errdat;//��ǰ���
        //float zhuanjiao;
       //float GKD;//������΢��
        float   P;
        float   I;
        float   D;
        float last_derivative;//�ϴ���������ϴ����֮��
        float lastperr_errdat;//���ϴ����
        float last_pidout;
        uint32_t pidout;//PID���������
}PID;
extern PID steerx;
extern PID steery;
void pid_inti(PID *pid);
void PidIncCtrl(PID *pid);
void PidLocCtrl(PID *pid, float limit);
#endif
