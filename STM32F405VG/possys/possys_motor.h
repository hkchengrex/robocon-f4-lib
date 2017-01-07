#ifndef	__POSSYS_MOTOR_H
#define	__POSSYS_MOTOR_H

#include "possys.h"

s16* possys_motor_get_vel(void);
void possys_motor_set_front_dir(u16 dir);
void possys_motor_reset_vel(void);
void possys_motor_add_trans(u16 angle, s16 vel);
void possys_motor_add_rot(s16 vel);
void possys_motor_add_coeff(double coeff);
void possys_motor_set_vel(void);
void possys_motor_lock(void);

#endif
