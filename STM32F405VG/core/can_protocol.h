#ifndef _CAN_PROTOCOL_H
#define _CAN_PROTOCOL_H

#include <stdbool.h>
#include <stm32f4xx.h>
#include <stm32f4xx_can.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include "gpio.h"

//CAN Information
#define CAN1_RCC RCC_APB1Periph_CAN1
#define CAN2_RCC RCC_APB1Periph_CAN2

#define CAN1_RX_GPIO PA11
#define CAN1_TX_GPIO PA12

#define CAN2_RX_GPIO PB6
#define CAN2_TX_GPIO PB12

#define CAN1_TX_QUEUE_MAX_SIZE 100
#define CAN2_TX_QUEUE_MAX_SIZE 100

/** For 32-bit mask filter, there can be at most 14 filters, shared by CAN1 and CAN2
* You can make it into 16-bit filter (28 filters), or identity list (*2 filter)
*/
#define	CAN_RX_FILTER_LIMIT 14 

#define CAN1_FILTER_LIMIT 10
#define CAN2_FILTER_LIMIT CAN_RX_FILTER_LIMIT—CAN1_FILTER_LIMIT

typedef enum{
	CAN_1,
	CAN_2
}CanID;

typedef struct{
	u16 id;					// 11-bit ID: 0x000 ~ 0x7FF
	u8 length;			// 0 ~ 8
	u8 data[8];
}CanMessage;

typedef struct{
	u16 head;
	u16 tail;
	u16 size;
	CanMessage* queue;
}CanQueue;

typedef void (*CanRxHandler)(CanRxMsg* msg);

/**
* X = ID bits that are checked
* ? = ID bits that are not checked
*/
																						/***   11-bit ID   	(example range) 		***/
#define CAN_RX_MASK_EXACT						0x7FF		/*** XXX XXXX XXXX	(Exactly same ID)		***/
#define CAN_RX_MASK_DIGIT_0_F				0x7F0		/*** XXX XXXX ???? 	(0xAB0 - 0xABF) 		***/
#define	CAN_RX_MASK_DIGIT_0_7				0x7F8		/*** XXX XXXX X??? 	(0xAB0 - 0xAB7)			***/
#define	CAN_RX_MASK_DIGIT_0_3				0x7FC		/*** XXX XXXX XX?? 	(0xAB0 - 0xAB3)			***/
#define	CAN_RX_MASK_DIGIT_0_1				0x7FE		/*** XXX XXXX XXX? 	(0xAB0 - 0xAB1)			***/

//Init both CAN1 and CAN2
void can_init(void);

/** Return the queue size of CAN1/CAN2
* @param id: which CAN queue to look at
* @return size of the specified queue
*/
inline u16 get_can_queue_size(CanID id);

/** Put a CAN message into the CAN queue for transmission
* @param id: which CAN to use
* @param msg: The message to be sent
* @return true if successful
*/
bool can_tx_enqueue(CanID id, CanMessage msg);

// Reset the entire CAN TX message queue
void can_tx_queue_clear(CanID id);

/*** CAN Rx ***/

// Initialize CAN RX interrupt
void can_rx_init(void);

/**
	* @brief Add filter to the can data received (involves bitwise calculation)
	* @warning can only be called for 14 / 28 times. Check the function IS_CAN_FILTER_NUMBER for detail
	* @param id: 11-bit ID (0x000 to 0x7FF)
	* @param mask: 11-bit mask, corresponding to the 11-bit ID	(0x000 to 0x7FF)
	* @param FIFO_num: Which FIFO to use, 0 or 1
	* @param handler: function pointer for the corresponding CAN ID filter
	* @example can_rx_add_filter(0x000, 0x000) will receive CAN message with ANY ID
	* @example can_rx_add_filter(0x0CD, 0x7FF) will receive CAN message with ID 0xCD
	* @example can_rx_add_filter(0x0A0, 0x7F0) will receive CAN message with ID from 0xA0 to 0xAF
	* @example can_rx_add_filter(0x000, 0x7FA) will receive CAN message with ID from 0x00 to 0x03
	*/
void can_rx_add_filter(u16 id, u16 mask, u8 FIFO_num, void (*handler)(CanRxMsg* msg));

/*** Protocol Encoding / Decoding function ***/

/** @brief Convert one n-byte variable to n one-byte variable (ENCODE)
	* @param n: the nth byte 
	* @param num: the nth byte number (can be unsigned)
	* @retval The nth byte variable
	*/
u8 one_to_n_bytes(s32 num, u8 n);

/** @brief Convert n one-byte variable to an array of n bytes (DECODE)
	* @param n: the number of bytes
	* @param array: the array of n bytes
	*/
s32 n_bytes_to_one(u8* array, u8 n);

#endif /* __CAN_PROTOCOL_H */
