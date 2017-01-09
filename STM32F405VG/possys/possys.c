/**
  ******************************************************************************
  * @file    possys.c
  * @brief   Library providing functions for positioning system.
  * @details It includes initialization, update, and print debug information
  *          functions. It is aimed at guiding a wheelbase system moving 
  *          towards a designated target through PID error correction. 
  * @author  Hong Wing Pang
  * @date    Jul 2016
  *
  ******************************************************************************
  * @attention
  *
  * This module is expected to be called multiple times until the wheelbase
  * arrives at the given target position. It handles only ONE target position
  * at a given moment, and logic control over the target position is expected
  * to be handled by an external application-specific finite state machine.
  *
  ******************************************************************************
  */

#include "possys.h"

COORD3 pos_cur;
COORD3 pos_prev;
COORD3 pos_tar;
extern COORD3 PidTargetBuffer;
PID pid_trans;
PID pid_rot;

u16 range_trans;
u16 range_rot;

s16 err_trans;
s16 err_rot;
// curve, curve_flags
s16 full_vel;

static u16 front_dir;
static u32 begin_time;

/** Setters **/
void possys_set_coord(COORD3 * coord, s16 x, s16 y, u16 deg) {
	coord->x = x;
	coord->y = y;
	coord->deg = deg;
	//validate deg in future
}

void possys_set_pid(PID * pid_obj, double p, double i, double d) {
	pid_obj->kp = p;
	pid_obj->ki = i;
	pid_obj->kd = d;
}

/**
  * @brief  Initialization of positioning system.
  * @param  *Possys_InitStruct: pointer of struct carrying init parameters.
  * @retval None
  */
void possys_init(Possys_InitTypeDef *Possys_InitStruct) {
	possys_set_coord(&pos_cur, get_X(), get_Y(), get_angle());
	pos_prev = PidTargetBuffer;
	pos_tar = *(Possys_InitStruct->Possys_Target);
	
	range_trans = Possys_InitStruct->Possys_RangeTrans;
	range_rot = Possys_InitStruct->Possys_RangeRot;
	pid_trans = *(Possys_InitStruct->Possys_PidTrans);
	pid_rot = *(Possys_InitStruct->Possys_PidRot);
	full_vel = Possys_InitStruct->Possys_InitVel;
	
	begin_time = get_ticks();
}

/**
	* @brief  Updates the whole positioning system. To be called regularly.
  * @param  None
  * @retval POSSYS_RUNNING = 0 (not arrived yet); or POSSYS_ARRIVED = 1
	*/
POSSYS_STATE possys_update() {
	possys_set_coord(&pos_cur, get_X(), get_Y(), get_angle());
	front_dir = possys_arctan(&pos_cur, &pos_tar);
	
	if (possys_pythag(&pos_cur, &pos_tar) > range_trans) {
		err_trans = possys_pid_get_trans_err();
		err_rot = possys_pid_get_rot_err();
		possys_motor_reset_vel();
		possys_motor_add_trans(possys_mod(front_dir-pos_cur.deg, 3600), full_vel);
		possys_motor_add_trans(possys_mod(front_dir-pos_cur.deg-900, 3600), err_trans);
		possys_motor_add_rot(err_rot);
		possys_motor_add_coeff(possys_acc_linear(begin_time, 2000));
		possys_motor_add_coeff(possys_dec_linear(400));
		possys_motor_set_vel();
	} else {
		PidTargetBuffer = pos_tar;
		return POSSYS_ARRIVED;
	}
	return POSSYS_RUNNING;
}

/**
  * @brief   Outputs debug information of positioning system. To be called regularly.
  * @warning Hogs up resources so that each cycle takes more than 10ms. Use only when
  *          the debug information is really needed.
  * @param   None
  * @retval  None
  */
void possys_debug() {	
	tft_clear();
	tft_prints(0, 0, "X:   %5d", pos_cur.x);
	tft_prints(0, 1, "Y:   %5d", pos_cur.y);
	tft_prints(0, 2, "DEG: %4d", pos_cur.deg);
	tft_prints(8, 2, ".%d", pos_cur.deg%10);
	if (pos_cur.deg < 10) tft_prints(7, 2, "0");
	tft_prints(0, 3, "ERR: %4d %3d", err_trans, err_rot);
	tft_prints(0, 5, "%d %d %d", possys_motor_get_vel()[0], possys_motor_get_vel()[1], possys_motor_get_vel()[2]);
	//tft_prints(0, 6, "%d", front_dir);
	tft_prints(0, 9, "%d", get_ticks());
	tft_update();
}

/**
  * @brief   Updates calibrate function. To be called regularly.
  * @details Automatically self rotates for 510 degrees. A circle can be obtained by
  *          plotting raw coordinates (with X/Y_FLIP imposed) from 90 through 450
  *          degrees. The center of this circle is the desired SHIFT_X/Y values to be
  *          set in gyro.h so that the coordinates do not change much during rotation.
  * @warning X_FLIP / Y_FLIP in gyro.c must be already set so that positive direction
  *          corresponds to the right for X encoder and the front for Y encoder. 
  * @param   None
  * @retval  None
  */
void possys_calibrate(void) {
	static double cal_coeff = 1.0;
	static u8 looped, finished = 0;
	static u16 angle = 0;
	
	static COORD3 max, min, orig = {0};
	s16 cal_x, cal_y = 0;
	
	angle = get_angle();
	cal_x = get_pos()->x;
	cal_y = get_pos()->y * -1;
	
	if (!looped && (angle > 1800)) looped = 1;
	if (!looped && (angle < 900)) cal_coeff = (double)(angle + 300) / 1200;
	if (looped && (angle > 900) && (angle < 1500)) cal_coeff = (double)(1800 - angle) / 900;
	
	if (looped && (angle >= 1500) && (angle < 1800)) {
		possys_motor_lock();
		finished = 1;
	} else {
		possys_motor_reset_vel();
		possys_motor_add_rot(-40 * cal_coeff);
		possys_motor_set_vel();
	}
	
	if (cal_x > max.x) max.x = cal_x;
	if (cal_y > max.y) max.y = cal_y;
	if (cal_x < min.x) min.x = cal_x;
	if (cal_y < min.y) min.y = cal_y;
	if ((angle > 3590) || (angle == 0)) {
		orig.x = cal_x;
		orig.y = cal_y;
		orig.deg = angle;
	}
	
	tft_clear();
	tft_prints(0, 0, "%d %d", (u8)(cal_coeff * 100), looped);
	if (finished) {
		tft_prints(0, 2, "SHIFT_X: %d", (max.x + min.x)/2 - orig.x);
		tft_prints(0, 3, "SHIFT_Y: %d", (max.y + min.y)/2 - orig.y);
		tft_prints(0, 4, "%d", orig.deg);
	}
	tft_update();
}
