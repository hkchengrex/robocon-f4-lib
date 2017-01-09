#include "possys_acc.h"
#include "math.h"

extern COORD3 pos_cur;
extern COORD3 pos_prev;
extern COORD3 pos_tar;

/**
  * @brief  Gives acceleration coefficient on a linear curve.
  * @param  start: Timeframe when PID motion starts
  * @param  period: Length of period from zero to full velocity
  * @retval Coefficient ranging from 0 to 1
  */
double possys_acc_linear(u32 start, u16 period) {
	double accel_coeff = (double)(get_ticks() - start)/period;
	return (accel_coeff > 1.0) ? 1.0 : ((accel_coeff < 0.0) ? 0.0 : accel_coeff);
}

double possys_dec_linear(s16 dist) {
	double decel_coeff = (double)possys_pythag(&pos_cur, &pos_tar)/dist;
	return (decel_coeff > 1.0) ? 1.0 : ((decel_coeff < 0.0) ? 0.0 : decel_coeff);
}

double possys_dec_sqrt(s16 dist) {
	double decel_coeff = sqrt(possys_pythag(&pos_cur, &pos_tar) / dist);
	return (decel_coeff > 1.0) ? 1.0 : ((decel_coeff < 0.0) ? 0.0 : decel_coeff);
}
