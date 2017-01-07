#ifndef	__POSSYS_H
#define	__POSSYS_H

#include "system.h"
#include "stm32f10x.h"
#include "approx_math.h"
#include "stdlib.h"

typedef	struct {
	s16 x;
	s16 y;
	u16 deg;
} COORD3;

typedef	struct {
	double kp;
	double ki;
	double kd;
} PID;

typedef enum {
	POSSYS_RUNNING,
	POSSYS_ARRIVED
} POSSYS_STATE;

typedef struct
{
	COORD3* Possys_Target;
	PID* Possys_PidTrans;
	PID* Possys_PidRot;
	u16 Possys_RangeTrans;
	u16 Possys_RangeRot;
	s16 Possys_InitVel;
} Possys_InitTypeDef;

#include "possys_pid.h"
#include "possys_motor.h"
#include "possys_acc.h"
#include "possys_math.h"

#define WHEEL_NUM 3

void possys_set_coord(COORD3 * coord, s16 x, s16 y, u16 deg);
void possys_set_pid(PID * pid_obj, double p, double i, double d);

void possys_init(Possys_InitTypeDef *Possys_InitStruct);
POSSYS_STATE possys_update(void);
void possys_debug(void);
void possys_calibrate(void);

#endif
