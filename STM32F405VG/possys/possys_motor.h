#ifndef	__POSSYS_MOTOR_H
#define	__POSSYS_MOTOR_H

#include "possys.h"

/*-- No. of wheels in the wheelbase --*/
#define WHEEL_NUM 3
//#define WHEEL_NUM 4

/*-- Angle of each wheel, in ascending order from MOTOR1, multiplied by 10 --*/
static s16 MOTOR_ANGLE[WHEEL_NUM] = {0, 1200, 2400};
//static s16 MOTOR_ANGLE[WHEEL_NUM] = {1800, 3000, 600};
//static s16 MOTOR_ANGLE[WHEEL_NUM] = {150, 1650, 1950, 3450};

s16* possys_motor_get_vel(void);
void possys_motor_reset_vel(void);
void possys_motor_add_trans(u16 angle, s16 vel);
void possys_motor_add_rot(s16 vel);
void possys_motor_add_coeff(double coeff);
void possys_motor_set_vel(void);
void possys_motor_lock(void);

#endif
