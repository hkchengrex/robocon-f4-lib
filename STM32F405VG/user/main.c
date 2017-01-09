/**
** H K U S T
** Robocon 2017
**
** F4 Library
**
** @Author Rex Cheng
** @Contact hkchengad@connect.ust.hk
*/

#include "main.h"

//#define CALIBRATE_MODE

#define GYRO_INIT_TIME 5000
typedef void (*StateFunc)(void);
StateFunc PathState = path_gyro_cal;

static COORD3 PidTarget;
static PID PidTrans;
static PID PidRot;
static Possys_InitTypeDef Possys_InitStructure;

void path_gyro_cal(void) {
	if (get_ticks() > GYRO_INIT_TIME) {
		#ifdef CALIBRATE_MODE
			possys_calibrate();
			return;
		#endif
		PathState = path_init;
	}
}

void path_init(void) {
	possys_set_coord(&PidTarget, -1500, 1500, 0);
	possys_set_pid(&PidTrans, 0.85, 0.0, 0.0);
	possys_set_pid(&PidRot, 0.3, 0.0, 0.0);
	
	Possys_InitStructure.Possys_Target = &PidTarget;
	Possys_InitStructure.Possys_PidTrans = &PidTrans;
	Possys_InitStructure.Possys_PidRot = &PidRot;
	Possys_InitStructure.Possys_RangeTrans = 20;
	Possys_InitStructure.Possys_RangeRot = 10;
	Possys_InitStructure.Possys_InitVel = 80;
	
	possys_init(&Possys_InitStructure);
	PathState = path_update;
}

void path_update(void) {
	if (possys_update()) {
		PathState = path_end;
	}
	possys_debug();
}

void path_end(void) {
	possys_motor_lock();
}

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	gpio_rcc_init_all();
	
	ticks_init();
	tft_init((TFT_ORIENTATION)ORIENTATION_SETTING, BLACK, WHITE, RED);
	
	can_init();
	can_rx_init();
	can_motor_init();
	gyro_init();
	uart_init(COM1, 115200);
	
	u16 ticks_img = 0;
	
	while(1){
		
		if (ticks_img != get_ticks()) {
			if (ticks_img % 20 == 0){
				//uart_tx(COM1, (u8 *)"%d\n", ticks_img);
				PathState();
			}
			ticks_img = get_ticks();
		}
	}
}
