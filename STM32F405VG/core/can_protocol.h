#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include <cstring>
#include <stdio.h>
#include <stdlib.h> 
#include <stm32f4xx.h>
#include <stm32f4xx_can.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include <stm32f4xx_gpio.h>

/*** CAN Config ***/
#define	CANn							CAN1
#define CAN_RCC						RCC_APB1Periph_CAN1

#define CAN_Rx_GPIO GPIO_Pin_11
#define CAN_Tx_GPIO GPIO_Pin_12
#define CAN_GPIO GPIOA

#define CAN_GPIO_RCC			RCC_AHB1Periph_GPIOA

#define CAN_TX_QUEUE_MAX_SIZE				300
#define	CAN_RX_FILTER_LIMIT					28		// The number of filters can be applied at most


/*** X = the ID bit that must be equal 	***/
/*** ? = the ID bit that can varies 		***/
																						/***   11-bit ID   	(example range) 		***/
#define CAN_RX_MASK_EXACT						0x7FF		/*** XXX XXXX XXXX	(Exactly same ID)		***/
#define CAN_RX_MASK_DIGIT_0_F				0x7F0		/*** XXX XXXX ???? 	(0xAB0 - 0xABF) 		***/
#define	CAN_RX_MASK_DIGIT_0_7				0x7F8		/*** XXX XXXX X??? 	(0xAB0 - 0xAB7)			***/
#define	CAN_RX_MASK_DIGIT_0_3				0x7FC		/*** XXX XXXX XX?? 	(0xAB0 - 0xAB3)			***/
#define	CAN_RX_MASK_DIGIT_0_1				0x7FE		/*** XXX XXXX XXX? 	(0xAB0 - 0xAB1)			***/


typedef struct CAN_MESSAGE {
	u16 id;					/*** 11-bit ID: 0x000 to 0x7FF ***/
	u8 length;			/*** 0 to 8 ***/
	u8 data[8];
}CAN_MESSAGE;

typedef struct CAN_QUEUE {
	u16 head;											/*** Current head of queue ***/
	u16 tail;											/*** Current tail of queue ***/
	struct CAN_MESSAGE* queue;		/*** The can message queue (array) ***/
}CAN_QUEUE;


void can_init(void);

/*** CAN Tx ***/

/** @brief	Get the current CAN_TX queue size
	* @retval	The current queue size (0 to CAN_TX_QUEUE_MAX_SIZE-1)
	*/
u16 can_tx_queue_size(void);

/** @brief Check if the CAN_TX queue is empty
	* @retval True if the queue is empty
	*/
u8 can_tx_queue_empty(void);


/**  @brief Add a new tx message to the CAN Tx queue
	* @param msg: The can message that will be added
	* @retval 0: Fail to enqueue due to the exceeding size, 1: Successfully enqueued
	*/
u8 can_tx_enqueue(struct CAN_MESSAGE msg);	

/** @brief	Process and transfer ONE can message in the queue and dequeue.
	*					To be through interrupt and the enqueue function.
	* @param 	None
	*	@retval True if the queue is not empty after dequeue
	*/
u8 can_tx_dequeue(void);							// <--- To be called through interrupt

/** @brief Force clear the CAN_TX queue without process
	* @param None.
	* @retval None.
	*/
void can_tx_queue_clear(void);	

/*** CAN Rx ***/

/**
	* @brief Initialize the CAN_RX interrupt handler
	* @param None.
	* @retval None.
	*/
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
