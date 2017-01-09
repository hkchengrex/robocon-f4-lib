#include "possys_motor.h"

static s16 motor_vel[WHEEL_NUM] = {0};
static u8 front_dir = 0;

/** Interfacing with vel array **/
s16* possys_motor_get_vel() {
	return motor_vel;
}

void possys_motor_reset_vel() {
	for (u8 i = 0; i < WHEEL_NUM; i++) {
		motor_vel[i] = 0;
	}
}

void possys_motor_add_trans(u16 angle, s16 vel) {
	for (int i = 0; i < WHEEL_NUM; i++) {
		motor_vel[i] += int_sin(angle + MOTOR_ANGLE[i])*vel/(-10000);
	}
}

void possys_motor_add_rot(s16 vel) {
	for (int i = 0; i < WHEEL_NUM; i++) {
		motor_vel[i] += vel;
	}
}

void possys_motor_add_coeff(double coeff) {
	for (int i = 0; i < WHEEL_NUM; i++) {
		motor_vel[i] *= coeff;
	}
}

/** Setting motor motion **/
void possys_motor_set_vel() {
	for (u8 i = 0; i < WHEEL_NUM; i++) {
		motor_set_vel((MOTOR_ID)i, motor_vel[i], CLOSE_LOOP);
	}
}

void possys_motor_lock() {
	for (u8 i = 0; i < WHEEL_NUM; i++) {
		motor_set_vel((MOTOR_ID)i, 0, CLOSE_LOOP);
		motor_lock((MOTOR_ID)i);
	}
}
