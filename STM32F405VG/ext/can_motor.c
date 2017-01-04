#include "can_motor.h"

#define get_can_motor_id(motor_id)	(CAN_MOTOR_BASE + (u8)motor_id)

static s32 can_motor_encoder_value[CAN_MOTOR_COUNT] = {0};

/**
* @brief Handler for decoding motor CAN feedback message
* @param msg: the CAN msg to be decoded
*/
static void can_motor_feedback_decoding(CanRxMsg* msg) {
	switch (msg->Data[0]) {
		case CAN_ENCODER_FEEDBACK:
			if (msg->DLC == CAN_ENCODER_FEEDBACK_LENGTH) {
				// Range check 
				if (msg->StdId >= CAN_MOTOR_BASE && msg->StdId < CAN_MOTOR_BASE + CAN_MOTOR_COUNT) {
					s32 feedback = n_bytes_to_one(&msg->Data[1], 4);
					can_motor_encoder_value[msg->StdId - CAN_MOTOR_BASE] = feedback;
				}
			}
		break;
	}
}

//Init CAN Motor
void can_motor_init(){
	can_rx_add_filter(CAN_MOTOR_BASE, CAN_RX_MASK_DIGIT_0_F, 0, MOTOR_CAN, can_motor_feedback_decoding);
}


/**
* @brief Set motor velocity (CAN)
* @param motor_id: MOTORx, which motor to control
* @param velocity. Open loop: (-1799~1799); Close loop: (-150~150);
* @param loop: Open loop or close loop control
*/
void motor_set_vel(MOTOR_ID motor_id, s32 vel, CLOSE_LOOP_FLAG loop){
	CanMessage msg;
	
	assert_param((u8)motor_id < CAN_MOTOR_COUNT);

	msg.id = get_can_motor_id(motor_id);
	msg.length = CAN_MOTOR_VEL_LENGTH;
	msg.data[0] = CAN_MOTOR_VEL_CMD;
	msg.data[1] = (u8)(one_to_n_bytes(vel, 0));
	msg.data[2] = (u8)(one_to_n_bytes(vel, 1));
	msg.data[3] = (u8)(one_to_n_bytes(vel, 2));
	msg.data[4] = (u8)(one_to_n_bytes(vel, 3));
	msg.data[5] = (u8)(loop);
	
	can_tx_enqueue(MOTOR_CAN, msg);
}

/**
* @brief Set motor position (CAN)
* @param motor_id: MOTORx, which motor to control
* @param vel (vel of close_loop is not corresponded to open_loop)
* @param pos: The position need to move to relative to current encoder value.
*/
void motor_set_pos(MOTOR_ID motor_id, u16 vel, s32 pos){
	CanMessage msg;
	
	assert_param((u8)motor_id < CAN_MOTOR_COUNT);
	
	msg.id = get_can_motor_id(motor_id);
	msg.length = CAN_MOTOR_POS_LENGTH;
	msg.data[0] = CAN_MOTOR_POS_CMD;
	msg.data[1] = (u8)(one_to_n_bytes(vel, 0));
	msg.data[2] = (u8)(one_to_n_bytes(vel, 1));
	msg.data[3] = (u8)(one_to_n_bytes(pos, 0));
	msg.data[4] = (u8)(one_to_n_bytes(pos, 1));
	msg.data[5] = (u8)(one_to_n_bytes(pos, 2));
	msg.data[6] = (u8)(one_to_n_bytes(pos, 3));

	can_tx_enqueue(MOTOR_CAN, msg);
}

/**
* @brief Set motor acceleration (CAN)
* @param motor_id: MOTORx, which motor to control
* @param accel: acceleration parameter of motor
*/
void motor_set_acceleration(MOTOR_ID motor_id, u16 accel){
	CanMessage msg;
	
	assert_param((u8)motor_id < CAN_MOTOR_COUNT);
	
	msg.id = get_can_motor_id(motor_id);
	msg.length = CAN_MOTOR_PARAMETER_LENGTH;
	msg.data[0] = CAN_MOTOR_PARAMETER_CMD;
	msg.data[1] = (u8)(one_to_n_bytes(accel, 0));
	msg.data[2] = (u8)(one_to_n_bytes(accel, 1));

	can_tx_enqueue(MOTOR_CAN, msg);
}

/**
* @brief Lock and stop motor immediately (CAN)
* @param motor_id: MOTORx, which motor to control
*/
void motor_lock(MOTOR_ID motor_id){
	CanMessage msg;
	
	assert_param((u8)motor_id < CAN_MOTOR_COUNT);
	
	msg.id = get_can_motor_id(motor_id);
	msg.length = CAN_MOTOR_LOCK_LENGTH;
	msg.data[0] = CAN_MOTOR_LOCK_CMD;

	can_tx_enqueue(MOTOR_CAN, msg);
}


/*** RX ***/

/**
* @brief Get the motor encoder value (based on CAN rx result)
* @param motor_id: MOTORx, which motor to control
*/
s32 get_encoder_value(MOTOR_ID motor_id){
	return can_motor_encoder_value[motor_id];
}


