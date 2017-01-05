SysTick 
	- Ticks increment

IRQ interrupt priority:

	- Preemption Priority 0
		1 - Encoder overflow/underflow interrupt
		3 - CAN 1 TX complete interrupt
		4 - CAN 2 TX complete interrupt

	- Preemption Priority 1
		1 - UART receive interrupt
		7 - CAN1 RX interrupt
		8 - CAN2 RX interrupt
		
	- Preemption Priority 2
		4 - Timer event
	
DMA Channel:

	- DMA 1
		Stream 3 - Channel 4 - USART3 TX
		Stream 4 - Channel 4 - USART4 TX
		Stream 6 - Channel 4 - USART2 TX
		Stream 7 - Channel 4 - UART5 TX
	- DMA 2
		Stream 7 - Channel 4 - USART1 TX
		
Timer:
	-Timer 3
		- Channel 1-4 - Servo 1-4
		
	-Timer 4
		- Channel 3 - Encoder A phase
		- Channel 4 - Encoder B phase

	-Timer 7 (Basic timer)
		-Handling timer event
		
	-Timer 11
		- Channel 1 - Servo 5
		
	-Timer 13
		-Channel 1 - Servo 6
		
		
SPI:
	- SPI 1 - TFT