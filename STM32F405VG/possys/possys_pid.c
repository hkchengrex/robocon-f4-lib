#include "possys_pid.h"

extern COORD3 pos_cur;
extern COORD3 pos_prev;
extern COORD3 pos_tar;

extern PID pid_trans;
extern PID pid_rot;

s16 possys_pid_get_trans_err() {
	static s16 err_p = 0;
	static s16 err_d = 0;
	static s16 prev_err = 0;
	s32 trans_err;
	
	trans_err = ((pos_tar.y - pos_prev.y) * pos_cur.x) - ((pos_tar.x - pos_prev.x) * pos_cur.y);
	trans_err += ((pos_tar.x * pos_prev.y) - (pos_tar.y * pos_prev.x));
	trans_err /= possys_pythag(&pos_prev, &pos_tar);
	
	trans_err = trans_err * pid_trans.kp + err_p * pid_trans.ki + err_d * pid_trans.kd;
	err_p += trans_err;
	err_d = trans_err - prev_err;
	prev_err = trans_err;
	
	return (s16)trans_err;
}

s16 possys_pid_get_rot_err(void) {
	static s16 err_p = 0;
	static s16 err_d = 0;
	static s16 prev_err = 0;
	s16 rot_err;

	rot_err = possys_mod(pos_cur.deg - pos_tar.deg, 3600);
	rot_err = (rot_err >= 1800) ? rot_err - 3600 : rot_err;
	
	rot_err = rot_err * pid_rot.kp + err_p * pid_rot.ki + err_d * pid_rot.kd;
	err_p += rot_err;
	err_d = rot_err - prev_err;
	prev_err = rot_err;
	
	return rot_err;
}
